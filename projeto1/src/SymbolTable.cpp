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

void Symbol::setDataType(Symbol::DataType type) {
    this->dataType = type;
}

Symbol::DataType Symbol::getDataType() const {
    return this->dataType;
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

Symbol::DataType SymbolTable::getSymbolType(std::string id) {
    return entryList[id].getDataType();
}

TreeNode* SymbolTable::newVariable(std::string id, TreeNode::ClassType dataType) {
    if(checkId(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, 0, false)); // Adds variable to symbol table
    
    return new Variable(id); //Creates variable node anyway
}

TreeNode* SymbolTable::assignVariable(std::string id, TreeNode::ClassType assignedType) {
    if(!checkId(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    
    if(getSymbolType(id) != classToDataType(assignedType))
        yyerror("semantic error: attribution operation expected %s but received %s\n",
                dataTypeToString(getSymbolType(id)).c_str(), classToString(assignedType).c_str());
    else
        entryList[id].initialized = true;
    
    return new Variable(id); //Creates variable node anyway
}

TreeNode* SymbolTable::newAssignedVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType) {
    if(checkId(id))
        yyerror("semantic error: re-declaration of variable %s\n", id.c_str());
    else
       addSymbol(id, Symbol(classToDataType(dataType), Symbol::VARIABLE, 0, false)); // Adds variable to symbol table
    
    if(dataType != assignedType)
        yyerror("semantic error: attribution operation expected %s but received %s\n",
                classToString(dataType).c_str(), classToString(assignedType).c_str());
    else
        entryList[id].initialized = true;
    
    return new Variable(id);
}

TreeNode* SymbolTable::useVariable(std::string id) {
    if(!checkId(id))
        yyerror("semantic error: undeclared variable %s\n", id.c_str());
    if(!entryList[id].initialized)
        yyerror("semantic error: uninitialized variable %s\n", id.c_str());
    
    return new Variable(id); //Creates variable node anyway
}

Symbol::DataType SymbolTable::classToDataType(TreeNode::ClassType type) const {
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

std::string SymbolTable::classToString(TreeNode::ClassType type) const {
    return dataTypeToString(classToDataType(type));
}

std::string SymbolTable::dataTypeToString(Symbol::DataType type) const {
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