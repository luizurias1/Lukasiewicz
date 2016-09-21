#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <map>
#include <string>

/**
 * Símbolo gerado na análise sintática.
 */
class Symbol {
    
    friend class SymbolTable;
    
    public:
        enum DataType {
            BOOLEAN,
            FLOAT,
            INTEGER,
            UNKNOWN
        };
    
        enum IdentifierType {
            VARIABLE
        };
    
        Symbol();
        Symbol(DataType dataType, IdentifierType idType, bool initialized);
        virtual ~Symbol();
        DataType getDataType() const;
        void setDataType(DataType type);
    
    private:
        DataType dataType;
        IdentifierType idType;
        bool initialized;
    
};

/**
 * Tabela de símbolos gerada na análise sintática.
 */
class SymbolTable {
    
    public:
        SymbolTable();
        virtual ~SymbolTable();
    
        bool existsVariable(std::string varId) const;
        bool isVariableInitialized(std::string varId) const;
        Symbol::DataType getSymbolType(std::string varId) const;
    
        void addSymbol(const std::string varId, Symbol newSymbol);
        void setInitializedVariable(const std::string varId);
    
    private:
        std::map<std::string, Symbol> entryList;
    
};

#endif