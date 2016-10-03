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

    if (left->dataType() != right->dataType()){
      if(left->dataType() == Data::POINTER_INTEGER){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::VALUE
      | pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::INTEGER);
        }
      if(left->dataType() == Data::POINTER_FLOAT){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::VALUE
      | pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::FLOAT);
        }
      if(left->dataType() == Data::POINTER_BOOLEAN){
        Pointer* pointer = (Pointer*) left;
        if(pointer->typeOfAddress() == Pointer::ADDRESS::VALUE
      | pointer->typeOfAddress() == Pointer::ADDRESS::ADDR)
          left->setType(Data::BOOLEAN);
        }
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

    if(left->dataType() == Data::ARRAY_INTEGER)
      left->setType(Data::INTEGER);
    if(left->dataType() == Data::ARRAY_FLOAT)
      left->setType(Data::FLOAT);
    if(left->dataType() == Data::ARRAY_BOOLEAN)
      left->setType(Data::BOOLEAN);

    //Se index do array nao for int, gera erro semântico
    if(left->classType() == TreeNode::ARRAY){
      Array *a = (Array*) left;
      if(a->getNode()->classType() == TreeNode::VARIABLE){
        Variable *v = (Variable*) a->getNode();
        Data::Type tipo = getSymbolType(v->getId(), Symbol::VARIABLE);
        if (tipo != Data::INTEGER){
          yyerror("semantic error: index operator expects integer but received %s\n",TreeNode::toString(tipo).c_str());
        }
      }
    }

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

void SemanticAnalyzer::analyzeRerefenceOperation(TreeNode* node){
  Data::Type type = node->dataType();
  if(type != Data::POINTER_INTEGER &&
      type != Data::POINTER_FLOAT && type != Data::POINTER_BOOLEAN)
      yyerror("semantic error: reference operation expects a pointer\n");
}

void SemanticAnalyzer::analyzeAddressOperation(TreeNode* node){
  TreeNode::ClassType type = node->classType();
  if(type != TreeNode::VARIABLE && type != TreeNode::ARRAY)
       yyerror("semantic error: address operation expects a variable or array item\n");
}

TreeNode* SemanticAnalyzer::assignPointer(std::string id, TreeNode::ClassType assignedType, Pointer::ADDRESS address_type, Array* array) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else {
        setInitializedSymbol(id, Symbol::VARIABLE);
        if(array != NULL){
          if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_INTEGER)
            return new Pointer(id, Data::POINTER_INTEGER, address_type, 0, Pointer::UNIQUE, array);
          if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_FLOAT)
            return new Pointer(id, Data::POINTER_FLOAT, address_type, 0, Pointer::UNIQUE, array);
          if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_BOOLEAN)
            return new Pointer(id, Data::POINTER_BOOLEAN, address_type, 0, Pointer::UNIQUE, array);
        }
        if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_INTEGER)
          return new Pointer(id, Data::POINTER_INTEGER, address_type, 0);
        if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_FLOAT)
          return new Pointer(id, Data::POINTER_FLOAT, address_type, 0);
        if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_BOOLEAN)
          return new Pointer(id, Data::POINTER_BOOLEAN, address_type, 0);

        return new Variable(id, getSymbolType(id, Symbol::VARIABLE));
    }
}

