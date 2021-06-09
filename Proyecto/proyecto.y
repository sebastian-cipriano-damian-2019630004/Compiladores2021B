%{
	#include<stdio.h>
	void yyerror(char *mensaje){
		printf("Error: %s\n",mensaje);
	}
%}



%token VARIABLE NUMERO
%%
entrada:
%empty
;

entrada: entrada linea
;
linea: '\n'
;
linea: expresion '\n'
;
linea: num '\n'
;
num: NUMERO			
;
expresion: VARIABLE			
;
expresion: expresion '=' num	 	{printf("Le asigna a una variable el valor indicado");}
;
expresion: expresion '<' expresion 	{printf("Dice si la primera variable es menor que la segunda variable: a < b");}
;
expresion: expresion '>' expresion 	{printf("Dice si la primera variable es mayor que la segunda variable:a > b");}
;
expresion: expresion '=''=' expresion 	{printf("Dice si la primera variable es igual a la segunda variable: a == b");}
;
expresion: expresion '>''=' expresion 	{printf("Dice si la primera variable es mayor o igual a la segunda variable: a >= b");}
;
expresion: expresion '<''=' expresion 	{printf("Dice si la primera variable es menor o igual a la segunda variable: a <= b");}
;
expresion: expresion '!''=' expresion 	{printf("Dice si la primera variable es diferente a la segunda variable: a != b");}
;
expresion: expresion '+' expresion 	{printf("Raliza la suma de a y b: a + b");}
;
expresion: expresion '-' expresion 	{printf("Realiza la diferencia de a y b : a - b");}
;
expresion: expresion '*' expresion 	{printf("Realiza el producto de a y b: a * b");}
;
expresion: expresion '/''/' expresion 	{printf("Obtiene el residuo de division entera de a y b");}
;
expresion: expresion '/' expresion 	{printf("Realiza la  razon a y b");
					printf(" donde la primera variable toma el papel del dividendo");
					printf(" y donde la segunda variable toma el papel del divisor: a / b");}
;
expresion: '-' expresion 	{printf("Multiplica por -1 el contenido de una variable");}
;
%%


int main(){
	yyparse();
	return 0;
}
