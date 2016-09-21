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
            BOOLEAN,
            FLOAT,
            INTEGER,
            UNKNOWN
        };
    
        enum IdentifierType {
            VARIABLE
        };
    
        Symbol();
        Symbol(DataType dataType, IdentifierType idType, int64_t value, bool initialized);
        virtual ~Symbol();
        void setDataType(DataType type);
        DataType getDataType() const;
    
    private:
        DataType dataType;
        IdentifierType idType;
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
        Symbol::DataType getSymbolType(std::string id);
        
        // Nova variável
        TreeNode* newVariable(std::string id, TreeNode::ClassType dataType);
    
        // Marcar variável como inicializada
        TreeNode* assignVariable(std::string id, TreeNode::ClassType assignedType);
    
        TreeNode* newAssignedVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType);
        
        // Usar variável
        TreeNode* useVariable(std::string id);
    
    private:
        Symbol::DataType classToDataType(TreeNode::ClassType type) const;
        std::string classToString(TreeNode::ClassType type) const;
        std::string dataTypeToString(Symbol::DataType type) const; 
        std::map<std::string, Symbol> entryList;
    
};

#endif