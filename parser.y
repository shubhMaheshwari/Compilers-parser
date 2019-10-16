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

%token IMPORT FUNCTION_DEC

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
| Function_defination SEMICOLON{;}
| Assign SEMICOLON
| Expr SEMICOLON {;}
| Cond SEMICOLON {;}
| Loop SEMICOLON {;}
;

Import_Statement: IMPORT ID_LIST {;}
;

Function_defination: FUNCTION_DEC ID OPEN_PAREN Parameter_list CLOSE_PAREN
OPEN_BRACKET Block CLOSE_BRACKET {;}
;

Assign: ID ASSIGN Expr {;}
| Datatype ID Assign Expr {;}
;

// Causing Shift reduce conflict
Expr: ID {;}
| ID OPEN_PAREN ID_LIST CLOSE_BRACKET {;} 
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

Cond: IF_list Elif_list Else_list {;};

IF_list: IF OPEN_PAREN Expr CLOSE_PAREN Run_Condition 

Elif_list: 
| ELIF OPEN_PAREN Expr CLOSE_PAREN Run_Condition Elif_list  
;

Else_list: 
| ELSE OPEN_PAREN Expr CLOSE_PAREN Run_Condition;

Run_Condition: Line | OPEN_BRACKET Block CLOSE_BRACKET;


Loop: WHILE OPEN_PAREN Expr CLOSE_PAREN Run_Condition;

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