#include "TreeNode.h"

TreeNode::TreeNode(Data::Type type) {
    this->type = type;
}

TreeNode::~TreeNode() {
}

Data::Type TreeNode::dataType() const {
    return this->type;
}

void TreeNode::setType(Data::Type type) {
    this->type = type;
}

BinaryOperation::BinaryOperation(TreeNode* left, BinaryOperation::Type operation, TreeNode* right) : TreeNode(Data::UNKNOWN) {
    this->left = left;
    this->operation = operation;
    this->right = right;
}

BinaryOperation::~BinaryOperation() {
}

TreeNode::ClassType BinaryOperation::classType() const {
    return TreeNode::BINARY_OPERATION;
}

std::string BinaryOperation::printInOrder() {
    std::string output = left->printInOrder();
    if(operation != COMMA)
        output += " ";

    output += operationToString(operation) + " ";
    return output + right->printInOrder();
}

std::string BinaryOperation::printPreOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    output += left->printPreOrder();
    return output + right->printPreOrder();
}

std::string BinaryOperation::operationToString(BinaryOperation::Type operation) const {
    switch(operation) {
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case TIMES:
            return "*";
        case DIVIDE:
            return "/";
        case ASSIGN:
            return "=";
        case COMMA:
            return ",";
        case EQUAL:
            return "==";
        case GREATER:
            return ">";
        case GREATER_EQUAL:
            return ">=";
        case LOWER:
            return "<";
        case LOWER_EQUAL:
            return "<=";
        case AND:
            return "&";
        case OR:
            return "|";
        default:
            return "unknown";
    }
}

const char* BinaryOperation::operationName(BinaryOperation::Type operation) {
    switch(operation) {
        case PLUS:
            return "addition";
        case MINUS:
            return "subtraction";
        case TIMES:
            return "multiplication";
        case DIVIDE:
            return "division";
        case ASSIGN:
            return "attribution";
        case COMMA:
            return "commation #haha";
        case EQUAL:
            return "comparison";
        case GREATER:
            return "comparison";
        case GREATER_EQUAL:
            return "comparison";
        case LOWER:
            return "comparison";
        case LOWER_EQUAL:
            return "comparison";
        case AND:
            return "and";
        case OR:
            return "or";
        default:
            return "unknown";
    }
}

UnaryOperation::UnaryOperation(UnaryOperation::Type operation, TreeNode* right) : TreeNode(Data::UNKNOWN) {
    this->operation = operation;
    this->right = right;
}

UnaryOperation::~UnaryOperation() {
}

TreeNode::ClassType UnaryOperation::classType() const {
    return TreeNode::UNARY_OPERATION;
}

std::string UnaryOperation::printInOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    return output + right->printInOrder();
}

std::string UnaryOperation::printPreOrder() {
    std::string output = "";
    output += operationToString(operation) + " ";
    return output + right->printPreOrder();
}

std::string UnaryOperation::operationToString(UnaryOperation::Type operation) {
    switch(operation) {
        case MINUS:
            return "-u";
        case NOT:
            return "!";
        default:
            return "unknown";
    }
}

Boolean::Boolean(bool value) : TreeNode(Data::BOOLEAN) {
    this->value = value;
}

Boolean::~Boolean() {
}

TreeNode::ClassType Boolean::classType() const {
    return TreeNode::BOOLEAN;
}

std::string Boolean::printPreOrder() {
    switch(value) {
      case true:
          return "true ";
      default:
          return "false ";
    }
}

std::string Boolean::printInOrder() {
  switch(value) {
    case true:
       return "true";
    default:
       return "false";
  }
}

Float::Float(std::string value) : TreeNode(Data::FLOAT) {
    this->value = value;
}

Float::~Float() {
}

TreeNode::ClassType Float::classType() const {
    return TreeNode::FLOAT;
}

std::string Float::printPreOrder() {
    return value + " ";
}

std::string Float::printInOrder() {
    return value;
}

Integer::Integer(int value) : TreeNode(Data::INTEGER) {
    this->value = value;
}

Integer::~Integer() {
}

TreeNode::ClassType Integer::classType() const {
    return TreeNode::INTEGER;
}

std::string Integer::printPreOrder() {
    return std::to_string(value) + " ";
}

std::string Integer::printInOrder() {
    return std::to_string(value);
}

Variable::Variable(std::string id, Data::Type type) : TreeNode(type) {
    this->id = id;
}

Variable::~Variable() {
}

TreeNode::ClassType Variable::classType() const {
    return TreeNode::VARIABLE;
}

std::string Variable::getId() const {
    return id;
}

std::string Variable::printInOrder() {
    return id;
}

std::string Variable::printPreOrder() {
    return id + " ";
}

VariableDeclaration::VariableDeclaration(Data::Type type, TreeNode* next) : TreeNode(type) {
    this->next = next;
}

VariableDeclaration::~VariableDeclaration() {
}

