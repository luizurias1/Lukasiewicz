#include "SyntaxTree.h"

SyntaxTree::SyntaxTree() {   
}

SyntaxTree::~SyntaxTree() {
}

std::string SyntaxTree::print() {
    return "Syntax Tree";
}

void SyntaxTree::pushBackLine(TreeNode* line) {
    this->lines.push_back(line);
}