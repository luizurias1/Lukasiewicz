#include "TreeNode.h"

Data::Type TreeNode::classToDataType(TreeNode::ClassType type) {
    switch(type) {
        case TreeNode::BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::FLOAT:
            return Data::FLOAT;
        case TreeNode::INTEGER:
            return Data::INTEGER;
        case TreeNode::ARRAY_BOOLEAN:
            return Data::BOOLEAN;
        case TreeNode::ARRAY_FLOAT:
            return Data::FLOAT;
        case TreeNode::ARRAY_INTEGER:
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
        case NOT_EQUAL:
            return "!=";
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

int Integer::getValue(){
  return value;
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

Array::Array(std::string id, Data::Type type, int size) : TreeNode(type){
    this->id = id;
    this->size = size;
    this->n = NULL;
}
Array::Array(std::string id, Data::Type type, TreeNode* n) : TreeNode(type){
    this->id = id;
    this->size = 0;
    this->n = n;
}

std::string Array::printInOrder() {
    return id +" (size: " + getSize() + ")";
}

TreeNode* Array::getNode(){
  return n;
}
std::string Array::printPreOrder() {
      return "[index] " + id +" "+ n->printPreOrder();
}

std::string Array::getSize(){
  return std::to_string(size);
}
TreeNode::ClassType Array::classType() const {
    return TreeNode::ARRAY;
}

Array::~Array() {
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
    return printPreOrder();
}

std::string VariableDeclaration::printPreOrder() {
    std::string output = typeToString(this->type);
    if (next->classType() == TreeNode::ARRAY) {
        output+= " array: " + next->printInOrder();
    }else if (next->classType() == TreeNode::BINARY_OPERATION){
        BinaryOperation *b = (BinaryOperation*) next;
        if (b->left->classType() == TreeNode::ARRAY){
            output+= " array: " + next->printInOrder();
        }else if(b->left->classType() == TreeNode::POINTER){
          output = typeToString(this->type);
          output += next->printInOrder();
      }else{
            output+= " var: ";
            output += next->printInOrder();
        }
    } else {
        output+= " var: ";
        output += next->printInOrder();
    }
    if (next->classType() == TreeNode::POINTER){
        output = typeToString(this->type);
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
        case Data::POINTER:
            return "int ref";
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
        case Data::POINTER:
            return "ref";
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
    return printPreOrder();
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

Pointer::Pointer(std::string id, Data::Type type, ADDRESS a, int count, Declaration declaration, Array* array) : TreeNode(type) {
    this->id = id;
    this->a = a;
    this->count = count;
    this->type = type;
    this->declaration = declaration;
    this->array = array;
}

Pointer::~Pointer() {
}

TreeNode::ClassType Pointer::classType() const {
    return TreeNode::POINTER;
}

std::string Pointer::printInOrder() {
  std::string output = numberOfRefs(0);
    if(declaration == Declaration::UNIQUE){
      if(array != NULL){
        output += " array: ";
        output += array->printInOrder();
        return output;
      }
      output += " var: ";
      output += id;
      return  output;
    }else{
      output = id;
      return  output;
    }
}

std::string Pointer::printPreOrder() {
    std::string output = "";
    if(a == ADDRESS::ADDR){
      if(array != NULL){
        output = array->printPreOrder();
        output += "[addr] ";
        return output;
      }
      output += id;
      output += " [addr] ";
    }else{
      output = numberOfRefs(1) + id + " ";
    }

    return  output;
}

std::string Pointer::numberOfRefs(int number){
  int i;
  std::string output = "";
  for(i = 1; i <= count; i++){
    if(number == 0)
      output += " ref";
    else
      output += "[ref] ";
  }
  return output;
}

Pointer::ADDRESS Pointer::typeOfAddress(){
  return a;
}

void Pointer::setSize(int s){
  count = s;
}

Pointer::Declaration Pointer::getDeclaration(){
  return declaration;
}

void Pointer::setDeclaration(Pointer::Declaration declar){
  declaration = declar;
}
