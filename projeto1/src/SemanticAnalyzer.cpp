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
    // if(conditionalOp->condition->classType() == TreeNode::BINARY_OPERATION){
    //   BinaryOperation *bin = (BinaryOperation*) conditionalOp->getCondition();
    //     if (bin->operation == BinaryOperation::EQUAL || bin->operation == BinaryOperation::GREATER
    //        || bin->operation == BinaryOperation::GREATER_EQUAL
    //        || bin->operation == BinaryOperation::LOWER
    //        || bin->operation == BinaryOperation::LOWER_EQUAL
    //        || bin->operation == BinaryOperation::AND
    //        || bin->operation == BinaryOperation::OR){
    //          conditionalOp->condition->setType(Data::BOOLEAN);
    //     }
    // }
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

    // Se é uma operação relacional, define tipo como booleano
    if(op == BinaryOperation::GREATER
       || op == BinaryOperation::LOWER
       || op == BinaryOperation::GREATER_EQUAL
       || op == BinaryOperation::LOWER_EQUAL
       || op == BinaryOperation::EQUAL
       || op == BinaryOperation::NOT_EQUAL) {
        binaryOp->setType(Data::BOOLEAN);
    } else { // Define o tipo da operação binária de acordo com o operando esquerdo
        binaryOp->setType(left->dataType());
    }

    // Se algum dos tipos é desconhecido, a análise de operandos não se aplica
    if(left->dataType() == Data::UNKNOWN || right->dataType() == Data::UNKNOWN)
        return;

    if(left->dataType() == Data::ARRAY)
      left->setType(Data::INTEGER);

    //Se index do array nao for int, gera erro semântico
    if(left->classType() == TreeNode::ARRAY){
      Array *a = (Array*) left;
      if(a->getNode()->classType() == TreeNode::VARIABLE){
        Variable *v = (Variable*) a->getNode();
        Data::Type tipo = symbolTable.getSymbolType(v->getId());
        if (tipo != Data::INTEGER){
          yyerror("semantic error: index operator expects integer but received %s\n",dataTypeToString(tipo).c_str());
        }
      }
    }

    // Se os operandos diferem, gera erro semântico
    if(left->dataType() != right->dataType()) {
        yyerror("semantic error: %s operation expected %s but received %s\n",
            BinaryOperation::operationName(op),
            dataTypeToString(left->dataType()).c_str(),
            dataTypeToString(right->dataType()).c_str());
    }
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, TreeNode::ClassType dataType, int size) {
    if(symbolTable.existsVariable(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, false)); // Adds variable to symbol table
    if ((dataType == TreeNode::ARRAY_INTEGER) || (dataType == TreeNode::ARRAY_FLOAT) || (dataType == TreeNode::ARRAY_BOOLEAN)){
       return new Array(id, classToDataType(dataType), size);
    }else
        return new Variable(id, classToDataType(dataType)); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, TreeNode::ClassType assignedType, TreeNode* n) {

    if(!symbolTable.existsVariable(id)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    }  else if (symbolTable.getSymbolType(id) == Data::ARRAY){

        symbolTable.setInitializedVariable(id);
        return new Array (id,symbolTable.getSymbolType(id),n);
    } else{
          symbolTable.setInitializedVariable(id);
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
        case TreeNode::ARRAY_INTEGER:
            return Data::ARRAY;
        case TreeNode::ARRAY_FLOAT:
            return Data::ARRAY;
        case TreeNode::ARRAY_BOOLEAN:
            return Data::ARRAY;
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
