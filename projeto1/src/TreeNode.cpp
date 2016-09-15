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

void BinaryOperation::printTree() {
    left->printTree();
    std::cout << " " << operationToString(operation) << " ";
    right->printTree();
}

std::string BinaryOperation::operationToString(Type operation) {
    switch(operation) {
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case TIMES:
            return "*";
        case DIVIDE:
            return "/";
        case ASSIGN:
            return "=";
        default:
            return "unknown";        
    }
}

Integer::Integer(int value) : TreeNode() {
    this->value = value;    
}

Integer::~Integer() {
}

void Integer::printTree() {
    std::cout << value;
}

Variable::Variable(std::string id, TreeNode* next) : TreeNode() {
    this->id = id;
    this->next = next;
}

Variable::~Variable() {
}

void Variable::printTree() {
    if (next != NULL){
        next->printTree();
        std::cout << ", ";
    }
    std::cout << id;
}