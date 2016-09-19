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

void BinaryOperation::printInOrder() {
    left->printInOrder();
    if(operation != COMMA)
        std::cout << " ";
    
    std::cout << operationToString(operation) << " ";
    right->printInOrder();
}

void BinaryOperation::printPreOrder() {
    std::cout << operationToString(operation) << " ";
    left->printPreOrder();
    right->printPreOrder();
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
        case COMMA:
            return ",";
        default:
            return "unknown";
    }
}

UnaryOperation::UnaryOperation(Type operation, TreeNode* right) : TreeNode() {
    this->operation = operation;
    this->right = right;
}

UnaryOperation::~UnaryOperation() {
}

void UnaryOperation::printInOrder() {
    std::cout << operationToString(operation) << "u ";
    right->printInOrder();
}

void UnaryOperation::printPreOrder() {
    std::cout << operationToString(operation) << "u ";
    right->printPreOrder();
}

std::string UnaryOperation::operationToString(Type operation) {
    switch(operation) {
        case MINUS:
            return "-";
        default:
            return "unknown";        
    }
}

Integer::Integer(int value) : TreeNode() {
    this->value = value;    
}

Integer::~Integer() {
}

void Integer::printPreOrder() {
    std::cout << value << " ";
}

void Integer::printInOrder() {
    std::cout << value;
}

Variable::Variable(std::string id, TreeNode* next) : TreeNode() {
    this->id = id;
    this->next = next;
}

Variable::~Variable() {
}

void Variable::printInOrder() {
    std::cout << id << " ";
    if (next != NULL) {
        next->printInOrder();
        std::cout << ", ";
    }
}

void Variable::printPreOrder() {
    std::cout << id << " ";
    if (next != NULL) {
        next->printPreOrder();
        std::cout << ", ";
    }
}

VariableDeclaration::VariableDeclaration(Type type, TreeNode* next) : TreeNode() {
    this->type = type;
    this->next = next;
}

VariableDeclaration::~VariableDeclaration() {
}

void VariableDeclaration::printInOrder() {
    std::cout << typeToString(this->type) << " var: ";
    if (next != NULL) {
        next->printInOrder();
    }
}

void VariableDeclaration::printPreOrder() {
    std::cout << typeToString(this->type) << " var: ";
    if (next != NULL) {
        next->printInOrder();
    }
}

std::string VariableDeclaration::typeToString(Type type) {
    switch(type) {
        case INTEGER:
            return "int";
        default:
            return "unknown";        
    }
}