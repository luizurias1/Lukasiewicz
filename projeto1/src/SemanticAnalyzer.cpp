#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::analyzeBinaryOperation(std::string leftVar,
                BinaryOperation::Type op, TreeNode* right) const {
    if(symbolTable.existsVariable(leftVar))
        analyzeBinaryOperation(new Variable(leftVar), op, right);
}

void SemanticAnalyzer::analyzeBinaryOperation(TreeNode* left,
                BinaryOperation::Type op, TreeNode* right) const {
    bool rightIsVar = right->classType() == TreeNode::VARIABLE;
    Symbol::DataType rightType = Symbol::UNKNOWN;
    
    if(rightIsVar)
        rightType = symbolTable.getSymbolType(((Variable*) right)->getId());
    
    switch(left->classType()) {
        case TreeNode::BOOLEAN:
            if(right->classType() != TreeNode::BOOLEAN)
                yyerror("semantic error: %s operation expected %s but received %s\n",
                    BinaryOperation::operationName(op),
                    classToString(left->classType()).c_str(),
                    classToString(right->classType()).c_str());
            break;
        case TreeNode::FLOAT:
            if(right->classType() != TreeNode::FLOAT)
                yyerror("semantic error: %s operation expected %s but received %s\n",
                    BinaryOperation::operationName(op),
                    classToString(left->classType()).c_str(),
                    classToString(right->classType()).c_str());
            break;
        case TreeNode::INTEGER:
            if(right->classType() != TreeNode::INTEGER) {
                yyerror("semantic error: %s operation expected %s but received %s\n",
                    BinaryOperation::operationName(op),
                    classToString(left->classType()).c_str(),
                    classToString(right->classType()).c_str());
            }
            break;
        case TreeNode::VARIABLE:
            Variable* leftVar = (Variable*) left;
            Symbol::DataType leftType = symbolTable.getSymbolType(leftVar->getId());
            
            if(classToDataType(right->classType()) != Symbol::UNKNOWN &&
                leftType != classToDataType(right->classType())) {
                yyerror("semantic error: %s operation expected %s but received %s\n",
                    BinaryOperation::operationName(op),
                    dataTypeToString(leftType).c_str(),
                    classToString(right->classType()).c_str());
            }
            break;          
    }
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, TreeNode::ClassType dataType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    return new Variable(id); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, TreeNode::ClassType assignedType) {
    if(!symbolTable.existsVariable(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    else
        symbolTable.setInitializedVariable(id);
    
    return new Variable(id); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    symbolTable.setInitializedVariable(id);
    
    return new Variable(id);
}

TreeNode* SemanticAnalyzer::useVariable(std::string id) {
    if(!symbolTable.existsVariable(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    else if(!symbolTable.isVariableInitialized(id))
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    
    return new Variable(id); //Creates variable node anyway
}

Symbol::DataType SemanticAnalyzer::classToDataType(TreeNode::ClassType type) const {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Symbol::BOOLEAN;
        case TreeNode::FLOAT:
            return Symbol::FLOAT;
        case TreeNode::INTEGER:
            return Symbol::INTEGER;
        default:
            return Symbol::UNKNOWN;
    }
}

std::string SemanticAnalyzer::classToString(TreeNode::ClassType type) const {
    return dataTypeToString(classToDataType(type));
}

std::string SemanticAnalyzer::dataTypeToString(Symbol::DataType type) const {
    switch(type) {
        case Symbol::BOOLEAN:
            return "boolean";
        case Symbol::FLOAT:
            return "float";
        case Symbol::INTEGER:
            return "integer";
        default:
            return "unknown";
    }    
}