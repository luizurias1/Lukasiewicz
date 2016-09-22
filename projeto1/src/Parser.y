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
    class MyVector;

}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    TreeNode* node;
    SyntaxTree* syntaxTree;
    int integer;
    char *id;
    MyVector* linesIf;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <id> T_ID T_FLOAT
%token <id> T_TYPE_BOOL T_TYPE_FLOAT T_TYPE_INT
%token T_PLUS T_TIMES T_MINUS T_DIVIDE T_OPEN_PAR T_CLOSING_PAR T_NL
%token T_ATT T T_COMMA T_TRUE T_FALSE
%token T_EQUAL T_NOT_EQUAL T_GREATER T_LOWER T_GREATER_EQUAL T_LOWER_EQUAL
%token T_AND T_OR T_NOT
%token T_IF T_ELSE T_THEN T_OPEN_BRACE T_CLOSE_BRACE

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <syntaxTree> lines program
%type <node> line expr declar_int declar_float declar_bool type op_relation op_binary if
%type <linesIf> then else

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

%left T_PLUS T_MINUS
%left T_TIMES
%left T_DIVIDE
%nonassoc U_MINUS error

/*relational operators*/
%left T_OR T_AND
%left T_GREATER T_LOWER T_GREATER_EQUAL T_LOWER_EQUAL T_NOT_EQUAL

/*unart operator*/
%nonassoc T_NOT

/* Starting rule
 */
%start program

%%

// Programa
program:
    lines { SYNTAX_TREE = $1; std::cout << "passouGramatica" << std::endl; }
    ;

// linhas
lines:
    line { $$ = new SyntaxTree(); if($1 != NULL) $$->insertLine($1); std::cout << "newLine$ " << std::endl; }
    | line lines { $$ = $2; if($1 != NULL) $2->insertLine($1); std::cout << "newLine$ LINES " << std::endl; }
    ;

// Linha
line:
    T_NL { $$ = NULL; }
    | error T_NL { yyerrok; $$ = NULL; }
    | T_TYPE_INT declar_int { $$ = new VariableDeclaration(VariableDeclaration::INTEGER, $2); }
    | T_TYPE_FLOAT declar_float { $$ = new VariableDeclaration(VariableDeclaration::FLOAT, $2); }
    | T_TYPE_BOOL declar_bool { $$ = new VariableDeclaration(VariableDeclaration::BOOLEAN, $2); }
    | T_ID T_ATT expr { $$ = new BinaryOperation(SYMBOL_TABLE.assignVariable($1, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3); }
    | if
    ;

if:
    T_IF op_relation T_NL T_THEN T_OPEN_BRACE T_NL then T_CLOSE_BRACE else { $$ = new ConditionalOperation($2, $7->v, $7->v); std::cout << "IF THEN" << std::endl; }
    | T_IF T_OPEN_PAR op_relation T_CLOSING_PAR T_NL T_THEN T_OPEN_BRACE T_NL then T_CLOSE_BRACE else { $$ = new ConditionalOperation($3, $9->v, $9->v); std::cout << "IF THEN" << std::endl; }

    | T_IF T_ID T_NL T_THEN T_OPEN_BRACE T_NL then T_CLOSE_BRACE else { $$ = new ConditionalOperation(SYMBOL_TABLE.useVariable($2), $7->v, $7->v); std::cout << "IF THEN" << std::endl; }
    | T_IF T_OPEN_PAR T_ID T_CLOSING_PAR T_NL T_THEN T_OPEN_BRACE T_NL then T_CLOSE_BRACE else { $$ = new ConditionalOperation(SYMBOL_TABLE.useVariable($3), $9->v, $9->v); std::cout << "IF THEN" << std::endl;  }
    ;

// Ramo then do if
then:
    line { $$ = new MyVector(); $$->v.push_back($1); std::cout << "newLine " << $$->v.size() << std::endl; }
    ;

// Ramo else do if
else:
    T_ELSE T_OPEN_BRACE  then T_CLOSE_BRACE { $$ = $3; std::cout << "ELSE" << std::endl;  }
    | epsilon { $$ = NULL; std::cout << "NOELSE" << std::endl;  }
    ;

epsilon:
    ;

// Expressão
expr:
    T_INT { $$ = new Integer($1); }
    | T_FLOAT { $$ = new Float($1); }
    | T_TRUE { $$ = new Boolean(true); }
    | T_FALSE { $$ = new Boolean(false); }
    | T_ID { $$ = SYMBOL_TABLE.useVariable($1); }
    | T_MINUS expr %prec U_MINUS { $$ = new UnaryOperation(UnaryOperation::MINUS, $2); }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; }
    | T_NOT expr { $$ = new UnaryOperation(UnaryOperation::NOT, $2); }
    | op_relation
    | op_binary
    ;

