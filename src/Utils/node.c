// IFJ Project 2021
/**
 * @file node.c
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file implements struct Node and it's interface
 */
#include "node.h"
#include "logger.h"
#include "symbolelement.h"

#include <stdlib.h>
#include <string.h>

struct Node_t
{
    NodeType nodeType;
    void *data;
    Vector *sons;
    Vector *returns;
    SemanticType semanticType;
    int count;
    void (*DataDtor)(void*);
    PrecedenceItemType operation;    //! use only when it is operation
};

SemanticType Node_GetSemantic(Node *root)
{
    return root->semanticType;
}

void Node_SetSemantic(Node *root, SemanticType semanticType)
{
    root->semanticType = semanticType;
}

NodeType Node_GetType(Node *root)
{
    return root->nodeType;
}

PrecedenceItemType Node_GetOperation(Node *node)
{
    return node->nodeType == NODE_OPERATION ? node->operation : P_VOID;
}

bool Node_IsOperation(Node *node)
{
    return node->nodeType == NODE_OPERATION;
}

Node *Node_Init(NodeType nodeType, void *data, SemanticType semanticType, void (*DataDtor)(void*), PrecedenceItemType operation)
{
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL)
        ERROR("Allocation failed");

    node->sons = Vector_Init(NULL);
    if (node->sons == NULL)
    {
        free(node);
        ERROR("Allocation failed");
    }

    node->returns = Vector_Init((void(*)(void*))NULL);
    if (node->returns == NULL)
    {
        Vector_Destroy(node->sons);
        free(node);
        ERROR("Allocation failed");
    }

    node->nodeType = nodeType;
    node->data = data;
    node->semanticType = semanticType;
    node->count = 0;
    node->DataDtor = DataDtor;
    node->operation = nodeType == NODE_OPERATION ? operation : P_VOID;

    return node;
}

bool Node_CompareWithNil(Node *old)
{
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL)
        ERROR("Allocation failed");

    node->sons = Vector_Init(NULL);
    if (node->sons == NULL)
    {
        free(node);
        ERROR("Allocation failed");
    }

    node->returns = Vector_Init((void(*)(void*))Node_Destroy);
    if (node->returns == NULL)
    {
        Vector_Destroy(node->sons);
        free(node);
        ERROR("Allocation failed");
    }

    node->nodeType = old->nodeType;             old->nodeType = NODE_OPERATION;
    node->data = old->data;                     old->data = NULL;
    node->sons = old->sons;                     old->sons = Vector_Init(NULL);              // NULL returned not checked
    node->returns = old->returns;               old->returns = Vector_Init((void(*)(void*))Node_Destroy);   // NULL returned not checked
    node->semanticType = old->semanticType;     old->semanticType = SEMANTIC_BOOLEAN;
    node->count = old->count;                   old->count = 0;
    node->DataDtor = old->DataDtor;             old->DataDtor = NULL;
    node->operation = old->operation;           old->operation = P_EQ;

    if (old->sons == NULL || old->returns == NULL)
    {
        Node_Destroy(node, false);
        free(old);  // not correct but shit happens
        return false;
    }

    Node_AppendSon(old, node);
    Node_AppendSon(old, Node_Init(NODE_NIL, NULL, SEMANTIC_NIL, (void(*)(void*))NULL, P_VOID));

    return true;
}

void Node_Destroy(Node *node, bool destroy)
{
    if (destroy)
    {
        for (int i = 0; i < Vector_Size(node->sons); i++)
            Node_Destroy((Node*)Vector_GetElement(node->sons, i), true);
        for (int i = 0; i < Vector_Size(node->returns); i++)
            Node_Destroy((Node*)Vector_GetElement(node->returns, i), true);
    }


    Vector_Destroy(node->returns);
    Vector_Destroy(node->sons);
    free(node);
}

bool Node_AppendSon(Node *node, Node *son)
{
    return Vector_PushBack(node->sons, son);
}

bool Node_AppendReturn(Node *parent, SemanticType semanticType)
{
    Node *new_return = Node_Init(NODE_POP, NULL, semanticType, (void(*)(void*))NULL, P_VOID);
    if (new_return == NULL)
        return false;
    return Vector_PushBack(parent->returns, new_return);
}

void *Node_GetData(Node *node)
{
    return node->data;
}

void Node_SetData(Node *node, void *data)
{
    node->data = data;
}

void Node_SetParamCount(Node *node, int count)
{
    node->count = count;
}

