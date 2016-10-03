#include "TreeNode.h"

Data::Type TreeNode::classToDataType(TreeNode::ClassType type) {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::FLOAT:
            return Data::FLOAT;
        case TreeNode::INTEGER:
            return Data::INTEGER;
        default:
            return Data::UNKNOWN;
    }
}

std::string TreeNode::toString(TreeNode::ClassType type) {
    return toString(classToDataType(type));
}
std::string TreeNode::toString(Data::Type type) {
    switch(type) {
        case Data::BOOLEAN:
            return "boolean";
        case Data::FLOAT:
            return "float";
        case Data::INTEGER:
            return "integer";
        default:
            return "unknown";
    }
}

std::string TreeNode::toShortString(TreeNode::ClassType type) {
    return toShortString(classToDataType(type));
}
std::string TreeNode::toShortString(Data::Type type) {
    switch(type) {
        case Data::BOOLEAN:
            return "bool";
        case Data::FLOAT:
            return "float";
        case Data::INTEGER:
            return "int";
        default:
            return "unknown";
    }
}

Vector::Vector() {
}

Vector::~Vector() {
}

int Vector::size() const {
    return internalVector.size();
}

TreeNode* Vector::popFront() {
    TreeNode* node = internalVector.front();
    internalVector.erase(internalVector.begin());
    return node;
}

void Vector::pushFront(TreeNode* node) {
    internalVector.insert(internalVector.begin(), node);
}

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

ConditionalOperation::ConditionalOperation(TreeNode* condition, Vector* then, Vector* el) : TreeNode(Data::UNKNOWN) {
    this->condition = condition;
    this->then = then->internalVector;
    this->el = el->internalVector;
}

ConditionalOperation::ConditionalOperation(TreeNode* condition, Vector* then) : TreeNode(Data::UNKNOWN) {
    this->condition = condition;
    this->then = then->internalVector;
}

ConditionalOperation::~ConditionalOperation() {
}

TreeNode::ClassType ConditionalOperation::classType() const {
    return TreeNode::CONDITIONAL;
}

TreeNode* ConditionalOperation::getCondition() {
    return this->condition;
}

std::string ConditionalOperation::printInOrder() {
    return this->printPreOrder();
}

std::string ConditionalOperation::printPreOrder() {
    std::string output = "if: ";
    std::string identation = "";
    output += condition->printPreOrder();
    if(output.back() == ' ')
        output = output.substr(0, output.length()-1);
    output += "\nthen:";

    for (TreeNode* line: then) {
        if (line->classType() == TreeNode::CONDITIONAL) {
            ConditionalOperation* c = (ConditionalOperation*) line;
            output+="\n"+returnIfThen(c, identation);
        } else if (line->classType() == TreeNode::LOOP_DECLARATION) {
            LoopDeclaration* body_local = (LoopDeclaration*) line;
            body_local->setTab(identation.length()/2 + 1);
            output += '\n'+body_local->printPreOrder();
        } else if (line->classType() == TreeNode::FUNCTION) {
            Function* body_local = (Function*) line;
            body_local->tab = identation.length()/2 + 1;
            output += '\n'+body_local->printPreOrder();
        } else {
            output += "\n  " + line->printPreOrder();
            if(output.back() == ' ')
            output = output.substr(0, output.length()-1);
        }
    }
    
    if(el.size() > 0) {
        output += identation+"\nelse:";
        for (TreeNode* line: el) {
            if(line->classType() == TreeNode::CONDITIONAL) {
                ConditionalOperation* c = (ConditionalOperation*) line;
                output += "\n"+returnIfThen(c,identation);
            }else if (line->classType() == TreeNode::LOOP_DECLARATION) {
                LoopDeclaration* body_local = (LoopDeclaration*) line;
                body_local->setTab(identation.length()/2 + 1);
                output += '\n'+body_local->printPreOrder();
            }else if (line->classType() == TreeNode::FUNCTION) {
                Function* body_local = (Function*) line;
                body_local->tab = identation.length()/2 + 1;
                output += '\n'+body_local->printPreOrder();
            } else {
                output += "\n  " + line->printPreOrder();
                if(output.back() == ' ')
                    output = output.substr(0, output.length()-1);
            }
        }
    }
    return output;
}

