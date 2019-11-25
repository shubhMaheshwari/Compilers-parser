// Initialization and push back code for running when parser calls these nodes

#include <bits/stdc++.h>
using namespace std;
#include "AST.h"

// Main Node
RootNode::RootNode(BlockNode* block){
	this->block = block;
}

// Block functions
void BlockNode::push_back(class LineNode* line){
	this->line_list.push_back(line);
}

// Parameter for function and classes
ParameterNode::ParameterNode(string* datatype, string* name){
	this->datatype = datatype;
	this->name = name;
}

void ParameterListNode::push_back(string* datatype, string* name){

	class ParameterNode* par = new ParameterNode(datatype,name);
	this->parameter_list.push_back(par);

}

// Define varaiable already declared
LocationNode::LocationNode(string* name){
	this->name = name;
}

// Define varaiable already declared
LocationNode::LocationNode(string* name,class ExprNode* expr){
	this->name = name;
	this->array_location = array_location;
}

// Define varaiable already declared
LocationNode::LocationNode(string* datatype,string* name){
	this->datatype = datatype;
	this->name = name;
}

// Define varaiable already declared
LocationNode::LocationNode(string* datatype,string* name, class ExprNode* expr){
	this->datatype = datatype;
	this->name = name;
	this->array_location = expr;
}

// Import 
ImportNode::ImportNode(class VariableListNode* import_list){
	this->import_list = import_list;
}


// Class Nodes
ClassNode::ClassNode(string* name, class BlockNode* block){
	this->name = name;
	this->block = block;	
}


// Function Nodes
FunctionNode::FunctionNode(string* name, class ParameterListNode* parameter_list, class BlockNode* block){
	this->name = name;
	this->parameter_list = parameter_list;	
	this->block = block;
}

ReturnNode::ReturnNode(class ExprNode* expr){
	this->expr = expr;
}

// Declaration 
DeclarationNode::DeclarationNode(class VariableListNode* variable_list){
	this->variable_list = variable_list;
}

// Expresssion Nodes
void ExprListNode::push_back(class ExprNode* expr){
	this->expr_list.push_back(expr);
}



// Literal Nodes
LiteralNode::LiteralNode(int num){
	this->num = num;
	this->datatype = 'i';
}
LiteralNode::LiteralNode(double fnum){
	this->fnum = fnum;
	this->datatype = 'f';
}
LiteralNode::LiteralNode(char ch){
	this->ch = ch;
	this->datatype = 'c';
}
LiteralNode::LiteralNode(string* s){
	this->s = s;
	this->datatype = 's';
}

// Variables
VariableNode::VariableNode(string* name){
	this->is_array = false;
	this->name = name;
}

VariableNode::VariableNode(string* name,class ExprNode* array_location)
{
	this->is_array = true;
	this->name = name;
	this->array_location = array_location;
}

void VariableListNode::push_back(string* name){
	class VariableNode* v = new VariableNode(name);
	this->variable_list.push_back(v);
}

void VariableListNode::push_back(string* name, class ExprNode* array_location){

	class VariableNode* v = new VariableNode(name,array_location);
	this->variable_list.push_back(v);
}

// Function Calls
CallMethodNode::CallMethodNode(string* name, class ExprListNode* expr_list){
	this->name = name;
	this->expr_list = expr_list;

}

// Define operation to print
std::string OpToString(Op op) {
  switch (op) {
    case Op::ADD:
      return "+";
    case Op::MIN:
      return "-";
    case Op::MUL:
      return "*";
    case Op::DIV:
      return "/";
    case Op::MOD:
      return "%";
    case Op::EQUALS:
      return "==";
    case Op::NOT_EQUALS:
      return "!=";
    case Op::LT:
      return "<";
    case Op::LTE:
      return "<=";
    case Op::GT:
      return ">";
    case Op::GTE:
      return ">=";
    case Op::OP_OR:
      return "||";
    case Op::OP_AND:
      return "&&";
    case Op::COND_OR:
      return "|";
    case Op::COND_AND:
      return "&";

    default:
      return "Operation Not defined";
  }
}

// Binary operation
BinaryOperationNode::BinaryOperationNode(class ExprNode* expr1,class ExprNode* expr2, Op op){
	this->expr1 = expr1;
	this->expr2 = expr2;
	this->op = op;
}

NotOperationNode::NotOperationNode(class ExprNode* expr){
	this->expr = expr;
}

// Define operation to print
std::string AssignOpToString(AssignOp op) {
  switch (op) {
    
    case AssignOp::UNI_ADD:
    	return "+=";
    case AssignOp::UNI_MIN:
    	return "-=";
    case AssignOp::UNI_MUL:
    	return "*=";
    case AssignOp::UNI_DIV:
    	return "/=";
    case AssignOp::UNI_MOD:
    	return "%=";
    default:
      return "Operation Not defined";
  }
}

// Assign Variable
AssignNode::AssignNode(class LocationNode* variable_name,string* op,class ExprNode* expr){
	this->variable_name = variable_name;
	this->op = op;
	this->value = expr;

}

// Condition
IfNode::IfNode(class ExprNode* expr, class BlockNode* block){
	this->condition = expr;
	this->block = block;
}

ElseNode::ElseNode(class BlockNode* block){
	this->block = block;
}

CondNode::CondNode(class IfNode* if_block,class ElseNode* else_block){
	this->if_block = if_block;
	this->else_block = else_block;	
}

WhileNode::WhileNode(class ExprNode* expr, class BlockNode* block){
	this->expr = expr;
	this->block = block;
}

ForNode::ForNode(class AssignNode* assign,class ExprNode* expr,class AssignNode* update,class BlockNode* block){
	this->assign = assign;
	this->expr = expr;
	this->update = update;
	this->block = block;
}