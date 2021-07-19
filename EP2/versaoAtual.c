// Thais de Souza Rodrigues - 11796941
// AED - EP2
// Arvore B+

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define t 3 //grau minimo

typedef int TipoChave;

typedef struct str_no {
  TipoChave chave[2*t-1]; //max chaves do no
  struct str_no* filhos[2*t]; //max filhos do no
  int numChaves; //num chaves armazenadas
  bool folha; //folha ou no interno
  struct str_no* prox;
} NO;

typedef struct {
  NO* raiz;
} ArvB;

ArvB* criaArvoreB(ArvB* T);
NO* alocaNo();
NO* busca(NO* raiz,int chave);
bool split(NO* x, NO* y);
void insercaoNaoCheia(NO* x,int k);
bool insercao(ArvB* T, int k);
bool remover(NO* raiz,int chave);
void removerDaRaiz(ArvB* T, int k); 
void imprimir(ArvB* T, NO* raiz);

ArvB* criaArvoreB(ArvB* T) {
  ArvB* criar = (ArvB*) malloc(sizeof(ArvB));
  NO* x;
  if(!(x = (NO*) malloc(sizeof(NO)))) return NULL;
  x->folha = true;
  x->numChaves = 0;
  x->prox = NULL;
  criar->raiz = x;
  return criar;
}

NO* alocaNo() {
  NO* x;
  if(!(x = (NO*) malloc(sizeof(NO)))) return false;
  return x;
}

NO* busca(NO* raiz,int chave) {
  int i = 1;
  while(i <= raiz->numChaves && chave > raiz->chave[i]) i++;
  if(i <= raiz->numChaves && chave == raiz->chave[i]) {
    if(raiz->folha) return (raiz);
    else return busca(raiz->filhos[i],chave);
  }
  else return NULL;
} 

bool split(NO* x, NO* y) { //lidando com o split que é do no interno
  printf("SPLIT \n");
  NO* z = alocaNo();
  z->folha = y->folha; 
  z->numChaves = t;

  for(int j = 1; j <= t;j++) z->chave[j] = y->chave[j+t-1]; //pegar a mediana também 
  if(!y->folha) for(int j = 1; j <= t+1;j++) z->filhos[j] = y->filhos[j+t]; 
  y->numChaves = t-1; 

  for(int j = x->numChaves+1; j > 2;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[2] = z; 

  for(int j = x->numChaves; j > 1; j--) x->chave[j+1] = x->chave[j];
  x->chave[1] = y->chave[t]; 
  x->numChaves++;
  return true;
} 

void insercaoNaoCheia(NO* x,int k) {
  printf("insercaoNaoCheia do %i \n",k);
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
      split(x,x->filhos[i]);
      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k);
  }
}

bool insercao(ArvB* T, int k) {
  printf("INSERCAO %i \n",k);
  if(busca(T->raiz,k)) { //TIRAR DEPOIS, usar so para testar se funciona
    printf("REPETIDO \n");
    return false;
  }
  NO* r = T->raiz;
  if(r->numChaves == (2*t-1)) { //está cheio na raiz
    NO* s = alocaNo();
    T->raiz = s;
    s->folha = false;
    s->numChaves = 0;
    s->filhos[1] = r;
    split(s,r);
    printf("aqui1 \n");
    imprimir(T,T->raiz); //TIRAR DEPOIS, SÓ PRA TESTAR
    insercaoNaoCheia(s,k);
  }
  else insercaoNaoCheia(r,k);
  return true;

}

bool remover(NO* raiz,int chave) {
  printf("REMOVER \n");
}
void removerDaRaiz(ArvB* T, int k) {}

void imprimir(ArvB* T, NO* raiz) {
  int j = 1;
  NO* andando = raiz;
  if(!andando) return;
  if(T->raiz->numChaves == 0) {
    printf("vazia \n");
    return;
  } 
  for(int i = 1; i <= andando->numChaves; i++){
    printf("(");
    if(!andando->folha) {
      while(andando->filhos[j]) {
        imprimir(T,andando->filhos[i]);
        j++;
      }
    }
    printf("%i ",andando->chave[i]);
    if(!andando->folha) printf(")");
  }
  printf(")");
}

int main(int argc,char *argv[]) {
  FILE *f,*fsaida;
  if(!(f = fopen(argv[1],"r"))) return 0;
  else {
    fsaida = fopen(argv[2],"w+");
    ArvB* arvore = criaArvoreB(arvore);
    char comando;
    fscanf(f,"%c",&comando);
    while (comando != 'f'){
      int num;
      fscanf(f,"%i",&num);
      switch (comando) {
        case 'i' : insercao(arvore,num); break;
        case 'r' : remover(arvore->raiz,num); break;
        case 'p' : imprimir(arvore,arvore->raiz); break;
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
