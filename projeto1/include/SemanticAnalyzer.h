#ifndef SEMANTICANALYZER_H_
#define SEMANTICANALYZER_H_

#include "SymbolTable.h"
#include <string>

class SemanticAnalyzer;

extern SemanticAnalyzer SEMANTIC_ANALYZER;
extern void yyerror(const char* s, ...);

/**
 * Analisador semântico.
 */
class SemanticAnalyzer {
    
    public:
        SemanticAnalyzer();
        virtual ~SemanticAnalyzer();
    
        void analyzeBinaryOperation(BinaryOperation* binaryOp);
        TreeNode* declareVariable(std::string varId, TreeNode::ClassType dataType);
        TreeNode* assignVariable(std::string varId, TreeNode::ClassType assignedType);
        TreeNode* declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType);
        TreeNode* useVariable(std::string varId);
    
    private:
        Data::Type classToDataType(TreeNode::ClassType type) const;
        std::string classToString(TreeNode::ClassType type) const;
        std::string dataTypeToString(Data::Type type) const;
        SymbolTable symbolTable;
    
};

#endif