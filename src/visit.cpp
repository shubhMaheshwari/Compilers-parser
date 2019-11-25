// This file contains the code to implement visitor design pattern 
// Main aim of the code present here is to implement the function when a visitor calls a particular node

#include <bits/stdc++.h>
#include "AST.h"
#include "parser.tab.h"
#include "visit.h"
using namespace std;
using namespace llvm;

string ptab = "";

// LLVM code generator
// Basics
static llvm::LLVMContext Context;			// Context maintains all the global variables and needs to be defined
static llvm::IRBuilder<> Builder(Context);	// Builds all the SSA forms needed for each node
static std::unique_ptr<Module> MainModule = std::make_unique<Module>("JIT", Context);
static std::map<std::string, Value *> NamedValues;

llvm::Value* ret;

Value *LogErrorV(const char *Str) {
  fprintf(stderr,"%s\n",Str);
  return nullptr;
}


// Convert to LLVM type
// llvm::Type* CodeGenVisitor::TypeToLLVMType(Type t) {
//   if (t == Type::INT_TYPE) {
//     return llvm::Type::getInt32Ty(context);
//   } else if (t == Type::BOOLEAN_TYPE) {
//     return llvm::Type::getInt1Ty(context);
//   } else if (t == Type::VOID_TYPE) {
//     return llvm::Type::getVoidTy(context);
//   } else {
//     std::cerr << "UNHANDLED TYPE!\n";
//   }
// }


MainVisitor::MainVisitor(){

	// //Basics

	// // For loop
	// for_inside_bb = NULL;
	// for_after_bb = NULL;
}


// Root node containing the main blocks
void MainVisitor::visit(class RootNode* node){
	printf("%sRootNode\n",ptab.c_str());
	node->block->accept(this);
	printf("\n");
}

void MainVisitor::visit(class BlockNode* node){
	// Iterate over all root node lines 
	for (auto x = node->line_list.begin(); x != node->line_list.end(); x++){ 
		(*x)->accept(this);
	}
}
void MainVisitor::visit(class ParameterNode* node){
	// Print all parameters in a line
	printf(" %s %s,",node->datatype->c_str(),node->name->c_str());
}

void MainVisitor::visit(class ParameterListNode* node){
	// Call parameters list
	ptab.push_back('\t');
	printf("%sParameterListNode:",ptab.c_str());
	for (auto x = node->parameter_list.begin(); x != node->parameter_list.end(); x++){ 
		(*x)->accept(this);
	}
	printf("\n");
	ptab.pop_back();
}
void MainVisitor::visit(class LocationNode* node){

	if(node->datatype != nullptr)
		printf("%s ",node->datatype->c_str());

	printf("%s",node->name->c_str());

	if(node->array_location != nullptr){
		printf("[");
		node->array_location->accept(this);	
		printf("]");
	}

}
void MainVisitor::visit(class LineNode* node){
}

void MainVisitor::visit(class ExprNode* node){
}

void MainVisitor::visit(class ExprListNode* node){
	for (auto x = node->expr_list.begin(); x != node->expr_list.end(); x++){ 
		(*x)->accept(this);
	}
}

void MainVisitor::visit(class LiteralNode* node){
	if(node->datatype=='i'){
		printf("int %d ",node->num);
		ret = ConstantInt::get(Context, APInt(32,node->num,true));
		return;
	}

	else if(node->datatype=='f'){
		printf("float %lf ",node->fnum);
		ret = ConstantInt::get(Context, APInt(64,node->fnum,true));
		return;
	}

	else if(node->datatype=='c'){
		printf("char %c ",node->ch);
		ret = ConstantInt::get(Context, APInt(8, node->ch, false));
		return;

	}
	
	else if(node->datatype=='s'){
		printf("string %s ",node->s->c_str());
		// ret = Builder.CreateGlobalStringPtr(StringRef(node->s));
		ret = ConstantInt::get(Context, APInt(32,node->num,true));

		return;
	}
		

}

void MainVisitor::visit(class VariableNode* node){

	printf("%s",node->name->c_str());


	// Already declared node
	Value *V = global_vars_table[*(node->name)].first;

	if(node->array_location != nullptr){
		printf("[");
		node->array_location->accept(this);
		printf("]");
	}
	printf(",");

	ret = V;


}
void MainVisitor::visit(class VariableListNode* node){
	for(auto x=node->variable_list.begin(); x != node->variable_list.end(); x++)
		(*x)->accept(this);	

}
void MainVisitor::visit(class CallMethodNode* node){
	ptab.push_back('\t');
	printf("CallMethodNode %s: ",node->name->c_str());

	// Create Callee
	Function* Callee = MainModule->getFunction(*(node->name));
	if (!Callee){
    	LogErrorV("Unknown function referenced");
	}

    // if (Callee->arg_size() != node->expr_list->expr_list.size()){
    // 	LogErrorV("Incorrect # arguments passed");
    // }

    // Create argument list
	vector<Value *> ArgsV;
	for (auto x = node->expr_list->expr_list.begin(); x != node->expr_list->expr_list.end(); x++){ 
		(*x)->accept(this);
		if(!ret){
			LogErrorV("Argument to CallMethodNode not parsed");
		}
		ArgsV.push_back(ret);


	}

	// node->expr_list->accept(this); // Not needed anymore as calling the function here iteself
	ret = Builder.CreateCall(Callee, ArgsV, "calltmp");
	ptab.pop_back();
}

