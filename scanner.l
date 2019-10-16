%{
#include <stdio.h>
#include "parser.tab.h"
extern YYSTYPE yylval;
    
int lineno = 0;
%}

/* Scanner using regex */  
%%
"==" {return EQUALS;}
"!=" {return NOT_EQUALS;}
"~=" {return NOT_EQUALS;}

"<=" {return GTE;} /* Comp operaterors */
">=" {return LTE;}
"=<" {return GTE;}
"=>" {return LTE;}
"<" {return GT;}
">" {return LT;}

"+=" {return UNI_ADD;} /* Urinary Operaters */
"-=" {return UNI_MIN;}
"*=" {return UNI_MUL;}
"/=" {return UNI_DIV;}
"%=" {return UNI_MOD;}

"=+" {return UNI_ADD;} /* Urinary Operaters */
"=-" {return UNI_MIN;}
"=*" {return UNI_MUL;}
"=/" {return UNI_DIV;}
"=%" {return UNI_MOD;}

"+" {return ADD;} /* Operaters */
"-" {return MIN;}
"*" {return MUL;}
"/" {return DIV;}
"%" {return MOD;}
"=" {return ASSIGN;}

"&&" {return COND_AND;} /* Conditional Expressions */
"||" {return COND_OR;}

"|" {return OP_OR;} /* Boolean operators */
"&" {return OP_AND;}
"~" {return OP_NOT;}
"!" {return OP_NOT;}


"," { return COMMA; } /* Syntax using brackets */
";" { return SEMICOLON; }
"(" { return OPEN_PAREN; }
")" { return CLOSE_PAREN; }
"[" { return OPEN_BRACKET; }
"]" { return CLOSE_BRACKET; }
"{" { return OPEN_BRACE; }
"}" { return CLOSE_BRACE; }


"if" {return IF;} /* Condition statements */  
"elif" {return ELIF;}
"else" {return ELSE;}

"for" {return FOR;} /* Loop statements */
"while" {return WHILE;} /* Loop statements */
"break" {return BREAK;}
"continue" {return CONTINUE;}

"int" {return INT_DATA_TYPE;} /* Numeric Datatypes statements */
"float" {return FLOAT_DATA_TYPE;}

"char" {return CHAR_DATA_TYPE;} /* Character Datatypes statements */
"string" {return STRING_DATA_TYPE;}

"bool" {return BOOL_DATA_TYPE;} /* Boolean Datatypes statements */

"def" {return FUNCTION_DEC;}
"import" {return IMPORT;} /* Import function */


(true)|(false) {return BOOL;}
[a-zA-z][a-zA-Z0-9]* {return ID;}

[+-]?[0-9]+  {return INT;}
[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+) {return FLOAT;}


(0x[0-9a-fA-F]+)|([0-9]+) {return HEXAGON;}

"\""[^\"]"\"" {return CHAR;}
"\""[^\"]*"\"" {return STRING;}
[ \t\v\r\n]+ {  } /* ignore whitespace */
"//".* { } /* ignore comments */ 
. { printf("Unknown character %c cannot parse it.\n", *yytext); }

%%
