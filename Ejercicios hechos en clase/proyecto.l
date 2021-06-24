%{
	#include "proyecto.tab.h"
	#include<stdlib.h>
%}

%%

[a-zA-Z]+		{return VARIABLE;}
[0-9]+			{yylval=atoi(yytext); return NUMERO;}
[<]			{return '<';}
[>]			{return '>';}
[=]			{return '=';}
[+]			{return '+';}
[-]			{return '-';}
[*]			{return '*';}
[/]			{return '/';}
[!]			{return '!';}
\n			{return '\n';}
.

