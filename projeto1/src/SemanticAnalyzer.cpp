#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer() {
}

SemanticAnalyzer::~SemanticAnalyzer() {
}

void SemanticAnalyzer::newScope() {
    scopes.push_back(this->symbolTable);
    this->symbolTable.clear();
}

void SemanticAnalyzer::returnScope() {
    this->symbolTable = scopes.back();
    scopes.pop_back();
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
            TreeNode::toString(left->dataType()).c_str(),
            TreeNode::toString(right->dataType()).c_str());
    }
}

void SemanticAnalyzer::analyzeConditionalOperation(ConditionalOperation* conditionalOp) {
    if(conditionalOp->condition->dataType() != Data::BOOLEAN)
        yyerror("semantic error: test operation expected boolean but received %s\n",
            TreeNode::toString(conditionalOp->condition->dataType()).c_str());
}

void SemanticAnalyzer::analyzeLoopDeclaration(LoopDeclaration* loop) {
    if(loop->test->dataType() != Data::BOOLEAN)
        yyerror("semantic error: test operation expected boolean but received %s\n",
            TreeNode::toString(loop->test->dataType()).c_str());
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, Data::Type dataType) {
    if(symbolExists(id, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table

    return new Variable(id, dataType); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, Data::Type assignedType) {
    if(!symbolExists(id, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else {
        setInitializedSymbol(id);
        return new Variable(id, getSymbolType(id));
    }

}

TreeNode* SemanticAnalyzer::declareAssignVariable(std::string id, Data::Type dataType, Data::Type assignedType) {
    if(symbolExists(id, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table

    setInitializedSymbol(id);
    return new Variable(id, dataType);
}

TreeNode* SemanticAnalyzer::useVariable(std::string id) {
    if(!symbolExists(id, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if(!isSymbolInitialized(id, true)) {
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    return new Variable(id, getSymbolType(id));
}

Data::Type SemanticAnalyzer::getSymbolType(std::string varId) const {
    if(symbolTable.existsVariable(varId))
        return symbolTable.getSymbolType(varId);
    
    for(int i = scopes.size() - 1; i >= 0; i--) {
        SymbolTable t = scopes[i];
        if(t.existsVariable(varId))
            return t.getSymbolType(varId);
    }
    
    // Dark zone: you shouldn't reach this zone!
    return Data::UNKNOWN;
}

bool SemanticAnalyzer::symbolExists(std::string id, bool checkParentScope) {
    if(symbolTable.existsVariable(id))
        return true;
    
    if(checkParentScope) {
        for(SymbolTable t : scopes)
            if(t.existsVariable(id))
                return true;
        return false;
    }
    
    return false;
}

bool SemanticAnalyzer::isSymbolInitialized(std::string id, bool checkParentScope) const { 
    if(symbolTable.existsVariable(id))
        return symbolTable.isVariableInitialized(id);
    
    if(checkParentScope) {
        for(int i = scopes.size() - 1; i >= 0; i--) {
            SymbolTable t = scopes[i];
            if(t.existsVariable(id))
                return t.isVariableInitialized(id);
        }
    }
    
    // Dark zone: you shouldn't reach this zone!
    return false;
}

void SemanticAnalyzer::setInitializedSymbol(std::string id) {
    if(symbolTable.existsVariable(id))
        symbolTable.setInitializedVariable(id);
    else
        for(int i = scopes.size() - 1; i >= 0; i--) {
            SymbolTable t = scopes[i];
            if(scopes[i].existsVariable(id)) {
                scopes[i].setInitializedVariable(id);
                break;
            }
        }
}
