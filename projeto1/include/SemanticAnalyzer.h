#ifndef SEMANTICANALYZER_H_
#define SEMANTICANALYZER_H_

#include "SymbolTable.h"
#include <string>
#include <vector>

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

        void newScope();
        void returnScope();
        void analyzeBinaryOperation(ConditionalOperation* conditionalOp);
        void analyzeBinaryOperation(LoopDeclaration* loop);
        void analyzeBinaryOperation(BinaryOperation* binaryOp);
        void analyzeRerefenceOperation(TreeNode* node);
        void analyzeAddressOperation(TreeNode* node);
        TreeNode* declareVariable(std::string varId, TreeNode::ClassType dataType, int size = 0, Pointer::ADDRESS address_type = Pointer::ADDRESS::UNKNOWN,
        Data::Type pointer_type = Data::Type::UNKNOWN);
        TreeNode* assignVariable(std::string varId, TreeNode::ClassType assignedType, Pointer::ADDRESS address_type = Pointer::ADDRESS::REF);
        TreeNode* declareAssignVariable(std::string id, TreeNode::ClassType dataType, TreeNode::ClassType assignedType);
        TreeNode* useVariable(std::string varId, int size = 0);

    private:
        Data::Type classToDataType(TreeNode::ClassType type) const;
        std::string classToString(TreeNode::ClassType type) const;
        std::string dataTypeToString(Data::Type type) const;
        SymbolTable symbolTable;
        std::vector<SymbolTable> scopes;

};

#endif
