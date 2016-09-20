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
        virtual std::string printInOrder() = 0;
        virtual std::string printPreOrder() = 0;

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
        std::string printInOrder();
        std::string printPreOrder();
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
        std::string printInOrder();
        std::string printPreOrder();
        std::string operationToString(Type operation);

    private:
        Type operation;
        TreeNode* right;

};

class Integer : public TreeNode {

    public:
        Integer(int value);
        virtual ~Integer();
        std::string printInOrder();
        std::string printPreOrder();

    private:
        int value;

};

class Variable : public TreeNode {

    public:
        Variable(std::string id);
        virtual ~Variable();
        std::string printInOrder();
        std::string printPreOrder();

    private:
        std::string id;

};

class VariableDeclaration : public TreeNode {

    public:
        enum Type {
            INTEGER
        };

        VariableDeclaration(Type type, TreeNode* next);
        virtual ~VariableDeclaration();
        std::string printInOrder();
        std::string printPreOrder();
        std::string typeToString(Type type);

    private:
        Type type;
        TreeNode* next;

};


#endif
