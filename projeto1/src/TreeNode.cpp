#include "TreeNode.h"

TreeNode::TreeNode() {
}

TreeNode::~TreeNode() {
}

BinaryOperation::BinaryOperation(TreeNode* left, Type operation, TreeNode* right) : TreeNode() {
    this->left = left;
    this->operation = operation;
    this->right = right;
}

BinaryOperation::~BinaryOperation() {
}

Integer::Integer(int value) : TreeNode() {
    this->value = value;    
}

Integer::~Integer() {
}

Variable::Variable(std::string id, TreeNode* next) : TreeNode() {
    this->id = id;
    this->next = next;
}

Variable::~Variable() {
}
