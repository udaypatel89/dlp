%{
#include <stdio.h>
%}

DIGIT    [0-9]+
IDENT    [a-zA-Z_][a-zA-Z0-9_]*
KEYWORD  "int"|"char"|"return"|"if"|"else"|"while"|"for"|"do"|"void"
OPERATOR [+\-*/=<>!&|]
PUNCT    [(){};,]
STRING   \"([^\\"]|\\.)*\"
COMMENT_SINGLE  "//".*
COMMENT_MULTI   "/*"([^*]|\*+[^*/])*\*+"/"

%%  // FIRST `%%` MUST BE HERE

{KEYWORD}       { printf("Keyword: %s\n", yytext); }

{IDENT}         { printf("Identifier: %s\n", yytext); }

{DIGIT}         { printf("Constant: %s\n", yytext); }

{STRING}        { printf("String: %s\n", yytext); }

{OPERATOR}      { printf("Operator: %s\n", yytext); }

{PUNCT}         { printf("Punctuation: %s\n", yytext); }

{COMMENT_SINGLE} { /* Ignore single-line comments */ }

{COMMENT_MULTI}  { /* Ignore multi-line comments */ }

[ \t\n]+        { /* Ignore whitespace */ }

.               { printf("Lexical Error: %s\n", yytext); }

%%  // SECOND `%%` MUST BE HERE

int main() {
    yylex();  // Start lexical analysis
    return 0;
}

int yywrap() {
    return 1;
}
