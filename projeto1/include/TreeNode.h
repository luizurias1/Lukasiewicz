#ifndef TREENODE_H_
#define TREENODE_H_

class SyntaxTree;

class TreeNode {
    
    friend class SyntaxTree;
    
    public:
        TreeNode();
        virtual ~TreeNode();
    
    private:
        TreeNode* left;
        TreeNode* right;
    
};

#endif