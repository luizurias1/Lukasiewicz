#ifndef TREENODE_H_
#define TREENODE_H_

#include <string>
#include <vector>

class SemanticAnalyzer;
class SyntaxTree;

/**
 * Namespace de dados.
 */
namespace Data {
    enum Type {
        BOOLEAN = 0,
        FLOAT = 1,
        INTEGER = 2,
        UNKNOWN = 3
    };
}

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
            TYPE_CASTING,
            VARIABLE,
            VARIABLE_DECLARATION,
            LOOP_DECLARATION,
            UNKNOWN
        };

        TreeNode(Data::Type type);
        virtual ~TreeNode();
        Data::Type dataType() const;
        void setType(Data::Type type);
        virtual TreeNode::ClassType classType() const = 0;
        virtual std::string printInOrder() = 0;
        virtual std::string printPreOrder() = 0;

    protected:
        Data::Type type;

};

class LoopDeclaration : public TreeNode {

    public:
        enum Type {
            FOR,
            DO
        };

        LoopDeclaration(TreeNode* init, TreeNode* test, TreeNode* interation, std::vector<TreeNode*> body);
        virtual ~LoopDeclaration();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string operationToString(Type operation) const;
        void setTab(int number);
        std::string getTab();

    private:
        TreeNode* init;
        TreeNode* test;
        TreeNode* interation;
        std::vector<TreeNode*> body;
        int tab = 0;

};

class BinaryOperation : public TreeNode {

    friend class SemanticAnalyzer;

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
        Variable(std::string id, Data::Type type);
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
        VariableDeclaration(Data::Type type, TreeNode* next);
        virtual ~VariableDeclaration();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string typeToString(Data::Type type);

    private:
        TreeNode* next;

};

class TypeCasting : public TreeNode {

    public:
        TypeCasting(Data::Type type, TreeNode* next);
        virtual ~TypeCasting();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string typeToString(Data::Type type);

    private:
        TreeNode* next;

};


#endif
