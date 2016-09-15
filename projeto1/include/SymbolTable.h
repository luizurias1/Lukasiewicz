#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "TreeNode.h"
#include <map>
#include <string>

class SymbolTable;

extern SymbolTable SYMBOL_TABLE;
extern void yyerror(const char* s, ...);

/**
 * Símbolo gerado na análise sintática.
 */
class Symbol {
    
    friend class SymbolTable;
    
    public:
        enum DataType {
            INTEGER
        };
    
        enum IdentifierType {
            VARIABLE
        };
    
        Symbol();
        Symbol(DataType dataType, IdentifierType idType, int64_t value, bool initialized);
        virtual ~Symbol();
    
    private:
        Symbol::DataType dataType;
        Symbol::IdentifierType idType;
        int64_t value;      /*Space to store a value while we are doing interpretation.*/
        bool initialized;   /*Defines if symbol has been initialized or not.*/
    
};

/**
 * Tabela de símbolos gerada na análise sintática.
 */
class SymbolTable {
    
    public:    
        SymbolTable();
        virtual ~SymbolTable();
    
        /*checkId returns true if the variable has been defined and false if it does not exist*/
        bool checkId(std::string id);
        void addSymbol(std::string id, Symbol newSymbol);
        
        // Nova variável
        TreeNode* newVariable(std::string id, TreeNode* next);
    
        // Marcar variável como inicializada
        TreeNode* assignVariable(std::string id, TreeNode* next);
    
        TreeNode* newAssignedVariable(std::string id, TreeNode* next);
        
        // Usar variável
        TreeNode* useVariable(std::string id);
    
    private:
        std::map<std::string, Symbol> entryList;
    
};

#endif