#ifndef TREENODE_H_
#define TREENODE_H_

#include <iostream>
#include <string>

class SyntaxTree;

/**
 * Nodo da árvore sintática.
 */
class TreeNode {
    
    public:    
        TreeNode();
        virtual ~TreeNode();
        virtual void printTree() = 0;
    
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
        void printTree();
        std::string operationToString(Type operation);
    
    private:
        Type operation;
        TreeNode* left;
        TreeNode* right;
    
};

class UnaryOperation : public TreeNode {
    
    public:
        enum Type {
            MINUS
        };
    
        UnaryOperation(Type operation, TreeNode* right);
        virtual ~UnaryOperation();
        void printTree();
        std::string operationToString(Type operation);
    
    private:
        Type operation;
        TreeNode* right;
    
};

class Integer : public TreeNode {
  
    public:
        Integer(int value);
        virtual ~Integer();
        void printTree();
    
    private:
        int value;
    
};

class Variable : public TreeNode {
    
    public:
        Variable(std::string id, TreeNode* next);
        virtual ~Variable();
        void printTree();
    
    private:
        std::string id;
        TreeNode* next;
    
};


#endif