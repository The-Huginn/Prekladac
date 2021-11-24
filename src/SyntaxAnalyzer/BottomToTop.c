/**
 * @file BottomToTop.c
 * @author Rastislav Budinsky
 * @brief This file contains implementation of interface of BottomToTop parser
 */
#include "BottomToTop.h"
#include "SemanticActions.h"

#include "../Utils/list.h"

#include "../LexicalAnalyzer/LexicalAnalyzer.h"

#include <stdlib.h>
#include <string.h>

#define DEBUG_SYNTAX

typedef struct
{
    void *element;  // pointer to data in TS or malloced & should be freed
    bool inTS;              // if saved in TS or immediate operand
    SemanticType type;
}Identifier;

void BottomToTop_DestroyString(void *data)
{
    free(data);
}

bool EqualsRule(PrecedenceItem *rule, PrecedenceItem *top)
{
    if (PrecedenceItem_GetType(rule) == PrecedenceItem_GetType(top))
        return true;
    
    if (PrecedenceItem_GetType(rule) == PrecedenceItem_IsBinaryOperator(top) &&
        PrecedenceItem_IsBinaryOperator(top) != P_VOID)
        return true;

    if (PrecedenceItem_GetType(rule) == PrecedenceItem_IsUnaryOperator(top) &&
        PrecedenceItem_IsUnaryOperator(top) != P_VOID)
        return true;

    return false;
}

int ApplyPrecedenceRule(LList* list, Vector* expressions, Identifier *last_element)
{
    bool indexes[] = { 1, 1, 1, 1, 1, 1, 1};
    int index = 0;
    bool succ = false;

    PrecedenceItemType top_terminal = PrecedenceItem_GetType((PrecedenceItem*) List_GetData(list, NULL));
    PrecedenceItem* top = (PrecedenceItem*) List_GetFirst(list);

    int return_value = -1;
    int possible = PRECEDENCE_RULE_ARRAY_SIZE;
    while (possible > 0)
    {
        int i = 0;
        // check all rules's first rule, second etc
        for (; i < PRECEDENCE_RULE_ARRAY_SIZE; i++)
        {
            if (top == NULL)
                return 2;
            if (indexes[i])
            {
                if (!EqualsRule(&(precedence_rules[i].right_side[index]), top))
                {
                    indexes[i] = 0;
                    possible--;
                }
                else if (index == precedence_rules[i].size - 1)
                {
                    succ = true;
                    break;
                }
            }
        }


        index++;
        List_RemoveFirst(list);
        top = (PrecedenceItem*)List_GetFirst(list);
        if (succ)
        {
            #ifdef DEBUG_SYNTAX
                fprintf(stderr, "Applying rule:\n");
            #endif

            if (PrecedenceItem_GetChar(top) == '<')
            {
                #ifdef DEBUG_SYNTAX
                    fprintf(stderr, "Rule number: %d\n", i);
                #endif

                Node *node = NULL;
                NodeType nodeType = last_element->inTS == true ? NODE_ID : NODE_VALUE;  // This should be in case 0 but doesnt work idk why
                switch (i)
                {
                    case 0: // E->I
                        node = Node_Init(nodeType, last_element->element, last_element->type, (void (*)(void*))(nodeType == NODE_VALUE ? BottomToTop_DestroyString : NULL), P_I);
                        if (node == NULL)
                            return 99;
                        break;
                    case 1: // E->VOID
                        node = Node_Init(NODE_VOID, NULL, SEMANTIC_VOID, NULL, P_VOID);
                        if (node == NULL)
                            return 99;
                        break;
                    case 2: // E->UNARY E
                        node = Node_Init(NODE_OPERATION, NULL, SEMANTIC_VOID, NULL, top_terminal);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);

                        return_value = AbstractSemanticTree_UnaryOperator(node);
                        if (return_value != -1)
                            return return_value;

                        break;
                    case 3: // E->E BINARY E
                        node = Node_Init(NODE_OPERATION, NULL, SEMANTIC_VOID, NULL, top_terminal);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);

                        return_value = AbstractSemanticTree_BinaryOperator(node);
                        if (return_value != -1)
                            return return_value;

                        break;
                    case 4: // E->( E )
                        // Nothing to do
                        break;
                    case 5: // E->i ( E )
                        node = Node_Init(NODE_FUNCTION, last_element->element, SEMANTIC_VOID, NULL, P_FUNCTION);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);

                        
                        if (AbstractSemanticTree_UpdateFunctionCalls(node) == false)
                        {
                            Node_Destroy(node, true);
                            return 99;
                        }
                        return_value = AbstractSemanticTree_VerifyFunctionCall(node);
                        if (return_value != -1)
                            return return_value;

                        break;
                    case 6: // E->E , E
                        node = Node_Init(NODE_OPERATION, NULL, SEMANTIC_VOID, NULL, top_terminal);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        Node_AppendSon(node, (Node*)Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        break;
                    
                    default:
                        break;
                }
                if (node != NULL)
                    Vector_PushBack(expressions, node);

                top->character = '\0';
                List_AddFirst(list, PrecedenceItem_Init(P_E, '\0'));
                return -1;
            }
            else
                return 2;
        }
    }

    return 2;
}

bool PrecedenceItem_IsTerminal(PrecedenceItem* a, PrecedenceItem* b)
{
    return PrecedenceItem_GetType(a) != P_E;
}

