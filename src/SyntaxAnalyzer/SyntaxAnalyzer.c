/**
 * @file SyntaxAnalyzer.c
 * @brief This file implements interface of SyntaxAnalyzer.h, contains custom functions
 * @author Rastislav Budinsky
 */
#include "SyntaxAnalyzer.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "LLTable.h"
#include "PrecedenceTable.h"
#include "../Utils/stack.h"
#include "../Utils/list.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"


#include <stdlib.h>

#define DEBUG_SYNTAX

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

int ApplyPrecedenceRule(LList* list)
{
    bool indexes[] = { 1, 1, 1, 1, 1, 1, 1};
    int index = 0;
    bool succ = false;

    PrecedenceItem* top = List_GetFirst(list);

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
        top = List_GetFirst(list);
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

/**
 * @brief 
 * @param input 
 * @param output 
 * @param error_output 
 * @return -1 upon success
*/
int BottomToTop(FILE* input, FILE* output, FILE* error_output)
{
    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "------- Entering Precedence table! -------\n");
    #endif

    LList* bottomToTopList = List_Init((void (*) (void*))PrecedenceItem_Destroy, (const bool (*)(void*,const void*))PrecedenceItem_IsTerminal);
    List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_$, '\0'));

    Token* token = getToken(input);
    int ret = -1;

    PrecedenceItem* top = List_GetData(bottomToTopList, NULL);
    while ((Token_ToPrecedenceItemType(token) != P_$ || PrecedenceItem_GetType(top) != P_$) && ret == -1)
    {
        PrecedenceItemType token_type = Token_ToPrecedenceItemType(token);

                                                                            // P_FUNCTION is not valid token, so we want to skip this as it is in the order the table
        char action = precedenceTable[PrecedenceItem_GetType(top) - P_LEFT][ (token_type - P_LEFT) - (token_type > P_FUNCTION ? 1 : 0)];

        switch (action)
        {
        case '<':
            // special case for $E stack and i as token we want to break as that id does not belong to us
            if (PrecedenceItem_GetType(List_GetFirst(bottomToTopList)) == P_E && token_type == P_I)
                goto end;

            top->character = '<';

            // pushing comma on stack is done in beta
            if (token_type != P_COMMA)
                List_AddFirst(bottomToTopList, PrecedenceItem_Init(token_type, '\0'));

            free(token);
            token = getToken(input);
            break;
        case '>':
            if (ApplyPrecedenceRule(bottomToTopList) == -1)
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
            ApplyPrecedenceRule(bottomToTopList);
            
            free(token);
            token = getToken(input);

            break;
        case 'b':
            // change top terminal's char to <
            PrecedenceItem_SetChar(List_GetData(bottomToTopList, NULL), '<');
            // push , on Top
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_COMMA, '\0'));

            returnToken(token);

            ret = BottomToTop(input, output, error_output);
            // if we dont get P_E as result we will break either way from the cycle as ret == -1 condition is not met
            List_AddFirst(bottomToTopList, PrecedenceItem_Init(P_E, '\0'));

            token = getToken(token);
            break;
        case 'X':

            while (!List_IsEmpty(bottomToTopList))
            {
                if (PrecedenceItem_GetType(List_GetFirst(bottomToTopList)) == P_E &&
                    PrecedenceItem_GetType(List_GetData(bottomToTopList, NULL)) == P_$)
                goto end;

                ApplyPrecedenceRule(bottomToTopList);
            }

            ret = 2;
            // every time we want to leave
            goto end;
            break;
        }
        top = List_GetData(bottomToTopList, NULL);
    }

end:
    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "------- Leaving Precedence table! -------\n");
    #endif

    returnToken(token);
    return ret;
}

/**
 * @brief Top to Bottom syntax method implemented as predictive parsing
 * @param input source code stream
 * @param output assembly output stream
 * @param error_output stream for everything else
 * @param clear whether this function was called only to clear stack
 * @return -1 upon work in progress
 *          1-9 & 99 upon an error
 *          0 upon successful finish
 */
