#ifndef __NONTERMINAL_H__
#define __NONTERMINAL_H__

typedef enum NonTerminal { ASSIGN, BINARY_OPERATOR, CONDITION, CONDITION_BRANCH, DATATYPE, DATATYPES, DATATYPES_, DECLARE, DECLARE_ASSIGN, EXPRESSION, EXPRESSION_, EXPRESSION_2, EXPRESSION_3, EXPRESSION_3_, FUNCTION_CALL, FUNCTION_DECLARE, FUNCTION_DEFINE, GLOBAL_SCOPE, ID, ID_, IF, LVALUE, LVALUES, LVALUES_, PARAMETER, PARAMETER_DEFINED, PARAMETER_NAME, PARAMETERS, PARAMETERS_, PARAMETERS_DEFINED, PARAMETERS_DEFINED_, PROGRAM, RETURN, RETURN_, RETURNING, RVALUE, RVALUES, RVALUES_, SCOPE, UNARY_OPERATOR, WHILE} NonTerminal;

#endif // !__NONTERMINAL_H__