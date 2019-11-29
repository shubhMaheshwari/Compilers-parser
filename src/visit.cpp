// This file contains the code to implement visitor design pattern 
// Main aim of the code present here is to implement the function when a visitor calls a particular node

#include <bits/stdc++.h>
#include "AST.h"
#include "parser.tab.h"
#include "visit.h"
using namespace std;
using namespace llvm;
using namespace llvm::sys;

// LLVM code generator
// Basics
static llvm::LLVMContext Context;			// Context maintains all the global variables and needs to be defined
static llvm::IRBuilder<> Builder(Context);	// Builds all the SSA forms needed for each node
static std::unique_ptr<Module> MainModule = std::make_unique<Module>("JIT", Context);
static std::map<std::string, Value *> NamedValues;


// Global Variables 
llvm::Value* ret; // Return Variable per visit
llvm::Value* function_return_value; // Return variable after every function call

VarTable vars; // Variable table (holds all the variables currently being used)
unordered_map<string,unsigned int> array_lengths;

// While Loop number
unsigned int while_loop_number=0;


// Method to log errors
Value *LogErrorV(const char *Str) {
  fprintf(stderr,"[CodeGenerationError]:%s\n",Str);
 	
  return nullptr;
}

llvm::Constant* TypeToDefaultValue(string datatype) {
	if(datatype[0]=='i'){
		return ConstantInt::get(Context, APInt(32,0,true));
	}

	else if(datatype[0]=='f'){
		return ConstantInt::get(Context, APInt(64,0.0,true));
	}

	else if(datatype[0]=='c'){
		return ConstantInt::get(Context, APInt(8, 0, false));
	}
	
	else if(datatype[0]=='s'){
		return ConstantInt::get(Context, APInt(64, StringRef("0"), false));
	}
	else{
		LogErrorV("Type not defined");
	}
}


// Convert string variable to LLVM variable type
llvm::Type* TypeToLLVMType(string t) {
  if (t[0] == 'i') {		//int
	return llvm::Type::getInt32Ty(Context);
  } else if (t[0] == 'f') {	//float
	return llvm::Type::getDoubleTy(Context);
  } else if (t[0] == 'c') {	//char
	return llvm::Type::getInt8Ty(Context);
  } else if (t[0] == 'b') {	// Boolean
	return llvm::Type::getInt1Ty(Context);
  } else if (t[0] == 's') {	// String
	return llvm::Type::getInt1Ty(Context);
  } else if (t[0] == 'v') {	// Void
	return llvm::Type::getVoidTy(Context);
  } else {
	LogErrorV("UNHANDLED DATATYPE during defination!\n");
	return nullptr;
  }
}


// Get parent of current function
Function* CurrentFunction() {
	return Builder.GetInsertBlock()->getParent();
}

bool CurrentBlockDone() {
	return Builder.GetInsertBlock()->getTerminator();
}

// Return Constant value  of int
Constant* GetConstIntN(unsigned N, int value) {
	return ConstantInt::get(
		llvm::Type::getIntNTy(Context, N), APInt(N, value, true));
}


// Create allocation for new function
AllocaInst *CreateEntryBlockAlloca(Function *fn, llvm::Type* type, const string &id, Value* array_size = nullptr) {
	IRBuilder<> temp_builder(&fn->getEntryBlock(),
								 fn->getEntryBlock().begin());
	return temp_builder.CreateAlloca(type, nullptr,id.c_str());
}

// Call C functions
Value* GetCallout(const std::string& id, llvm::FunctionType *prototype, std::vector<Value*> args) {
	Function *fn = MainModule->getFunction(id);
	if (!prototype) {
		LogErrorV("could not create prototype\n returning nullptr\n");
		return nullptr;
	}
	if (!fn) {
		fn = Function::Create(prototype, Function::ExternalLinkage, id, MainModule.get());
	}
	Value *callee = fn;
	if (fn->getFunctionType() != prototype) {
		callee = Builder.CreateBitCast(fn, PointerType::get(prototype, 0), "BIT_CAST_CALLOUT");
	}

	if (!callee) {
		LogErrorV("could not get callee\n returning nullptr\n");
		return nullptr;
	}

	return Builder.CreateCall(callee, args);
}

