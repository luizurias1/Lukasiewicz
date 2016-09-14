#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_

#include "TreeNode.h"
#include <string>
#include <vector>

/**
 * Árvore sintática abstrata.
 */
class SyntaxTree {
    
    public:        
        SyntaxTree();
        virtual ~SyntaxTree();
        std::string print();
    
    private:
        std::vector<TreeNode*> lines;
    
};

#endif