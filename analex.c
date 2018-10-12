#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"

char Categorias[][10]=
{
	"ID",
	"INTCON",
	"REALCON",
	"PR",
	"OPREL",
	"OPARIT",
	"OPLOG",
	"CR",
	"ERRO"
};

char OperadoresRelacionais[][12]=
{
	"MAIOR",
	"MENOR",
	"IGUAL",
	"IGUALIGUAL",
	"MAIORIGUAL",
	"MENORIGUAL",
	"DIFERENTE"
};

char OperadoresAritmeticos[][13]=
{
	"MAIS",
	"MENOS",
	"MULTIPLICACAO",
	"DIVISAO"
};

char OperadoresLogicos[][6]=
{
	".AND.",
	".OR.",
	".NOT."
};

char PalavrasReservadas[][10]=
{
	"bool",	
	"call",
	"char",
	"display",
	"dup",
	"else",	
	"endfor",
	"endfunc",
	"endif",
	"endprog",
	"endvar",
	"endwhile",
	"for",
	"fwd",
	"if",
	"int",
	"keyboard",
	"ndproc",
	"noparam",
	"pl",
	"proc",
	"prog",
	"real",
	"return",
	"var",
	"while",
	
};

char* getCategoria (int indice){
	return Categorias[indice];
}

char* getPalavraReservada(int indice){
	return PalavrasReservadas[indice];
}

char* getOperadorAritmetico(int indice){
	return OperadoresAritmeticos[indice];
}

char* getOperadorRelacional(int indice){
	return OperadoresRelacionais[indice];
}

char* getOperadorLogico(int indice){
	return OperadoresLogicos[indice];
}

int isPalavraReservada(char *buffer, int menor, int maior){
   int meio, cmp;
   
   if (menor > maior) return -1;
   meio = (menor + maior) / 2;
   cmp = strcmp(PalavrasReservadas[meio], buffer);
   if (cmp == 0 ) 
      return meio;
   if (cmp > 0) 
      return isPalavraReservada(buffer, menor, meio - 1); 
   else 
      return isPalavraReservada(buffer, meio + 1, maior); 
}

