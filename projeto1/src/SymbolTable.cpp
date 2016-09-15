#include "SymbolTable.h"

Symbol::Symbol() {
    dataType = Symbol::INTEGER;
    idType = Symbol::VARIABLE;
    value = 0;
    initialized = false;
}

Symbol::Symbol(DataType dataType, IdentifierType idType, int64_t value, bool initialized) : dataType(dataType), idType(idType), value(value), initialized(initialized) {
}

Symbol::~Symbol() {
}

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::checkId(std::string id) {
    return entryList.find(id) != entryList.end();
}

void SymbolTable::addSymbol(std::string id, Symbol newSymbol) {
    entryList[id] = newSymbol;
}

TreeNode* SymbolTable::newVariable(std::string id, TreeNode* next) {
    if(checkId(id))
        yyerror("Variable redefinition! %s\n", id.c_str());
    else
       addSymbol(id, Symbol(Symbol::INTEGER, Symbol::VARIABLE, 0, false)); // Adds variable to symbol table
    
    return new Variable(id, next); //Creates variable node anyway
}

TreeNode* SymbolTable::assignVariable(std::string id, TreeNode* next) {
    if(!checkId(id))
        yyerror("Variable not defined yet! %s\n", id.c_str());
    entryList[id].initialized = true;
    
    return new Variable(id, next); //Creates variable node anyway
}

TreeNode* SymbolTable::newAssignedVariable(std::string id, TreeNode* next) {
    if(checkId(id))
        yyerror("Variable redefinition! %s\n", id.c_str());
    else
       addSymbol(id, Symbol(Symbol::INTEGER, Symbol::VARIABLE, 0, false)); // Adds variable to symbol table
    entryList[id].initialized = true;
    
    return newVariable(id, next);
}

TreeNode* SymbolTable::useVariable(std::string id) {
    if(!checkId(id))
        yyerror("Variable not defined yet! %s\n", id.c_str());
    if(!entryList[id].initialized)
        yyerror("Variable not initialized yet! %s\n", id.c_str());
    
    return new Variable(id, NULL); //Creates variable node anyway
}