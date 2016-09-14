#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_

#include "TreeNode.h"
#include <string>

/**
 * Árvore sintática abstrata.
 */
class SyntaxTree {
    
    public:
        SyntaxTree(TreeNode* root);
        virtual ~SyntaxTree();
        std::string printInOrder();
    
    private:
        TreeNode* root;
    
};

#endif