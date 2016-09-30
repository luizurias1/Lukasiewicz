%{
    #include "SemanticAnalyzer.h"
    #include "SyntaxTree.h"
    #include "TreeNode.h"

    SemanticAnalyzer SEMANTIC_ANALYZER;
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
    MyVector* vector;
    int dataType;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <id> T_ID T_FLOAT
%token T_TRUE T_FALSE
%token <dataType> T_TYPE_BOOL T_TYPE_FLOAT T_TYPE_INT
%token T_OPEN_PAR T_CLOSING_PAR T_OPEN_BRACKET T_CLOSING_BRACKET T_OPEN_BRACE T_CLOSING_BRACE T_NL
%token T_PLUS T_TIMES T_MINUS T_DIVIDE
%token T_ATT T T_COMMA
%token T_EQUAL T_NOT_EQUAL T_GREATER T_LOWER T_GREATER_EQUAL T_LOWER_EQUAL
%token T_AND T_OR T_NOT
%token T_IF T_ELSE T_THEN T_FOR
%token T_LVALUE T_POINTER

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <syntaxTree> lines program
%type <node> line expr declar_int declar_float declar_bool data comparison connective op_binary attribution
%type <vector> else scope change_scope
%type <dataType> data_type pointer

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 * left, right, nonassoc
 */

/*relational operators*/
%left T_OR T_AND
%left T_GREATER T_LOWER T_GREATER_EQUAL T_LOWER_EQUAL T_NOT_EQUAL

%left T_PLUS T_MINUS
%left T_TIMES
%left T_DIVIDE
%nonassoc U_MINUS error

/*unart operator*/
%nonassoc T_NOT

/* Starting rule
 */
%start program

%%

// Programa
program:
    lines { SYNTAX_TREE = $1; }
    ;

// Linhas
lines:
    line { $$ = new SyntaxTree(); if($1 != NULL) $$->insertLine($1);  }
    | line T_NL lines { $$ = $3; if($1 != NULL) $3->insertLine($1);  }
    | error T_NL { yyerrok; $$ = NULL; }
    ;

// Linha
line:
    attribution
    | T_TYPE_INT declar_int { $$ = new VariableDeclaration(Data::INTEGER, $2); }
    | T_TYPE_FLOAT declar_float { $$ = new VariableDeclaration(Data::FLOAT, $2); }
    | T_TYPE_BOOL declar_bool { $$ = new VariableDeclaration(Data::BOOLEAN, $2); }
    | T_IF expr T_NL T_THEN T_OPEN_BRACE T_NL change_scope else { $$ = new ConditionalOperation($2, $7->v, $8->v);
            SEMANTIC_ANALYZER.analyzeBinaryOperation((ConditionalOperation*) $$); }
    | T_FOR attribution T_COMMA comparison T_COMMA attribution T_OPEN_BRACE T_NL change_scope { $$ = new LoopDeclaration($2, $4, $6, $9->v);
            SEMANTIC_ANALYZER.analyzeBinaryOperation((LoopDeclaration*) $$); }
    ;

change_scope:
    scope { SEMANTIC_ANALYZER.returnScope(); }
    ;

// Escopo
scope:
    T_CLOSING_BRACE { $$ = new MyVector(); SEMANTIC_ANALYZER.newScope(); }
    | line T_NL scope {$$ = $3; if($1 != NULL) $$->v.insert( $$->v.begin(), $1);  }
    ;

// Ramo else do if
else:
    { $$ = new MyVector(); }
    | T_ELSE T_OPEN_BRACE T_NL change_scope { $$ = $4; }
    ;

// Atribuição
attribution:
    {$$ = NULL;}
    | pointer T_ID T_ATT data
    | T_ID T_ATT T_LVALUE T_ID
    | T_ID T_ATT expr { $$ = new BinaryOperation(
                                SEMANTIC_ANALYZER.assignVariable($1, $3->classType()),
                                BinaryOperation::ASSIGN, $3);
                        SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Expressão
expr:
    T_POINTER T_ID
    | T_INT { $$ = new Integer($1); }
    | T_FLOAT { $$ = new Float($1); }
    | T_TRUE { $$ = new Boolean(true); }
    | T_FALSE { $$ = new Boolean(false); }
    | T_ID { $$ = SEMANTIC_ANALYZER.useVariable($1); }
    | T_MINUS expr %prec U_MINUS { $$ = new UnaryOperation(UnaryOperation::MINUS, $2); }
    | T_OPEN_PAR expr T_CLOSING_PAR { $$ = $2; }
    | T_NOT expr { $$ = new UnaryOperation(UnaryOperation::NOT, $2); }
    | T_OPEN_BRACKET data_type T_CLOSING_BRACKET expr { $$ = new TypeCasting((Data::Type) $2, $4); }
    | op_binary
    | comparison
    | connective
    ;

pointer:
    T_POINTER {$$ = 1;}
    | T_POINTER pointer {$$ += 1;}
    ;

// Operações binárias
op_binary:
    expr T_PLUS expr { $$ = new BinaryOperation($1, BinaryOperation::PLUS, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_MINUS expr { $$ = new BinaryOperation($1, BinaryOperation::MINUS, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_TIMES expr { $$ = new BinaryOperation($1, BinaryOperation::TIMES, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_DIVIDE expr { $$ = new BinaryOperation($1, BinaryOperation::DIVIDE, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Operações binárias relacionais
comparison:
    expr T_GREATER expr { $$ = new BinaryOperation($1, BinaryOperation::GREATER, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_GREATER_EQUAL expr { $$ = new BinaryOperation($1, BinaryOperation::GREATER_EQUAL, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_LOWER expr { $$ = new BinaryOperation($1, BinaryOperation::LOWER, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    | expr T_LOWER_EQUAL expr { $$ = new BinaryOperation($1, BinaryOperation::LOWER_EQUAL, $3);
                         SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Conectivo lógico
connective:
    expr T_AND expr { $$ = new BinaryOperation($1, BinaryOperation::AND, $3); }
    | expr T_OR expr { $$ = new BinaryOperation($1, BinaryOperation::OR, $3); }
    ;

// Declaração de booleano
declar_bool:
    pointer T_ID
    | pointer T_ID T_COMMA declar_bool
    | T_ID T_COMMA declar_bool { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareVariable($1, TreeNode::BOOLEAN),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SEMANTIC_ANALYZER.declareVariable($1, TreeNode::BOOLEAN); }
    | T_ID T_ATT data T_COMMA declar_bool { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SEMANTIC_ANALYZER.declareAssignVariable($1, TreeNode::BOOLEAN, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT data { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareAssignVariable(
                                                    $1, TreeNode::BOOLEAN, $3->classType()),
                                                 BinaryOperation::ASSIGN, $3);
                        SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Declaração de ponto flutuante
declar_float:
    pointer T_ID
    | pointer T_ID T_COMMA declar_float
    | T_ID T_COMMA declar_float { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareVariable($1, TreeNode::FLOAT),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SEMANTIC_ANALYZER.declareVariable($1, TreeNode::FLOAT); }
    | T_ID T_ATT data T_COMMA declar_float { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SEMANTIC_ANALYZER.declareAssignVariable($1, TreeNode::FLOAT, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT data { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareAssignVariable(
                                                    $1, TreeNode::FLOAT, $3->classType()),
                                                 BinaryOperation::ASSIGN, $3);
                        SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Declaração de inteiro
declar_int:
    pointer T_ID { $$ = Pointer(TreeNode::INTEGER, $2, $1); }
    | pointer T_ID T_COMMA declar_int
    | T_ID T_COMMA declar_int { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareVariable($1, TreeNode::INTEGER),
                                                    BinaryOperation::COMMA, $3); }
    | T_ID { $$ = SEMANTIC_ANALYZER.declareVariable($1, TreeNode::INTEGER); }
    | T_ID T_ATT data T_COMMA declar_int { $$ = new BinaryOperation(
                                                  new BinaryOperation(
                                                    SEMANTIC_ANALYZER.declareAssignVariable($1, TreeNode::INTEGER, $3->classType()),
                                                    BinaryOperation::ASSIGN, $3),
                                                    BinaryOperation::COMMA, $5); }
    | T_ID T_ATT data { $$ = new BinaryOperation(SEMANTIC_ANALYZER.declareAssignVariable(
                                                    $1, TreeNode::INTEGER, $3->classType()),
                                                 BinaryOperation::ASSIGN, $3);
                        SEMANTIC_ANALYZER.analyzeBinaryOperation((BinaryOperation*) $$); }
    ;

// Dados
data:
    pointer T_ID
    | T_INT { $$ = new Integer($1); }
    | T_FLOAT { $$ = new Float($1); }
    | T_TRUE { $$ = new Boolean(true); }
    | T_FALSE { $$ = new Boolean(false); }
    ;

// Tipos de dados
data_type:
    T_TYPE_BOOL
    | T_TYPE_FLOAT
    | T_TYPE_INT
    ;

%%
