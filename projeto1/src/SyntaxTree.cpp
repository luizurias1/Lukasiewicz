#include "SyntaxTree.h"

SyntaxTree::SyntaxTree() {   
}

SyntaxTree::~SyntaxTree() {
}

void SyntaxTree::print() {
    for (TreeNode* line: lines) {
        line->printPreOrder();
        std::cout << std::endl;
    }
}

void SyntaxTree::insertLine(TreeNode* line) {
    this->lines.insert(lines.begin(), line);
}