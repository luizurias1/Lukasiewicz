#include "SyntaxTree.h"

extern SyntaxTree* SYNTAX_TREE;
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv) {
    // yydebug = 1;
    
    // Parsing da entrada
    yyparse();
    
    // Imprime a árvore sintática
    SYNTAX_TREE->print();

    return 0;
}