void GenErrorAndExitInst(const std::string& error) {
	FunctionType *exit_prototype = FunctionType::get(
		llvm::Type::getVoidTy(Context),
		{llvm::Type::getInt32Ty(Context)},
		false
	);
	FunctionType *printf_prototype = FunctionType::get(
		llvm::Type::getInt32Ty(Context),
		{llvm::Type::getInt8PtrTy(Context)},
		false
	);
	Value* error_string = Builder.CreateGlobalStringPtr(error);
	GetCallout("printf", printf_prototype, {error_string});
	GetCallout("exit", exit_prototype, {GetConstIntN(32, 1)});
	llvm::Type* return_type = CurrentFunction()->getReturnType();
	if (return_type->isVoidTy()) {
		Builder.CreateRetVoid();
	} else {
		Builder.CreateRet(Constant::getNullValue(return_type));
	}
}

// Finding index in array
Value* MainVisitor::GEPFromLocationNode(VariableNode *node) {
	if (node->array_location == nullptr) {
		if (vars[*(node->name)].second->isArrayTy()) {
			return Builder.CreateGEP(vars[*(node->name)].first, ArrayRef<Value *>({GetConstIntN(32, 0), GetConstIntN(32, 0)}), "pointer_to_start_of_array");
		} else {
			return vars[*(node->name)].first;
		}
	} else {
		Value* index_value; 
		node->array_location->accept(this); 
		index_value = ret;
		if (!index_value) {
			std::cerr << "index value is null!\nreturning nullptr.\n";
			return nullptr;
		}

		Value *check_lt_length = Builder.CreateICmpSLT(index_value, GetConstIntN(32, array_lengths[*(node->name)]), "gep_check_lt_length");
		Value *check_nonneg = Builder.CreateICmpSGE(index_value, GetConstIntN(32, 0), "gep_check_nonneg");
		Value *cond_value = Builder.CreateAnd(check_lt_length, check_nonneg, "gep_bounds_check");
		if (!cond_value) {
			std::cerr << "gep_bounds_check val is null\n returning nullptr\n";
			return nullptr;
		}

		BasicBlock *out_of_bounds_bb = BasicBlock::Create(Context, "gep_bounds_check_fail", CurrentFunction());
		BasicBlock *bounds_check_success_bb = BasicBlock::Create(Context, "gep_succesful_bounds_check");
		Builder.CreateCondBr(cond_value, bounds_check_success_bb, out_of_bounds_bb);
		Builder.SetInsertPoint(out_of_bounds_bb);
		GenErrorAndExitInst("\nruntime error: array access to " + *(node->name) + " out of bounds.\n");

		CurrentFunction()->getBasicBlockList().push_back(bounds_check_success_bb);
		Builder.SetInsertPoint(bounds_check_success_bb);
		return Builder.CreateGEP(vars[*(node->name)].first, ArrayRef<Value *>({GetConstIntN(32, 0), index_value}), "indexed location");
	}
}


// Add Variable to the current scope 
void AddScopedVar(std::string& name, std::pair<Value*,llvm::Type*> alloca, VarTable& shadow_list) {
	if (vars.count(name) > 0) {
		// two params with same name
		if (shadow_list.count(name) == 0) {
			shadow_list[name] = vars[name]; // shadowing
		}
	} else {
		// store a placeholder so we know to remove the id later
		shadow_list[name] = {nullptr, nullptr};
	}
	vars[name] = alloca;
}


// Restore variables after bb done
void RestoreShadowedVars(const VarTable& shadow_list) {
	for (auto pr : shadow_list) {
		std::cout << "Restoring " << pr.first << '\n';
		if (pr.second.first == nullptr && pr.second.second == nullptr) {
			vars.erase(pr.first);
		} else {
			vars[pr.first] = pr.second; // unshadow old variables
		}
	}
}

MainVisitor::MainVisitor(){
	// Create print function
}


