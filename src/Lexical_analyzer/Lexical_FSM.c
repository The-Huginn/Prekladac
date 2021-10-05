/**
 * @file Lexical_FSM.c
 * @brief Implements Lexical_FSM.h for finding one valid lexeme, contains functions
 * for initialization of the struct from Lexical_FSM.h, adding char to this struct
 * and getting last char and others
 * @author Rastislav Budinsky
 */

#include <stdlib.h>
#include "Lexical_FSM.h"

/**
 * @brief Initializes LexicalOutput
 * @param output struct to be initialized
 */
void LexicalOutput_Init(LexicalOutput *output)
{
    output->state = START;
    output->pos = -1;
}

/**
 * @brief Desides whether we are in the final state or not
 * @param output struct holding current state
 * @return 1 if we are in final state, else 0
 */
int LexicalOutput_IsFinal(LexicalOutput *output)
{
    return output->state >= ERROR_STATE ? 1 : 0;
}

/**
 * @brief Adds char to the end of read lexeme
 * @param output struct holding currently read lexeme
 * @param data int value to be converted to char and appended
 */
void LexicalOutput_AddChar(LexicalOutput *output, int data)
{
    output->lexem[++(output->pos)] = (char) data;
}

/**
 * @brief returns lastly read character
 * @return the last character from lexeme
 */
char LexicalOutput_GetLast(LexicalOutput *output)
{
    return output->lexem[output->pos];
}

/**
 * @brief changes last character to '\0'
 */
void LexicalOutput_NullLast(LexicalOutput *output)
{
    output->lexem[output->pos] = '\0';
}

