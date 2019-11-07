%{
// Include statements
#include <stdio.h>

void yyerror(char *s);
int yylex(void);
%}

// Union
%union {
 int int_val;
 float float_val;
 bool bool_val;
 char char_val;
 char* string_val;
}

/* declare tokens */ 
%token EQUALS NOT_EQUALS GTE LTE GT LT /* Comp operaterors */

%token UNI_ADD UNI_MIN UNI_MUL UNI_DIV UNI_MOD /* Urinary Operaters */

%token ADD MIN MUL DIV MOD ASSIGN /* Operaters */

%token COND_AND COND_OR /* Conditional Expressions */

%token OP_OR OP_AND OP_NOT /* Boolean operators */ 

%token COMMA  SEMICOLON  OPEN_PAREN  CLOSE_PAREN  OPEN_BRACKET  CLOSE_BRACKET  OPEN_BRACE  CLOSE_BRACE  /* Syntax using brackets */

%token IF ELIF ELSE /* Condition statements */

%token FOR WHILE BREAK CONTINUE /* Loop statements */

%token INT_DATA_TYPE FLOAT_DATA_TYPE /* Numeric Datatypes statements */

%token CHAR_DATA_TYPE STRING_DATA_TYPE/* Character Datatypes statements */

%token BOOL_DATA_TYPE /* Boolean Datatypes statements */

%token ID INT FLOAT HEXAGON CHAR STRING BOOL 

%token CLASS IMPORT FUNCTION_DEC RETURN /* Function and class definations */

// Left Associativity
%left EQUALS NOT_EQUALS
%left OP_OR OP_AND
%left LT LTE GTE GT
%left ADD MIN MUL DIV MOD
%left COND_OR COND_AND
%nonassoc OP_NOT

%%
 
S:  /* Do nothing*/ {;}| 
Block {;}
;

Block: /* Do nothing*/ {;}
| Line Block
; 

Line: Import_Statement SEMICOLON{;}
| Class_defination SEMICOLON {;}
| Function_defination SEMICOLON{;}
| Assign SEMICOLON
| Expr SEMICOLON {;}
| Cond SEMICOLON {;}
| Loop1 SEMICOLON {;}
| Loop2 SEMICOLON {;}
| Return_data SEMICOLON {;}
| BREAK SEMICOLON {;}
| CONTINUE SEMICOLON {;}
;

Import_Statement: IMPORT ID_LIST {;}
;

Class_defination: CLASS ID OPEN_BRACE Block CLOSE_BRACE {;}

Function_defination: FUNCTION_DEC ID OPEN_PAREN Parameter_list CLOSE_PAREN
OPEN_BRACE Block CLOSE_BRACE {;}
;

Return_data: RETURN Expr {;}
| RETURN {;}
;

Assign: ID ASSIGN Expr {;}
| ID OPEN_BRACKET Expr CLOSE_BRACKET ASSIGN Expr {;}
| Datatype ID {;}
| Datatype ID Assign Expr {;} 
| Datatype ID OPEN_BRACKET Expr CLOSE_BRACKET {;}
| Datatype ID OPEN_BRACKET Expr CLOSE_BRACKET ASSIGN Expr {;}
; 

// Causing Shift reduce conflict
Expr: ID {;} /* Value */ 
| ID OPEN_BRACKET Expr OPEN_BRACKET {;} /* Array */ 
| ID OPEN_PAREN ID_LIST CLOSE_PAREN {;} /* Function */ 
| OPEN_PAREN Expr CLOSE_PAREN {;}
| INT {;}
| CHAR {;}
| STRING {;}
| FLOAT {;}
| BOOL {;}
| Expr OP_OR Expr {;}
| Expr OP_AND Expr {;}
| Expr ADD Expr {;}
| Expr MIN Expr {;}
| Expr MUL Expr {;}
| Expr DIV Expr {;}
| Expr MOD Expr {;}
| Expr GT Expr  {;}
| Expr GTE Expr {;}
| Expr LT Expr {;}
| Expr LTE Expr  {;}
| OP_NOT Expr {;}
| MIN Expr  {;}
;

Cond: IF_list Else_list {;};

Run_Condition: Line | OPEN_BRACE Block CLOSE_BRACE;

IF_list: IF OPEN_PAREN Expr CLOSE_PAREN Run_Condition {;}

Else_list: /* Do nothing */ {;} 
| ELSE  Run_Condition {;}
;



Loop1: WHILE OPEN_PAREN Expr CLOSE_PAREN Run_Condition;
Loop2: FOR OPEN_PAREN Assign SEMICOLON Expr SEMICOLON Assign CLOSE_PAREN Run_Condition

Parameter_list: /* Do nothing*/ {;}
| Datatype ID {;}
| Datatype ID COMMA Parameter_list {;}
;

Datatype: INT_DATA_TYPE {;}
| FLOAT_DATA_TYPE{;}
| CHAR_DATA_TYPE {;}
| STRING_DATA_TYPE {;}
| BOOL_DATA_TYPE {;}
;


ID_LIST: /* Do nothing */
| ID COMMA ID_LIST {;}
| ID {;}
;

%%


void yyerror(char *s)
{
        fprintf(stderr, "error: %s\n", s);
}


int main(int argc, char **argv)
{
        yyparse();
        printf("Parsing Over\n");
}