#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"

int linha=0;

int contadorLinha(int *pt_linha){
	*pt_linha = &pt_linha + 1;
	return &pt_linha;
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
	char buffer[256]="";
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
							estado = 9;
							
						}else if (ch == '\n'){
							estado = 12;
							
						}else if (ch == '\0'){
							buffer[strlen(buffer)] = ch; 
							estado = 13;
							
						}else if (ch == '\"'){
							estado = 14;
							
						}else if (ch == '.'){ 
							buffer[strlen(buffer)] = ch; 
							estado = 16;
							
						}else if (ch == ' '){ 
							estado = 23;
							
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
						
						}else if (ch == '{'){ 
							estado = 43;
						
						}else if (ch == '}'){ 
							estado = 44;
							
						}else if (ch == '('){ 
							estado = 45;
						
						}else if (ch == ')'){ 
							estado = 46;
							
						}else if (ch == '['){ 
							estado = 47;
						
						}else if (ch == ']'){ 
							estado = 48;
						
						}else if (ch == ','){ 
							estado = 49;
						
						}else if (ch == ';'){ 
							estado = 50;
	
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
					
					case 9 :// RECONHECIMENTO DE CARACON
						ch=getc(fp);
						if (ch == '\''){
							estado = -1;
							tk.cat = ERRO;
							strcpy(tk.valor.s,"ERRO LEXICO");
							return (tk);
						}
						else if ((isprint(ch)) && (ch != '\\')){
							
							buffer[strlen(buffer)] = ch;
							estado = 10;
							break;
						}
						else if(ch == '\\'){// leitura caso falhe em casos normais fora o '\''
							buffer[strlen(buffer)] = ch;
							ch=getc(fp); 
							buffer[strlen(buffer)] = ch;
						}
						
						if (!(strcmp(buffer,"\\'"))){ // comparar se � '\''
							buffer[0] = '\0';
							buffer[1] = '\0';
							buffer[0] = '\'';
							estado = 10;
						}
						else if (!(strcmp(buffer,"\\\\"))){// comparar se � '\\'
							buffer[0] = '\0';
							buffer[1] = '\0';
							buffer[0] = '\\';
							estado = 10;
						}
						
					
					break;
						
					case 10 :
						ch=getc(fp);
						
						if (ch == '\''){ // delimitador final de CARACON
							estado = 11;
						}
						else{
							estado = -1;
							tk.cat = ERRO;
							strcpy(tk.valor.s,"ERRO LEXICO");
							return (tk);
						}
					break;
					
					case 11 : // GERANDO UM TOKEN CARACON
						tk.cat = CARACON;
						tk.valor.c = buffer[0];
					return(tk);
					
						
					case 12 : // TRATANDO O \N
						estado = 0;
						linha++;
						
					break;				
					
					case 14 : //RECONHECIMENTO DE CADEIACON
					ch=getc(fp);
						if (ch == '\"'){ 
							estado = 15;							
						}else {
							buffer[strlen(buffer)] = ch;
						}
						
					break;
					
					case 15 : // RECONHECIMENTO DE CADEIACON
						tk.cat = CADEIACON;
						strcpy(tk.valor.s,buffer);
						return (tk);
										
					
					case 16 : // RECONHECIMENTO DOS OPERADORES L�GICOS
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						if(!(strcmp(buffer,".or"))) {
							estado = 19;
							break;
						}
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						if(!(strcmp(buffer,".and"))) {
							estado = 17;
							break;
						}else if (!(strcmp(buffer,".not"))) {
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
						
									
					case 23 : // TRATANDO O CARACTER BRANCO
						estado = 0;
					break;
					
					case 24: // TRATANDO O \T (TAB)
						estado = 0;
					break;	
	
					case 25 :
						tk.cat=OPARIT;
						tk.valor.numInt = MULTIPLICACAO;
					return (tk);
					
					case 27 :
						ch=getc(fp);
						if(ch=='/') estado = 29;
						else estado = 28; 
					break;
	
					case 28 : //RECONHECIMENTO DE DIVIS�O 
						ungetc(ch,fp);	
						tk.cat=OPARIT;
						tk.valor.numInt = DIVISAO;
					return (tk);
					
					case 29 : //RECONHECIMENTO DE COMENTARIO
						ch=getc(fp);
						if(ch=='\n'){
							estado = 30;
							linha++;
						}else if (ch==EOF) estado = 30;
						else buffer[strlen(buffer)] = ch;
					break;
					
					case 30 :
						tk.cat=COMENTARIO;
						strcpy(tk.valor.s,buffer);
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
					
					case 43 :
						tk.cat = CARACON;
						tk.valor.c = '{';
					return (tk);
					
					case 44 :
						tk.cat = CARACON;
						tk.valor.c = '}';
					return (tk);					
					
					case 45 :
						tk.cat = CARACON;
						tk.valor.c = '(';
					return (tk);
					
					case 46 :
						tk.cat = CARACON;
						tk.valor.c = ')';
					return (tk);
					
					case 47 :
						tk.cat = CARACON;
						tk.valor.c = '[';
					return (tk);
					
					case 48 :
						tk.cat = CARACON;
						tk.valor.c = ']';
					return (tk);
					
					case 49 :
						tk.cat = CARACON;
						tk.valor.c = ',';
					return (tk);
					
					case 50 :
						tk.cat = CARACON;
						tk.valor.c = ';';
					return (tk);
				}
			
		}
		

}

int main(int argc, char *argv[]) {
	FILE *fp;
	Token tk;
	
	if((fp = fopen("teste5.txt","r"))==NULL) printf("Arquivo n�o pode ser aberto\n");
	
	do{	
		tk = AnalisadorLexico(fp);
		if(tk.cat==PR) printf("<%s, %s> ", Categorias[tk.cat],(toupper(PalavrasReservadas[tk.valor.numInt])));
		else if(tk.cat==ID) printf("<%s, %s> ",Categorias[tk.cat],tk.valor.s);
		else if(tk.cat==OPREL) printf("<%s, %s> ",Categorias[tk.cat],OperadoresRelacionais[tk.valor.numInt]);
		else if(tk.cat==OPARIT) printf("<%s, %s> ",Categorias[tk.cat],OperadoresAritmeticos[tk.valor.numInt]);
		else if(tk.cat==OPLOG) printf("<%s, %s> ",Categorias[tk.cat],OperadoresLogicos[tk.valor.numInt]);
		else if(tk.cat==INTCON) printf("<%s, %d> ",Categorias[tk.cat],tk.valor.numInt);
		else if(tk.cat==REALCON) printf("<%s, %.2f> ",Categorias[tk.cat],tk.valor.numFloat);
		else if (tk.cat==CARACON) printf("<%s, %c> ",Categorias[tk.cat],tk.valor.c);
		else if(tk.cat==CADEIACON) printf("<%s, %s> ",Categorias[tk.cat],tk.valor.s);
		else if(tk.cat==COMENTARIO) printf("<%s, %s> ",Categorias[tk.cat],tk.valor.s);
		//else printf("<%s, %s> ",Categorias[tk.cat], tk.valor.s);
	}while(tk.cat!= ERRO);
	
	fclose(fp);
	
	return 0;
}