int Node_GetParamCount(Node *node)
{
    return node->count;
}

Vector *Node_GetSons(Node *parent)
{
    return parent->sons;
}

Vector *Node_GetReturns(Node *parent)
{
    return parent->returns;
}

void Number_Destroy(void *data)
{
    free(data);
}

void *Number_Init(int number)
{
    void *createdNumber = (void*)malloc(sizeof(int));
    if (createdNumber == NULL)
        return NULL;

    *((int*)createdNumber) = number;
    
    return createdNumber;
}

bool Node_IsBinaryOperator(PrecedenceItemType type)
{
    return
        (type == P_MUL ||
        type == P_DIV ||
        type == P_INT_DIV ||
        type == P_PLUS ||
        type == P_MINUS ||
        type == P_CONCAT ||
        type == P_GRT ||
        type == P_LESS ||
        type == P_GEQ ||
        type == P_LEQ ||
        type == P_EQ ||
        type == P_NEQ ||
        type == P_AND ||
        type == P_OR)

        ? true : false;
}

/**
 * @brief Generates code for all expressions and returns Vector of numbers in order from left to right where in TMP() are saved
 * @param node NODE should be of type NODE_RVALUES
 * @param buffer Buffers
 * @param destroy True to destroy sons
 * @param lvalues Amount of lvalues / returns of function
 *      @note important to know if last expression is function call and we need to know how much more we need
 * @return Vector of int*
 */
Vector *Node_GetRValues(Node *node, Buffers *buffer, bool destroy, int lvalues, RecursionType type)
{    
    Vector *returns = Vector_Init(Number_Destroy);

    // rvalues
    Vector *sons = Node_GetSons(node);

    for (int i = 0; i < Vector_Size(sons); i++)
    {
        Vector* tmp = NULL;
        if (i == Vector_Size(sons) - 1)
            tmp = Node_PostOrder(
                (Node*)Vector_GetElement(sons, i),
                destroy,
                buffer,
                lvalues - Vector_Size(sons) + 1,
                type
            );
        else
            tmp = Node_PostOrder(
                (Node*)Vector_GetElement(sons, i),
                destroy,
                buffer,
                1,
                type
            );

        if (tmp == NULL)
            ERROR("Missing expressions");

        for (int i = 0; i < Vector_Size(tmp); i++)
            Vector_PushBack(returns, Number_Init(*((int*)Vector_GetElement(tmp, i))));

        Vector_Destroy(tmp);
    }

    return returns;
}

void Node_GenerateAssign(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *sons = Node_GetSons(node);

    Vector *lvalues = Node_GetSons((Node*)Vector_GetElement(sons, 0));
    Node *rvalues = (Node*)Vector_GetElement(sons, 1);

    // move to node.c Node_PostOrder

    // sons[0] are lvalues sons[1] are rvalues
    Vector *assignments = Node_GetRValues(rvalues, buffer, destroy, Vector_Size(lvalues), type);
    if (assignments == NULL)
        return;

    for (int i = Vector_Size(lvalues) - 1; i >= 0; i--)
        if (type != ONLY_DEF)
            fprintf(buffer->output, "MOVE LF@%s%d LF@%s%d\n", ELEMENT((Node*)Vector_GetElement(lvalues, i)), TMP(*((int*)Vector_GetElement(assignments, i))));

    Vector_Destroy(assignments);
}

void Node_GenerateReturn(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *returns = Node_GetRValues(node, buffer, destroy, Element_FunctionReturns_Size(buffer->current_function), type);
    if (returns == NULL)
        return;

    for (int i = Vector_Size(returns) - 1; i >= 0; i--)
        if (type != ONLY_DEF)
            fprintf(buffer->output, "PUSHS LF@%s%d\n", TMP(*((int*)Vector_GetElement(returns, i))));

    Vector_Destroy(returns);

    if (type != ONLY_DEF)
    {
        fprintf(buffer->output, "POPFRAME\n");
        fprintf(buffer->output, "RETURN\n");
    }
}

void Node_GenerateConditionLabel(Buffers *buffer, Node *expression, bool destroy, int id, int label_count, char *label, RecursionType type)
{
    // should be already bool
    Vector *condition = Node_PostOrder(expression, destroy, buffer, 1, type);

    if (Vector_IsEmpty(condition))
        return;

    if (type != ONLY_DEF)
        fprintf(buffer->output, "JUMPIFNEQ %s%d_%d LF@%s%d bool@true\n", label, id, label_count, TMP(*((int*)Vector_GetElement(condition, 0))));

    Vector_Destroy(condition);
}

