#include "SyntaxTree.h"

SyntaxTree::SyntaxTree() {   
}

SyntaxTree::~SyntaxTree() {
}

void SyntaxTree::print() {
    for (TreeNode* line: lines) {
        std::string toPrint = line->printPreOrder();
        
        if(toPrint.back() == ' ')
            toPrint.back() = '\0';
        
        std::cout << toPrint << std::endl;
    }
}

void SyntaxTree::insertLine(TreeNode* line) {
    this->lines.insert(lines.begin(), line);
}