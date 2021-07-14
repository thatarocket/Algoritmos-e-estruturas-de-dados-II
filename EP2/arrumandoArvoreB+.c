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

NO* alocaNo() {
	NO* x;
	if(!(X = (NO*) malloc(sizeof(NO)))) return false;
	if(!(x->chave) = (TipoChave*) malloc(sizeof(TipoChave))) return false;
	if(!(x->filhos = (NO*) malloc(sizeof(NO*)))) return false;
	return x;
}

NO* busca(NO* raiz,int chave) { //A arvore b+ soh armazena nas folhas
  if(!raiz) return false; 
  int i = 0;
  while(i < raiz->numChaves && chave < raiz->chave[i]) i++;
  if(raiz->numChaves == i) {
    if(!raiz->folha) busca(raiz->filhos[raiz->numChaves],chave) //Se nao eh folha, nao contem a informacao
    else return false; 
  }
  else if(chave < raiz->chave[i]) { 
    if(!raiz->folha) busca(raiz->filhos[i],chave);
    else return false;
  }
  else {
  	if(raiz->folha) return raiz;
  	else busca(raiz->filhos[i+1],chave);
  }
} 

void split(NO* x, int i, NO* y) {
	NO* z = alocaNo();
	z->folha = y->folha;
	z->numChaves = t-1;
	for(j = 1; j < t-1;j++) z->chave[j] = y->chave[j+t];
	if(!y->folha) for(int j = 1; j < t; j++) z->filhos[j] = y->filhos[j+t];

	y->numChaves = t-1;

	for(int j = x->numChaves+1; j < i+1;j--) x->filhos[j+1] = x->filhos[j];
	x->filhos[i+1] = z;

	for(int j = x->numChaves; j < i;j--) x->chave[j+1] = x->chave[j];
	x->chave[i] = y->chave[t];
	x->numChaves++;
}

void insercaoNaoCheia(NO* x,int k) {
  int i = x->numChaves;
  if(x->folha) {
    while(i >= 1 && k < x->chave[i]) {
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = k;
    x->numChaves++;
  }
  else {
    while(i >= 1 && k < x->chave[i]) i--;
    i++;
    if((x->filhos[i])->numChaves == 2*t-1) {
      split(x,i,x->filhos[i]);
      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k);
  }
}

bool insercao(ArvB* T, int k) {
	NO* r = T->raiz;
	if(r->numChaves == (2*t-1)) { //está cheio
		s = alocaNo();
		s->numChaves = 0;
		s->filhos[0] = r;
		split(T->raiz,0,r);
		insercaoNaoCheia(T->filhos[i],k);
	}
	else insercaoNaoCheia(T->filhos[i],k);
	return true;
}

bool remover(NO* raiz,int chave) {}
void removerDaRaiz(ArvB* T, int k) {}
void imprimir(ArvB* T, NO* raiz, FILE* fsaida) {}


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