void Node_GenerateIf(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *sons = Node_GetSons(node);
    int id = Node_GetParamCount(node);
    int label_count = 0;

    Node_GenerateConditionLabel(buffer, (Node*)Node_GetData(node), destroy, id, label_count, IF_LABEL, type);

    // scope of other statemets
    for (int i = 0; i < Vector_Size(sons); i++)
    {
        Node *current = (Node*)Vector_GetElement(sons, i);
        switch (Node_GetType(current))
        {
        case NODE_ELSEIF:
            if (type != ONLY_DEF)
            {
                fprintf(buffer->output, "JUMP %s%d\n", IF_END_LABEL, id);
                fprintf(buffer->output, "LABEL %s%d_%d\n", IF_LABEL, id, label_count++); // increases label for next one
            }

            Node_GenerateConditionLabel(buffer, (Node*)Node_GetData(current), destroy, id, label_count, IF_LABEL, type);
            break;

        case NODE_ELSE:
            if (type != ONLY_DEF)
            {
                fprintf(buffer->output, "JUMP %s%d\n", IF_END_LABEL, id);
                fprintf(buffer->output, "LABEL %s%d_%d\n", IF_LABEL, id, label_count++); // increases label for next one
            }
             
            
            break;
        
        default:
            Vector_Destroy(Node_PostOrder(current, destroy, buffer, 0, type));
            break;
        }
    }

    if (type != ONLY_DEF)
        fprintf(buffer->output, "LABEL %s%d\n", IF_END_LABEL, id);
}

void Node_GenerateWhile(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *sons = Node_GetSons(node);
    int id = Node_GetParamCount(node);

    if (type != ONLY_DEF)
        fprintf(buffer->output, "LABEL %s%d_%d\n", WHILE_LABEL, id, 0);

    Node_GenerateConditionLabel(buffer, (Node*)Node_GetData(node), destroy, id, 0, END_LOOP, type);  // +1 as we want to jump to end and first label is while

    for (int i = 0; i < Vector_Size(sons); i++)
    {
        Node *current = (Node*)Vector_GetElement(sons, i);
        Vector_Destroy(Node_PostOrder(current, destroy, buffer, 0, type));
    }

    if (type != ONLY_DEF)
    {
        fprintf(buffer->output, "JUMP %s%d_%d\n", WHILE_LABEL, id, 0);
        fprintf(buffer->output, "LABEL %s%d_%d\n", END_LOOP, id, 0);
    }
}

void Node_GenerateFor(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *sons = Node_GetSons(node);
    int id = Node_GetParamCount(node);

    // Initialize for loop variable
    if (type != EXCEPT_DEF)
        fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(node));
    
    Vector *initialize = Node_PostOrder(Vector_GetElement(sons, 0), destroy, buffer, 1, type);

    if (type != ONLY_DEF)
    {
        // Assign initial value for for loop variable
        fprintf(buffer->output, "MOVE LF@%s%d LF@%s%d\n", ELEMENT(node), TMP(*((int*)Vector_GetElement(initialize, 0))));
        fprintf(buffer->output, "LABEL %s%d_%d\n", FOR_LABEL, id, 0);

    }

    Vector_Clear(initialize);
    initialize = Node_PostOrder(Vector_GetElement(sons, 1), destroy, buffer, 1, type);
    
    if (type == ONLY_DEF)
        DEF_TMP(buffer->tmp_offset++);
    else if (type == ALL)
        DEF_TMP(buffer->tmp_offset);

    if (type != ONLY_DEF)
    {
        // compare for loop variable with statement
        fprintf(buffer->output, "EQ LF@%s%d LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), ELEMENT(node), TMP(*((int*)Vector_GetElement(initialize, 0))));
        fprintf(buffer->output, "JUMPIFEQ %s%d_%d LF@%s%d bool@true\n", END_LOOP, id, 0, TMP(buffer->tmp_offset++));
    }

    Vector_Clear(initialize);

    for (int i = 3; i < Vector_Size(sons); i++)
    {
        Node *current = (Node*)Vector_GetElement(sons, i);
        Vector_Destroy(Node_PostOrder(current, destroy, buffer, 0, type));
    }

    initialize = Node_PostOrder(Vector_GetElement(sons, 2), destroy, buffer, 1, type);

    // increment for loop variable
    if (type != ONLY_DEF)
    {
        fprintf(buffer->output, "ADD LF@%s%d LF@%s%d LF@%s%d\n", ELEMENT(node), ELEMENT(node), TMP(*((int*)Vector_GetElement(initialize, 0))));
        fprintf(buffer->output, "JUMP %s%d_%d\n", FOR_LABEL, id, 0);
        fprintf(buffer->output, "LABEL %s%d_%d\n", END_LOOP, id, 0);
    }

    Vector_Destroy(initialize);
}

