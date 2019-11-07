#include <bits/stdc++.h>
using namespace std;
#include "defs.h"





Prog::Prog(Decl_List* decl_list, Method_List* stmts){
  	this->decls = decl_list;
  	this->stmts = stmts;
}



Variable::Variable(string declType, string* var_name){
	this->declType = declType;
	this->var_name = var_name;
}

Variable::Variable(string declType, string* var_name,int value){
	this->declType = declType;
	this->var_name = var_name;
	this->value = value;
}

Variable::Variable(string declType, string* var_name,Expr* expr_var){
	this->declType = declType;
	this->var_name = var_name;
	this->expr_var = expr_var;
}

Method::Method(string* method_name,Arg_List* args,Block* block){
	
	this->method_name = method_name;
	this->args = args;
	this->block = block;
}

Method::Method(string* method_name,Block* block){
	
	this->method_name = method_name;
	this->args = new Arg_List;
	this->block = block;
}

Method::Method(string* dataType, string* method_name,Arg_List* args,Block* block){
	
	this->method_name = method_name;
	this->args = args;
	this->block = block;
	this->dataType = dataType;
}


Method::Method(string* dataType, string* method_name,Block* block){
	
	this->method_name = method_name;
	this->args = new Arg_List;
	this->block = block;
	this->dataType = dataType;
}


Block::Block(class Var_Decl_List* var_decl_list, class Statement_List* statement_list){
	this->var_decl_list = var_decl_list;
	this->statement_list = statement_list;
}


Assign_Stmt::Assign_Stmt(class Location* location, string* assign_op, class Expr* expr){
	this->location=location;
    this->assign_op=assign_op;
    this->expr=expr;
}

If_Stmt::If_Stmt(class Expr* expr, class Block* if_block){
	this->expr = expr;
	this->if_block = if_block;
}


If_Stmt::If_Stmt(class Expr* expr, class Block* if_block, class Block* else_block){
	this->expr = expr;
	this->if_block = if_block;
	this->else_block = else_block;
}	

Method_Call::Method_Call(string* method_name, class Expr_List* expr_list){
	this->method_name = method_name;
	this->expr_list = expr_list;
}

Method_Call::Method_Call(string* method_name){
	this->method_name = method_name;
	this->expr_list = new Expr_List;
}

Return_Stmt::Return_Stmt(class Expr* expr){
	this->has_return = true;
	this->return_expr = expr;
}

For_Stmt::For_Stmt(string* var,class Expr* expr1, class Expr* expr2, class Block* block){
	this->iter_var = var;
    this->start_expr = expr1;
    this->increment_expr = expr2;
    this->block = block;
}


Location::Location(string loc_type,string* var_name){
	this->loc_type = loc_type;
	this->var_name = var_name;
}

Location::Location(string loc_type,string* var_name, class Expr* expr){
	this->loc_type = loc_type;
	this->var_name = var_name;
	this->loc_expr = expr;
}


// Expr::Expr(string expr_type, class Location* loc){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->loc = loc;
// }

// Expr::Expr(string expr_type, class Method_Call* method){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->method = method;
// }	

// Expr::Expr(string expr_type, string* lit){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->lit = lit;
// }

// Expr::Expr(string expr_type, int num){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->num = num;
// }

// Expr::Expr(string expr_type, class Expr* lexpr, string* op, class Expr* rexpr){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->lhs_expr = lexpr;
// 	this->bin_op = op;
// 	this->rhs_expr = rexpr;
// }

// Expr::Expr(string expr_type, string* op, class Expr* expr){
// 	this->brackets = 0;
// 	this->expr_type = expr_type;
// 	this->uni_op = op;
// 	this->uni_expr = expr;
// }

Bin_Expr::Bin_Expr(class Expr* lhs, string* op, class Expr* rhs){
    this->lhs_expr = lhs;
    this->rhs_expr = rhs;
    this->bin_op = op;
}

Un_Expr::Un_Expr(string* op, class Expr* expr){
    this->uni_op = op;
    this->uni_expr = expr;
}

Literal::Literal(string* lit){
    this->lit = lit;
}

Literal::Literal(int num){
    this->num = num;
}

Callout_Stmt::Callout_Stmt(string* first){
	this->first_str = first;
}

Callout_Stmt::Callout_Stmt(string* first, class Callout_Args_List* callout_args_list){
	this->first_str = first;
	this->callout_args_list = callout_args_list;
}

Block_Stmt::Block_Stmt(Block* block){
	this->block = block;
}

Callout_Arg::Callout_Arg(string* str){
	this->arg_str = str;
}

Callout_Arg::Callout_Arg(class Expr* expr){
	this->arg_expr = expr;
}





void Decl_List::push_back(class Variable_List* var){
	decl_list.push_back(var);
	cnt++;
}

void Variable_List::push_back(class Variable* var){
	variable_list.insert(variable_list.begin(), var);
	cnt++;
}

void Method_List::push_back(class Method* method){
	
	
	method_list.insert(method_list.begin(),method);
	cnt++;
}

void Arg_List::push_back(string* type,string* name){
	
	// vector<pair<string*,string*>> temp;
	// temp.push_back(make_pair(type,name));
	// arg_list.extend(temp);
	// arg_list.insert(arg_list.end(),temp.begin(),temp.end());
	arg_list.push_back(make_pair(type,name));
	cout << *name<<endl;
	cnt++;
}

void Var_Decl_List::push_back(class Variable_List* var){
	var_decl_list.push_back(var);
	cnt++;
}

void Statement_List::push_back(class Statement* stmt){
	stmt_list.push_back(stmt);
	cnt++;
}

void Expr_List::push_back(class Expr* expr){
	expr_list.push_back(expr);
	cnt++;
}

void Callout_Args_List::push_back(class Callout_Arg* arg){
	callout_args.push_back(arg);
	cout << "PUSH BACKED into callout\n";
	cnt++;
}

bool Block::has_return(){
	vector<class Statement* > stmts = statement_list->stmt_list;
	for (int i = 0; i < (stmts).size(); i++) {
        if (stmts[i]->has_return) {
            return true;
        }
    }
	return false;
}

bool Block::has_break(){
	vector<class Statement* > stmts = statement_list->stmt_list;
	for (int i = 0; i < (stmts).size(); i++) {
        if (stmts[i]->has_break) {
            return true;
        }
    }
	return false;
}

bool Block::has_cont(){
	vector<class Statement* > stmts = statement_list->stmt_list;
	for (int i = 0; i < (stmts).size(); i++) {
        if (stmts[i]->has_cont) {
            return true;
        }
    }
	return false;
}

