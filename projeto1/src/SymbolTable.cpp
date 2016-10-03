#include "SymbolTable.h"

Symbol::Symbol() {
    this->dataType = Data::UNKNOWN;
    this->idType = Symbol::VARIABLE;
    this->initialized = false;
}

Symbol::Symbol(Data::Type dataType, IdentifierType idType, bool initialized, TreeNode* data) {
    this->dataType = dataType;
    this->idType = idType;
    this->initialized = initialized;
    this->data = data;
}

Symbol::~Symbol() {
}

const TreeNode* Symbol::getData() const {
    return this->data;
}

Data::Type Symbol::getDataType() const {
    return this->dataType;
}

Symbol::IdentifierType Symbol::getSymbolType() const {
    return this->idType;
}

void Symbol::setDataType(Data::Type type) {
    this->dataType = type;
}

std::string Symbol::SYMBOL_PREFIX(Symbol::IdentifierType idType) {
    switch(idType) {
        case Symbol::FUNCTION:
            return "FUN_";
        case Symbol::VARIABLE:
            return "VAR_";
        default:
            return "";
    }
}

SymbolTable::SymbolTable() {
}

SymbolTable& SymbolTable::operator=(const SymbolTable& table) {
    this->entryList = table.entryList;
    return *this;
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::clear() {
    entryList.clear();
}

bool SymbolTable::existsSymbol(std::string id, Symbol::IdentifierType type) const {
    return entryList.find(Symbol::SYMBOL_PREFIX(type) + id) != entryList.end();
}

bool SymbolTable::isSymbolInitialized(std::string id, Symbol::IdentifierType type) const {
    return entryList.at(Symbol::SYMBOL_PREFIX(type) + id).initialized;
}

Symbol SymbolTable::getSymbol(std::string id, Symbol::IdentifierType type) {
    return entryList.at(Symbol::SYMBOL_PREFIX(type) + id);
}

Data::Type SymbolTable::getSymbolType(std::string id, Symbol::IdentifierType type) const {
    return entryList.at(Symbol::SYMBOL_PREFIX(type) + id).dataType;
}

std::vector<std::string> SymbolTable::getUninitializedFunctions() {
    std::map<std::string, Symbol>::iterator iter;
    std::vector<std::string> functions;
    std::string prefix = Symbol::SYMBOL_PREFIX(Symbol::FUNCTION);
    
    for(iter = entryList.begin(); iter != entryList.end(); iter++) {
        if(iter->first.compare(0, prefix.size(), prefix) == 0
           && !entryList[iter->first].initialized)
            functions.push_back(iter->first.substr(prefix.size(), iter->first.size()));
    }
    
    return functions;
}

void SymbolTable::addSymbol(std::string id, Symbol newSymbol) {
    entryList[Symbol::SYMBOL_PREFIX(newSymbol.getSymbolType()) + id] = newSymbol;
}

void SymbolTable::setInitializedSymbol(std::string id, Symbol::IdentifierType type) {
    entryList[Symbol::SYMBOL_PREFIX(type) + id].initialized = true;
}

void SymbolTable::setSymbolData(const std::string id, Symbol::IdentifierType type, TreeNode* data) {
    if(entryList[Symbol::SYMBOL_PREFIX(type) + id].data) {
        delete entryList[Symbol::SYMBOL_PREFIX(type) + id].data;
        entryList[Symbol::SYMBOL_PREFIX(type) + id].data = 0;
    }
    
    entryList[Symbol::SYMBOL_PREFIX(type) + id].data = data;    
}