std::string ConditionalOperation::returnIfThen(ConditionalOperation* c, std::string identation) {
    identation += "  ";
    std::string output = identation+"if: ";
    output += c->condition->printPreOrder();
    if(output.back() == ' ')
      output = output.substr(0, output.length()-1);
    output += "\n" + identation + "then:";

    for(TreeNode* line: c->then) {
      if(line->classType() == TreeNode::CONDITIONAL) {
        ConditionalOperation* c = (ConditionalOperation*) line;
        output+="\n"+returnIfThen(c,identation);
      }else if (line->classType() == TreeNode::LOOP_DECLARATION) {
        LoopDeclaration* body_local = (LoopDeclaration*) line;
        body_local->setTab(identation.length()/2 + 1);
        output += '\n'+body_local->printPreOrder();
      } else if (line->classType() == TreeNode::FUNCTION) {
        Function* body_local = (Function*) line;
        body_local->tab = identation.length()/2 + 1;
        output += '\n'+body_local->printPreOrder();
      } else {
        output+="\n"+identation+"  "+line->printPreOrder();
        if(output.back() == ' ')
          output = output.substr(0, output.length()-1);
      }
    }

    if(c->el.size() > 0) {
        output += "\n"+ identation+"else:";
        for (TreeNode* line: c->el) {
          if(line->classType() == TreeNode::CONDITIONAL) {
            ConditionalOperation* c = (ConditionalOperation*) line;
            output+="\n"+returnIfThen(c,identation);
          } else if (line->classType() == TreeNode::LOOP_DECLARATION) {
            LoopDeclaration* body_local = (LoopDeclaration*) line;
            body_local->setTab(identation.length()/2 + 1);
            output += '\n'+body_local->printPreOrder();
          } else if (line->classType() == TreeNode::FUNCTION) {
            Function* body_local = (Function*) line;
            body_local->tab = identation.length()/2 + 1;
            output += '\n'+body_local->printPreOrder();
          } else {
            output+="\n"+identation+"  "+line->printPreOrder();
            if(output.back() == ' ')
              output = output.substr(0, output.length()-1);
          }
        }
    }
    return output;
}

LoopDeclaration::LoopDeclaration(TreeNode* init, TreeNode* test, TreeNode* interation, Vector* body) : TreeNode(Data::UNKNOWN) {
    this->init = init;
    this->test = test;
    this->interation = interation;
    this->body = body->internalVector;
    this->tab = 0;
}

LoopDeclaration::~LoopDeclaration() {
}

TreeNode::ClassType LoopDeclaration::classType() const {
    return TreeNode::LOOP_DECLARATION;
}

std::string LoopDeclaration::printInOrder() {
    std::string identation = getTab();
    std::string output = identation;
    output += "for: ";
    if (init != NULL) {
        output += init->printPreOrder();
        output = output.substr(0, output.size()-1);
    }
    output += ", ";

    output += test->printPreOrder();
    output = output.substr(0, output.size()-1);

    output += ", ";
    if (interation != NULL) {
        output += interation->printPreOrder();
    }
    output += "\n";
    output += getTab();
    output += "do:";

    if (body.size() > 0) {
        int i;
        for (i = 0; i < body.size(); i ++) {
            output += "\n";
            if (body[i]->classType() == BINARY_OPERATION) {
                identation += "  ";
                output += identation;
                output += body[i]->printPreOrder();
            }
            if (body[i]->classType() == LOOP_DECLARATION) {
                LoopDeclaration* body_local = (LoopDeclaration*) body[i];
                body_local->setTab(tab + 1);
                output += body_local->printPreOrder();
            } else if (body[i]->classType() == FUNCTION) {
                Function* body_local = (Function*) body[i];
                body_local->tab = tab + 1;
                output += body_local->printPreOrder();
            }
        }
    } else {
        output += "\n";
    }

    return output;
}

