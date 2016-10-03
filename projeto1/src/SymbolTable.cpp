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
            return "FUNC_";
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

const Symbol SymbolTable::getSymbol(std::string id, Symbol::IdentifierType type) const {
    return entryList.at(Symbol::SYMBOL_PREFIX(type) + id);
}

Data::Type SymbolTable::getSymbolType(std::string id, Symbol::IdentifierType type) const {
    return entryList.at(Symbol::SYMBOL_PREFIX(type) + id).dataType;
}

void SymbolTable::addSymbol(std::string id, Symbol newSymbol) {
    entryList[Symbol::SYMBOL_PREFIX(newSymbol.getSymbolType()) + id] = newSymbol;
}

void SymbolTable::setInitializedSymbol(std::string id, Symbol::IdentifierType type) {
    entryList[Symbol::SYMBOL_PREFIX(type) + id].initialized = true;
}

void SymbolTable::setSymbolData(const std::string id, Symbol::IdentifierType type, TreeNode* data) {
    entryList[Symbol::SYMBOL_PREFIX(type) + id].data = data;    
}