// Root node containing the main blocks
void MainVisitor::visit(class RootNode* node){

	// Create all nodes
	node->block->accept(this);

  	InitializeAllTargetInfos();
  	InitializeAllTargets();
  	InitializeAllTargetMCs();
  	InitializeAllAsmParsers();
  	InitializeAllAsmPrinters();

  	auto TargetTriple = sys::getDefaultTargetTriple();
  	MainModule->setTargetTriple(TargetTriple);

  	string Error;
  	auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  	// Print an error and exit if we couldn't find the requested target.
  	// This generally occurs if we've forgotten to initialise the
  	// TargetRegistry or we have a bogus target triple.
  	if (!Target) {
	    errs() << Error;
   		 return ;
  	}

	auto CPU = "generic";
  	auto Features = "";

  	TargetOptions opt;
  	auto RM = Optional<Reloc::Model>();
  	auto TheTargetMachine =
    Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  	MainModule->setDataLayout(TheTargetMachine->createDataLayout());

	auto Filename = "output.o";
  	std::error_code EC;
  	raw_fd_ostream dest(Filename, EC, fs::F_None);

  	if (EC) {
    	errs() << "Could not open file: " << EC.message();
    	return ;
	}

  	legacy::PassManager pass;
  	auto FileType = TargetMachine::CGFT_ObjectFile;

  	if (TheTargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    	errs() << "TheTargetMachine can't emit a file of this type";
	    return ;
  	}

  	pass.run(*MainModule);
  	dest.flush();

  	outs() << "Wrote " << Filename << "\n";




	// Print errors 
	printf("===============================LLVM Generated Code=======================\n");

	MainModule->print(outs(), nullptr);
	printf("=========================================================================\n");
}

void MainVisitor::visit(class BlockNode* node){

	VarTable shadow_list;
	// Iterate over all root node lines 
	for (auto x : node->line_list){ 
		x->accept(this);
		// if (CurrentBlockDone()) {
		// 	break; // no code paths to this location.
		// }
	}

	// [TODO]: Nothing to restore
	RestoreShadowedVars(shadow_list);
}

void MainVisitor::visit(class ParameterListNode* node){
	// Call parameters list
	for (auto x : node->parameter_list){ 
		x->accept(this);
	}
}

void MainVisitor::visit(class LocationNode* node){
	VarTable shadow_list;
	Type *type;
	Constant *constant;
	if(!node->datatype){
		if(vars.count(*(node->variable->name)) >0){
			type = vars[*(node->variable->name)].second;
			constant = TypeToDefaultValue("i");
		}
		else{
			string no_init_err = "Variable " + *(node->variable->name) + "  not initilizated";
			LogErrorV(no_init_err.c_str());
		}
	}
	else{
		type = TypeToLLVMType(*(node->datatype));
		constant = TypeToDefaultValue(*(node->datatype) );

	}
	GlobalVariable *ptr;
	if (node->variable->is_array) {
		type = ArrayType::get(type, node->array_size);
		ptr = (GlobalVariable*) MainModule->getOrInsertGlobal(*(node->variable->name), type);
		ptr->setInitializer(ConstantArray::get((llvm::ArrayType*)type,
		ArrayRef<Constant *>(std::vector<Constant*> (node->array_size, constant ))));

	} else {
		ptr = (GlobalVariable*) MainModule->getOrInsertGlobal(*(node->variable->name), type);
		ptr->setInitializer(constant);
	}
	AddScopedVar(*(node->variable->name), {ptr, type}, shadow_list);
}

void MainVisitor::visit(class ExprListNode* node){
	for (auto x : node->expr_list){ 
		x->accept(this);
	}
}

void MainVisitor::visit(class LiteralNode* node){
	if(node->datatype=='i'){
		ret = ConstantInt::get(Context, APInt(32,node->num,true));
		return;
	}

	else if(node->datatype=='f'){
		ret = ConstantInt::get(Context, APInt(64,node->fnum,true));
		return;
	}

	else if(node->datatype=='c'){
		ret = ConstantInt::get(Context, APInt(8, node->ch, false));
		return;
	}
	
	else if(node->datatype=='s'){
		ret = Builder.CreateGlobalStringPtr(StringRef( *(node->s) ));
		// ret = ConstantInt::get(Context, APInt(32,node->num,true));
		return;
	}
}

void MainVisitor::visit(class VariableNode* node){
	

	ret = nullptr;
	// Create the variable if not defined
	Value* elem_ptr = GEPFromLocationNode(node);
	if (!elem_ptr) {
		LogErrorV("elem_ptr is null!\n");
		return;
	}
	if (vars[*(node->name)].second->isArrayTy() && node->array_location == nullptr) {
		ret = elem_ptr;
	} else {
		ret = Builder.CreateLoad(elem_ptr, *(node->name) + (node->array_location != nullptr ? "[]" : ""));
	}
}