int BottomToTop(FILE* input, FILE* output, FILE* error_output, Node** expression, Symtable *symtable)
{
    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "------- Entering Precedence table! -------\n");
    #endif

    LList* bottomToTopList = List_Init((void (*) (void*))PrecedenceItem_Destroy, (const bool (*)(void*,const void*))PrecedenceItem_IsTerminal);
    if (bottomToTopList == NULL)
        return 99;

    List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_$, '\0'));

    Vector *expressions = Vector_Init(NULL);
    if (expressions == NULL)
    {
        List_Destroy(bottomToTopList);
        return 99;
    }

    
    Token* token = getToken(input);
    if (token == NULL)
    {
        List_Destroy(bottomToTopList);
        Vector_Destroy(expressions);
        return 99;
    }

    Identifier last_element = {.inTS = false, .element = NULL};

    int ret = -1;

    PrecedenceItem* top = (PrecedenceItem*)List_GetData(bottomToTopList, NULL);
    while ((Token_ToPrecedenceItemType(token) != P_$ || PrecedenceItem_GetType(top) != P_$) && ret == -1)
    {
        PrecedenceItemType token_type = Token_ToPrecedenceItemType(token);

                                                                            // P_FUNCTION is not valid token, so we want to skip this as it is in the order the table
        char action = precedenceTable[PrecedenceItem_GetType(top) - P_LEFT][ (token_type - P_LEFT) - (token_type > P_FUNCTION ? 1 : 0)];

        switch (action)
        {
        case '<':
            // special case for $E stack and i as token we want to break as that id does not belong to us
            if (PrecedenceItem_GetType((PrecedenceItem*)List_GetFirst(bottomToTopList)) == P_E && token_type == P_I)
                goto end;

            top->character = '<';

            // we want to save the Element we found and check, if it is defined
            if (token_type == P_I)
            {
                if (Token_getType(token) == T_ID)
                {
                    last_element.element = Symtable_GetElement(symtable, Token_getData(token));
                    if (last_element.element == NULL)
                    {
                        ret = 3;
                        break;
                    }
                    
                    // using undefined variable
                    if (Element_IsVariable(last_element.element) && !Element_IsDefined(last_element.element))
                        ret = 3;

                    last_element.inTS = true;
                    last_element.type = Element_GetSemantic(last_element.element);
                }
                else
                {
                    // we notify that P_I is not in symbol table and should be as immediate operator
                    last_element.inTS = false;
                    last_element.element = strdup(Token_getData(token));
                    if (last_element.element == NULL)
                        ret = 99;
                    switch (Token_getType(token))
                    {
                    case T_STRING:
                        last_element.type = SEMANTIC_STRING;
                        break;
                    case T_NUMBER:
                        last_element.type = SEMANTIC_NUMBER;
                        break;
                    case T_INTEGER:
                        last_element.type = SEMANTIC_INTEGER;
                        break;
                    case K_TRUE:
                    case K_FALSE:
                        last_element.type = SEMANTIC_BOOLEAN;
                        break;
                    case K_NIL:
                        last_element.type = SEMANTIC_NIL;
                        break;

                    default:
                        break;
                    }
                }
            }

            // pushing comma on stack is done in beta
            if (token_type != P_COMMA)
                List_AddFirst(bottomToTopList, PrecedenceItem_Init(token_type, '\0'));

            Token_Destroy(token);
            token = getToken(input);
            break;
        case '>':
            ret = ApplyPrecedenceRule(bottomToTopList, expressions, &last_element);
            break;
        case '=':
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(token_type, '\0'));
            Token_Destroy(token);
            token = getToken(input);
            break;
        case 'a':
            // < for P_FUNCTION to reduce the next void expression
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_FUNCTION, '<'));   // This < might get skipped upon function call with 0 arguments
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_VOID, '\0'));
            ret = ApplyPrecedenceRule(bottomToTopList, expressions, &last_element);
            
            Token_Destroy(token);
            token = getToken(input);

            break;
        case 'b':
            // change top terminal's char to <
            PrecedenceItem_SetChar((PrecedenceItem*)List_GetData(bottomToTopList, NULL), '<');
            // push , on Top
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_COMMA, '\0'));

            returnToken(token);

            Node* returned_expression;
            ret = BottomToTop(input, output, error_output, &returned_expression, symtable);
            // TODO
            Vector_PushBack(expressions, returned_expression);


            // if we dont get P_E as result we will break either way from the cycle as ret == -1 condition is not met
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_E, '\0'));

            token = getToken(input);
            break;
        case 'X':

            while (!List_IsEmpty(bottomToTopList))
            {
                if (PrecedenceItem_GetType((PrecedenceItem*)List_GetFirst(bottomToTopList)) == P_E &&
                    PrecedenceItem_GetType((PrecedenceItem*)List_GetData(bottomToTopList, NULL)) == P_$)
                goto end;

                ret = ApplyPrecedenceRule(bottomToTopList, expressions, &last_element);

                if (ret != -1)
                    goto end;
            }

            ret = 2;
            // every time we want to leave
            goto end;
            break;
        }
        top = (PrecedenceItem*)List_GetData(bottomToTopList, NULL);
    }

end:
    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "------- Leaving Precedence table! -------\n");
    #endif

    (*expression) = (Node*)Vector_GetElement(expressions, 0);

    // if we dont have only one expression then there must have been an error
    if (Vector_Size(expressions) != 1)
        if (ret == -1)
            ret = 2;

    int size = 0;
    while (!List_IsEmpty(bottomToTopList))
    {
        size++;
        List_RemoveFirst(bottomToTopList);
    }

    if (size != 2)
        if (ret == -1)
            ret = 2;

    Vector_Destroy(expressions);
    List_Destroy(bottomToTopList);

    returnToken(token);
    return ret;
}