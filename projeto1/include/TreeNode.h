#ifndef TREENODE_H_
#define TREENODE_H_

class SyntaxTree;

/**
 * Nodo da árvore sintática.
 */
class TreeNode {
    
    friend class SyntaxTree;
    
    public:
        TreeNode(TreeNode* left, TreeNode* right);
        virtual ~TreeNode();
    
    private:
        TreeNode* left;
        TreeNode* right;
    
};

#endif