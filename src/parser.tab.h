/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    UNI_ADD = 258,
    UNI_MIN = 259,
    UNI_MUL = 260,
    UNI_DIV = 261,
    UNI_MOD = 262,
    EQUALS = 263,
    NOT_EQUALS = 264,
    GTE = 265,
    LTE = 266,
    GT = 267,
    LT = 268,
    ADD = 269,
    MIN = 270,
    MUL = 271,
    DIV = 272,
    MOD = 273,
    ASSIGN = 274,
    COND_AND = 275,
    COND_OR = 276,
    OP_OR = 277,
    OP_AND = 278,
    OP_NOT = 279,
    COMMA = 280,
    SEMICOLON = 281,
    OPEN_PAREN = 282,
    CLOSE_PAREN = 283,
    OPEN_BRACKET = 284,
    CLOSE_BRACKET = 285,
    OPEN_BRACE = 286,
    CLOSE_BRACE = 287,
    IF = 288,
    ELIF = 289,
    ELSE = 290,
    FOR = 291,
    WHILE = 292,
    BREAK = 293,
    CONTINUE = 294,
    INT_DATA_TYPE = 295,
    FLOAT_DATA_TYPE = 296,
    CHAR_DATA_TYPE = 297,
    STRING_DATA_TYPE = 298,
    BOOL_DATA_TYPE = 299,
    ID = 300,
    INT = 301,
    FLOAT = 302,
    HEXAGON = 303,
    CHAR = 304,
    STRING = 305,
    BOOL = 306,
    CLASS = 307,
    IMPORT = 308,
    FUNCTION_DEC = 309,
    RETURN = 310
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
