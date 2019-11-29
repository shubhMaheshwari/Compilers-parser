// For semantic parsing we need to compute the AST of our code
// AST is calculated using our CFG. 
// This file contains the class declaration which are the nodes of the AST 
#include <bits/stdc++.h>
#ifndef __CLASS_DEF__
#define __CLASS_DEF__


using namespace std;

// Define Union here
union Node{

	// Basic
	class BaseNode* base_node; 
	class RootNode* root_node;
	class BlockNode* block_node;

	// Parameter
	class ParameterListNode* parameter_list;

	// Line
	class LineNode* line_node;
	class ImportNode* import_node; 
	class ClassNode* class_node;
	class FunctionNode* function_node; 
	class DeclarationNode* declaration_node;
	class AssignNode* assign_node; 
	class ExprNode* expr_node;
	class ExprListNode* expr_list_node; 
	class CondNode* cond_node; 
	class WhileNode* while_node; 
	class ForNode* for_node; 
	class ReturnNode* return_node; 
	class ContinueNode* continue_node; 
	class BreakNode* break_node; 

	// Location/Defination
	class LocationNode* location_node;		 
	class LocationListNode* location_list_node;

	// Expression Nodes
	class LiteralNode* literal_node;
	class VariableNode* variable_node;

	// Function Call 
	class CallMethodNode* call_method_node;
	class BinaryOperationNode* binary_operation_node;
	class NonOperationNode* not_operation_node;

	// Condition
	class IfNode* if_node;
	class ElseNode* else_node;

	string* lit; 
	int num;
	double fnum;

	Node(){

		// Basic
		base_node = NULL;
		root_node = NULL;
		block_node = NULL; 

		// Parameter List
		parameter_list = NULL;

		// Line
		line_node = NULL;
		import_node = NULL; 
		function_node = NULL; 
		class_node = NULL;
		declaration_node = NULL;
		assign_node = NULL; 
		expr_node = NULL; 
		expr_list_node = NULL;
		cond_node = NULL; 
		while_node = NULL; 
		for_node = NULL; 
		return_node = NULL; 
		continue_node = NULL; 
		break_node = NULL; 

		// Assign Node
		assign_node = NULL;
		location_node = NULL;
		location_list_node = NULL;

		// Expression Nodes
		expr_node = NULL;
		literal_node = NULL;
		variable_node = NULL;

		// Function Call 
		call_method_node = NULL;
		binary_operation_node = NULL;
		not_operation_node = NULL;		

		// Condition
		if_node = NULL;
		else_node = NULL;

		lit = NULL;
	} 

	~Node() = default;

};

typedef union Node YYSTYPE;

#define YYSTYPE_IS_DECLARED 1

// Define Fake Visitor class here
// Because the original visitor is in another file we need to pass a fake class to compile code.
class Visitor{
	public:
		Visitor(){};

		virtual void visit(class RootNode*){};
		virtual void visit(class BlockNode*){};
		virtual void visit(class ParameterListNode*){};
		virtual void visit(class LocationNode*){};
		virtual void visit(class LocationListNode*){};
		virtual void visit(class LineNode*){};
		virtual void visit(class ExprNode*){};
		virtual void visit(class ExprListNode*){};
		virtual void visit(class LiteralNode*){};
		virtual void visit(class VariableNode*){};
		virtual void visit(class CallMethodNode*){};
		virtual void visit(class BinaryOperationNode*){};
		virtual void visit(class NotOperationNode*){};
		virtual void visit(class ImportNode*){};
		virtual void visit(class ClassNode*){};
		virtual void visit(class FunctionNode*){};
		virtual void visit(class AssignNode*){};
		virtual void visit(class DeclarationNode*){};
		virtual void visit(class IfNode*){};
		virtual void visit(class ElseNode*){};
		virtual void visit(class CondNode*){};
		virtual void visit(class WhileNode*){};
		virtual void visit(class ForNode*){};
		virtual void visit(class ReturnNode*){};
		virtual void visit(class ContinueNode*){};
		virtual void visit(class BreakNode*){};
};

class SemanticVisitor: public Visitor{
	public:
		// Visitor for each node
		void visit(class RootNode*);
		void visit(class BlockNode*);
		void visit(class ParameterListNode*);
		void visit(class LocationNode*);
		void visit(class LocationListNode*);
		void visit(class LineNode*);
		void visit(class ExprNode*);
		void visit(class ExprListNode*);
		void visit(class LiteralNode*);
		void visit(class VariableNode*);
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
		
		SemanticVisitor(){};
};

enum class Op{
	ADD,MIN,MUL,DIV,MOD,COND_AND,COND_OR,OP_OR,OP_AND,OP_NOT,EQUALS,NOT_EQUALS,GTE,LTE,GT,LT 
};
string OpToString(Op);