// Operações binárias
op_binary:
    expr T_PLUS expr { $$ = new BinaryOperation($1, BinaryOperation::PLUS, $3); }
    | expr T_MINUS expr { $$ = new BinaryOperation($1, BinaryOperation::MINUS, $3); }
    | expr T_TIMES expr { $$ = new BinaryOperation($1, BinaryOperation::TIMES, $3); }
    | expr T_DIVIDE expr { $$ = new BinaryOperation($1, BinaryOperation::DIVIDE, $3); }
    ;

// Operações relacionais
op_relation:
    expr T_GREATER expr { $$ = new BinaryOperation($1, BinaryOperation::GREATER, $3); }
    | expr T_GREATER_EQUAL expr { $$ = new BinaryOperation($1, BinaryOperation::GREATER_EQUAL, $3); }
    | expr T_LOWER expr { $$ = new BinaryOperation($1, BinaryOperation::LOWER, $3); }
    | expr T_LOWER_EQUAL expr { $$ = new BinaryOperation($1, BinaryOperation::LOWER_EQUAL, $3); }
    | expr T_AND expr { $$ = new BinaryOperation($1, BinaryOperation::AND, $3); }
    | expr T_OR expr { $$ = new BinaryOperation($1, BinaryOperation::OR, $3); }
    ;

// Declaração de booleano
declar_bool:
    T_ID T_COMMA declar_bool { $$ = new BinaryOperation(SYMBOL_TABLE.newVariable($1, TreeNode::BOOLEAN),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SYMBOL_TABLE.newVariable($1, TreeNode::BOOLEAN); }
    | T_ID T_ATT type T_COMMA declar_bool { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SYMBOL_TABLE.newAssignedVariable($1, TreeNode::BOOLEAN, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT type { $$ = new BinaryOperation(SYMBOL_TABLE.newAssignedVariable($1, TreeNode::BOOLEAN, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3); }
    ;

// Declaração de ponto flutuante
declar_float:
    T_ID T_COMMA declar_float { $$ = new BinaryOperation(SYMBOL_TABLE.newVariable($1, TreeNode::FLOAT),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SYMBOL_TABLE.newVariable($1, TreeNode::FLOAT); }
    | T_ID T_ATT type T_COMMA declar_float { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SYMBOL_TABLE.newAssignedVariable($1, TreeNode::FLOAT, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT type { $$ = new BinaryOperation(SYMBOL_TABLE.newAssignedVariable($1, TreeNode::FLOAT, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3); }
    ;

// Declaração de inteiro
declar_int:
    T_ID T_COMMA declar_int { $$ = new BinaryOperation(SYMBOL_TABLE.newVariable($1, TreeNode::INTEGER),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SYMBOL_TABLE.newVariable($1, TreeNode::INTEGER); }
    | T_ID T_ATT type T_COMMA declar_int { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SYMBOL_TABLE.newAssignedVariable($1, TreeNode::INTEGER, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT type { $$ = new BinaryOperation(SYMBOL_TABLE.newAssignedVariable($1, TreeNode::INTEGER, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3); }
    ;
// Tipos
type:
    T_INT {$$ = new Integer($1);}
    | T_FLOAT {$$ = new Float($1);}
    | T_TRUE {$$ = new Boolean(true);}
    | T_FALSE {$$ = new Boolean(false);}

%%
