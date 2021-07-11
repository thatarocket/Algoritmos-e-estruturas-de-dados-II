// Thais de Souza Rodrigues - 11796941
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define t 3 //grau minimo
// Se nao eh raiz: min t-1 chaves e 1 filhos

typedef int TipoChave;

typedef struct str_no {
	TipoChave chave[2*t-1]; //max chaves do no
	struct str_no* filhos[2*t]; //max filhos do no
	int numChaves; //num chaves armazenadas
	bool folha; //folha ou no interno
} NO;

typedef struct {
  NO* raiz;
} ArvB;

bool criaArvoreB(ArvB* T) {
  NO* x;
  if(!(x = (NO*) malloc(sizeof(NO)))) return false; //nao conseguiu alocar
  x->folha = true;
  x->numChaves = 0;
  T->raiz = x;
  return true;
}

NO* busca(NO* raiz,int chave) {
  int i = 0;
  while(i < raiz->numChaves && chave > raiz->chave[i]) i++;
  if(i < raiz->numChaves && chave == raiz->chave[i]) return (raiz);
  if(raiz->folha) return NULL;
  else busca(raiz->filhos[i],chave);
} 

// Divisao de um no na arvore 
bool separaFilho(NO* x, int indice, NO* irmao) { //(x,i,y) e z = inserir
  NO* inserir;
  if(!(inserir = (NO*) malloc(sizeof(NO)))) return false;
  inserir->folha = irmao->folha;
  inserir->numChaves = t-1;

  for(int i = 0; i < t-1;i++) inserir->chave[i] = irmao->chave[i+t]; 
  if(!irmao->folha) for(int j = 0; j < t;j++) inserir->filhos[j] = irmao->filhos[j+t];
  irmao->numChaves = t-1;

  for(int j = x->numChaves+1; j > indice+1;j--) {
    printf("entrei \n");
    x->filhos[j+1] = x->filhos[j];
  }
  x->filhos[indice+1] = inserir;

  for(int j = x->numChaves+1; j > indice+1; j--) x->chave[j+1] = x->chave[j];
  x->chave[indice] = irmao->chave[t-1];
  x->numChaves++;
  return true;
} 

void insercaoNaoCheia(NO* x,int k) {

  int i = x->numChaves-1;
  if(x->folha) {
    while(i >= 0 && k < x->chave[i]) {
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = k;
    x->numChaves++;
  }
  else {
    while(i >= 0 && k < x->chave[i]) i--;
    i++;
    if((x->filhos[i])->numChaves == 2*t-1) {
      separaFilho(x,i,x->filhos[i]);
      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k);
  }
}

bool insercao(ArvB* T, int k) { //raiz T
  //printf("INSERCAO DO %i \n",k);
  if(busca(T->raiz,k)) return false;
  NO* r = T->raiz;
  if(r->numChaves == (2*t-1)) {
    NO* s;
    if(!(s = (NO*) malloc(sizeof(NO)))) return false;

    T->raiz = s;
    s->folha = false;
    s->numChaves = 0;
    s->filhos[0] = r;
    separaFilho(s,0,r);
    insercaoNaoCheia(s,k);
  }
  else insercaoNaoCheia(r,k);
  return true;
}

bool remover(NO* raiz,int chave) {
  NO* eliminar = busca(raiz,chave);
  if(!eliminar) return false; //se nao ha o valor na arvore
}

void removerDaRaiz(ArvB* T, int k) {
  NÓ* r;
  if(!(r = (NO*) malloc(sizeof(NO)))) return false;
  r = T->raiz;
  if(r->numChaves == 0) return;
  else remover(r,k);
  if(r->numChaves == 0 && (!r->folha)) {
    T->raiz = r->filhos[0];
    free(r);
  }
}


void imprimir(ArvB* T,NO* raiz,FILE* fsaida) {
    if(!raiz) return;
    for(int i = 0; i < raiz->numChaves; i++){
      if(!raiz->folha) if(raiz->filhos[i]) imprimir(T,raiz->filhos[i],fsaida);
      printf("%i ",raiz->chave[i]);
    }
    if(!raiz->folha) if(raiz->filhos[raiz->numChaves]) imprimir(T,raiz->filhos[raiz->numChaves],fsaida);
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
       0 case 'i' : insercao(arvore,num); break;
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
