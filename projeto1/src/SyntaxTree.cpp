#include "SyntaxTree.h"

SyntaxTree::SyntaxTree(TreeNode* root) {
    this->root = root;    
}

SyntaxTree::~SyntaxTree() {
    if(root)
        delete root;
}

std::string SyntaxTree::printInOrder() {
    return "Syntax Tree";
}