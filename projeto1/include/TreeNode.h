#ifndef TREENODE_H_
#define TREENODE_H_

#include <string>
#include <vector>

class SemanticAnalyzer;
class SyntaxTree;
class TreeNode;

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
 * Vetor de nodos da árvore sintática.
 */
class Vector {

    friend class SemanticAnalyzer;
    
    public:
        Vector();
        virtual ~Vector();
        TreeNode* popFront();
        void pushFront(TreeNode* node);
        int size() const;
        std::vector<TreeNode*> internalVector;

};

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
            CONDITIONAL,
            LOOP_DECLARATION,
            FUNCTION,
            FUNCTION_CALL,
            UNKNOWN
        };

        TreeNode(Data::Type type);
        virtual ~TreeNode();
        Data::Type dataType() const;
        void setType(Data::Type type);
        virtual TreeNode::ClassType classType() const = 0;
        virtual std::string printInOrder() = 0;
        virtual std::string printPreOrder() = 0;
    
        static Data::Type classToDataType(TreeNode::ClassType type);
        static std::string toString(TreeNode::ClassType type);
        static std::string toString(Data::Type type);
        static std::string toShortString(TreeNode::ClassType type);
        static std::string toShortString(Data::Type type);

    protected:
        Data::Type type;

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

class ConditionalOperation : public TreeNode {

    friend class SemanticAnalyzer;

    public:
      ConditionalOperation(TreeNode* condition, Vector* then, Vector* el);
      ConditionalOperation(TreeNode* condition, Vector* then);
      virtual ~ConditionalOperation();
      TreeNode::ClassType classType() const;
      TreeNode* getCondition();
      std::string printInOrder();
      std::string printPreOrder();
      std::string returnIfThen(ConditionalOperation* c, std::string identation);

    private:
        TreeNode* condition;
        std::vector<TreeNode*> then;
        std::vector<TreeNode*> el;

};

class LoopDeclaration : public TreeNode {

    friend class SemanticAnalyzer;

    public:
        LoopDeclaration(TreeNode* init, TreeNode* test, TreeNode* interation, Vector* body);
        virtual ~LoopDeclaration();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        void setTab(int number);
        std::string getTab();

    private:
        TreeNode* init;
        TreeNode* test;
        TreeNode* interation;
        std::vector<TreeNode*> body;
        int tab;

};

class Function : public TreeNode {

    friend class ConditionalOperation;
    friend class LoopDeclaration;
    friend class SemanticAnalyzer;

    public:
        Function(std::string id, Vector* params, Vector* body, TreeNode* returnValue);
        virtual ~Function();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();
        std::string getTab();

    private:
        std::string id;
        std::vector<TreeNode*> params;
        std::vector<TreeNode*> body;
        TreeNode* returnValue;
        int tab;

};

class FunctionCall : public TreeNode {

    friend class SemanticAnalyzer;

    public:
        FunctionCall(std::string id, Vector* params);
        virtual ~FunctionCall();
        TreeNode::ClassType classType() const;
        std::string printInOrder();
        std::string printPreOrder();

    private:
        std::string id;
        std::vector<TreeNode*> params;

};

#endif
