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

void SemanticAnalyzer::analyzeFunctions() {
    // Analisa se todas as funções declaradas foram definidas.
    for(std::string functionId : symbolTable.getUninitializedFunctions())
        yyerror("semantic error: function %s is declared but never defined\n", 
                    functionId.c_str());
}

void SemanticAnalyzer::analyzeLoopDeclaration(LoopDeclaration* loop) {
    if(loop->test->dataType() != Data::BOOLEAN)
        yyerror("semantic error: test operation expected boolean but received %s\n",
            TreeNode::toString(loop->test->dataType()).c_str());
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, Data::Type dataType) {
    if(symbolExists(id, Symbol::VARIABLE, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table

    return new Variable(id, dataType); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, Data::Type assignedType) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else {
        setInitializedSymbol(id, Symbol::VARIABLE);
        return new Variable(id, getSymbolType(id, Symbol::VARIABLE));
    }

}

TreeNode* SemanticAnalyzer::declareAssignVariable(std::string id, Data::Type dataType, Data::Type assignedType) {
    if(symbolExists(id, Symbol::VARIABLE, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table

    setInitializedSymbol(id, Symbol::VARIABLE);
    return new Variable(id, dataType);
}

TreeNode* SemanticAnalyzer::useVariable(std::string id) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if(!isSymbolInitialized(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    return new Variable(id, getSymbolType(id, Symbol::VARIABLE));
}

TreeNode* SemanticAnalyzer::declareFunctionHeader(std::string functionId, Vector* params, Data::Type returnType) {
    
    if(symbolExists(functionId, Symbol::FUNCTION, false))
        yyerror("semantic error: re-definition of function %s\n", functionId.c_str());
    else {
        TreeNode* functionHeader = new FunctionCall(functionId, params);
        functionHeader->setType(returnType);
        symbolTable.addSymbol(functionId, Symbol(returnType, Symbol::FUNCTION, false, functionHeader));
    }

    return NULL;
}

TreeNode* SemanticAnalyzer::declareFunction(std::string functionId, Vector* params, Vector* body, TreeNode* returnValue) {
    TreeNode* newFunction = new Function(functionId, params, body, returnValue);
    
    if(symbolExists(functionId, Symbol::FUNCTION, false) && isSymbolInitialized(functionId, Symbol::FUNCTION, false)) {
        yyerror("semantic error: re-definition of function %s\n", functionId.c_str());
    } else if(symbolExists(functionId, Symbol::FUNCTION, false)) {
        const FunctionCall* call = (const FunctionCall*) symbolTable.getSymbol(functionId, Symbol::FUNCTION).getData();
        
        if(call->params.size() != params->size())
            yyerror("semantic error: re-definition of function %s\n", functionId.c_str());
        else {
            for(int i = 0; i < call->params.size(); i++) {
                if(((Variable*) call->params[i])->getId().compare(((Variable*) params->internalVector[i])->getId()) != 0) {
                    yyerror("semantic error: re-definition of function %s\n",           
                                functionId.c_str());
                    break;
                }    
            }
            
            // TODO Check params and delete old data
            symbolTable.setSymbolData(functionId, Symbol::FUNCTION, newFunction);
            setInitializedSymbol(functionId, Symbol::FUNCTION);
        }
    } else {
       symbolTable.addSymbol(functionId, Symbol(returnValue->dataType(), Symbol::FUNCTION, true, newFunction));
    }

    return newFunction;
}

TreeNode* SemanticAnalyzer::callFunction(std::string functionId, Vector* params) {
    FunctionCall* functionCall = new FunctionCall(functionId, params);
    
    if(!symbolExists(functionId, Symbol::FUNCTION, true)) {
        yyerror("semantic error: undeclared function %s\n", functionId.c_str());
        return functionCall;
    }
    functionCall->setType(getSymbolType(functionId, Symbol::FUNCTION));
    
    return functionCall;
}

Data::Type SemanticAnalyzer::getSymbolType(std::string id, Symbol::IdentifierType type) const {
    if(symbolTable.existsSymbol(id, type))
        return symbolTable.getSymbolType(id, type);
    
    for(int i = scopes.size() - 1; i >= 0; i--) {
        SymbolTable t = scopes[i];
        if(t.existsSymbol(id, type))
            return t.getSymbolType(id, type);
    }
    
    // Dark zone: you shouldn't reach this zone!
    return Data::UNKNOWN;
}

bool SemanticAnalyzer::symbolExists(std::string id, Symbol::IdentifierType type, bool checkParentScope) {
    if(symbolTable.existsSymbol(id, type))
        return true;
    
    if(checkParentScope) {
        for(SymbolTable t : scopes)
            if(t.existsSymbol(id, type))
                return true;
        return false;
    }
    
    return false;
}

bool SemanticAnalyzer::isSymbolInitialized(std::string id, Symbol::IdentifierType type, bool checkParentScope) const { 
    if(symbolTable.existsSymbol(id, type))
        return symbolTable.isSymbolInitialized(id, type);
    
    if(checkParentScope) {
        for(int i = scopes.size() - 1; i >= 0; i--) {
            SymbolTable t = scopes[i];
            if(t.existsSymbol(id, type))
                return t.isSymbolInitialized(id, type);
        }
    }
    
    // Dark zone: you shouldn't reach this zone!
    return false;
}

void SemanticAnalyzer::setInitializedSymbol(std::string id, Symbol::IdentifierType type) {
    if(symbolTable.existsSymbol(id, type))
        symbolTable.setInitializedSymbol(id, type);
    else
        for(int i = scopes.size() - 1; i >= 0; i--) {
            SymbolTable t = scopes[i];
            if(scopes[i].existsSymbol(id, type)) {
                scopes[i].setInitializedSymbol(id, type);
                break;
            }
        }
}
