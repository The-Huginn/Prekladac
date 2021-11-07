#ifndef __NONTERMINAL_H__
#define __NONTERMINAL_H__

typedef enum NonTerminal { NT_ASSIGN, NT_BINARY_OPERATOR, NT_CALLED_PARAMETERS, NT_CONDITION, NT_CONDITION_BRANCH, NT_DATATYPE, NT_DATATYPES, NT_DATATYPES_, NT_DECLARE, NT_DECLARE_ASSIGN, NT_EXPRESSION, NT_EXPRESSION_, NT_EXPRESSION_2, NT_EXPRESSION_3, NT_EXPRESSION_3_, NT_FUNCTION_CALL, NT_FUNCTION_DECLARE, NT_FUNCTION_DEFINE, NT_GLOBAL_SCOPE, NT_ID, NT_ID_, NT_IF, NT_LVALUE, NT_LVALUES, NT_LVALUES_, NT_PARAMETER, NT_PARAMETER_DEFINED, NT_PARAMETER_NAME, NT_PARAMETERS, NT_PARAMETERS_, NT_PARAMETERS_DEFINED, NT_PARAMETERS_DEFINED_, NT_PROGRAM, NT_RETURN, NT_RETURN_, NT_RETURNING, NT_RVALUE, NT_RVALUES, NT_RVALUES_, NT_SCOPE, NT_SCOPE_RETURN, NT_SCOPE_STATEMENTS, NT_STATEMENT, NT_STATEMENTS, NT_STATEMENTS_, NT_UNARY_OPERATOR, NT_WHILE} NonTerminal;

#endif // !__NONTERMINAL_H__
