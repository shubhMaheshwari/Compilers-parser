// This file contains the code to implement visitor design pattern 
// Main aim of the code present here is to implement the function when a visitor calls a particular node

#include <bits/stdc++.h>
#include "AST.h"
#include "parser.tab.h"
using namespace std;

string ptab = "";

// Root node containing the main blocks
void SemanticVisitor::visit(class RootNode* node){
	printf("===============================Semantic Generated============================\n");
	printf("%sRootNode",ptab.c_str());
	node->block->accept(this);
	printf("\n");
	printf("=============================================================================\n");

}

void SemanticVisitor::visit(class BlockNode* node){
	// Iterate over all root node lines 
	ptab.push_back('\t');
	for (auto x : node->line_list){ 
		printf("\n%s",ptab.c_str());
		x->accept(this);

	}
	ptab.pop_back();
}


void SemanticVisitor::visit(class ParameterListNode* node){
	// Call parameters list
	ptab.push_back('\t');
	printf("\n%sParameterListNode:",ptab.c_str());
	for (auto x : node->parameter_list){ 
		x->accept(this);
	}
	ptab.pop_back();
}
void SemanticVisitor::visit(class LocationNode* node){

	if(node->datatype != nullptr)
		printf("%s ",node->datatype->c_str());

	node->variable->accept(this);

	if(node->array_size != NULL){
		printf("[");
		node->array_size->accept(this);
		printf("]");
	}

}
void SemanticVisitor::visit(class LineNode* node){
}

void SemanticVisitor::visit(class ExprNode* node){
}

void SemanticVisitor::visit(class ExprListNode* node){
	for (auto x : node->expr_list){ 
		x->accept(this);
		printf(",");
	}
}

void SemanticVisitor::visit(class LiteralNode* node){
	if(node->datatype=='i')
		printf("int %d ",node->num);

	else if(node->datatype=='f')
		printf("float %lf ",node->fnum);

	else if(node->datatype=='c')
		printf("char %c ",node->ch);
	
	else if(node->datatype=='s')
		printf("string %s ",node->s->c_str());
}

void SemanticVisitor::visit(class VariableNode* node){

	printf("%s",node->name->c_str());

	if(node->array_location != nullptr){
		printf("[");
		node->array_location->accept(this);
		printf("]");
	}
	printf(",");
}
void SemanticVisitor::visit(class LocationListNode* node){
	for(auto x:node->location_list)
		x->accept(this);	
}
void SemanticVisitor::visit(class CallMethodNode* node){
	ptab.push_back('\t');
	printf("CallMethodNode %s: ",node->name->c_str());


	node->expr_list->accept(this); // Not needed anymore as calling the function here iteself
	ptab.pop_back();
}

void SemanticVisitor::visit(class BinaryOperationNode* node){

	node->expr1->accept(this);
	printf(" %s ",OpToString(node->op).c_str());
	node->expr2->accept(this);
}
void SemanticVisitor::visit(class NotOperationNode* node){
	printf("NotOperationNode:");
	node->expr->accept(this);
}
void SemanticVisitor::visit(class ImportNode* node){
	printf("ImportNode");
}
void SemanticVisitor::visit(class ClassNode* node){
	printf("ClassNode %s: ",node->name->c_str());
	node->block->accept(this);
}
void SemanticVisitor::visit(class FunctionNode* node){
	printf("FunctionNode %s %s: ",node->return_type->c_str(), node->name->c_str());

	// Print All functions for ast
	node->parameter_list->accept(this);
	// Create the bb required by the classes
	node->block->accept(this);
}
void SemanticVisitor::visit(class AssignNode* node){
	// Print defination
	printf("AssignNode: ");
	node->location->accept(this);
	printf(" %s ",node->op->c_str());
	node->value->accept(this);
}
void SemanticVisitor::visit(class DeclarationNode* node){
	printf("DeclarationNode: ");
	node->location_list->accept(this);
}
void SemanticVisitor::visit(class IfNode* node){
	printf("\n%sIfNode: ",ptab.c_str());
	node->condition->accept(this);
	node->block->accept(this);
}
void SemanticVisitor::visit(class ElseNode* node){
	printf("\n%sElseNode:",ptab.c_str());
	node->block->accept(this);
}
void SemanticVisitor::visit(class CondNode* node){
	printf("CondNode:");

	node->if_block->accept(this);
	if(node->else_block->block != nullptr)
		node->else_block->accept(this);
}
void SemanticVisitor::visit(class WhileNode* node){
	printf("WhileNode: ");
	node->expr->accept(this);
	node->block->accept(this);
}
void SemanticVisitor::visit(class ForNode* node){
	printf("ForNode:");
	ptab.push_back('\t');
	printf("\n%s",ptab.c_str());
	node->assign->accept(this);
	printf("\n%sEndStep:",ptab.c_str());
	node->expr->accept(this);
	printf("\n%s",ptab.c_str());
	node->update->accept(this);
	printf("\n%sLoop Block",ptab.c_str());
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class ReturnNode* node){
	printf("ReturnNode: ");
	if(node->expr != nullptr) 
		node->expr->accept(this);
}
void SemanticVisitor::visit(class ContinueNode* node){
	printf("ContinueNode");
}
void SemanticVisitor::visit(class BreakNode* node){
	printf("BreakNode");
}