#include "SyntaxTree.h"

SyntaxTree::SyntaxTree() {   
}

SyntaxTree::~SyntaxTree() {
}

void SyntaxTree::print() {
    for (TreeNode* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

void SyntaxTree::pushBackLine(TreeNode* line) {
    this->lines.push_back(line);
}