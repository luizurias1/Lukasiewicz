#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "TreeNode.h"
#include <map>
#include <string>
#include <vector>

/**
 * Símbolo gerado na análise sintática.
 */
class Symbol {
    
    friend class SymbolTable;
    
    public:    
        enum IdentifierType {
            FUNCTION,
            POINTER,
            VARIABLE
        };
    
        Symbol();
        Symbol(Data::Type dataType, IdentifierType idType, bool initialized, TreeNode* data = NULL);
        virtual ~Symbol();
        const TreeNode* getData() const;
        Data::Type getDataType() const;
        IdentifierType getSymbolType() const;
        void setDataType(Data::Type type);
    
    private:
        static std::string SYMBOL_PREFIX(IdentifierType idType);
        Data::Type dataType;
        IdentifierType idType;
        bool initialized;
        TreeNode* data;
    
};

/**
 * Tabela de símbolos gerada na análise sintática.
 */
class SymbolTable {
    
    friend class SemanticAnalyzer;
    
    public:
        SymbolTable();
        SymbolTable& operator=(const SymbolTable& table);
        virtual ~SymbolTable();
        void clear();
        bool existsSymbol(std::string id, Symbol::IdentifierType type) const;
        bool isSymbolInitialized(std::string id, Symbol::IdentifierType type) const;
        Symbol getSymbol(std::string id, Symbol::IdentifierType type);
        Data::Type getSymbolType(std::string id, Symbol::IdentifierType type) const;
        std::vector<std::string> getUninitializedFunctions();
    
        void addSymbol(const std::string id, Symbol newSymbol);
        void setInitializedSymbol(const std::string id, Symbol::IdentifierType type);
        void setSymbolData(const std::string id, Symbol::IdentifierType type, TreeNode* data);
    
    private:
        std::map<std::string, Symbol> entryList;
    
};

#endif