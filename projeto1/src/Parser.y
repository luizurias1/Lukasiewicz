%{
#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "TreeNode.h"
#include <iostream>

SymbolTable SYMBOL_TABLE;
SyntaxTree* SYNTAX_TREE;
extern int yylex();
extern void yyerror(const char* s, ...);
%}

%define parse.trace

%code requires {
    class TreeNode;
    class SyntaxTree;
}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    TreeNode* node;
    SyntaxTree* syntaxTree;
    int integer;
    char *id;
    char  *real;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <id> T_ID
%token <real> T_FLOAT
%token T_PLUS T_TIMES T_MINUS T_DIVIDE T_OPEN_PAR T_CLOSING_PAR T_NL
%token T_ATT T T_TYPE_INT T_COMMA T_TYPE_FLOAT T_TYPE_BOOL T_TRUE T_FALSE
%token T_EQUAL T_NOT_EQUAL T_GREATER T_LOWER T_GREATER_EQUAL T_LOWER_EQUAL
%token T_AND T_OR T_NOT

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <syntaxTree> lines program
%type <node> line expr declar type op_relation

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */
%left T_PLUS T_MINUS
%left T_TIMES
%left T_DIVIDE
%nonassoc U_MINUS error

/* Starting rule
 */
%start program

%%

program:
    lines { SYNTAX_TREE = $1; std::cout <<"one"<< std::endl;}
    ;

lines:
    line { $$ = new SyntaxTree(); if($1 != NULL) $$->insertLine($1); }
    | line lines { $$ = $2; if($1 != NULL) $2->insertLine($1); }
    ;

line:
    T_NL { $$ = NULL; }
    | error T_NL { yyerrok; $$ = NULL; }
    | T_TYPE_INT declar { $$ = new VariableDeclaration(VariableDeclaration::INTEGER, $2);}
    | T_TYPE_FLOAT declar
    | T_TYPE_BOOL declar
    | T_ID T_ATT expr { $$ = new BinaryOperation(SYMBOL_TABLE.assignVariable($1),
                                                    BinaryOperation::ASSIGN, $3); }
    ;

expr:
    T_INT { $$ = new Integer($1); }
    | T_FLOAT
    | T_TRUE
    | T_FALSE
    | T_ID { $$ = SYMBOL_TABLE.useVariable($1); }
    | T_MINUS expr %prec U_MINUS { $$ = new UnaryOperation(UnaryOperation::MINUS, $2); }
    | expr T_PLUS expr { $$ = new BinaryOperation($1, BinaryOperation::PLUS, $3); }
    | expr T_MINUS expr { $$ = new BinaryOperation($1, BinaryOperation::MINUS, $3); }
    | expr T_TIMES expr { $$ = new BinaryOperation($1, BinaryOperation::TIMES, $3); }
    | expr T_DIVIDE expr { $$ = new BinaryOperation($1, BinaryOperation::DIVIDE, $3); }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; }
    | T_NOT expr
    | expr op_relation expr
    ;

op_relation:
    T_GREATER
    | T_OR
    | T_LOWER
    | T_GREATER_EQUAL
    | T_LOWER_EQUAL
    | T_AND
    ;

declar:
    T_ID T_COMMA declar { $$ = new BinaryOperation(SYMBOL_TABLE.newVariable($1),
                                                    BinaryOperation::COMMA, $3);}
    | T_ID { $$ = SYMBOL_TABLE.newVariable($1);}
    | T_ID T_ATT type T_COMMA declar { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SYMBOL_TABLE.newAssignedVariable($1),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT type { $$ = new BinaryOperation(SYMBOL_TABLE.newAssignedVariable($1),
                                                    BinaryOperation::ASSIGN, $3); }
    ;

type:
    T_INT {$$ = new Integer($1);}
    |T_FLOAT
    |T_TRUE
    |T_FALSE

%%
