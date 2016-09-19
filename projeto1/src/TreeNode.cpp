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

std::string BinaryOperation::printInOrder() {
    std::string output = left->printInOrder();
    if(operation != COMMA)
        output += " ";
    
    output += operationToString(operation) + " ";
    return output + right->printInOrder();
}

std::string BinaryOperation::printPreOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    output += left->printPreOrder();
    return output + right->printPreOrder();
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

std::string UnaryOperation::printInOrder() {
    std::string output = "";
    output += operationToString(operation) + "u ";
    return output + right->printInOrder();
}

std::string UnaryOperation::printPreOrder() {
    std::string output = "";
    output += operationToString(operation) + "u ";
    return output + right->printPreOrder();
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

std::string Integer::printPreOrder() {
    return std::to_string(value) + " ";
}

std::string Integer::printInOrder() {
    return std::to_string(value);
}

Variable::Variable(std::string id) : TreeNode() {
    this->id = id;
}

Variable::~Variable() {
}

std::string Variable::printInOrder() {
    return id;
}

std::string Variable::printPreOrder() {
    return id + " ";
}

VariableDeclaration::VariableDeclaration(Type type, TreeNode* next) : TreeNode() {
    this->type = type;
    this->next = next;
}

VariableDeclaration::~VariableDeclaration() {
}

std::string VariableDeclaration::printInOrder() {
    std::string output = typeToString(this->type) + " var: ";
    if (next != NULL) {
        output += next->printInOrder();
    }
    return output;
}

std::string VariableDeclaration::printPreOrder() {
    std::string output = typeToString(this->type) + " var: ";
    if (next != NULL) {
        output += next->printInOrder();
    }
    return output;
}

std::string VariableDeclaration::typeToString(Type type) {
    switch(type) {
        case INTEGER:
            return "int";
        default:
            return "unknown";        
    }
}