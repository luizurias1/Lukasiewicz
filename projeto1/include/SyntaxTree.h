#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_

#include "TreeNode.h"

class SyntaxTree {
    
    public:
        SyntaxTree();
        virtual ~SyntaxTree();
    
    private:
        TreeNode* root;
    
};

#endif