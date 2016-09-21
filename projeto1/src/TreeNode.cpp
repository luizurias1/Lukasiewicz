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

TreeNode::ClassType BinaryOperation::classType() const {
    return TreeNode::BINARY_OPERATION;
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
        case EQUAL:
            return "==";
        case GREATER:
            return ">";
        case GREATER_EQUAL:
            return ">=";
        case LOWER:
            return "<";
        case LOWER_EQUAL:
            return "<=";
        case AND:
            return "&";
        case OR:
            return "|";
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

TreeNode::ClassType UnaryOperation::classType() const {
    return TreeNode::UNARY_OPERATION;
}

std::string UnaryOperation::printInOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    return output + right->printInOrder();
}

std::string UnaryOperation::printPreOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    return output + right->printPreOrder();
}

std::string UnaryOperation::operationToString(Type operation) {
    switch(operation) {
        case MINUS:
            return "-u";
        case NOT:
            return "!";
        default:
            return "unknown";
    }
}

Boolean::Boolean(bool value) : TreeNode() {
    this->value = value;
}

Boolean::~Boolean() {
}

TreeNode::ClassType Boolean::classType() const {
    return TreeNode::BOOLEAN;
}

std::string Boolean::printPreOrder() {
    switch(value) {
      case true:
          return "true ";
      default:
          return "false ";
    }
}

std::string Boolean::printInOrder() {
  switch(value) {
    case true:
       return "true";
    default:
       return "false";
  }
}

Float::Float(std::string value) : TreeNode() {
    this->value = value;
}

Float::~Float() {
}

TreeNode::ClassType Float::classType() const {
    return TreeNode::FLOAT;
}

std::string Float::printPreOrder() {
    return value + " ";
}

std::string Float::printInOrder() {
    return value;
}

Integer::Integer(int value) : TreeNode() {
    this->value = value;
}

Integer::~Integer() {
}

TreeNode::ClassType Integer::classType() const {
    return TreeNode::INTEGER;
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

TreeNode::ClassType Variable::classType() const {
    return TreeNode::VARIABLE;
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

TreeNode::ClassType VariableDeclaration::classType() const {
    return TreeNode::VARIABLE_DECLARATION;
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
        case BOOLEAN:
            return "bool";
        case FLOAT:
            return "float";
        default:
            return "unknown";
    }
}
