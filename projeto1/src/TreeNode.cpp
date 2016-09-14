#include "TreeNode.h"

TreeNode::TreeNode(TreeNode* left, TreeNode* right) {
    this->left = left;
    this->right = right;
}

TreeNode::~TreeNode() {
    if(left)
        delete left;
    
    if(right)
        delete right;
}

BinaryOperation::BinaryOperation() : TreeNode(0, 0) {
    
}

BinaryOperation::~BinaryOperation() {
    
}