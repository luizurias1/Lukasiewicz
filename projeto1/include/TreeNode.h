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
        virtual void printInOrder() = 0;
        virtual void printPreOrder() = 0;
    
};

class BinaryOperation : public TreeNode {
    
    public:
        enum Type {
            PLUS,
            MINUS,
            TIMES,
            DIVIDE,
            ASSIGN,
            COMMA
        };
    
        BinaryOperation(TreeNode* left, Type operation, TreeNode* right);
        virtual ~BinaryOperation();
        void printInOrder();
        void printPreOrder();
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
        void printInOrder();
        void printPreOrder();
        std::string operationToString(Type operation);
    
    private:
        Type operation;
        TreeNode* right;
    
};

class Integer : public TreeNode {
  
    public:
        Integer(int value);
        virtual ~Integer();
        void printInOrder();
        void printPreOrder();
    
    private:
        int value;
    
};

class Variable : public TreeNode {
    
    public:
        Variable(std::string id, TreeNode* next);
        virtual ~Variable();
        void printInOrder();
        void printPreOrder();
    
    private:
        std::string id;
        TreeNode* next;
    
};

class VariableDeclaration : public TreeNode {
    
    public:
        enum Type {
            INTEGER
        };
    
        VariableDeclaration(Type type, TreeNode* next);
        virtual ~VariableDeclaration();
        void printInOrder();
        void printPreOrder();
        std::string typeToString(Type type);
    
    private:
        Type type;
        TreeNode* next;
    
};


#endif