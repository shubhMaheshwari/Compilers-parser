// This file contains the code to implement visitor design pattern 
// Main aim of the code present here is to implement the function when a visitor calls a particular node

#include <bits/stdc++.h>
#include "AST.h"
#include "parser.tab.h"
using namespace std;

string ptab = "";

// Root node containing the main blocks
void SemanticVisitor::visit(class RootNode* node){
	printf("===============================Semantic Generated ===========================\n");
	printf("%sRootNode\n",ptab.c_str());
	node->block->accept(this);
	printf("=============================================================================\n");

}

void SemanticVisitor::visit(class BlockNode* node){
	// Iterate over all root node lines 
	for (auto x : node->line_list){ 
		x->accept(this);
		printf("\n%s");
	}
}
void SemanticVisitor::visit(class ParameterNode* node){
	// Print all parameters in a line
	printf(" %s %s,",node->datatype->c_str(),node->name->c_str());
}

void SemanticVisitor::visit(class ParameterListNode* node){
	// Call parameters list
	ptab.push_back('\t');
	printf("%sParameterListNode:",ptab.c_str());
	for (auto x : node->parameter_list){ 
		x->accept(this);
	}
	printf("\n");
	ptab.pop_back();
}
void SemanticVisitor::visit(class LocationNode* node){

	if(node->datatype != nullptr)
		printf("%s ",node->datatype->c_str());

	printf("%s",node->name->c_str());

	if(node->array_location != nullptr){
		printf("[");
		node->array_location->accept(this);	
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
void SemanticVisitor::visit(class VariableListNode* node){
	for(auto x:node->variable_list)
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
	ptab.push_back('\t');
	printf("%sImportNode\n",ptab.c_str());
	ptab.pop_back();
}
void SemanticVisitor::visit(class ClassNode* node){
	ptab.push_back('\t');
	printf("%sClassNode %s: \n",ptab.c_str(),node->name->c_str());
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class FunctionNode* node){
	ptab.push_back('\t');
	printf("%sFunctionNode %s: \n",ptab.c_str(),node->name->c_str());

	// Print All functions for ast
	node->parameter_list->accept(this);
	// Create the bb required by the classes
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class AssignNode* node){
	ptab.push_back('\t');
	// Print defination
	printf("%sAssignNode:",ptab.c_str());
	node->variable_name->accept(this);
	printf(" %s ",node->op->c_str());
	node->value->accept(this);
	printf("\n");
	ptab.pop_back();
}
void SemanticVisitor::visit(class DeclarationNode* node){
	ptab.push_back('\t');
	printf("%sDeclarationNode: ",ptab.c_str());
	node->variable_list->accept(this);
	printf("\n");
	ptab.pop_back();
}
void SemanticVisitor::visit(class IfNode* node){
	ptab.push_back('\t');
	printf("%sIfNode:",ptab.c_str());
	node->condition->accept(this);
	printf("\n");
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class ElseNode* node){
	ptab.push_back('\t');
	printf("%sElseNode:\n",ptab.c_str());
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class CondNode* node){
	ptab.push_back('\t');
	printf("%sCondNode\n",ptab.c_str());

	node->if_block->accept(this);
	if(node->else_block->block != nullptr)
		node->else_block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class WhileNode* node){
	ptab.push_back('\t');
	printf("%sWhileNode: ",ptab.c_str());
	node->expr->accept(this);
	printf("\n");
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class ForNode* node){
	ptab.push_back('\t');
	printf("%sForNode:\n",ptab.c_str());
	node->assign->accept(this);
	printf("%s\tEndStep:",ptab.c_str());
	node->expr->accept(this);
	printf("\n");
	node->update->accept(this);
	printf("\n");
	node->block->accept(this);
	ptab.pop_back();
}
void SemanticVisitor::visit(class ReturnNode* node){
	ptab.push_back('\t');
	printf("%sReturnNode:",ptab.c_str());
	if(node->expr != nullptr) 
		node->expr->accept(this);
	printf("\n");
	ptab.pop_back();
}
void SemanticVisitor::visit(class ContinueNode* node){
	ptab.push_back('\t');
	printf("%sContinueNode\n",ptab.c_str());
	ptab.pop_back();
}
void SemanticVisitor::visit(class BreakNode* node){
	ptab.push_back('\t');
	printf("%sBreakNode\n",ptab.c_str());
	ptab.pop_back();
}