Token AnalisadorLexico(FILE *fp){
	Token tk;
	char buffer[10]="";
	char ch;
	int estado = 0;
	int indice;
			
		while (estado>=0){
		
		switch(estado){
			
				case 0 :
					ch=getc(fp);
					
					if (isalpha(ch)){
						buffer[strlen(buffer)] = ch;
						estado = 1;
						
					} else if (isdigit(ch)){
						buffer[strlen(buffer)] = ch;
						estado = 4;
						
					}else if (ch == '\''){
						buffer[strlen(buffer)] = ch;
						estado = 9;
						
					}else if (ch == '\n'){
						buffer[strlen(buffer)] = ch; 
						estado = 12;
						
					}else if (ch == '\0'){
						buffer[strlen(buffer)] = ch; 
						estado = 13;
						
					}else if (ch == "\""){
						buffer[strlen(buffer)] = ch; 
						estado = 14;
						
					}else if (ch == '.'){ 
						buffer[strlen(buffer)] = ch; 
						estado = 16;
						
					}else if (ch == " "){ 
						
					}else if (ch == '\t'){ 
						estado = 24;
						
					}else if (ch == '*'){ 
						estado = 25;
						
					}else if (ch == '/'){ 
						estado = 27;
						
					}else if (ch == '-'){ 
						estado = 31;
						
					}else if (ch == '+'){ 
						estado = 32;
						
					}else if (ch == '#'){ 
						estado = 33;
						
					}else if (ch == '>'){ 
						estado = 34;
						
					}else if (ch == '<'){ 
						estado = 37;
						
					}else if (ch == '='){ 
						estado = 40;

					}else if (ch == EOF){ 
						tk.cat = ERRO;
						return (tk);
					}
					
					break;
				
				case 1 : // RECONHECIMENTO DE ID
					ch=getc(fp);
					if (isalpha(ch)){
						buffer[strlen(buffer)] = ch;
									
					}else if(isdigit(ch)){ 
						buffer[strlen(buffer)] = ch;
						estado = 2;
						
					}else{
						estado = 3;
					}
					
					break;	
				
				
				case 2 : // RECONHECIMENTO DE ID
					ch=getc(fp);
					if (isdigit(ch)){ 
						buffer[strlen(buffer)] = ch;
					}else if (isalpha(ch)){
						buffer[strlen(buffer)] = ch;
						estado = 1;
					}else{
						estado = 3;
					}
										
					break;
					
				case 3 : // RECONHECIMENTO DE ID
					ungetc(ch,fp);
					indice = isPalavraReservada(buffer,0,25);
					if(indice==-1){
						tk.cat = ID;
						strcpy(tk.valor.s,buffer);
					}else{
						tk.cat=PR;	
						tk.valor.numInt=indice;	
					} 
										
					return (tk);
				
				case 4 : // RECONHECIMENTO DE INTEIRO
					ch=getc(fp);
					if (isdigit(ch)){ 
						buffer[strlen(buffer)] = ch;
					}else if (ch=='.'){
						buffer[strlen(buffer)] = ch;
						estado = 6;
					}else{
						estado = 5;
					}
					break;
				
				case 5 : // RECONHECIMENTO DE INTEIRO
					ungetc(ch,fp);
					tk.cat = INTCON;
					tk.valor.numInt = atoll(buffer);
				return (tk);
				
					
				case 6 : // RECONHECIMENTO DE FLOAT
					ch=getc(fp);
					if(isdigit(ch)){
						buffer[strlen(buffer)] = ch;
						estado = 7;
					}else { // RECONHECIMENTO DE INTEIRO
						ungetc(ch,fp);
						ch='.';
						estado = 5;
					}
				break;	
					
					
				case 7:
					ch=getc(fp);
					if(isdigit(ch)){
						buffer[strlen(buffer)] = ch;
					}else {
						estado = 8;
					}
				break;
				
				
				case 8:
					ungetc(ch,fp);
					tk.cat = REALCON;
					tk.valor.numFloat = atof(buffer);
				return (tk);
				
					
				case 12 :
					estado = 0;
				break;				
				
				
				case 16 : // RECONHECIMENTO DOS OPERADORES L�GICOS
					ch=getc(fp);
					buffer[strlen(buffer)] = ch;
					ch=getc(fp);
					buffer[strlen(buffer)] = ch;
					if(!(strcmp(buffer,".OR"))) {
						estado = 19;
						break;
					}
					ch=getc(fp);
					buffer[strlen(buffer)] = ch;
					if(!(strcmp(buffer,".AND"))) {
						estado = 17;
						break;
					}else if (!(strcmp(buffer,".NOT"))) {
						estado = 21;
						break;
					}else {
						//MENSAGEM DE ERRO NA LINHA RESULTANTE
					}
					
				break;
				
				
				case 17 : // RECONHECIMENTO DO .AND.
					ch=getc(fp);
					if(ch=='.'){
						buffer[strlen(buffer)] = ch;
						estado = 18;
					}else {
						//MENSAGEM DE ERRO NA LINHA RESULTANTE
					}
				break;
				
				
				case 18 : // RECONHECIMENTO DO .AND.
					tk.cat = OPLOG;
					tk.valor.numInt = AND;
				return (tk);
				
				
				case 19 : // RECONHECIMENTO DO .OR.
					ch=getc(fp);
					if(ch=='.'){
						buffer[strlen(buffer)] = ch;
						estado = 20;
					}else {
						//MENSAGEM DE ERRO NA LINHA RESULTANTE
					}
				break;
				
				
				case 20 : // RECONHECIMENTO DO .OR.
					tk.cat = OPLOG;
					tk.valor.numInt = OR;
				return (tk);
				
				
				case 21 : // RECONHECIMENTO DO .NOT.
					ch=getc(fp);
					if(ch=='.'){
						buffer[strlen(buffer)] = ch;
						estado = 22;
					}else {
						//MENSAGEM DE ERRO NA LINHA RESULTANTE
					}
				break;
				
				
				case 22 : // RECONHECIMENTO DO .NOT.
					tk.cat = OPLOG;
					tk.valor.numInt = NOT;
				return (tk);
					
								
				case 23 :
					estado = 0;
				break;	


				case 25 :
					tk.cat=OPARIT;
					tk.valor.numInt = MULTIPLICACAO;
				return (tk);

					
				case 31 :
					tk.cat=OPARIT;
					tk.valor.numInt = MENOS;
				return (tk);

				
				case 32 :
					tk.cat=OPARIT;
					tk.valor.numInt = MAIS;
				return (tk);			

				
				case 33 :
					tk.cat=OPREL;
					tk.valor.numInt = DIFERENTE;
				return (tk);
				
									
				case 34 :
					ch=getc(fp);
					if(ch=='=') estado = 35;
					else estado = 36; 
				break;
				
				
				case 35 :
					tk.cat = OPREL;
					tk.valor.numInt = MAIORIGUAL;
				return (tk);
				
				
				case 36 :
					ungetc(ch,fp);
					tk.cat = OPREL;
					tk.valor.numInt = MAIOR;
				return (tk);
				
				
				case 37 :
					ch=getc(fp);
					if(ch=='=') estado = 38;
					else estado = 39; 
				break;	
				
					
				case 38 :
					tk.cat = OPREL;
					tk.valor.numInt = MENORIGUAL;
				return (tk);


				case 39 :
					ungetc(ch,fp);
					tk.cat = OPREL;
					tk.valor.numInt = MENOR;
				return (tk);

					
				case 40 :
					ch=getc(fp);
					if(ch=='=') estado = 41;
					else estado = 42; 
				break;	

					
				case 41 :
					tk.cat = OPREL;
					tk.valor.numInt = IGUALIGUAL;
				return (tk);


				case 42 :
					ungetc(ch,fp);
					tk.cat = OPREL;
					tk.valor.numInt = IGUAL;
				return (tk);					
			}
			
		}
		

}