void Node_GenerateRepeat(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{
    Vector *sons = Node_GetSons(node);
    int id = Node_GetParamCount(node);

    if (type != ONLY_DEF)
        fprintf(buffer->output, "LABEL %s%d_%d\n", REPEAT_LABEL, id, 0);

    for (int i = 0; i < Vector_Size(sons); i++)
    {
        Node *current = (Node*)Vector_GetElement(sons, i);
        Vector_Destroy(Node_PostOrder(current, destroy, buffer, 0, type));
    }

    Vector *condition = Node_PostOrder(Node_GetData(node), destroy, buffer, 1, type);

    if (type != ONLY_DEF)
    {
        fprintf(buffer->output, "JUMPIFNEQ %s%d_%d LF@%s%d bool@true\n", REPEAT_LABEL, id, 0, TMP(*((int*)Vector_GetElement(condition, 0))));
        fprintf(buffer->output, "LABEL %s%d_%d\n", END_LOOP, id, 0);
    }

    Vector_Destroy(condition);
}

Vector *Node_GenerateSons(Node *node, Buffers *buffer, bool destroy, RecursionType type)
{    
    Vector *sons = Vector_Init(Number_Destroy);
    if (sons == NULL)
        return NULL;

    for (int i = 0; i < Vector_Size(Node_GetSons(node)); i++)
    {
        Vector *returned_values = NULL;

        // This condition is probably unnecessary and we can just use the first expression as Node_GetParamCount is by default 0
        // we need from last function as parameter maybe more returns
        if (Node_GetType(node) == NODE_FUNCTION && i == Vector_Size(Node_GetSons(node)) - 1)
            // 1 + Node_GetParamCount(node) as we want at least always one parameter and maybe more
            returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetSons(node), i), destroy, buffer, 1 + Node_GetParamCount(node), type);
        else
            returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetSons(node), i), destroy, buffer, 1, type);
        if (returned_values == NULL)
            break;
        
        for (int j = 0; j < Vector_Size(returned_values); j++)
            Vector_PushBack(sons, Number_Init(*((int*)Vector_GetElement(returned_values, j))));

        Vector_Destroy(returned_values);
    }

    return sons;
}

