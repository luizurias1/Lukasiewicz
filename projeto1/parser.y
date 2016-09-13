%{
#include <iostream>
#include <math.h>
extern int yylex();
extern void yyerror(const char* s, ...);
%}

%define parse.trace

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int value;
}

/* token defines our terminal symbols (tokens).
 */
%token <value> T_INT
%token T_PLUS T_TIMES T_MINUS T_DIVIDE T_OPEN_PAR T_CLOSING_PAR T_POW T_NL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <value> program lines line expr

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */
%left T_PLUS T_MINUS
%left T_TIMES T_DIVIDE
%left T_POW
%nonassoc U_MINUS

/* Starting rule 
 */
%start program

%%

program: /*use ctrl+d to stop*/
    lines /*$$ = $1 when nothing is said*/
    ;

lines: 
    line /*$$ = $1 when nothing is said*/
    | lines line
    ;

line: 
    T_NL { $$ = 0 /*NULL*/; } /*nothing here to be used */
    | expr T_NL { std::cout << "Res: " << $1 << std::endl; }
    ;

expr: 
    T_INT { $$ = $1; } /*Could write nothing here since we are just copying*/
    | T_MINUS expr %prec U_MINUS { $$ = -$2; std::cout << " -" << $2 << std::endl; }
    | expr T_PLUS expr { $$ = $1 + $3; std::cout << $1 << " + " << $3 << std::endl; }
    | expr T_MINUS expr { $$ = $1 - $3; std::cout << $1 << " - " << $3 << std::endl; }
    | expr T_TIMES expr { $$ = $1 * $3; std::cout << $1 << " * " << $3 << std::endl; }
    | expr T_DIVIDE expr { $$ = $1 / $3; std::cout << $1 << " / " << $3 << std::endl; }
    | expr T_POW expr { $$ = pow($1, $3); std::cout << $1 << " ^ " << $3 << std::endl; }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; std::cout << "( " << $2 << " )" << std::endl; }
    ;

%%
