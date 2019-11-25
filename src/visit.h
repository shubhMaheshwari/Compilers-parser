#include<bits/stdc++.h>
#include "AST.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#ifndef __Visit_DEF__
#define __Visit_DEF__

class MainVisitor: public Visitor{
	public:
		// Visitor for each node
		void visit(class RootNode*);
		void visit(class BlockNode*);
		void visit(class ParameterNode*);
		void visit(class ParameterListNode*);
		void visit(class LocationNode*);
		void visit(class LineNode*);
		void visit(class ExprNode*);
		void visit(class ExprListNode*);
		void visit(class LiteralNode*);
		void visit(class VariableNode*);
		void visit(class VariableListNode*);
		void visit(class CallMethodNode*);
		void visit(class BinaryOperationNode*);
		void visit(class NotOperationNode*);
		void visit(class ImportNode*);
		void visit(class ClassNode*);
		void visit(class FunctionNode*);
		void visit(class AssignNode*);
		void visit(class DeclarationNode*);
		void visit(class IfNode*);
		void visit(class ElseNode*);
		void visit(class CondNode*);
		void visit(class WhileNode*);
		void visit(class ForNode*);
		void visit(class ReturnNode*);
		void visit(class ContinueNode*);
		void visit(class BreakNode*);
		
		MainVisitor();


	private:
		using VarTable = std::unordered_map<std::string,std::pair<llvm::Value*,llvm::Type*>>;
		

		// Variables 
  		// llvm::Type* TypeToLLVMType(Type t); // Convert each variable to LLVM format
		VarTable global_vars_table; // Create a global variable location for the program to add global variables, function name and arrays


		llvm::BasicBlock *for_inside_bb, *for_after_bb; // We can define basic block for innermost for loop, and when to end the for loop



};

// Log llvm errors
llvm::Value *LogErrorV(const char *Str);


#endif