Vector* Node_PostOrder(Node *node, bool destroy, Buffers *buffer, int expected_amount, RecursionType type)
{
    Vector *return_values = Vector_Init(Number_Destroy);
    if (return_values == NULL)
        return NULL;

    Vector *sons = NULL;
    if (Node_GetType(node) == NODE_FUNCTION || Node_IsOperation(node))
        sons = Node_GenerateSons(node, buffer, destroy, type);

    switch (Node_GetType(node))
    {
    case NODE_OPERATION:
    
        if (Node_IsBinaryOperator(Node_GetOperation(node)))
        {
            bool compare_with_not = false;

            if (type == ONLY_DEF)
                DEF_TMP(buffer->tmp_offset++);
            else if (type == ALL)
                DEF_TMP(buffer->tmp_offset);
            
            if (type != ONLY_DEF)
            {
                switch (Node_GetOperation(node))
                {
                case P_MUL:
                    fprintf(buffer->output, "MUL");
                    break;
                
                case P_DIV:
                    fprintf(buffer->output, "DIV");
                    break;
                
                case P_INT_DIV:
                    fprintf(buffer->output, "IDIV");
                    break;

                case P_PLUS:
                    fprintf(buffer->output, "ADD");
                    break;

                case P_MINUS:
                    fprintf(buffer->output, "SUB");
                    break;

                case P_CONCAT:
                    fprintf(buffer->output, "CONCAT");
                    break;

                case P_GRT:
                    fprintf(buffer->output, "GT");
                    break;
                
                case P_LESS:
                    fprintf(buffer->output, "LT");
                    break;

                case P_GEQ: // we need to compare it LT and then NOT
                    fprintf(buffer->output, "LT");
                    compare_with_not = true;
                    break;

                case P_LEQ: // we need to compare with GT and then NOT
                    fprintf(buffer->output, "GT");
                    compare_with_not = true;
                    break;

                case P_EQ:
                    fprintf(buffer->output, "EQ");
                    break;

                case P_NEQ: // we need to compare with EQ and then NOT
                    fprintf(buffer->output, "EQ");
                    compare_with_not = true;
                    break;

                case P_AND:
                    fprintf(buffer->output, "AND");
                    break;

                case P_OR:
                    fprintf(buffer->output, "OR");
                    break;
                
                default:
                    break;
                }
            }
            else
            {
                switch (Node_GetOperation(node))
                {
                case P_GEQ:
                case P_LEQ:
                case P_NEQ:
                    compare_with_not = true;
                    break;
                
                default:
                    break;
                }
            }
            

            // DEF_TMP is above
            if (type != ONLY_DEF)
                fprintf(buffer->output, " LF@%s%d LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset++), TMP(*((int*)Vector_GetElement(sons, 0))), TMP(*((int*)Vector_GetElement(sons, 1))));

            if (compare_with_not)
            {
                if (type == ONLY_DEF)
                    DEF_TMP(buffer->tmp_offset++);
                else if (type == ALL)
                    DEF_TMP(buffer->tmp_offset + 1);
                    
                if (type != ONLY_DEF)
                {
                    fprintf(buffer->output, "NOT LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), TMP(buffer->tmp_offset - 1));
                    buffer->tmp_offset++;
                }
            }

            Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));
        }
        else    // Unary
        {

            if (type != ONLY_DEF)
            {
                switch (Node_GetOperation(node))
                {

                case P_LEN:
                    fprintf(buffer->output, "STRLEN");
                    break;

                case P_NOT:
                    fprintf(buffer->output, "NOT");
                    break;

                default:
                    break;
                }
            }

            if (type == ONLY_DEF)
                DEF_TMP(buffer->tmp_offset++);
            else if (type == ALL)
                DEF_TMP(buffer->tmp_offset);

            if (type != ONLY_DEF)
            {
                fprintf(buffer->output, " LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset++), TMP(*((int*)Vector_GetElement(sons, 0))));
                Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));
            }
        }

        Vector_Destroy(sons);
        break;

    case NODE_ID:
        if (type == ONLY_DEF)
            DEF_TMP(buffer->tmp_offset++);
        else if (type == ALL)
            DEF_TMP(buffer->tmp_offset);

        if (type != ONLY_DEF)
        {
            fprintf(buffer->output, "MOVE LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset++), ELEMENT(node));
            Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));
        }

        break;

    case NODE_VALUE:
        if (type == ONLY_DEF)
            DEF_TMP(buffer->tmp_offset++);
        else if (type == ALL)
            DEF_TMP(buffer->tmp_offset);

        if (type != ONLY_DEF)
        {
            fprintf(buffer->output, "MOVE LF@%s%d ", TMP(buffer->tmp_offset++));
            switch (Node_GetSemantic(node))
            {
            case SEMANTIC_BOOLEAN:
                fprintf(buffer->output, "bool@%s\n", (const char*)Node_GetData(node));
                break;

            case SEMANTIC_INTEGER:
                fprintf(buffer->output, "int@%d\n", atoi((const char*)Node_GetData(node)));
                break;
                
            case SEMANTIC_STRING:
                fprintf(buffer->output, "string@");
                char* a = (char*)Node_GetData(node);
                while (*a != '\0')
                {
                    if ((int)(*a) <= 32 || (int)(*a) == 35)
                        fprintf(buffer->output, "\\0%d", (int)(*a));
                    else if ((int)*a == 92)
                    {
                        if ((int)*(a+1) >= '0' && (int)*(a+1) <= '2')
                            fprintf(buffer->output, "\\");
                        else
                            fprintf(buffer->output, "\\092");
                    }
                    else
                        fprintf(buffer->output, "%c", (*a));

                    a++;
                }
                fprintf(buffer->output,"\n");
                break;
            
            case SEMANTIC_NUMBER:
                fprintf(buffer->output, "float@%a\n", strtod((const char*)Node_GetData(node), NULL));
                break;

            default:
                WARNING("other NODE_VALUE semantic type");
                break;
            }
        }

        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));

        break;

    case NODE_FUNCTION:

        for (int i = Vector_Size(sons) - 1; i >= 0; i--)
            if (type != ONLY_DEF)
                fprintf(buffer->output, "PUSHS LF@%s%d\n", TMP(*((int*)Vector_GetElement(sons, i))));

        // because of convention we have decided to push number of variadic parameters in function write
        if (strcmp(Element_GetKey((Element*)Node_GetData(node)), "write\0") == 0)
            if (type != ONLY_DEF)
                fprintf(buffer->output, "PUSHS int@%d\n", Vector_Size(sons));

        if (type != ONLY_DEF)
            fprintf(buffer->output, "CALL %s%d\n", Element_GetKey((Element*)Node_GetData(node)), Element_GetID((Element*)Node_GetData(node)));

        Vector *returns = Vector_Init(Number_Destroy);
        if (returns == NULL)
            return NULL;

        // get all returns from function in returns Vector
        for (int i = 0; i < Vector_Size(Node_GetReturns(node)); i++)
        {
            Vector *returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetReturns(node), i), destroy, buffer, 1, type);
            if (returned_values == NULL)
                break;
            
            for (int j = 0; j < Vector_Size(returned_values); j++)
                Vector_PushBack(returns, Number_Init(*((int*)Vector_GetElement(returned_values, j))));

            Vector_Destroy(returned_values);
        }

        // only first few parameters return
        for (int i = 0; i < expected_amount; i++)
            Vector_PushBack(return_values, Number_Init(*((int*)Vector_GetElement(returns, i))));
        
        Vector_Destroy(returns);
        Vector_Destroy(sons);

        break;

    case NODE_POP:
        if (type == ONLY_DEF)
            DEF_TMP(buffer->tmp_offset++);
        else if (type == ALL)
            DEF_TMP(buffer->tmp_offset);
        if (type != ONLY_DEF)
            fprintf(buffer->output, "POPS LF@%s%d\n", TMP(buffer->tmp_offset++));

        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));
        break;
    
    case NODE_NIL:
        if (type == ONLY_DEF)
            DEF_TMP(buffer->tmp_offset++);
        else if (type == ALL)
            DEF_TMP(buffer->tmp_offset);

        if (type != ONLY_DEF)
            fprintf(buffer->output, "MOVE LF@%s%d nil@nil\n", TMP(buffer->tmp_offset++));

        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset - 1));
        break;

    case NODE_ASSIGN:
        Node_GenerateAssign(node, buffer, destroy, type);
        break;

    case NODE_RETURN:
        Node_GenerateReturn(node, buffer, destroy, type);
        break;

    case NODE_DECLARE:
        if (type != EXCEPT_DEF)
            fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(node));
        break;

    case NODE_FUNCTION_POP:
        if (type != EXCEPT_DEF)
            fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(node));

        if (type != ONLY_DEF)
            fprintf(buffer->output, "POPS LF@%s%d\n", ELEMENT(node));

        break;

    case NODE_BREAK:
        if (type != ONLY_DEF)
            fprintf(buffer->output, "JUMP %s%d_%d\n", END_LOOP, Node_GetParamCount(node), 0);
        break;

    case NODE_IF:
        Node_GenerateIf(node, buffer, destroy, type);
        break;

    case NODE_WHILE:
        Node_GenerateWhile(node, buffer, destroy, type);
        break;

    case NODE_FOR:
        Node_GenerateFor(node, buffer, destroy, type);
        break;

    case NODE_REPEAT:
        Node_GenerateRepeat(node, buffer, destroy, type);
        break;

    case NODE_FUNCTION_DEF:
        fprintf(buffer->output, "LABEL %s%d\n", Element_GetKey(buffer->current_function), Element_GetID(buffer->current_function));
        fprintf(buffer->output, "CREATEFRAME\n");
        fprintf(buffer->output, "PUSHFRAME\n");

        sons = Node_GetSons(node);
        for (int i = 0; i < Vector_Size(sons); i++)
        {
            int tmp = buffer->tmp_offset;
            Vector_Destroy(Node_PostOrder((Node*)Vector_GetElement(sons, i), false, buffer, 0, ONLY_DEF));
            buffer->tmp_offset = tmp;
            Vector_Destroy(Node_PostOrder((Node*)Vector_GetElement(sons, i), true, buffer, 0, EXCEPT_DEF));
        }

        fprintf(buffer->output, "POPFRAME\n");
        fprintf(buffer->output, "RETURN\n");

        break;

    default:
        break;
    }

    if (destroy)
        Node_Destroy(node, false);
    
    return return_values;
}