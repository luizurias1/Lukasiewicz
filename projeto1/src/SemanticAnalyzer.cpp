#include "SemanticAnalyzer.h"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::analyzeBinaryOperation(BinaryOperation* binaryOp) {
    TreeNode* left = binaryOp->left;
    BinaryOperation::Type op = binaryOp->operation;
    TreeNode* right = binaryOp->right;
    
    if(left->dataType() == right->dataType() &&
       left->dataType() == Data::INTEGER)
        binaryOp->setType(Data::INTEGER);
    else if(left->dataType() == right->dataType() &&
       left->dataType() == Data::FLOAT)
        binaryOp->setType(Data::FLOAT);
    else if(left->dataType() == right->dataType() &&
       left->dataType() == Data::BOOLEAN)
        binaryOp->setType(Data::BOOLEAN);
        
    switch(left->dataType()) {
        case Data::FLOAT:
            if(right->dataType() == Data::INTEGER) {
                binaryOp->right = new TypeCasting(Data::FLOAT, right);
            }
            break;
        case Data::INTEGER:
            if(right->dataType() == Data::FLOAT) {
                binaryOp->left = new TypeCasting(Data::FLOAT, left);
            }
            break;
        default:
            break;
    }
    
    if(left->dataType() == Data::UNKNOWN || right->dataType() == Data::UNKNOWN)
        return;
    
    if(binaryOp->left->dataType() != binaryOp->right->dataType())
        yyerror("semantic error: %s operation expected %s but received %s\n",
            BinaryOperation::operationName(op),
            dataTypeToString(binaryOp->left->dataType()).c_str(),
            dataTypeToString(binaryOp->right->dataType()).c_str());
    /*
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
            
            // Garantir que inteiro é convertido para float
            if((rightIsVar && rightType == Variable::INTEGER)
               || (right->classType() == TreeNode::INTEGER))
                right = new TypeCasting(Data::FLOAT, right);
            break;
        case TreeNode::INTEGER:
            if(right->classType() != TreeNode::INTEGER) {
                yyerror("semantic error: %s operation expected %s but received %s\n",
                    BinaryOperation::operationName(op),
                    classToString(left->classType()).c_str(),
                    classToString(right->classType()).c_str());
            }
            
            // Garantir que inteiro é convertido para float
            if((rightIsVar && rightType == Variable::FLOAT)
               || (right->classType() == TreeNode::FLOAT))
                left = new TypeCasting(Data::FLOAT, left);
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
            
            if((leftType == Variable::INTEGER && rightIsVar && rightType == Variable::FLOAT)
               || (leftType == Variable::INTEGER && right->classType() == TreeNode::FLOAT))
            // Garantir que inteiro é convertido para float
            if((rightIsVar && rightType == Variable::FLOAT)
               || (right->classType() == TreeNode::FLOAT))
                left = new TypeCasting(Data::FLOAT, left);
            break;          
    }
    */
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, TreeNode::ClassType dataType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    return new Variable(id, classToDataType(dataType)); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, TreeNode::ClassType assignedType) {
    if(!symbolTable.existsVariable(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    else
        symbolTable.setInitializedVariable(id);
    
    return new Variable(id, symbolTable.getSymbolType(id)); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    
    symbolTable.setInitializedVariable(id);
    
    return new Variable(id, classToDataType(dataType));
}

TreeNode* SemanticAnalyzer::useVariable(std::string id) {
    if(!symbolTable.existsVariable(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    else if(!symbolTable.isVariableInitialized(id))
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    
    return new Variable(id, symbolTable.getSymbolType(id)); //Creates variable node anyway
}

Data::Type SemanticAnalyzer::classToDataType(TreeNode::ClassType type) const {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::FLOAT:
            return Data::FLOAT;
        case TreeNode::INTEGER:
            return Data::INTEGER;
        default:
            return Data::UNKNOWN;
    }
}

std::string SemanticAnalyzer::classToString(TreeNode::ClassType type) const {
    return dataTypeToString(classToDataType(type));
}

std::string SemanticAnalyzer::dataTypeToString(Data::Type type) const {
    switch(type) {
        case Data::BOOLEAN:
            return "boolean";
        case Data::FLOAT:
            return "float";
        case Data::INTEGER:
            return "integer";
        default:
            return "unknown";
    }    
}