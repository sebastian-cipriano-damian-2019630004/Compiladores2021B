%{
	#include <stdio.h>
	#include <stdlib.h>
	void yyerror(char *mensaje){
		printf("Error: %s\n",mensaje);
	}
%}


%token INT FLOAT CHAR IF ELSE WHILE FOR VOID RETURN BREAK
%token OPADD OPSUBS OPMULT OPDIV OPMOD OPINCR OPDECR 
%token OPIGUL OPNIGUL OPMA OPME OPMAI OPMEI
%token IZQP DERP IZQC DERC IZQLL DERLL PC PUNTO COMA ASIG
%token ID ICONST FCONST CCONST STRING

%%

programa: declaraciones estructuras ;

declaraciones: 
    declaraciones declaracion |
    declaracion
;

declaracion: tipo nombres PC ;

tipo: INT |
    CHAR |
    FLOAT |
    VOID
;

nombres: 
    variable |
    nombres COMA variable
;

variable: ID |
    puntero ID |
    ID arreglo
;

puntero: 
    puntero OPMULT |
     OPMULT ;

arreglo: 
    arreglo IZQC ICONST DERC  |
    IZQC ICONST DERC  
;

estructuras: 
    estructuras estructura |
    estructura
;

estructura:
    ifEstructura |
    forEstructura |
    whileEstructura |
    asignacion |
    CONTINUE PC |
    BREAK PC |
    RETURN PC
;

ifEstructura: IF IZQP expresion DERP aux else_if_part else_part ;

else_if_part: 
    else_if_part ELSE IF IZQP expresion DERP aux |
    ELSE IF IZQP expresion DERP aux  
; 

else_part: ELSE aux; 

forEstructura: FOR IZQP expresion PC expresion PC expresion DERP aux ;

whileEstructura: WHILE IZQP expresion DERP aux ;

aux: estructura PC | IZQLL estructuras DERLL  ;

expresion:
    expresion OPADD expresion |
    expresion OPSUBS expresion |
    expresion OPMULT expresion |
    expresion OPDIV expresion |
    expresion OPMOD expresion |
    expresion OPINCR  |
    expresion OPDECR |
    OPINCR  expresion |
    OPDECR  expresion |
    expresion OROP expresion |
    expresion ANDOP expresion |
    NOTOP expresion |
    expresion OPIGUL expresion |
    expresion OPNIGUL expresion |
    expresion OPMA expresion |
    expresion OPME expresion |
    expresion OPMAI expresion |
    expresion OPMEI expresion |
    IZQP expresion DERP |
    variable |
    sign constante
;

sign: 
    OPADD |
    OPSUBS 
; 

constante:
    ICONST |
    FCONST |
    CCONST 
;

asignacion: referencia variable ASIG expresion PC ; 

referencia: REFER;

%%

int main(){
	yyparse();
	return 0;
}
