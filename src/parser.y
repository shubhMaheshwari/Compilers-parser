%{
// Include statements
#include <bits/stdc++.h>
#include "AST.h"
#include "visit.h"

// Extern defined variables 
extern "C" int yylex();
extern union Node yylval;

// Defined functions
void yyerror(string s);

class RootNode* rootAst = NULL;
int errors=0;
%}



/* declare tokens */ 
%token <lit> UNI_ADD UNI_MIN UNI_MUL UNI_DIV UNI_MOD /* Urinary Operaters */

%token <lit> EQUALS NOT_EQUALS GTE LTE GT LT /* Comp operaterors */

%token <lit> ADD MIN MUL DIV MOD ASSIGN /* Operaters */

%token <lit> COND_AND COND_OR /* Conditional Expressions */

%token <lit> OP_OR OP_AND OP_NOT /* Boolean operators */ 

%token <lit> COMMA  SEMICOLON  OPEN_PAREN  CLOSE_PAREN  OPEN_BRACKET  CLOSE_BRACKET  OPEN_BRACE  CLOSE_BRACE  /* Syntax using brackets */

%token <lit> IF ELIF ELSE /* Condition statements */

%token <lit> FOR WHILE BREAK CONTINUE /* Loop statements */

%token <lit> INT_DATA_TYPE FLOAT_DATA_TYPE /* Numeric Datatypes statements */

%token <lit> CHAR_DATA_TYPE STRING_DATA_TYPE/* Character Datatypes statements */

%token <lit> BOOL_DATA_TYPE /* Boolean Datatypes statements */

%token <lit> ID

%token <num> INT 
%token <fnum>FLOAT 
%token <lit> HEXAGON CHAR STRING BOOL 

%token <lit> CLASS IMPORT FUNCTION_DEC RETURN /* Function and class definations */

// Left Associativity
%left EQUALS NOT_EQUALS
%left OP_OR OP_AND
%left LT LTE GTE GT
%left ADD MIN MUL DIV MOD
%left COND_OR COND_AND
%nonassoc OP_NOT

// Type definations for union assignment

// Basic
%type <root_node> S;
%type <block_node> Block; 

// Parameter
%type <parameter_list> Parameter_list; 

// Line
%type <line_node> Line;
%type <import_node> Import_Statement; 
%type <class_node> Class_defination; 
%type <declaration_node> Declaration;
%type <function_node> Function_defination;  
%type <expr_node> Expr; 
%type <while_node> Loop1; 
%type <for_node> Loop2; 
%type <return_node> Return_data; 
%type <continue_node> Continue_data; 
%type <break_node> Break_data; 

// Location Node
%type <assign_node> Assign; 
%type <location_node> Location;

// Expression Nodes
%type <variable_list_node> ID_LIST;

// Condition
%type <cond_node> Cond;
%type <block_node> Run_Condition;
%type <if_node> IF_list;
%type <else_node> Else_list;


// Others
%type <lit> Datatype;
%type <lit> Assign_Op;

// ExprListNode
%type <expr_list_node> Expr_List;


%%
 
S: Block {$$ = new RootNode($1); rootAst = $$; };

Block: /* Do nothing*/ {$$ = new BlockNode(); }
|Block Line { $$ = $1; $$->push_back($2); }
; 

Line: Import_Statement SEMICOLON	{$$ = $1;}
| Class_defination SEMICOLON 		{$$ = $1;}
| Function_defination SEMICOLON		{$$ = $1;}
| Declaration SEMICOLON				{$$ = $1;}
| Assign SEMICOLON 					{$$ = $1;}
| Expr SEMICOLON 					{$$ = $1;}
| Cond SEMICOLON 					{$$ = $1;}
| Loop1 SEMICOLON 					{$$ = $1;}
| Loop2 SEMICOLON 					{$$ = $1;}
| Return_data SEMICOLON 			{$$ = $1;}
| Break_data SEMICOLON 				{$$ = $1;}
| Continue_data SEMICOLON 			{$$ = $1;}
| SEMICOLON 						{$$ = new LineNode();}
;

//[TODO]
Import_Statement: IMPORT ID_LIST {$$ = new ImportNode($2);}
;

Class_defination: CLASS ID OPEN_BRACE Block CLOSE_BRACE {$$ = new ClassNode($2,$4);} 
;

Declaration: Datatype ID_LIST { $$ = new DeclarationNode($2);};

//[TODO]
Function_defination: FUNCTION_DEC ID OPEN_PAREN Parameter_list CLOSE_PAREN OPEN_BRACE Block CLOSE_BRACE {$$ = new FunctionNode($2,$4,$7);} 
;

// [TODO] => Unable to add parameters inside a list
Parameter_list: /* Do nothing*/ {$$ = new ParameterListNode();}
| Datatype ID { $$ = new ParameterListNode();$$->push_back($1,$2);}
| Parameter_list COMMA Datatype ID   { $$ = $1; $$->push_back($3,$4); }
;

//[TODO]
Return_data: RETURN Expr { $$ = new ReturnNode($2);}
| RETURN { $$ = new ReturnNode();}
;

//[TODO]
Continue_data: CONTINUE { $$ = new ContinueNode();} 
;

//[TODO]
Break_data: BREAK {$$ = new BreakNode();} ;

