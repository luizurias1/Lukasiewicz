#ifndef TREENODE_H_
#define TREENODE_H_

#include <string>

class SyntaxTree;

/**
 * Nodo da árvore sintática.
 */
class TreeNode {

    public:
        enum ClassType {
            BINARY_OPERATION,
            UNARY_OPERATION,
            BOOLEAN,
            FLOAT,
            INTEGER,
            VARIABLE,
            VARIABLE_DECLARATION,
            UNKNOWN
        };
    
        TreeNode();
        virtual ~TreeNode();
        virtual TreeNode::ClassType classType() const = 0;
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
            COMMA,
            EQUAL,
            GREATER,
            GREATER_EQUAL,
            LOWER,
            LOWER_EQUAL,
            AND,
            OR
        };

        BinaryOperation(TreeNode* left, Type operation, TreeNode* right);
        virtual ~BinaryOperation();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string operationToString(Type operation) const;
        static const char* operationName(Type operation);

    private:
        Type operation;
        TreeNode* left;
        TreeNode* right;

};

class UnaryOperation : public TreeNode {

    public:
        enum Type {
            MINUS,
            NOT
        };

        UnaryOperation(Type operation, TreeNode* right);
        virtual ~UnaryOperation();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string operationToString(Type operation);

    private:
        Type operation;
        TreeNode* right;

};

class Boolean : public TreeNode {

    public:
        Boolean(bool value);
        virtual ~Boolean();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();

    private:
        bool value;

};

class Float : public TreeNode {

    public:
        Float(std::string value);
        virtual ~Float();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();

    private:
        std::string value;

};

class Integer : public TreeNode {

    public:
        Integer(int value);
        virtual ~Integer();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();

    private:
        int value;

};

class Variable : public TreeNode {

    public:
        Variable(std::string id);
        virtual ~Variable();
        TreeNode::ClassType classType() const;
        std::string getId() const;
        std::string printInOrder();
        std::string printPreOrder();

    private:
        std::string id;

};

class VariableDeclaration : public TreeNode {

    public:
        enum Type {
            BOOLEAN,
            FLOAT,
            INTEGER
        };

        VariableDeclaration(Type type, TreeNode* next);
        virtual ~VariableDeclaration();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string typeToString(Type type);

    private:
        Type type;
        TreeNode* next;

};


#endif
