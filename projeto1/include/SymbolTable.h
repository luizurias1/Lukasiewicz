#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "TreeNode.h"
#include <map>
#include <string>

/**
 * Símbolo gerado na análise sintática.
 */
class Symbol {
    
    friend class SymbolTable;
    
    public:    
        enum IdentifierType {
            VARIABLE
        };
    
        Symbol();
        Symbol(Data::Type dataType, IdentifierType idType, bool initialized);
        virtual ~Symbol();
        Data::Type getDataType() const;
        void setDataType(Data::Type type);
    
    private:
        Data::Type dataType;
        IdentifierType idType;
        bool initialized;
    
};

/**
 * Tabela de símbolos gerada na análise sintática.
 */
class SymbolTable {
    
    public:
        SymbolTable();
        SymbolTable& operator=(const SymbolTable& table);
        virtual ~SymbolTable();
        void clear();
        bool existsVariable(std::string varId) const;
        bool isVariableInitialized(std::string varId) const;
        Data::Type getSymbolType(std::string varId) const;
    
        void addSymbol(const std::string varId, Symbol newSymbol);
        void setInitializedVariable(const std::string varId);
    
    private:
        std::map<std::string, Symbol> entryList;
    
};

#endif