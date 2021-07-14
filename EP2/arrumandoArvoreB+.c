// Thais de Souza Rodrigues - 11796941
// AED - EP2
// Arvore B+

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define t 3 //grau minimo

typedef int TipoChave;

typedef struct struct_no {
	TipoChave chaves[2*t-1];
	struct str_no* filhos[2*t];
	int numChaves;
	bool folha;
} NO;

typedef struct {
	NO* raiz;
} ArvB;

bool criaArvoreB(ArvB* T) {
	NO* x;
	if(!(X = (NO*) malloc(sizeof(NO)))) return false;
	x->folha = true;
	x->numChaves = 0;
	T->raiz = x;
	retorna true;
}

bool alocaNo(bool folha) {
	NO* x;
	if(!(X = (NO*) malloc(sizeof(NO)))) return false;
	if(!(x->chave) = (TipoChave*) malloc(sizeof(TipoChave))) return false;
	if(!(folha)) {
		if(!(x->filhos = (NO*) malloc(sizeof(NO*)))) return false;
	}
	else x->filhos = NULL;
}

NO* busca(NO* raiz,int chave) { //A arvore b+ soh armazena nas folhas
  if(!raiz) return false; 
  int i = 0;
  while(i < raiz->numChaves && chave > raiz->chave[i]) i++;
  if(raiz->numChaves == i) {
    if(!raiz->folha) busca(raiz->filhos[raiz->numChaves],chave) //Se nao eh folha, nao contem a informacao
    else return false; 
  }
  else if(chave > raiz->chave[i]) { 
    if(!raiz->folha) busca(raiz->filhos[i],chave);
    else return false;
  }
  else {
  	if(raiz->folha) return raiz;
  	else busca(raiz->filhos[i+1],chave);
  }
} 

int main(int argc,char *argv[]) {
  FILE *f,*fsaida;
  if((f = fopen(argv[1],"r")) == NULL) return 0;
  else {
    fsaida = fopen(argv[2],"w+");
    ArvB* arvore = (ArvB*) malloc(sizeof(ArvB));
    criaArvoreB(arvore);
    char comando;
    fscanf(f,"%c",&comando);
    while (comando != 'f'){
    	int num;
      fscanf(f,"%i",&num);
      switch (comando) {
       	case 'i' : insercao(arvore,num); break;
        case 'r' : remover(arvore->raiz,num); break;
        case 'p' : imprimir(arvore,arvore->raiz,fsaida); break;
        default: { //caso digite algo diferente
      		while (comando != '\n') scanf("%c",&comando);
      	}; 
      }
      fscanf(f,"%c",&comando);
    }
  }
  fclose(f);
  return 0;
}