LexicalOutput *getLexeme(FILE *file)
{
    LexicalOutput *output = (LexicalOutput*) malloc(sizeof(LexicalOutput));
    LexicalOutput_Init(output);

    while(!LexicalOutput_IsFinal(output))
    {
        int c = fgetc(file);

        switch (output->state)
        {
        case START:
            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                output->state = LOAD_ID;
            else if (c >= '0' && c <= '9')
                output->state = LOAD_INT_INT_PART;
            else if (c == '"')
                output->state = LOAD_STRING;
            else if (c == '+')
                output->state = PLUS_FINALIZE;
            else if (c == '*')
                output->state = MUL_FINALIZE;
            else if (c == '/')
                output->state = DIV_STATE_INT_DIV_STATE;
            else if (c == ':')
                output->state = DEF_FINALIZE;
            else if (',')
                output->state = COMMA_FINALIZE;
            else if ('#')
                output->state = LEN_FINALIZE;
            else if ('(')
                output->state = LEFT_FINALIZE;
            else if (')')
                output->state = RIGHT_FINALIZE;
            else if ('<')
                output->state = LESS_STATE_LEQ_STATE;
            else if ('>')
                output->state = GRT_STATE_GEQ_STATE;
            else if ('.')
                output->state = CONCAT_STATE;
            else if ('=')
                output->state = ASS_STATE_EQ_STATE;
            else if ('~')
                output->state = NEQ_STATE;
            else if ('-')
                output->state = MINUS_EN_DASH;
            else if (c == ' ' || c == '\t' || '\n')
                output->state = START;
            else
                output->state = ERROR_STATE;
            break;
        case LOAD_ID:
            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
                output->state = LOAD_ID;
            else
                output->state = F_ID;
            break;
        case LOAD_INT_INT_PART:
            if (c >= '0' && c <= '9')
                output->state = LOAD_INT_INT_PART;
            else if (c == '.')
                output->state = POINT;
            else if (c == 'e' || c == 'E')
                output->state = EXPONENT;
            else
                output->state = F_INT;
            break;
        case POINT:
            if (c >= '0' && c <= '9')
                output->state = POINT;
            else if (c == 'e' || c == 'E')
                output->state = EXPONENT;
            else
                output->state = F_FLOAT;
            break;
        case EXPONENT:
            if (c >= '0' && c <= '9')
                output->state = EXP_PART;
            else if (c == '+' || c == '-')
                output->state = SIGN;
            else
                output->state = ERROR_STATE;
            break;
        case SIGN:
            if (c >= '0' && c <= '9')
                output->state = EXP_PART;
            else
                output->state = ERROR_STATE;
            break;
        case EXP_PART:
            if (c >= '0' && c <= '9')
                output->state = EXP_PART;
            else
                output->state = F_FLOAT;
            break;
        case LOAD_STRING:
            if ((c > 31 && c <= 255) && c != '"' && c != 92) // ascii(\) = 92
                output->state = LOAD_STRING;
            else if (c == '"')
                output->state = STRING_FINALIZE;
            else if (c == 92)
                output->state = ESC_SEQ;
            else
                output->state = ERROR_STATE;
            break;
        case STRING_FINALIZE:
            output->state = F_STRING;
            break;
        case ESC_SEQ:
            if (c == 92 || c == 'n' || c == 't' || c == '"')
                output->state = LOAD_STRING;
            else if (c == '2')
                output->state = LIMIT_2;
            else if (c == '0' || c == '1')
                output->state = NORMAL_1;
            else
                output->state = ERROR_STATE;
            break;
        case LIMIT_2:
            if (c == '5')
                output->state = LIMIT_5;
            else if (c >= '0' && c <= '4')
                output->state = NORMAL_2;
            else
                output->state = ERROR_STATE;
            break;
        case LIMIT_5:
            if (c >= '0' && c <= '5')
                output->state = LOAD_STRING;
            else
                output->state = ERROR_STATE;
            break;
        case NORMAL_1:
            if (c >= '0' && c <= '9')
                output->state = NORMAL_2;
            else
                output->state = ERROR_STATE;
            break;
        case NORMAL_2:
            if (c >= '0' && c <= '9')
                output->state = LOAD_STRING;
            else
                output->state = ERROR_STATE;
            break;
        case PLUS_FINALIZE:
            output->state = F_PLUS;
            break;
        case MUL_FINALIZE:
            output->state = F_MUL;
            break;
        case DIV_STATE_INT_DIV_STATE:
            if (c == '/')
                output->state = INT_DIV_FINALIZE;
            else
                output->state = F_DIV;
            break;
        case INT_DIV_FINALIZE:
            output->state = F_INT_DIV;
            break;
        case DEF_FINALIZE:
            output->state = F_DEF;
            break;
        case COMMA_FINALIZE:
            output->state = F_COMMA;
            break;
        case LEN_FINALIZE:
            output->state = F_LEN;
            break;
        case LEFT_FINALIZE:
            output->state = F_LEFT;
            break;
        case RIGHT_FINALIZE:
            output->state = F_RIGHT;
            break;
        case LESS_STATE_LEQ_STATE:
            if (c == '=')
                output->state = LEQ_FINALIZE;
            else
                output->state = F_LESS;
            break;
        case LEQ_FINALIZE:
            output->state = F_LEQ;
            break;
        case GRT_STATE_GEQ_STATE:
            if (c == '=')
                output->state = GEQ_FINALIZE;
            else
                output->state = F_GRT;
            break;
        case GEQ_FINALIZE:
            output->state = F_GEQ;
            break;
        case CONCAT_STATE:
            if (c == '.')
                output->state = CONCAT_FINALIZE;
            else
                output->state = ERROR_STATE;
            break;
        case CONCAT_FINALIZE:
            output->state = F_CONCAT;
            break;
        case ASS_STATE_EQ_STATE:
            if (c == '=')
                output->state = EQ_FINALIZE;
            else
                output->state = F_ASS;
            break;
        case EQ_FINALIZE:
            output->state = F_EQ;
            break;
        case NEQ_STATE:
            if (c == '=')
                output->state = NEQ_FINALIZE;
            else
                output->state = ERROR_STATE;
            break;
        case NEQ_FINALIZE:
            output->state = F_NEQ;
            break;
        case MINUS_EN_DASH:
            if (c == '-')
                output->state = EM_DASH;
            else
                output->state = F_MINUS;
            break;
        case EM_DASH:
            if ('[')
                output->state = EXP_BLOCK;
            else if (c == '\n')
                output->state = START;
            else
                output->state = LINE_COM;
            break;
        case LINE_COM:
            if (c == '\n')
                output->state = START;
            else
                output->state = LINE_COM;
            break;
        case EXP_BLOCK:
            if (c == '[')
                output->state = BLOCK_COM;
            else if ('\n')
                output->state = START;
            else
                output->state = LINE_COM;
            break;
        case BLOCK_COM:
            if (c == ']')
                output->state = END_BLOCK;
            else
                output->state = BLOCK_COM;
            break;
        case END_BLOCK:
            if (c == ']')
                output->state = START;
            else
                output->state = BLOCK_COM;
            break;
        
        default:
            break;
        }
    
        LexicalOutput_AddChar(output, c);
    }

    // return last read char to the beginning of the file
    if (ungetc(LexicalOutput_GetLast(output), file) == EOF)
    {
        // TO DO
    }

    LexicalOutput_NullLast(output);

    return output;
}