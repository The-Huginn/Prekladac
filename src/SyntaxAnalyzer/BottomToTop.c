/**
 * @file BottomToTop.c
 * @author Rastislav Budinsky
 * @brief This file contains implementation of interface of BottomToTop parser
 */
#include "BottomToTop.h"

#include "../Utils/list.h"

#include "../LexicalAnalyzer/LexicalAnalyzer.h"

#include <stdlib.h>

// #define DEBUG_SYNTAX

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

//              E
//            / , \
//          E      \
//        / , \     \
//       E     E     \
//     / , \    \     \
//    E    E    E     E
// Tree of arguments should look like this
// keep in mind this tree is mirrored as the rules are applied from the end. (Probably, works like this looks like)
bool AbstractSemanticTree_UpdateFunctionCalls(Node *root)
{
    Vector *new_sons = Vector_Init(NULL);
    if (new_sons == NULL)
        return false;

    Vector *children = Node_GetSons(root);
    Node *child = (Node*)Vector_GetElement(children, 0);

    while (Node_IsOperation(child) && Node_GetOperation(child) == P_COMMA)
    {
        children = Node_GetSons(child);

        // See function comments
        // This is why indexes 0 and 1 are switch compared to the picture above
        Vector_PushBack(new_sons, Vector_GetElement(children, 0));
        
        Node *next = (Node*)Vector_GetElement(children, 1);

        Node_Destroy(child, false);
        child = next;
    }

    Vector_PushBack(new_sons, child);

    // We want to ignore Void parameter
    Vector_PopBack(new_sons);

    // we clear current parameters
    Vector_Clear(Node_GetSons(root));


    // append new parameters in correct order
    while (!Vector_IsEmpty(new_sons))
    {
        if (Node_AppendSon(root, Vector_Back(new_sons)))
            Vector_PopBack(new_sons);
        else
        {
            Vector_Destroy(new_sons);
            return false;
        }
    }

    return true;
}

int ApplyPrecedenceRule(LList* list, Vector* expressions)
{
    bool indexes[] = { 1, 1, 1, 1, 1, 1, 1};
    int index = 0;
    bool succ = false;

    PrecedenceItemType top_terminal = PrecedenceItem_GetType((PrecedenceItem*) List_GetData(list, NULL));
    PrecedenceItem* top = (PrecedenceItem*) List_GetFirst(list);

    int possible = PRECEDENCE_RULE_ARRAY_SIZE;
    while (possible > 0)
    {
        int i = 0;
        // check all rules's first rule, second etc
        for (; i < PRECEDENCE_RULE_ARRAY_SIZE; i++)
        {
            if (top == NULL)
                return -1;
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
                switch (i)
                {
                    case 0: // E->I
                        // TODO set data to TS pointer or create malloc for constant in code
                        // use DataDtor for code constants to free them

                        // just temporary
                        node = Node_Init(NODE_VOID, NULL, SEMANTIC_VOID, NULL, P_VOID);
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
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        break;
                    case 3: // E->E BINARY E
                        node = Node_Init(NODE_OPERATION, NULL, SEMANTIC_VOID, NULL, top_terminal);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        break;
                    case 4: // E->( E )
                        // Nothing to do
                        break;
                    case 5: // E->i ( E )
                        node = Node_Init(NODE_FUNCTION, /*TS pointer*/ NULL, /*Semantic type of first return*/SEMANTIC_VOID, NULL, P_VOID);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);

                        
                        if (AbstractSemanticTree_UpdateFunctionCalls(node) == false)
                        {
                            Node_Destroy(node, true);
                            return 99;
                        }
                        break;
                    case 6: // E->E , E
                        node = Node_Init(NODE_OPERATION, NULL, SEMANTIC_VOID, NULL, top_terminal);
                        if (node == NULL)
                            return 99;
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        Node_AppendSon(node, Vector_Back(expressions));
                        Vector_PopBack(expressions);
                        break;
                    
                    default:
                        break;
                }
                if (node != NULL)
                    Vector_PushBack(expressions, node);

                top->character = '\0';
                List_AddFirst(list, PrecedenceItem_Init(P_E, '\0'));
                return i;
            }
            else
                return -1;
        }
    }

    return -1;
}

bool PrecedenceItem_IsTerminal(PrecedenceItem* a, PrecedenceItem* b)
{
    return PrecedenceItem_GetType(a) != P_E;
}

int BottomToTop(FILE* input, FILE* output, FILE* error_output, Node** expression)
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

            // pushing comma on stack is done in beta
            if (token_type != P_COMMA)
                List_AddFirst(bottomToTopList, PrecedenceItem_Init(token_type, '\0'));

            free(token);
            token = getToken(input);
            break;
        case '>':
            if (ApplyPrecedenceRule(bottomToTopList, expressions) == -1)
                ret = 2;
            break;
        case '=':
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(token_type, '\0'));
            free(token);
            token = getToken(input);
            break;
        case 'a':
            // < for P_FUNCTION to reduce the next void expression
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_FUNCTION, '<'));   // This < might get skipped upon function call with 0 arguments
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_VOID, '\0'));
            int rule_return = ApplyPrecedenceRule(bottomToTopList, expressions);
            if (rule_return == -1)
                ret = 2;
            else if (rule_return == 99)
                ret = 99;
            
            free(token);
            token = getToken(input);

            break;
        case 'b':
            // change top terminal's char to <
            PrecedenceItem_SetChar((PrecedenceItem*)List_GetData(bottomToTopList, NULL), '<');
            // push , on Top
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_COMMA, '\0'));

            returnToken(token);

            Node* returned_expression;
            ret = BottomToTop(input, output, error_output, &returned_expression);
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

                int rule_return = ApplyPrecedenceRule(bottomToTopList, expressions);
                if (rule_return == -1)
                {
                    ret = 2;
                    goto end;
                }
                else if (rule_return == 99)
                {
                    ret = 99;
                    goto end;
                }
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
        ret = 2;

    int size = 0;
    while (!List_IsEmpty(bottomToTopList))
    {
        size++;
        List_RemoveFirst(bottomToTopList);
    }

    if (size != 2)
        ret = 2;

    Vector_Destroy(expressions);
    List_Destroy(bottomToTopList);

    returnToken(token);
    return ret;
}