void MainVisitor::visit(class BinaryOperationNode* node){
	node->expr1->accept(this);
	Value* LHS = ret; // Store the value returned by left node
	printf(" %s ",OpToString(node->op).c_str());
	node->expr2->accept(this);
	Value* RHS = ret; // Store the value returned by right node
	if(!LHS || !RHS){
		ret = nullptr;
		return; 
	}	

	  switch (node->op) {
		case Op::ADD:
		  ret = Builder.CreateAdd(LHS, RHS, "binop_add");
		  break;
		case Op::MIN:
		  ret = Builder.CreateSub(LHS, RHS, "binop_sub");
		  break;
		case Op::MUL:
		  ret = Builder.CreateMul(LHS, RHS, "binop_mult");
		  break;
		case Op::DIV:
		  ret = Builder.CreateSDiv(LHS, RHS, "binop_div");
		  break;
		case Op::MOD:
		  ret = Builder.CreateSRem(LHS, RHS, "binop_mod");
		  break;
		case Op::EQUALS:
		  ret = Builder.CreateICmpEQ(LHS, RHS, "binop_eq");
		  break;
		case Op::NOT_EQUALS:
		  ret = Builder.CreateICmpNE(LHS, RHS, "binop_ne");
		  break;
		case Op::LT:
		  ret = Builder.CreateICmpSLT(LHS, RHS, "binop_lt");
		  break;
		case Op::LTE:
		  ret = Builder.CreateICmpSLE(LHS, RHS, "binop_le");
		  break;
		case Op::GT:
		  printf("sdaskdkjahdkashdskjdh\n");
		  ret = Builder.CreateICmpSGT(LHS, RHS, "binop_gt");
		  printf("not reaching here\n");
		  break;
		case Op::GTE:
		  ret = Builder.CreateICmpSGE(LHS, RHS, "binop_ge");
		  break;
		case Op::OP_OR:
		  ret = Builder.CreateOr(LHS, RHS, "binop_or");
		  break;
		case Op::OP_AND:
		  ret = Builder.CreateAnd(LHS, RHS, "binop_and");
		  break;
		default:
		  ret = nullptr;
		  string p_err = "Binary Operation:";
		  p_err.append(OpToString(node->op).c_str());
		  p_err.append(" not implemented");
		  LogErrorV(p_err.c_str());	
	  }
	return;
}
void MainVisitor::visit(class NotOperationNode* node){
	printf("NotOperationNode:");
	node->expr->accept(this);
}
void MainVisitor::visit(class ImportNode* node){
	ptab.push_back('\t');
	printf("%sImportNode\n",ptab.c_str());
	ptab.pop_back();
}
void MainVisitor::visit(class ClassNode* node){
	ptab.push_back('\t');
	printf("%sClassNode %s: \n",ptab.c_str(),node->name->c_str());
	node->block->accept(this);

	ptab.pop_back();
}
void MainVisitor::visit(class FunctionNode* node){
	ptab.push_back('\t');
	printf("%sFunctionNode %s: \n",ptab.c_str(),node->name->c_str());

	node->parameter_list->accept(this);
	node->block->accept(this);

	ptab.pop_back();
}
void MainVisitor::visit(class AssignNode* node){
	ptab.push_back('\t');

	// Print defination
	printf("%sAssignNode:",ptab.c_str());
	node->variable_name->accept(this);
	printf(" %s ",node->op->c_str());
	node->value->accept(this);
	printf("\n");

	ptab.pop_back();

}
void MainVisitor::visit(class DeclarationNode* node){
	ptab.push_back('\t');
	printf("%sDeclarationNode: ",ptab.c_str());
	node->variable_list->accept(this);
	printf("\n");
	ptab.pop_back();
}
void MainVisitor::visit(class IfNode* node){
	ptab.push_back('\t');

	printf("%sIfNode:",ptab.c_str());
	node->condition->accept(this);
	printf("\n");
	node->block->accept(this);
	ptab.pop_back();
}
void MainVisitor::visit(class ElseNode* node){
	ptab.push_back('\t');
	printf("%sElseNode:\n",ptab.c_str());
	node->block->accept(this);
	ptab.pop_back();
}
void MainVisitor::visit(class CondNode* node){
	ptab.push_back('\t');
	printf("%sCondNode\n",ptab.c_str());
	node->if_block->accept(this);
	node->else_block->accept(this);
	ptab.pop_back();
}
void MainVisitor::visit(class WhileNode* node){
	ptab.push_back('\t');
	printf("%sWhileNode: ",ptab.c_str());
	node->expr->accept(this);
	printf("\n");
	node->block->accept(this);
	ptab.pop_back();
}
void MainVisitor::visit(class ForNode* node){
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
void MainVisitor::visit(class ReturnNode* node){
	ptab.push_back('\t');

	printf("%sReturnNode:",ptab.c_str());

	if(node->expr != nullptr) 
		node->expr->accept(this);
	printf("\n");
	ptab.pop_back();
}
void MainVisitor::visit(class ContinueNode* node){
	ptab.push_back('\t');
	printf("%sContinueNode\n",ptab.c_str());
	ptab.pop_back();
}
void MainVisitor::visit(class BreakNode* node){
	ptab.push_back('\t');
	printf("%sBreakNode\n",ptab.c_str());
	ptab.pop_back();
}