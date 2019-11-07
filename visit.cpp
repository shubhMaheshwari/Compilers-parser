#include <bits/stdc++.h>
#include "defs.h"



using namespace std;

SomeVisitor::SomeVisitor() {};

void SomeVisitor::visit(class Location* node){
	cout << "location: " << (node->loc_type) <<": " ;
	cout<< *(node->var_name) <<endl;
	if(node->loc_type != "simple")
		node->loc_expr->accept(this);
}

void SomeVisitor::visit(class Prog* node) {
	cout << "\n\n Program node" << endl;
	node->decls->accept(this);
	node->stmts->accept(this);
}

void SomeVisitor::visit(class Decl_List* node){
	int cnt = 1;
	for(auto decl: node->decl_list){
		cout << "Field Decleration #" << cnt << ": ";
		decl->accept(this);
		cnt++;
	}
}

void SomeVisitor::visit(class Variable_List* node) {
	cout << "type: " << *(node->dataType) << " : ";
	
	for (auto vars: node->variable_list) {
		vars->accept(this); cout << ", ";
	}
	cout << endl;
}

void SomeVisitor::visit(class Variable* node) {
	cout << "Variable:" << node->declType << ": " << *(node->var_name) << ": ";
	if(node->declType == "array"){
		cout << "value: " << node->value;
	}
	
}

void SomeVisitor::visit(class Method_List* node) {
	cout << " Method Decleration List with size: "<< (node->method_list).size() << endl;
	
	int cnt = 1;
	for (auto decleration: node->method_list) {
		cout << "Method Decleration #" << cnt << ": ";
		decleration->accept(this);
		cnt++;
	}
	cout << endl;
}

void SomeVisitor::visit(class Method* node) {

	if(*(node->dataType) != "")
		cout << "Data Type: " << *(node->dataType) << "|";	
	else{
		cout << "Data Type: " << "VOID" << "|";	
	}
	cout << "Method name: " << *(node->method_name) << "|";
	cout << "Parameter list: ";
	node->args->accept(this);
	cout << " | Code block starts:\n" ;
	node->block->accept(this);
	cout << "Code block ends\n\n" ;
}


void SomeVisitor::visit(class Arg_List* node) {
	cout << "Arg List{" << endl;

	int cnt = 0;
	for(auto arg: node->arg_list){
		cout << *(arg.first)<< " "<< *(arg.second)<< ", ";
	}
	cout<<"}";
}

void SomeVisitor::visit(class Block* node) {
	node->var_decl_list->accept(this);
	node->statement_list->accept(this);
}

void SomeVisitor::visit(class Var_Decl_List* node){
	int num = 1;
	for (auto var_decl: node->var_decl_list) {
		var_decl->accept(this);
		cout << ", ";
		num++;
	}
}

void SomeVisitor::visit(class Statement_List* node){
	int stmt_num = 1;
	for (auto stmt: node->stmt_list) {
		cout << "Statement #" << stmt_num << ":" << endl;
		stmt->accept(this);
		stmt_num++;
	}
}
void SomeVisitor::visit(class Statement* node){
}

void SomeVisitor::visit(class Assign_Stmt* node) {
	cout << "Assign statement: ";
	node->location->accept(this); 
	cout << " " << *(node->assign_op) ;
	cout << " "; 
	node->expr->accept(this); 
	cout << endl;
}

void SomeVisitor::visit(class If_Stmt* node) {
	cout << "If-else statement:" << endl;
	cout << "condition: "; node->expr->accept(this); cout << endl;
	cout << "if-block: " ; node->if_block->accept(this); cout << endl;
	cout << "else-block: ";
	
	if(node->else_block != NULL)
		node->else_block->accept(this); 
	cout << endl;

}

void SomeVisitor::visit(class Method_Call* node){
	cout << "Method Call: ";
	cout << "method name: " << *(node->method_name);
	cout << "arg list: "; node->expr_list->accept(this);
	cout << endl;
}

void SomeVisitor::visit(class Expr_List* node){
	int expr_num = 1;
	for (auto &expr: node->expr_list) {
		cout << "Expr #" << expr_num << ":" << endl;
		expr->accept(this);
		expr_num++;
	}	
}

void SomeVisitor::visit(class Return_Stmt* node){
	cout << " return statement: return ";
	fflush(stdout);
	if(node->return_expr != NULL){
		node->return_expr->accept(this);
	}
}

void SomeVisitor::visit(class For_Stmt* node) {
	cout << " for statement: ";
	cout << "iter variable: " << *(node->iter_var); cout<<endl;
	cout << "initial value: "; node->start_expr->accept(this); cout<<endl;
	cout << "increment value: "; node->increment_expr->accept(this); cout<<endl;
	cout << "block: "; node->block->accept(this); cout<<endl;
	
}

void SomeVisitor::visit(class Block_Stmt* node){
	cout << " block statement:";
	node->block->accept(this);
}

void SomeVisitor::visit(class Continue_Stmt* node){
	cout << " continue statement"<<endl;
}

void SomeVisitor::visit(class Break_Stmt* node){
	cout << " break statement" << endl;
}

void SomeVisitor::visit(class Expr* node){
// 	if(node->brackets == 1)
// 		cout << "( ";
// 	cout << " expr statement: ";
// 	cout << "type of expr: " << node->expr_type <<": "<<endl;
// 	if(node->expr_type == "loc"){
// 		node->loc->accept(this);
// 	}
// 	else if(node->expr_type == "method"){
// 		node->method->accept(this);
// 	}
// 	else if(node->expr_type == "method"){
// 		node->method->accept(this);
// 	}
// 	else if(node->expr_type == "num"){
// 		cout << node->num ;
// 	}
// 	else if(node->expr_type == "bin"){
// 		cout << node->num ;
// 		node->lhs_expr->accept(this);
// 		cout << " "<< *(node->bin_op) << " ";
// 		node->rhs_expr->accept(this);
// 	}
// 	else if(node->expr_type == "uni"){
// 		cout << *(node->uni_op) << " ";
// 		node->uni_expr->accept(this);
// 	}
// 	if(node->brackets == 1)
// 		cout << " )";

}

void SomeVisitor::visit(class Bin_Expr* node){
	cout << "bin expr: "; node->lhs_expr->accept(this);
	cout << " " << *(node->bin_op) << " ";
	node->rhs_expr->accept(this); 
	cout<<endl;
}

void SomeVisitor::visit(class Un_Expr* node){
	cout << "Uni expr: " << *(node->uni_op) << " ";
	node->uni_expr->accept(this);
	cout<<endl; 
}

void SomeVisitor::visit(class Literal* node){
	if(node->lit == NULL){
		cout << "Num Lit: "<< node->num<< " ";
	}
	else{
		cout<<"String Lit: "<< *(node->lit) <<" ";
	}
}

void SomeVisitor::visit(class Callout_Stmt* node){
	cout << " callout statement" ; 
	cout << "Arguments: " << *(node->first_str);
	if(node->callout_args_list != NULL){
		cout<<", ";
		node->callout_args_list->accept(this);
	}
}

void SomeVisitor::visit(class Callout_Args_List* node){
	int num = 1;
	for (auto callout_arg: node->callout_args) {
		callout_arg->accept(this);
		cout<< ", ";
		num++;
	}		
}


void SomeVisitor::visit(class Callout_Arg* node){
	if(node->arg_expr == NULL)
		cout << *(node->arg_str) ;
	else
		node->arg_expr->accept(this);
}