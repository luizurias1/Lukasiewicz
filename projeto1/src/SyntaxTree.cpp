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

void SyntaxTree::insertLine(TreeNode* line) {
    this->lines.insert(lines.begin(), line);
}