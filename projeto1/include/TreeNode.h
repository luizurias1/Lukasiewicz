#ifndef TREENODE_H_
#define TREENODE_H_

#include <string>

class SyntaxTree;

/**
 * Nodo da árvore sintática.
 */
class TreeNode {
    
    public:    
        TreeNode();
        virtual ~TreeNode();
    
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
    
        BinaryOperation(TreeNode* left, Type operation, TreeNode* right);
        virtual ~BinaryOperation();
    
    private:
        Type operation;
        TreeNode* left;
        TreeNode* right;
    
};

class Integer : public TreeNode {
  
    public:
        Integer(int value);
        virtual ~Integer();
    
    private:
        int value;
    
};

class Variable : public TreeNode {
    
    public:
        Variable(std::string id, TreeNode* next);
        virtual ~Variable();
    
    private:
        std::string id;
        TreeNode* next;
    
};


#endif