TreeNode::ClassType VariableDeclaration::classType() const {
    return TreeNode::VARIABLE_DECLARATION;
}

std::string VariableDeclaration::printInOrder() {
    std::string output = typeToString(this->type) + " var: ";
    if (next != NULL) {
        output += next->printInOrder();
    }
    return output;
}

std::string VariableDeclaration::printPreOrder() {
    std::string output = typeToString(this->type) + " var: ";
    if (next != NULL) {
        output += next->printInOrder();
    }
    return output;
}

std::string VariableDeclaration::typeToString(Data::Type type) {
    switch(type) {
        case Data::INTEGER:
            return "int";
        case Data::BOOLEAN:
            return "bool";
        case Data::FLOAT:
            return "float";
        default:
            return "unknown";
    }
}

TypeCasting::TypeCasting(Data::Type type, TreeNode* next) : TreeNode(type) {
    this->next = next;
}

TypeCasting::~TypeCasting() {
}

TreeNode::ClassType TypeCasting::classType() const {
    return TreeNode::TYPE_CASTING;
}

std::string TypeCasting::printInOrder() {
    std::string output = "[" + typeToString(this->type) + "] ";
    if (next != NULL) {
        output += next->printInOrder();
    }
    return output;
}

std::string TypeCasting::printPreOrder() {
    std::string output = "[" + typeToString(this->type) + "] ";
    if (next != NULL) {
        output += next->printPreOrder();
    }
    return output;
}

std::string TypeCasting::typeToString(Data::Type type) {
    switch(type) {
        case Data::INTEGER:
            return "int";
        case Data::BOOLEAN:
            return "bool";
        case Data::FLOAT:
            return "float";
        default:
            return "unknown";
    }
}

ConditionalOperation::ConditionalOperation(TreeNode* condition, std::vector<TreeNode*> then, std::vector<TreeNode*> el) : TreeNode(Data::UNKNOWN) {
    this->condition = condition;
    this->then = then;
    this->el = el;
}

ConditionalOperation::ConditionalOperation(TreeNode* condition, std::vector<TreeNode*> then) : TreeNode(Data::UNKNOWN) {
    this->condition = condition;
    this->then = then;
}

ConditionalOperation::~ConditionalOperation() {
}

ConditionalOperation::ClassType ConditionalOperation::classType() const {
    return TreeNode::CONDITIONAL;
}

std::string ConditionalOperation::printInOrder(){
  std::string output = "if: ";
  std::string identation = "";
  output += condition->printPreOrder() + "\n" + identation+ "then:\n";
for (TreeNode* line: then){
    if (line->classType() == TreeNode::CONDITIONAL) {
            ConditionalOperation* c = (ConditionalOperation*) line;
            output+=returnIfThen(c,identation);
    } else {
      identation+= "  ";
      output+=identation+line->printPreOrder();
    }
}
  if(el.size() > 0) {
      output += identation+"\nelse:";
      for (TreeNode* line: el)
        output+= "\n"+identation+line->printPreOrder();
  }

  return output;
}

std::string ConditionalOperation::printPreOrder(){
    std::string output = "if: ";
    std::string identation = "";
    output += condition->printPreOrder() + "\n" + identation+ "then:";
    for (TreeNode* line: then){
      if (line->classType() == TreeNode::CONDITIONAL) {
              ConditionalOperation* c = (ConditionalOperation*) line;
              output+="\n"+returnIfThen(c,identation);
      } else {
        identation+= "  ";
        output+="\n"+identation+line->printPreOrder();
      }
    }
    if(el.size() > 0) {
        output += identation+"\nelse:";
        for (TreeNode* line: el){
          if(line->classType() == TreeNode::CONDITIONAL){
            ConditionalOperation* c = (ConditionalOperation*) line;
            output+="\n"+returnIfThen(c,identation);
          }else {
            identation+="  ";
            output+="\n"+identation+line->printPreOrder();
          }
    }
  }
    return output;
}

std::string ConditionalOperation::returnIfThen(ConditionalOperation* c, std::string identation){
    identation+="  ";
    std::string output = identation+"if: ";
    output+=c->condition->printPreOrder() + "\n" + identation + "then:";
      for(TreeNode* line: c->then){
        if(line->classType() == TreeNode::CONDITIONAL){
          ConditionalOperation* c = (ConditionalOperation*) line;
          output+="\n"+returnIfThen(c,identation);
        }else {
          identation+="  ";
          output+="\n"+identation+line->printPreOrder();
        }
      }
      if(c->el.size() > 0) {
          output += "\n"+ identation+"else:";
          for (TreeNode* line: c->el){
            if(line->classType() == TreeNode::CONDITIONAL){
              ConditionalOperation* c = (ConditionalOperation*) line;
              output+="\n"+returnIfThen(c,identation);
            }else {
              identation+="  ";
              output+="\n"+identation+line->printPreOrder();
            }
          }
      }
    return output;

}
