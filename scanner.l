%{
#include <stdio.h>


#include "defs.h"
#include "parser.tab.h"
extern YYSTYPE yylval;
    
int lineno = 0;
%}

/* Scanner using regex */  
%%
[ \t\v\r\n]+ { printf("NewLine\n");lineno++; } /* ignore whitespace */

";" { printf("Token:%s\n",yytext);return SEMICOLON; }
"==" {printf("Token:%s\n",yytext);return EQUALS;}
"!=" {printf("Token:%s\n",yytext);return NOT_EQUALS;}
"~=" {printf("Token:%s\n",yytext);return NOT_EQUALS;}

"<=" {printf("Token:%s\n",yytext);return GTE;} /* Comp operaterors */
">=" {printf("Token:%s\n",yytext);return LTE;}
"=<" {printf("Token:%s\n",yytext);return GTE;}
"=>" {printf("Token:%s\n",yytext);return LTE;}
"<" {printf("Token:%s\n",yytext);return GT;}
">" {printf("Token:%s\n",yytext);return LT;}

"+=" {printf("Token:%s\n",yytext);return UNI_ADD;} /* Urinary Operaters */
"-=" {printf("Token:%s\n",yytext);return UNI_MIN;}
"*=" {printf("Token:%s\n",yytext);return UNI_MUL;}
"/=" {printf("Token:%s\n",yytext);return UNI_DIV;}
"%=" {printf("Token:%s\n",yytext);return UNI_MOD;}

"=+" {printf("Token:%s\n",yytext);return UNI_ADD;} /* Urinary Operaters */
"=-" {printf("Token:%s\n",yytext);return UNI_MIN;}
"=*" {printf("Token:%s\n",yytext);return UNI_MUL;}
"=/" {printf("Token:%s\n",yytext);return UNI_DIV;}
"=%" {printf("Token:%s\n",yytext);return UNI_MOD;}

"+" {printf("Token:%s\n",yytext);return ADD;} /* Operaters */
"-" {printf("Token:%s\n",yytext);return MIN;}
"*" {printf("Token:%s\n",yytext);return MUL;}
"/" {printf("Token:%s\n",yytext);return DIV;}
"%" {printf("Token:%s\n",yytext);return MOD;}
"=" {printf("Token:%s\n",yytext);return ASSIGN;}

"&&" {printf("Token:%s\n",yytext);return COND_AND;} /* Conditional Expressions */
"||" {printf("Token:%s\n",yytext);return COND_OR;}

"|" {printf("Token:%s\n",yytext);return OP_OR;} /* Boolean operators */
"&" {printf("Token:%s\n",yytext);return OP_AND;}
"~" {printf("Token:%s\n",yytext);return OP_NOT;}
"!" {printf("Token:%s\n",yytext);return OP_NOT;}


"," { printf("Token:%s\n",yytext);return COMMA; } /* Syntax using brackets */
"(" { printf("Token:%s\n",yytext);return OPEN_PAREN; }
")" { printf("Token:%s\n",yytext);return CLOSE_PAREN; }
"[" { printf("Token:%s\n",yytext);return OPEN_BRACKET; }
"]" { printf("Token:%s\n",yytext);return CLOSE_BRACKET; }
"{" { printf("Token:%s\n",yytext);return OPEN_BRACE; }
"}" { printf("Token:%s\n",yytext);return CLOSE_BRACE; }


"if" {printf("Token:%s\n",yytext);return IF;} /* Condition statements */  
"elif" {printf("Token:%s\n",yytext);return ELIF;}
"else" {printf("Token:%s\n",yytext);return ELSE;}

"for" {printf("Token:%s\n",yytext);return FOR;} /* Loop statements */
"while" {printf("Token:%s\n",yytext);return WHILE;} /* Loop statements */
"break" {printf("Token:%s\n",yytext);return BREAK;}
"continue" {printf("Token:%s\n",yytext);return CONTINUE;}

"int" {printf("Token:%s\n",yytext);return INT_DATA_TYPE;} /* Numeric Datatypes statements */
"float" {printf("Token:%s\n",yytext);return FLOAT_DATA_TYPE;}

"char" {printf("Token:%s\n",yytext);return CHAR_DATA_TYPE;} /* Character Datatypes statements */
"string" {printf("Token:%s\n",yytext);return STRING_DATA_TYPE;}

"bool" {printf("Token:%s\n",yytext);return BOOL_DATA_TYPE;} /* Boolean Datatypes statements */

"def" {printf("Token:%s\n",yytext);return FUNCTION_DEC;}
"return" {printf("Token:%s\n",yytext);return RETURN;}
"import" {printf("Token:%s\n",yytext);return IMPORT;} /* Import function */
"class" {printf("Token:%s\n",yytext);return CLASS;} /* Import function */


(true)|(false) {printf("Token:%s\n",yytext);return BOOL;}
[a-zA-z][a-zA-Z0-9]* {printf("ID:%s\n",yytext);return ID;}

[+-]?[0-9]+  {printf("INT:%s\n",yytext);return INT;}
[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+) {printf("Float:%s\n",yytext);return FLOAT;}


(0x[0-9a-fA-F]+)|([0-9]+) {printf("Token:%s\n",yytext);return HEXAGON;}

"\""[^\"]"\"" {printf("Token:%s\n",yytext);return CHAR;}
"\""[^\"]*"\"" {printf("Token:%s\n",yytext);return STRING;}
"//".* { } /* ignore comments */ 
. { printf("Unknown character %c cannot parse it.\n", *yytext); }

%%
