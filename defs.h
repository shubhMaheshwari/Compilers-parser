#include <bits/stdc++.h>

#ifndef __CLASS_DEF__
#define __CLASS_DEF__
#include <bits/stdc++.h>


using namespace std;


union Node{
    class Prog* prog;
    class Decl_List* decl_list;
    class Variable_List* variable_list;
    class Variable* variable;
    class Method_List* method_list;
    class Method* method;
    class Arg_List* arg_list;
    class Block* block;
    class Var_Decl_List* var_decl_list;
    class Statement_List* statement_list;
    class Statement* statement;
    class Assign_Stmt* assign_stmt;
    class If_Stmt* if_stmt;
    class Method_Call* method_call;
    class Expr_List* expr_list;
    class Return_Stmt* return_stmt;
    class For_Stmt* for_stmt;
    class Block_Stmt* block_stmt;
    class Continue_Stmt* continue_stmt;
    class Break_Stmt* break_stmt;
    class Location* location;
    class Expr* expr;
    class Bin_Expr* bin_expr;
    class Un_Expr* un_expr;
    class Literal* literal;
    class Callout_Stmt* callout_stmt;
    class Callout_Args_List* callout_args_list;
    class Callout_Arg* callout_arg;
    string* lit;
    int num;
    Node()
    {
        prog = NULL;
        decl_list = NULL;
        variable_list = NULL;
        variable = NULL;
        method_list = NULL;
        method = NULL;
        arg_list = NULL;
        block = NULL;
        var_decl_list = NULL;
        statement_list = NULL;
        statement = NULL;
        assign_stmt = NULL;
        if_stmt = NULL;
        method_call = NULL;
        expr_list = NULL;
        return_stmt = NULL;
        for_stmt = NULL;
        block_stmt = NULL;
        continue_stmt = NULL;
        break_stmt = NULL;
        location = NULL;
        expr = NULL;
        bin_expr = NULL;
        un_expr = NULL;
        literal = NULL;
        callout_stmt = NULL;
        callout_args_list = NULL;
        callout_arg = NULL;
        lit = NULL;
        num = 0;
    }
    ~Node(){}
};



typedef union Node YYSTYPE;

#define YYSTYPE_IS_DECLARED 1


/* Visitor Classes */
class ASTVisitor {
    public:
        virtual void visit(class Prog*) = 0;
        virtual void visit(class Decl_List*) = 0;
        virtual void visit(class Variable_List*) = 0;
        virtual void visit(class Variable*) = 0;
        virtual void visit(class Method_List*) = 0;
        virtual void visit(class Method*) = 0;
        virtual void visit(class Arg_List*) = 0;
        virtual void visit(class Block*) = 0;
        virtual void visit(class Var_Decl_List*) = 0;
        virtual void visit(class Statement_List*) = 0;
        virtual void visit(class Statement*) = 0;
        virtual void visit(class Assign_Stmt*) = 0;
        virtual void visit(class If_Stmt*) = 0;
        virtual void visit(class Method_Call*) = 0;
        virtual void visit(class Expr_List*) = 0;
        virtual void visit(class Return_Stmt*) = 0;
        virtual void visit(class For_Stmt*) = 0;
        virtual void visit(class Block_Stmt*) = 0;
        virtual void visit(class Continue_Stmt*) = 0;
        virtual void visit(class Break_Stmt*) = 0;
        virtual void visit(class Location*) = 0;
        virtual void visit(class Expr*) = 0;
        virtual void visit(class Callout_Stmt*) = 0;
        virtual void visit(class Callout_Args_List*) = 0;
        virtual void visit(class Callout_Arg*) = 0;
        virtual void visit(class Bin_Expr*) = 0;
        virtual void visit(class Un_Expr*) = 0;
        virtual void visit(class Literal*) = 0;
};

class SomeVisitor: public ASTVisitor {
    public:
        SomeVisitor() ;