TreeNode* SemanticAnalyzer::declareVariable(std::string id, Data::Type dataType, int size) {
    if(symbolExists(id, Symbol::VARIABLE, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else {
        symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table
    }

    if (size > 0) {
       return new Array(id, dataType, size);
    }else
        return new Variable(id, dataType); //Creates variable node anyway
}

TreeNode* SemanticAnalyzer::declarePointer(std::string id, Data::Type dataType, int size, Pointer::ADDRESS address_type, Pointer::Declaration pointer_declaration,
Array* array) {
    if(symbolExists(id, Symbol::VARIABLE, false))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else {
        symbolTable.addSymbol(id, Symbol(dataType, Symbol::VARIABLE, false)); // Adds variable to symbol table
        return new Pointer(id, dataType, address_type, size, pointer_declaration, array);
    }
}

TreeNode* SemanticAnalyzer::assignVariable(std::string id, Data::Type assignedType, TreeNode* index) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if (index != NULL){
          setInitializedSymbol(id, Symbol::VARIABLE);
          return new Array(id, getSymbolType(id, Symbol::VARIABLE), index);
    }  else {
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

TreeNode* SemanticAnalyzer::usePointer(std::string id, int size, bool needed) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if(!isSymbolInitialized(id, Symbol::VARIABLE, true)) {
      if(needed)
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    if (getSymbolType(id, Symbol::VARIABLE) == Data::POINTER_INTEGER){
      int rest = size%2;
      if (rest != 0.0)
        return new Pointer(id, getSymbolType(id, Symbol::VARIABLE), Pointer::ADDRESS::VALUE, 1);
      else
        return new Pointer(id, getSymbolType(id, Symbol::VARIABLE), Pointer::ADDRESS::REF, 1);
   }

  return new Variable(id, getSymbolType(id, Symbol::VARIABLE));
}

TreeNode* SemanticAnalyzer::useVariable(std::string id, TreeNode* index) {
    if(!symbolExists(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
        return new Variable(id, Data::UNKNOWN); //Creates variable node anyway
    } else if (index != NULL){
        return new Array(id, getSymbolType(id, Symbol::VARIABLE), index);
    } else if(!isSymbolInitialized(id, Symbol::VARIABLE, true)) {
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    }
    return new Variable(id, getSymbolType(id, Symbol::VARIABLE));
}

TreeNode* SemanticAnalyzer::declareFunctionHeader(std::string functionId, Vector* params, Data::Type returnType) {

    if(symbolExists(functionId, Symbol::FUNCTION, false))
        yyerror("semantic error: re-definition of function %s\n", functionId.c_str());
    else {
        TreeNode* functionHeader = new Function(functionId, params, new Vector(), new Integer(0));
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
        const Function* header = (const Function*) symbolTable.getSymbol(functionId, Symbol::FUNCTION).getData();

        if(header->params.size() != params->size()) // If different # of params, error
            yyerror("semantic error: re-definition of function %s\n", functionId.c_str());
        else {
            // If different param names, error
            for(int i = 0; i < header->params.size(); i++) {
                if(((Variable*) header->params[i])->getId().compare(((Variable*) params->internalVector[i])->getId()) != 0) {
                    yyerror("semantic error: re-definition of function %s\n",
                                functionId.c_str());
                    break;
                }
            }

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

    const Function* function = (const Function*) getSymbol(functionId, Symbol::FUNCTION, true).getData();

    if(function->params.size() != functionCall->params.size())
        yyerror("semantic error: function %s expected %d parameters but received %d\n",
                functionId.c_str(), function->params.size(), functionCall->params.size());
    else
        for(int i = 0; i < function->params.size(); i++) {
            if(function->params[i]->dataType() != params->internalVector[i]->dataType()) {
                yyerror("semantic error: parameter %s expected %s but received %s\n",
                            ((Variable*)function->params[i])->getId().c_str(),
                            TreeNode::toString(function->params[i]->dataType()).c_str(),
                            TreeNode::toString(params->internalVector[i]->dataType()).c_str());
                break;
            }
        }

    return functionCall;
}

Symbol SemanticAnalyzer::getSymbol(std::string id, Symbol::IdentifierType type, bool checkParentScope) {
    if(symbolTable.existsSymbol(id, type))
        return symbolTable.getSymbol(id, type);

    for(int i = scopes.size() - 1; i >= 0; i--) {
        SymbolTable t = scopes[i];
        if(t.existsSymbol(id, type))
            return t.getSymbol(id, type);
    }

    // Dark zone: you shouldn't reach this zone!
    return Symbol(Data::UNKNOWN, Symbol::VARIABLE, false);
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