std::string LoopDeclaration::printPreOrder() {
    std::string identation = getTab();
    std::string output = identation;
    output += "for: ";
    if (init != NULL) {
        output += init->printPreOrder();
        output = output.substr(0, output.size()-1);
    }
    output += ", ";

    output += test->printPreOrder();
    output = output.substr(0, output.size()-1);

    output += ", ";
    if (interation != NULL) {
        output += interation->printPreOrder();
        if(output.back() == ' ')
            output = output.substr(0, output.length()-1);
    }
    output += "\n";
    output += getTab();
    output += "do:";
    ConditionalOperation* c = NULL;
    
    for (int i = 0; i < body.size(); i ++) {
        output += "\n";

        switch(body[i]->classType()) {
            case TreeNode::LOOP_DECLARATION:
                ((LoopDeclaration*) body[i])->setTab(tab + 1);
                output += body[i]->printPreOrder();
                break;
            case TreeNode::CONDITIONAL:
                c = (ConditionalOperation*) body[i];
                output+= c->returnIfThen(c,identation);
                break;
            case TreeNode::FUNCTION:
                ((LoopDeclaration*) body[i])->tab = tab + 1;;
                output += body[i]->printPreOrder();
                break;
            default:
                output += identation+"  "+body[i]->printPreOrder();
                if(output.back() == ' ')
                    output = output.substr(0, output.length()-1);
                break;
        }
    }
    return output;
}

void LoopDeclaration::setTab(int number) {
    tab = number;
}

std::string LoopDeclaration::getTab() {
    std::string tabulation = "";
    for (int i = 1; i <= tab; i++) {
        tabulation += "  ";
    }
    return tabulation;
}

Function::Function(std::string id, Vector* params, Vector* body, TreeNode* returnValue) : TreeNode(returnValue->dataType()) {
    this->id = id;
    this->params = params->internalVector;
    this->body = body->internalVector;
    this->returnValue = returnValue;
    this->tab = 0;
}

Function::~Function() {
}

TreeNode::ClassType Function::classType() const {
    return TreeNode::FUNCTION;
}

std::string Function::printInOrder() {
    return this->printPreOrder();
}

std::string Function::printPreOrder() {
    std::string identation = getTab();
    std::string output = identation;
    output += TreeNode::toShortString(returnValue->dataType()) + " fun: " + id + " (params: ";
    
    if(params.size() > 0) {
        output += TreeNode::toShortString(params.front()->dataType()) + " " + ((Variable*) params.front())->getId();
        
        for(int i = 1; i < params.size(); i++) {
            output += ", " + TreeNode::toShortString(params[i]->dataType()) + " " + ((Variable*) params[i])->getId();
        }
    }
    output += ")\n";
    
    ConditionalOperation* c = NULL;

    for(int i = 0; i < body.size(); i++) {
        switch(body[i]->classType()) {
            case TreeNode::FUNCTION:
                ((Function*) body[i])->tab = tab+1;
                output += body[i]->printPreOrder();
                break;
            case TreeNode::LOOP_DECLARATION:
                ((LoopDeclaration*) body[i])->setTab(tab+1);
                output += body[i]->printPreOrder();
                break;
            case TreeNode::CONDITIONAL:
                c = (ConditionalOperation*) body[i];
                output += c->returnIfThen(c, identation);
                break;
            default:
                output += identation + "  " + body[i]->printPreOrder();
                if(output.back() == ' ')
                    output = output.substr(0, output.length()-1);
                break;
        }
        
        output += "\n";
    }
    
    output += identation + "  ret " + returnValue->printPreOrder();
    if(output.back() == ' ')
        output = output.substr(0, output.length()-1);
    return output;
}

std::string Function::getTab() {
    std::string tabulation = "";
    for(int i = 1; i <= tab; i++) {
        tabulation += "  ";
    }
    return tabulation;
}

FunctionCall::FunctionCall(std::string id, Vector* params) : TreeNode(Data::UNKNOWN) {
    this->id = id;
    this->params = params->internalVector;
}

FunctionCall::~FunctionCall() {
}

TreeNode::ClassType FunctionCall::classType() const {
    return TreeNode::FUNCTION_CALL;
}

std::string FunctionCall::printInOrder() {
    return this->printPreOrder();
}

std::string FunctionCall::printPreOrder() {
    std::string output = id + "[" + std::to_string(params.size()) + " params]";
    
    if(params.size() > 0) {
        output += " ";
        for(int i = 0; i < params.size(); i++)
            output += params[i]->printPreOrder();
    }
    
    return output;
}