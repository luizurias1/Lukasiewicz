#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::newScope() {
    scopes.push_back(this->symbolTable);
}

void SemanticAnalyzer::returnScope() {
    this->symbolTable = scopes.back();
    scopes.pop_back();
}

void SemanticAnalyzer::analyzeBinaryOperation(ConditionalOperation* conditionalOp) {
    if(conditionalOp->condition->dataType() != Data::BOOLEAN)
        yyerror("semantic error: test operation expected boolean but received %s\n",
            dataTypeToString(conditionalOp->condition->dataType()).c_str());
}

void SemanticAnalyzer::analyzeBinaryOperation(LoopDeclaration* loop) {
    if(loop->test->dataType() != Data::BOOLEAN)
        yyerror("semantic error: test operation expected boolean but received %s\n",
            dataTypeToString(loop->test->dataType()).c_str());
}

void SemanticAnalyzer::analyzeBinaryOperation(BinaryOperation* binaryOp) {
    TreeNode* left = binaryOp->left;
    BinaryOperation::Type op = binaryOp->operation;
    TreeNode* right = binaryOp->right;

    // Adiciona uma conversão int -> float obrigatória caso necessário
    switch(left->dataType()) {
        case Data::FLOAT:
            if(right->dataType() == Data::INTEGER) {
                binaryOp->right = new TypeCasting(Data::FLOAT, right);
                right = binaryOp->right;
            }
            break;
        case Data::INTEGER:
            if(right->dataType() == Data::FLOAT && op != BinaryOperation::ASSIGN) {
                binaryOp->left = new TypeCasting(Data::FLOAT, left);
                left = binaryOp->left;
            }
            break;
        default:
            break;
    }

    if (left->dataType() != right->dataType()){
      if(left->dataType() == Data::POINTER_INTEGER){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::INTEGER);
        }
      if(left->dataType() == Data::POINTER_FLOAT){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::FLOAT);
        }
      if(left->dataType() == Data::POINTER_BOOLEAN){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::BOOLEAN);
        }
      }

    // Se é uma operação relacional, define tipo como booleano
    if(op == BinaryOperation::GREATER
       || op == BinaryOperation::LOWER
       || op == BinaryOperation::GREATER_EQUAL
       || op == BinaryOperation::LOWER_EQUAL) {
        binaryOp->setType(Data::BOOLEAN);
    } else { // Define o tipo da operação binária de acordo com o operando esquerdo
        binaryOp->setType(left->dataType());
    }

    // Se algum dos tipos é desconhecido, a análise de operandos não se aplica
    if(left->dataType() == Data::UNKNOWN || right->dataType() == Data::UNKNOWN)
        return;

    // Se os operandos diferem, gera erro semântico
    if(left->dataType() != right->dataType()) {
        yyerror("semantic error: %s operation expected %s but received %s\n",
            BinaryOperation::operationName(op),
            dataTypeToString(left->dataType()).c_str(),
            dataTypeToString(right->dataType()).c_str());
    }
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, TreeNode::ClassType dataType, int size, Pointer::ADDRESS address_type, Data::Type pointer_type) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else{
      if(dataType == TreeNode::POINTER){
        if (pointer_type == Data::INTEGER){
          symbolTable.addSymbol(id, Symbol(Data::POINTER_INTEGER, Symbol::VARIABLE, false)); // Adds variable to symbol table
          return new Pointer(id, classToDataType(dataType), address_type, size);;
        }
        if (pointer_type == Data::FLOAT){
          symbolTable.addSymbol(id, Symbol(Data::POINTER_FLOAT, Symbol::VARIABLE, false)); // Adds variable to symbol table
          return new Pointer(id, classToDataType(dataType), address_type, size);;
        }
        if (pointer_type == Data::BOOLEAN){
          symbolTable.addSymbol(id, Symbol(Data::POINTER_BOOLEAN, Symbol::VARIABLE, false)); // Adds variable to symbol table
          return new Pointer(id, classToDataType(dataType), address_type, size);;
        }
      }
      symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    }

    return new Variable(id, classToDataType(dataType)); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, TreeNode::ClassType assignedType, Pointer::ADDRESS address_type) {
    if(!symbolTable.existsVariable(id)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else {
        symbolTable.setInitializedVariable(id);

        if (symbolTable.getSymbolType(id) == Data::POINTER_INTEGER)
          return new Pointer(id, Data::POINTER_INTEGER, address_type, 0);
        if (symbolTable.getSymbolType(id) == Data::POINTER_FLOAT)
          return new Pointer(id, Data::POINTER_FLOAT, address_type, 0);
        if (symbolTable.getSymbolType(id) == Data::POINTER_BOOLEAN)
          return new Pointer(id, Data::POINTER_BOOLEAN, address_type, 0);

        return new Variable(id, symbolTable.getSymbolType(id));
    }
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
    if(!symbolTable.existsVariable(id)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if(!symbolTable.isVariableInitialized(id)) {
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    return new Variable(id, symbolTable.getSymbolType(id));
}

Data::Type SemanticAnalyzer::classToDataType(TreeNode::ClassType type) const {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::FLOAT:
            return Data::FLOAT;
        case TreeNode::INTEGER:
            return Data::INTEGER;
        case TreeNode::POINTER:
            return Data::POINTER;
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
        case Data::POINTER:
            return "pointer";
        case Data::POINTER_INTEGER:
            return "integer pointer";
        case Data::POINTER_FLOAT:
            return "float pointer";
        case Data::POINTER_BOOLEAN:
            return "bool pointer";
        default:
            return "unknown";
    }
}
