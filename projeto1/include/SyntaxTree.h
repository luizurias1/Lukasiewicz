#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_

#include "TreeNode.h"
#include <string>

/**
 * Árvore sintática abstrata.
 */
class SyntaxTree {
    
    public:
        enum BinaryOperation {
            PLUS,
            MINUS,
            TIMES,
            DIVIDE,
            ASSIGN
        };
        
        SyntaxTree(TreeNode* root);
        virtual ~SyntaxTree();
        std::string printInOrder();
    
    private:
        TreeNode* root;
    
};

#endif