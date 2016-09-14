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