        void visit(class Prog*);
        void visit(class Decl_List*);
        void visit(class Variable_List*);
        void visit(class Variable*);
        void visit(class Method_List*);
        void visit(class Method*);
        void visit(class Arg_List*);
        void visit(class Block*);
        void visit(class Var_Decl_List*);
        void visit(class Statement_List*);
        void visit(class Statement*);
        void visit(class Assign_Stmt*);
        void visit(class If_Stmt*);
        void visit(class Method_Call*);
        void visit(class Expr_List*);
        void visit(class Return_Stmt*);
        void visit(class For_Stmt*);
        void visit(class Block_Stmt*);
        void visit(class Continue_Stmt*);
        void visit(class Break_Stmt*);
        void visit(class Location*);
        void visit(class Expr*);
        void visit(class Callout_Stmt*);
        void visit(class Callout_Args_List*);
        void visit(class Callout_Arg*);
        void visit(class Bin_Expr*);
        void visit(class Un_Expr*);
        void visit(class Literal*);
};


class BaseAst{
public:
    virtual void accept(ASTVisitor* v) = 0;
    virtual Value* codegen(){};
};

class Prog:public BaseAst{
private:
public:
  	class Decl_List* decls;
  	class Method_List* stmts;
  	Prog(class Decl_List*, class Method_List*);
    Value* generateCode();
    void output_llvm_ir();
    Value* reportError();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Decl_List:public BaseAst{
private:
public:
  	vector<class Variable_List*> decl_list;
  	int cnt;
	Decl_List(){};
	void push_back(class Variable_List*);
    Value* generateCode();
	// vector<class Variable_List*> getDeclList();
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Variable_List:public BaseAst{
public:
  	vector<class Variable*> variable_list;
  	int cnt;
  	string* dataType;
	Variable_List(){cnt=0;}
	void push_back(class Variable*);
	vector<class Variable*> getVarsList();
    Value* generateCode();
    Value* generateCode(map<string, llvm::AllocaInst *> &);
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Variable:public BaseAst{
public:
	string declType;
  	string* var_name;
  	// string* dataType;
  	// unsigned int length;
  	int value;
  	class Expr* expr_var;
  	Variable(string,string*);
  	Variable(string,string*,int);
  	Variable(string,string*,Expr*);
    Value* generateCode();
  	// void setDataType(string*);
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Method_List:public BaseAst{
public:
	vector<class Method*> method_list;
	int cnt;
	string* dataType;
	Method_List(){}
	void push_back(class Method*);
	vector<class Method*> getMethodsList();
    void accept(ASTVisitor* v) { v->visit(this); };
    Value* generateCode();
};

class Method:public BaseAst{
public:
	// string* declType;
  	string* method_name;
  	class Arg_List* args;
  	class Block* block;
  	string* dataType;
  	Method(string*,Arg_List*,Block*);
  	Method(string*,string*,Arg_List*,Block*);
    Method(string*,Block*);
    Method(string*,string*,Block*);
    Function* generateCode();
  	// void setDataType(string*);	
    void accept(ASTVisitor* v) { v->visit(this); };
};



class Arg_List:public BaseAst{
public:
    vector<pair<string*,string*>> arg_list;
    int cnt;
    
    void push_back(string*,string*);
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Block:public BaseAst{
public:
   class Var_Decl_List* var_decl_list;
   class Statement_List* statement_list;
   Block(class Var_Decl_List*, class Statement_List*); 
   Value* generateCode();
   void accept(ASTVisitor* v) { v->visit(this); };
   bool has_return();
   bool has_break();
   bool has_cont();
};

class Var_Decl_List: public BaseAst{
public:
    vector<class Variable_List*> var_decl_list;
    int cnt;
    Var_Decl_List(){cnt=0;}
    void push_back(class Variable_List*);
    // vector<class Variable_List*> getDeclList();
    Value* generateCode(map<string, llvm::AllocaInst *> &);
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Statement_List:public BaseAst{
public:
    vector<class Statement*> stmt_list;
    int cnt;
    Statement_List(){cnt=0;}
    void push_back(class Statement*);
    // void push_back(class Statement*,string* label);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Statement:public BaseAst{
public:
    bool has_return;
    bool has_break;
    bool has_cont;
    Statement(){has_return = false;has_break=false;has_cont=false;}
    virtual void accept(ASTVisitor* v) {};
    virtual Value* generateCode() {};
};

class Assign_Stmt:public Statement{
public:
    class Location* location;
    string* assign_op;
    class Expr* expr;
    Assign_Stmt(class Location*, string*, class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class If_Stmt: public Statement{
public:
    class Expr* expr;
    class Block* if_block;
    class Block* else_block;
    If_Stmt(class Expr*, class Block*);
    If_Stmt(class Expr*, class Block*, class Block*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };

};


class Expr_List:public BaseAst{
public:
    vector<class Expr*> expr_list;
    int cnt;
    Expr_List(){cnt=0;}
    void push_back(class Expr*);
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Return_Stmt:public Statement{
public:
    class Expr* return_expr;
    Return_Stmt(){this->has_return = true; this->return_expr = NULL;}
    Return_Stmt(class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};


class For_Stmt:public Statement{
public:
    string* iter_var;
    class Expr* start_expr;
    class Expr* increment_expr;
    class Block* block;
    For_Stmt(string*,class Expr*, class Expr*, class Block*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Block_Stmt:public Statement{
public:
    class Block* block;
    Block_Stmt(class Block*);       
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Continue_Stmt:public Statement{
public:
    Continue_Stmt(){has_cont = true;}
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Break_Stmt:public Statement{
public:
    Break_Stmt(){has_break=true;}
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Expr:public BaseAst{
public:
    // string expr_type;
    // class Location* loc;
    // class Method_Call* method;
    // string* lit;
    // int num;
    // class Expr* lhs_expr;
    // class Expr* rhs_expr;
    // string* bin_op;
    // string* uni_op;
    // class Expr* uni_expr;
    // int brackets;
    // Expr(string, class Location*);
    // Expr(string, class Method_Call*);
    // Expr(string, string*);
    // Expr(string, int);
    // Expr(string, class Expr*, string*, class Expr*);
    // Expr(string, string*, class Expr*);
    
    virtual void accept(ASTVisitor* v) {};
    virtual Value* generateCode() {};
};

class Method_Call:public Statement,public Expr{
public:
    string* method_name;
    class Expr_List* expr_list;
    Method_Call(string*, class Expr_List*);
    Method_Call(string*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Location:public Expr{
public:
    string loc_type;
    string* var_name;
    class Expr* loc_expr;
    Location(string,string*);
    Location(string,string*,class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Bin_Expr:public Expr{
public:
    class Expr* lhs_expr;
    class Expr* rhs_expr;
    string* bin_op;
    Bin_Expr(class Expr*, string*, class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };    
};

class Un_Expr:public Expr{
public:
    string* uni_op;
    class Expr* uni_expr;
    Un_Expr(string*, class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };    
};

class Literal:public Expr{
public:
    int num;
    string* lit;
    Literal(string*);
    Literal(int);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };       
};

class Callout_Stmt:public Statement{
public:
    string* first_str;
    class Callout_Args_List* callout_args_list;
    Callout_Stmt(string*);
    Callout_Stmt(string*, class Callout_Args_List*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};


class Callout_Args_List:public BaseAst{
public:
    vector<class Callout_Arg*> callout_args;
    int cnt;
    Callout_Args_List(){cnt=0;}
    void push_back(class Callout_Arg*);
    void accept(ASTVisitor* v) { v->visit(this); };
};

class Callout_Arg:public BaseAst{
public:
    string* arg_str;
    class Expr* arg_expr;
    Callout_Arg(string*);
    Callout_Arg(class Expr*);
    Value* generateCode();
    void accept(ASTVisitor* v) { v->visit(this); };
};

class loopInfo {
    BasicBlock *afterBB, *checkBB, *loop_header;
    llvm::Value *condition;
    std::string loopVariable;
    PHINode *phiVariable;
public:
    loopInfo(BasicBlock *loop_header2, BasicBlock *afterBlock, BasicBlock *checkBlock, Value *cond, std::string var, PHINode *phiVar) {
        loop_header = loop_header2;
        afterBB = afterBlock;
        checkBB = checkBlock;
        condition = cond;
        loopVariable = var;
        phiVariable = phiVar;
    }

    BasicBlock *getLoopHeader() { cout<<"KOSHISH KIYE HO"<<endl;return loop_header; }

    BasicBlock *getAfterBlock() { return afterBB; }

    BasicBlock *getCheckBlock() { return checkBB; }

    llvm::Value *getCondition() { return condition; }

    PHINode *getPHINode() { return phiVariable; }

    std::string getLoopVariable() { return loopVariable; }
};

#endif //__CLASS_DEF__