// Base Class defination
class BaseNode{
	public:
		virtual void accept(Visitor* v) = 0;
		// virtual Value* codegen(){};
};

// Non-terminal classe

// Root node, main program
class RootNode:public BaseNode{
	public:
		class BlockNode* block; 	// Main block 
		RootNode(class BlockNode*);

		// [TODO]
		// Value* generateCode();
		// void output_llvm_ir();
		// Value* reportError();
		void accept(Visitor* v) { v->visit(this); };
};

class BlockNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		vector<class LineNode*> line_list;

		BlockNode(){}; // No initialization
		

		void push_back(class LineNode*); // Add a new expression to the list

		// [TODO]
		// Value* generateCode();
		// void output_llvm_ir();
		// Value* reportError();
		// void accept(ASTVisitor* v) { v->visit(this); };
};


// Parameter
class ParameterListNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		vector<class LocationNode*> parameter_list;
		ParameterListNode(){};
		void push_back(string* datatype, string* name, bool);
};

// Defination Node
class LocationNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };

		string* datatype;
		class VariableNode* variable;
		class ExprNode* array_size; 

		LocationNode(string*);
		LocationNode(string*, class ExprNode*);
		LocationNode(string*,string*);
		LocationNode(string*,string*, class ExprNode*);
};


// Basic building block for AST
class LineNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		LineNode(){}; // No initialization
};

// Expression
class ExprNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		ExprNode(){};
};

class ExprListNode:public BlockNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
			
		vector<class ExprNode*> expr_list;
		ExprListNode(){};

		void push_back(class ExprNode*);
};

class LiteralNode:public ExprNode{
	public:
		void accept(Visitor* v) { v->visit(this); };

		char datatype;
		int num;
		double fnum;
		char ch;
		string* s;

		LiteralNode(int);
		LiteralNode(double);
		LiteralNode(char);
		LiteralNode(string*);

};

class VariableNode:public ExprNode{
	// Variable used in an expression
	public:
		void accept(Visitor* v) { v->visit(this); };
		bool is_array; 
		string* name;
		class ExprNode* array_location; 
		VariableNode(string*,bool); 
		VariableNode(string*,class ExprNode*); 
};

class LocationListNode:public ExprNode{
	// Variable used in an expression
	public:
		void accept(Visitor* v) { v->visit(this); };
		vector<class LocationNode*> location_list;
		LocationListNode(){};

		void push_back(string*); 
		void push_back(string*, class ExprNode*); 
};

class CallMethodNode:public ExprNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		// Variable used in an expression
		string* name;
		class ExprListNode* expr_list;
		CallMethodNode(string*, class ExprListNode*); 
};

// Binary Operations
class BinaryOperationNode:public ExprNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprNode* expr1;
		class ExprNode* expr2;
		Op op;

		BinaryOperationNode(class ExprNode*,class ExprNode*,Op);
};

class NotOperationNode:public ExprNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprNode* expr;
		NotOperationNode(class ExprNode*);
};


// Imports
class ImportNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprListNode* import_list;
		ImportNode(class ExprListNode*);
};


// Class Defination 
class ClassNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		string* name;
		class BlockNode* block;
		ClassNode(string*, class BlockNode*);
};

// Function defination 
class FunctionNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		string* name;
		string* return_type; 
		class ParameterListNode* parameter_list;
		class BlockNode* block;

		FunctionNode(string*, string*, class ParameterListNode*, class BlockNode*);
};

// Assign defination 
class AssignNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class LocationNode* location;
		string* op;
		class ExprNode* value;
		AssignNode(class LocationNode*,string*,class ExprNode*);
};

// Variable Declaration
class DeclarationNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		string* datatype;
		class LocationListNode* location_list;
		DeclarationNode(string* ,class LocationListNode*);
};

// If-Else 
class IfNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprNode* condition;
		class BlockNode* block;

		IfNode(class ExprNode*,class BlockNode*);
};

class ElseNode:public BaseNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class BlockNode* block;

		ElseNode(){};
		ElseNode(class BlockNode*);
		
};

class CondNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class IfNode* if_block;
		class ElseNode* else_block;
		CondNode(class IfNode*, class ElseNode*);
};



// For loop
class WhileNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprNode* expr;
		class BlockNode* block;
		WhileNode(class ExprNode*, class BlockNode*);
};

// While Loop
class ForNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class AssignNode* assign;
		class ExprNode* expr;
		class AssignNode* update;
		class BlockNode* block;
		ForNode(class AssignNode*,class ExprNode*,class AssignNode*,class BlockNode*);
};

// Return Statement
class ReturnNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		class ExprNode* expr;
		ReturnNode(){};
		ReturnNode(class ExprNode*);
};

// Continue Statement
class ContinueNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		ContinueNode(){};
};

// Break Statement
class BreakNode:public LineNode{
	public:
		void accept(Visitor* v) { v->visit(this); };
		BreakNode(){};
};




#endif