int TopToBottom(FILE *input, FILE *output, FILE *error_output, bool clear)
{
    static Stack *topToBottomStack = NULL;
    static Token *token;

    if (clear)
    {
        Stack_Destroy(topToBottomStack);
        topToBottomStack = NULL;

        // maybe Token_Destroy in the future
        if (token != NULL)
            free(token);
        token = NULL;

        return 0;
    }

    // token and stack are NULLs
    if (topToBottomStack == NULL)
    {
        topToBottomStack = Stack_Init((void (*)(void*)) Symbol_Destroy);
        if (topToBottomStack == NULL)
            return -1;

        Symbol *a = Symbol_Init(true, $);
        Stack_Push(topToBottomStack, a);

        // NT_PROGRAM is starting NonTerminal
        a = Symbol_Init(false, NT_PROGRAM);
        Stack_Push(topToBottomStack, a);

        token = getToken(input);
    }

    Symbol *top = (Symbol *) Stack_Top(topToBottomStack);

    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "token [%d], stack top [%d, %d]\n", Token_getType(token), Symbol_IsTerminal(top), Symbol_GetValue(top));
    #endif
    
    if (Token_getType(token) == ERROR)
    {
        free(token);
        token = NULL;
        return 1;
    }

    if (Symbol_IsTerminal(top) && (Terminal)Symbol_GetValue(top) == $)
    {
        if (Token_getType(token) == $)
        {
            free(token);
            token = NULL;
            return 0;
        }
        else
        {
            free(token);
            token = NULL;
            return 2;
        }
    }
    else if (Symbol_IsTerminal(top))
    {
        if (Token_getType(token) == (Terminal) Symbol_GetValue(top))
        {
            Stack_Pop(topToBottomStack);
            free(token);
            token = getToken(input);
        }
        else
        {
            free(token);
            token = NULL;
            return 2;
        }
    }
    else
    {
        if (Symbol_GetValue(top) == NT_EXPRESSION)
        {
            returnToken(token);
            Stack_Pop(topToBottomStack);
            int ret = BottomToTop(input, output, error_output);
            token = getToken(input);
            return ret;
        }

        // -1 in index to skip ERROR enum
        int index = LLTable[Symbol_GetValue(top)][Token_getType(token) - 1];
        
        if (index == -1)
        {
            free(token);
            token = NULL;
            return 2;
        }
        
        Stack_Pop(topToBottomStack);
        for (int i = Rule_GetSize(&(rules[index])) - 1; i >= 0; i--)
        {
            Symbol *next = Symbol_Init(Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));

            if (Symbol_IsTerminal(next) && Symbol_GetValue(next) == EPSILON)
            {
                Symbol_Destroy(next);
                break;
            }

            Stack_Push(topToBottomStack, next);
        }

        #ifdef DEBUG_SYNTAX
            fprintf(error_output, "Apply new rule: %d\n", index);

            if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), 0)))
                fprintf(error_output, "<%d> -> %d", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            else
                fprintf(error_output, "<%d> -> <%d>", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            for (int i = 1 ; i < Rule_GetSize(&(rules[index])); i++)
            {
                if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)))
                    fprintf(error_output, ", %d", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
                else
                    fprintf(error_output, ", <%d>", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
            }
            fprintf(error_output, "\n");
        #endif
    }

    return -1;
}

int parseAndGenerate(FILE *input, FILE *output, FILE *error_output)
{
    int return_value = 0;
    while (true)
    {
        int a = TopToBottom(input, output, error_output, false);

        // success
        if (a == 0)
            break;

        // error
        if (a > 0)
        {
            // only the first error is propagated as output value
            if (return_value == 0)
                return_value = a;
            break;
        }
    }

    TopToBottom(NULL, NULL, NULL, true);
    LexicalDestroy();

    // might move to main later
    fprintf(error_output, "Exit code: %d\n", return_value);

    return return_value;
}