void MainVisitor::visit(class LocationListNode* node){
	for(auto x:node->location_list)
		x->accept(this);	
}

void MainVisitor::visit(class CallMethodNode* node){

	
	// Create argument list
	vector<Value *> ArgsV;
	for (auto x : node->expr_list->expr_list){
		x->accept(this);
		if(!ret){
			LogErrorV("Argument to CallMethodNode not parsed");
		}
		ArgsV.push_back(ret);
	}


	// Check if printf function
	if(node->name->compare("printf") == 0){
		FunctionType *printf_prototype = FunctionType::get(
			llvm::Type::getInt32Ty(Context),
			{llvm::Type::getInt8PtrTy(Context),llvm::Type::getInt32Ty(Context)},
			false);
		GetCallout("printf", printf_prototype, ArgsV);
		return;
	}


	// Create Callee
	Function* Callee = MainModule->getFunction(*(node->name));
	if (!Callee){
		string undefined_function = "Unknown function" + *(node->name) + " referenced";
		LogErrorV(undefined_function.c_str());
	}

	// Check argument requiremnts
	if (Callee->arg_size() != node->expr_list->expr_list.size()){
		string wrong_argument_passed_err = "Incorrect # arguments passed to " + *(node->name);
		wrong_argument_passed_err += to_string(Callee->arg_size()) + " required ";
		wrong_argument_passed_err += to_string(node->expr_list->expr_list.size()) + " passed";
		LogErrorV(wrong_argument_passed_err.c_str());
	}

	// Call function
	ret = Builder.CreateCall(Callee, ArgsV, "calltmp");
}