Assign: Location Assign_Op Expr {$$ = new AssignNode($1,$2,$3);}
;

Location: ID 									{$$ = new LocationNode($1);}
| ID OPEN_BRACKET Expr CLOSE_BRACKET			{$$ = new LocationNode($1,$3);}
| Datatype ID 									{$$ = new LocationNode($1,$2);}
| Datatype ID OPEN_BRACKET Expr CLOSE_BRACKET 	{$$ = new LocationNode($1,$2,$4);}
;

Assign_Op: ASSIGN 	{$$ = $1;}
| UNI_ADD 			{$$ = $1;}
| UNI_MIN 			{$$ = $1;}
| UNI_MUL 			{$$ = $1;}
| UNI_DIV 			{$$ = $1;}
| UNI_MOD 			{$$ = $1;}
;

// Causing Shift reduce conflict
Expr: ID { $$ = new VariableNode($1); } /* Value */ 
| ID OPEN_BRACKET Expr CLOSE_BRACKET { $$ = new VariableNode($1,$3);} /* Array */ 
| ID OPEN_PAREN Expr_List CLOSE_PAREN {$$ = new CallMethodNode($1,$3);} /* Function */ 
| OPEN_PAREN Expr CLOSE_PAREN { $$ = $2;}
| INT 		{$$ = new LiteralNode($1);}
| CHAR 		{$$ = new LiteralNode($1);}
| STRING 	{$$ = new LiteralNode($1);}
| FLOAT 	{$$ = new LiteralNode($1);}
| BOOL 		{$$ = new LiteralNode($1);}
| Expr OP_OR Expr 		{$$ = new BinaryOperationNode($1,$3,Op::OP_OR);}
| Expr OP_AND Expr 		{$$ = new BinaryOperationNode($1,$3,Op::OP_AND);}
| Expr ADD Expr 		{$$ = new BinaryOperationNode($1,$3,Op::ADD);}
| Expr MIN Expr 		{$$ = new BinaryOperationNode($1,$3,Op::MIN);}
| Expr MUL Expr 		{$$ = new BinaryOperationNode($1,$3,Op::MUL);}
| Expr DIV Expr 		{$$ = new BinaryOperationNode($1,$3,Op::DIV);}
| Expr MOD Expr 		{$$ = new BinaryOperationNode($1,$3,Op::MOD);}
| Expr EQUALS Expr  	{$$ = new BinaryOperationNode($1,$3,Op::EQUALS);}
| Expr NOT_EQUALS Expr  {$$ = new BinaryOperationNode($1,$3,Op::NOT_EQUALS);}
| Expr GT Expr 			{$$ = new BinaryOperationNode($1,$3,Op::GT);}
| Expr GTE Expr 		{$$ = new BinaryOperationNode($1,$3,Op::GTE);}
| Expr COND_AND Expr  	{$$ = new BinaryOperationNode($1,$3,Op::COND_AND);}
| Expr COND_OR Expr  	{$$ = new BinaryOperationNode($1,$3,Op::COND_OR);}
| Expr LT Expr 			{$$ = new BinaryOperationNode($1,$3,Op::LT);}
| Expr LTE Expr  		{$$ = new BinaryOperationNode($1,$3,Op::LTE);}
| OP_NOT Expr 			{$$ = new NotOperationNode($2);}
;

Run_Condition: Line {$$ = new BlockNode(); $$->push_back($1);}
| OPEN_BRACE Block CLOSE_BRACE {$$ = $2;}
;

Cond: IF_list Else_list { $$ = new CondNode($1,$2);};



IF_list: IF OPEN_PAREN Expr CLOSE_PAREN Run_Condition {$$ = new IfNode($3,$5);};

Else_list: /* Do nothing */ {$$ = new ElseNode();} 
| ELSE  Run_Condition {$$ = new ElseNode($2);}
;


Loop1: WHILE OPEN_PAREN Expr CLOSE_PAREN Run_Condition {$$ = new WhileNode($3,$5);};

Loop2: FOR OPEN_PAREN Assign SEMICOLON Expr SEMICOLON Assign CLOSE_PAREN Run_Condition {$$ = new ForNode($3,$5,$7,$9);};



Datatype: INT_DATA_TYPE {$$ = $1;}
| FLOAT_DATA_TYPE		{$$ = $1;}
| CHAR_DATA_TYPE 		{$$ = $1;}
| STRING_DATA_TYPE 		{$$ = $1;}
| BOOL_DATA_TYPE 		{$$ = $1;}
;


ID_LIST: /* Do nothing */ {$$ = new VariableListNode();}
| ID_LIST COMMA ID   {$$ = $1; $$->push_back($3);}
| ID OPEN_BRACKET Expr CLOSE_BRACKET {$$ = new VariableListNode(); $$->push_back($1,$3);} 
| ID {$$ = new VariableListNode();$$->push_back($1);}
;

Expr_List: /* Do nothing */ {$$ = new ExprListNode();}
| Expr_List COMMA Expr  {$$ = $1; $$->push_back($3);}
| Expr {$$ = new ExprListNode();$$->push_back($1);}
;



%%



int main(int argc, char **argv)
{
        yyparse();
        printf("Parsing Over\nCreating AST\n");

        class MainVisitor* visitor = new MainVisitor();
        visitor->visit(rootAst);
}