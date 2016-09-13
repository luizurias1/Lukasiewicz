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
    char* id;
}

/* token defines our terminal symbols (tokens).
 */
%token <value> T_INT
%token <id> T_ID
%token T_PLUS T_TIMES T_MINUS T_DIVIDE T_OPEN_PAR T_CLOSING_PAR T_NL T_ATT T T_TYPE_INT T_COMMA

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <value> program lines line expr declar

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

%left T_PLUS T_MINUS
%left T_TIMES T_DIVIDE
%nonassoc U_MINUS

/* Starting rule
 */
%start program

%%

program: /*use ctrl+d to stop*/
    lines /*$$ = $1 when nothing is said*/
    ;

lines:
    line T_NL { $$ = 0 /*NULL*/; } /*$$ = $1 when nothing is said*/
    | T_NL lines {$$ = 0;}
    | line T_NL lines
    ;

line:
    T_TYPE_INT declar {$$ = 0;}
    | T_ID T_ATT expr {$$ = 0;}
    ;

expr:
    T_INT { $$ = $1; }
    | T_ID
    | T_MINUS expr %prec U_MINUS { $$ = -$2; std::cout << " -" << $2 << std::endl; }
    | expr T_PLUS expr { $$ = $1 + $3; std::cout << $1 << " + " << $3 << std::endl; }
    | expr T_MINUS expr { $$ = $1 - $3; std::cout << $1 << " - " << $3 << std::endl; }
    | expr T_TIMES expr { $$ = $1 * $3; std::cout << $1 << " * " << $3 << std::endl; }
    | expr T_DIVIDE expr { $$ = $1 / $3; std::cout << $1 << " / " << $3 << std::endl; }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; std::cout << "( " << $2 << " )" << std::endl; }
    ;

declar:
    T_ID T_COMMA declar {$$ = 0;}
    | T_ID {$$ = 0;}
    | T_ID T_ATT expr T_COMMA declar {$$ = 0;}
    | T_ID T_ATT expr {$$ = 0;}
    ;

%%
