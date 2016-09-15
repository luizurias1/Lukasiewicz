#include "SyntaxTree.h"
#include <iostream>

extern SyntaxTree* SYNTAX_TREE;
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv) {
    //yydebug = 1;              // remove comment to have verbose debug messages
    yyparse();                  // parses whole input
    SYNTAX_TREE->print();   // prints the ASTs
    //std::cout << "##Computing the lines" << std::endl;
    //SYNTAX_TREE->computeTree(); // computes the ASTs
    
    return 0;
}
