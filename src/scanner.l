%{
#include <bits/stdc++.h>

#include "AST.h"
#include "parser.tab.h"

#define YY_DECL extern "C" int yylex()

extern union Node yylval;


int lineno = 0;

void yyerror(string s){
        fprintf(stderr, "[Line:%d] error: %s\n",lineno+1, s.c_str());
        exit(-1);
}



%}

/* Scanner using regex */  
%%
[\n] {lineno++;}
[ \t\v\r]+ {  } /* ignore whitespace */

";" { yylval.lit = new string(yytext, yyleng) ;return SEMICOLON; }
"==" {yylval.lit = new string(yytext, yyleng) ;return EQUALS;}
"!=" {yylval.lit = new string(yytext, yyleng) ;return NOT_EQUALS;}
"~=" {yylval.lit = new string(yytext, yyleng) ;return NOT_EQUALS;}

"<=" {yylval.lit = new string(yytext, yyleng) ;return GTE;} /* Comp operaterors */
">=" {yylval.lit = new string(yytext, yyleng) ;return LTE;}
"=<" {yylval.lit = new string(yytext, yyleng) ;return GTE;}
"=>" {yylval.lit = new string(yytext, yyleng) ;return LTE;}
"<" {yylval.lit = new string(yytext, yyleng) ;return GT;}
">" {yylval.lit = new string(yytext, yyleng) ;return LT;}

"+=" {yylval.lit = new string(yytext, yyleng) ;return UNI_ADD;} /* Urinary Operaters */
"-=" {yylval.lit = new string(yytext, yyleng) ;return UNI_MIN;}
"*=" {yylval.lit = new string(yytext, yyleng) ;return UNI_MUL;}
"/=" {yylval.lit = new string(yytext, yyleng) ;return UNI_DIV;}
"%=" {yylval.lit = new string(yytext, yyleng) ;return UNI_MOD;}

"=+" {yylval.lit = new string(yytext, yyleng) ;return UNI_ADD;} /* Urinary Operaters */
"=-" {yylval.lit = new string(yytext, yyleng) ;return UNI_MIN;}
"=*" {yylval.lit = new string(yytext, yyleng) ;return UNI_MUL;}
"=/" {yylval.lit = new string(yytext, yyleng) ;return UNI_DIV;}
"=%" {yylval.lit = new string(yytext, yyleng) ;return UNI_MOD;}

"+" {yylval.lit = new string(yytext, yyleng) ;return ADD;} /* Operaters */
"-" {yylval.lit = new string(yytext, yyleng) ;return MIN;}
"*" {yylval.lit = new string(yytext, yyleng) ;return MUL;}
"/" {yylval.lit = new string(yytext, yyleng) ;return DIV;}
"%" {yylval.lit = new string(yytext, yyleng) ;return MOD;}
"=" {yylval.lit = new string(yytext, yyleng) ;return ASSIGN;}

"&&" {yylval.lit = new string(yytext, yyleng) ;return COND_AND;} /* Conditional Expressions */
"||" {yylval.lit = new string(yytext, yyleng) ;return COND_OR;}

"|" {yylval.lit = new string(yytext, yyleng) ;return OP_OR;} /* Boolean operators */
"&" {yylval.lit = new string(yytext, yyleng) ;return OP_AND;}
"~" {yylval.lit = new string(yytext, yyleng) ;return OP_NOT;}
"!" {yylval.lit = new string(yytext, yyleng) ;return OP_NOT;}


"," { yylval.lit = new string(yytext, yyleng) ;return COMMA; } /* Syntax using brackets */
"(" { yylval.lit = new string(yytext, yyleng) ;return OPEN_PAREN; }
")" { yylval.lit = new string(yytext, yyleng) ;return CLOSE_PAREN; }
"[" { yylval.lit = new string(yytext, yyleng) ;return OPEN_BRACKET; }
"]" { yylval.lit = new string(yytext, yyleng) ;return CLOSE_BRACKET; }
"{" { yylval.lit = new string(yytext, yyleng) ;return OPEN_BRACE; }
"}" { yylval.lit = new string(yytext, yyleng) ;return CLOSE_BRACE; }


"if" {yylval.lit = new string(yytext, yyleng) ;return IF;} /* Condition statements */  
"elif" {yylval.lit = new string(yytext, yyleng) ;return ELIF;}
"else" {yylval.lit = new string(yytext, yyleng) ;return ELSE;}

"for" {yylval.lit = new string(yytext, yyleng) ;return FOR;} /* Loop statements */
"while" {yylval.lit = new string(yytext, yyleng) ;return WHILE;} /* Loop statements */
"break" {yylval.lit = new string(yytext, yyleng) ;return BREAK;}
"continue" {yylval.lit = new string(yytext, yyleng) ;return CONTINUE;}

"int" {yylval.lit = new string(yytext, yyleng) ;return INT_DATA_TYPE;} /* Numeric Datatypes statements */
"float" {yylval.lit = new string(yytext, yyleng) ;return FLOAT_DATA_TYPE;}

"char" {yylval.lit = new string(yytext, yyleng) ;return CHAR_DATA_TYPE;} /* Character Datatypes statements */
"string" {yylval.lit = new string(yytext, yyleng) ;return STRING_DATA_TYPE;}

"bool" {yylval.lit = new string(yytext, yyleng) ;return BOOL_DATA_TYPE;} /* Boolean Datatypes statements */
"void" {yylval.lit = new string(yytext, yyleng) ;return VOID_DATA_TYPE;} /* VOID Datatypes statements */

"def" {yylval.lit = new string(yytext, yyleng) ;return FUNCTION_DEC;}
"return" {yylval.lit = new string(yytext, yyleng) ;return RETURN;}
"import" {yylval.lit = new string(yytext, yyleng) ;return IMPORT;} /* Import function */
"class" {yylval.lit = new string(yytext, yyleng) ;return CLASS;} /* Import function */


(true)|(false) {yylval.lit = new string(yytext, yyleng) ;return BOOL;}
[a-zA-z][a-zA-Z0-9]* {yylval.lit = new string(yytext, yyleng) ;return ID;}

-?[0-9]+  {yylval.num = strtol(yytext,0,10);return INT;}
-?([0-9]+([.][0-9]*)?|[.][0-9]+) {yylval.fnum = std::stof(yytext);return FLOAT;}


(0x[0-9a-fA-F]+)|([0-9]+) {yylval.fnum = strtol(yytext,0,16);return HEXAGON;}

"\""[^\"]"\"" {yylval.lit = new string(yytext, yyleng) ;return CHAR;}
"\""[^\"]*"\"" {yylval.lit = new string(yytext, yyleng) ;return STRING;}
"//".* { } /* ignore comments */ 
. { printf("Unknown character %c",*yytext); }

%%