void MainVisitor::visit(class BinaryOperationNode* node){

	node->expr1->accept(this);
	Value* LHS = ret; // Store the value returned by left node
	node->expr2->accept(this);
	Value* RHS = ret; // Store the value returned by right node
	if(!LHS || !RHS){
		ret = nullptr;
		LogErrorV("[CodeGenerationError]: LHS or RHS nullptr in binary expression node");
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
		  ret = Builder.CreateICmpSGT(LHS, RHS, "binop_gt");
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
	Value* value;
	node->expr->accept(this);
	value = ret;
	ret = Builder.CreateNot(value,"not_op");
}
void MainVisitor::visit(class ImportNode* node){
}
void MainVisitor::visit(class ClassNode* node){
	node->block->accept(this);
}
void MainVisitor::visit(class FunctionNode* node){
	// Parse all parameters and create an array of parameters for llvm
	vector<llvm::Type*> param_types;
	// For no parameters to function 
	if(node->parameter_list->parameter_list.size() == 0)
		param_types.push_back(TypeToLLVMType("v"));
	else
		// Otherwise
		for (auto v : node->parameter_list->parameter_list) {
			param_types.push_back(TypeToLLVMType(*(v->datatype)));

		}
	FunctionType *prototype = FunctionType::get(
	TypeToLLVMType(*(node->return_type)),
	param_types,
	false);

	Function *fn = Function::Create(prototype, node->name->compare("main") == 0 ? Function::ExternalLinkage : Function::PrivateLinkage, node->name->c_str(), MainModule.get());
	if (!fn) {
		LogErrorV("Function prototype has nullptr value.\n");
		ret = nullptr;
		return;
	}

	// Create basic block for function
	BasicBlock *bb = BasicBlock::Create(Context, "entry", fn);
	Builder.SetInsertPoint(bb);

	// Add parameters to a variable list
	VarTable shadow_list;
	if(node->parameter_list->parameter_list.size() > 0){
		int idx = 0;
		for (auto &arg : fn->args()) {
			arg.setName(node->parameter_list->parameter_list[idx]->variable->name->c_str()); // Set variable name
			auto type = prototype->getParamType(idx);
			AllocaInst *alloca = CreateEntryBlockAlloca(fn, type, arg.getName());  // Allocate memory to variables
			Builder.CreateStore(&arg, alloca);
			AddScopedVar(*(node->parameter_list->parameter_list[idx]->variable->name), {alloca, type}, shadow_list); // Add to current list of variable
			idx++;
		}
	}
	// Create the bb required by the classes
	node->block->accept(this);

	// needed because every basic block *must* have a terminator
	// the return statement has no semantic effect.
	if (!CurrentBlockDone()) {
		if( node->return_type->c_str()[0] == 'v')
			Builder.CreateRetVoid();
		else
			GenErrorAndExitInst("\nruntime error: reached end of function with non-void return.\n");

	}
	verifyFunction(*fn, &errs());
	// fn->print(errs());
	// ret = fn;

	// [TODO]
	RestoreShadowedVars(shadow_list);

}
void MainVisitor::visit(class AssignNode* node){
	Value *value;
	node->value->accept(this); 
	value = ret;
	node->location->accept(this); 
	Value *elem_ptr = GEPFromLocationNode(node->location->variable);

	string op = *(node->op);

	if( op.compare("=") == 0){

	}
	else if( op.compare("+=") == 0  || op.compare("=+") == 0){
		Value *cur_value = Builder.CreateLoad(elem_ptr, node->location->variable->name);
		value = Builder.CreateAdd(cur_value, value, "plus_assign");
	}
	else if( op.compare("-=") == 0  || op.compare("=-") == 0){
		Value *cur_value = Builder.CreateLoad(elem_ptr, node->location->variable->name);
		value = Builder.CreateSub(cur_value, value, "minus_assign");
	}
	else if( op.compare("*=") == 0  || op.compare("=*") == 0){
		Value *cur_value = Builder.CreateLoad(elem_ptr, node->location->variable->name);
		value = Builder.CreateMul(cur_value, value, "mul_assign");
	}
	else if( op.compare("/=") == 0  || op.compare("=/") == 0){
		Value *cur_value = Builder.CreateLoad(elem_ptr, node->location->variable->name);
		value = Builder.CreateAdd(cur_value, value, "div_assign");
	}
	else if( op.compare("%=") == 0  || op.compare("=%") == 0){
		Value *cur_value = Builder.CreateLoad(elem_ptr, node->location->variable->name);
		value = Builder.CreateSRem(cur_value, value, "mod_assign");
	}
	else{
		std::cerr << "Unknown Assignment:" <<  op << "\n";
	}
	Builder.CreateStore(value, elem_ptr);
	ret = value;

}


void MainVisitor::visit(class DeclarationNode* node){
	node->location_list->accept(this);
}

void MainVisitor::visit(class CondNode* node){

	// Check the condition
	Value *cond_value; 
	node->if_block->condition->accept(this); 
	cond_value = ret;
	if (!cond_value) {
		LogErrorV("Cond is null!\n");
		return;
	}

	// Create bb for each block 
	BasicBlock *then_bb = BasicBlock::Create(Context, "then", CurrentFunction());
	BasicBlock *merge_bb = BasicBlock::Create(Context, "if_then_merge");
	BasicBlock *else_bb = (node->else_block->block != nullptr) ? BasicBlock::Create(Context, "else") : merge_bb;
	Builder.CreateCondBr(cond_value, then_bb, else_bb);
	Builder.SetInsertPoint(then_bb);

	// Call the if node
	Value* then_value;
	node->if_block->block->accept(this); 
	then_value = ret;
	if (!then_value) {
		LogErrorV("Then value is null!\n");
		return;
	}
	// Only if condition
	if (!CurrentBlockDone()) {
		Builder.CreateBr(merge_bb);
	}

	PHINode *PN = Builder.CreatePHI(then_value->getType(), 2, "iftmp");
	PN->addIncoming(then_value, then_bb);

	if (node->else_block->block != nullptr) {
		CurrentFunction()->getBasicBlockList().push_back(else_bb);
		Builder.SetInsertPoint(else_bb);
		Value* else_value; 
		node->else_block->block->accept(this); 
		else_value = ret;
		if (!else_value) {
			LogErrorV("else value is null!\n");
			return;
		}
		if (!CurrentBlockDone()) {
			Builder.CreateBr(merge_bb);
		}
	  	PN->addIncoming(else_value, else_bb);
	}

	CurrentFunction()->getBasicBlockList().push_back(merge_bb);
	Builder.SetInsertPoint(merge_bb);
	if (!CurrentBlockDone()) {
		Builder.CreateBr(merge_bb);
	}

  	ret = PN;
}

void MainVisitor::visit(class WhileNode* node){
	VarTable shadow_list;

	string loop_name = to_string(while_loop_number); // For later reference	
	while_loop_number++;

	BasicBlock *old_for_internal_bb = for_internal_bb;
	BasicBlock *old_for_after_bb = for_after_bb;
	BasicBlock *for_bb = BasicBlock::Create(Context, "while_" + loop_name , CurrentFunction());
	for_internal_bb = BasicBlock::Create(Context, "while_internal_" + loop_name);
	for_after_bb = BasicBlock::Create(Context, "while_after_" + loop_name);

	Value* end_value; 
	node->expr->accept(this);
	end_value = ret;
	if (!end_value) {
		LogErrorV("Expr value is null!\n");
		return;
	}	

	Value *brcond = end_value;
	Builder.CreateCondBr(brcond, for_bb, for_after_bb);
	Builder.SetInsertPoint(for_bb);
	
	node->block->accept(this);

	if (!CurrentBlockDone()) {
		Builder.CreateBr(for_internal_bb);
	}
	CurrentFunction()->getBasicBlockList().push_back(for_internal_bb);
	Builder.SetInsertPoint(for_internal_bb);
	Builder.CreateCondBr(end_value,for_bb, for_after_bb);

	CurrentFunction()->getBasicBlockList().push_back(for_after_bb);
	Builder.SetInsertPoint(for_after_bb);

	for_internal_bb = old_for_internal_bb;
	for_after_bb = old_for_after_bb;
	RestoreShadowedVars(shadow_list);
	while_loop_number--;
}
void MainVisitor::visit(class ForNode* node){


	string loop_name = *(node->assign->location->variable->name); // For later reference
	
	// Allocate memory to the new variable
	VarTable shadow_list;
	// Get the assignment value to compare
	Value* start_value;
	node->assign->accept(this);
	start_value = ret;

	if (!start_value) {
		LogErrorV("For loop initilization failed,start value is null!\n");
		return;
	}


	BasicBlock *old_for_internal_bb = for_internal_bb;
	BasicBlock *old_for_after_bb = for_after_bb;
	BasicBlock *for_bb = BasicBlock::Create(Context, "for_" + loop_name , CurrentFunction());
	for_internal_bb = BasicBlock::Create(Context, "for_internal_" + loop_name);
	for_after_bb = BasicBlock::Create(Context, "for_after_" + loop_name);

	Value* end_value; 
	node->expr->accept(this);

	end_value = ret;
	if (!end_value) {
		LogErrorV("end value is null!\n");
		return;
	}	

	
	Value *brcond = end_value;
	Builder.CreateCondBr(brcond, for_bb, for_after_bb);
	Builder.SetInsertPoint(for_bb);
	
	node->block->accept(this);

	if (!CurrentBlockDone()) {
		Builder.CreateBr(for_internal_bb);
	}
	CurrentFunction()->getBasicBlockList().push_back(for_internal_bb);
	Builder.SetInsertPoint(for_internal_bb);
	node->update->accept(this);
	Value* i_val = ret;
	Builder.CreateStore(i_val, vars[loop_name]);
	Builder.CreateCondBr(end_value,for_bb, for_after_bb);

	CurrentFunction()->getBasicBlockList().push_back(for_after_bb);
	Builder.SetInsertPoint(for_after_bb);

	for_internal_bb = old_for_internal_bb;
	for_after_bb = old_for_after_bb;
	RestoreShadowedVars(shadow_list);


}

void MainVisitor::visit(class ReturnNode* node){
	if(node->expr != nullptr){ 
		Value* value;
		node->expr->accept(this);
		value = ret; 
		Builder.CreateRet(value);
	}
	else{
		Builder.CreateRetVoid();
	}
}

void MainVisitor::visit(class ContinueNode* node){
	if (for_internal_bb == nullptr) {
		LogErrorV("Continue operation occured outside any loop!\n");
		return;
	}
	Builder.CreateBr(for_internal_bb);
}
void MainVisitor::visit(class BreakNode* node){
	if (for_internal_bb == nullptr) {
		LogErrorV("Break operation occured outside any loop!\n");
		return;
	}
	Builder.CreateBr(for_after_bb);
}