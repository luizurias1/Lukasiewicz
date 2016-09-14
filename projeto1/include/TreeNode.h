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

class BinaryOperation : public TreeNode {
    
    public:
        enum Type {
            PLUS,
            MINUS,
            TIMES,
            DIVIDE,
            ASSIGN
        };
    
        BinaryOperation();
        virtual ~BinaryOperation();
    
    private:
        Type operation;
    
};

#endif