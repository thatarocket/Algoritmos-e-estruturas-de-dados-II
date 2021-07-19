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
bool split(NO* x,int indice, NO* y);
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

bool split(NO* x,int i, NO* y) { //lidando com o split que é do no interno
  printf("split \n");
  NO* z = alocaNo();
  z->folha = y->folha; 
  z->numChaves = t;

  for(int j = 1; j <= t;j++) z->chave[j] = y->chave[j+t-1]; //pegar a mediana também 
  if(!y->folha) for(int j = 1; j <= t+1;j++) z->filhos[j] = y->filhos[j+t]; 
  y->numChaves = t-1; 

  for(int j = x->numChaves+1; j > i+1;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[i+1] = z; 

  for(int j = x->numChaves; j > i; j--) x->chave[j+1] = x->chave[j];
  x->chave[i] = y->chave[t]; 
  x->numChaves++;

  return true;
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
    if((x->filhos[i])->numChaves == 2*t-1) { // i = 2
      split(x,i,x->filhos[i]);
      printf("depois insercao \n");

      for(int i = 1; i <= x->numChaves;i++) printf("x: %i ",x->chave[i]);
      printf(" ");
      for(int i = 1; i <= x->filhos[i]->numChaves;i++) printf("x filho: %i ",x->filhos[i]->chave[i]);
      printf(" ");

      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k);
  }
}

bool insercao(ArvB* T, int k) {
  printf("inserindo o k %i \n",k);
  NO* r = T->raiz;
  if(r->numChaves == (2*t-1)) { //está cheio na raiz
    NO* s = alocaNo();
    T->raiz = s;
    s->folha = false;
    s->numChaves = 0;
    s->filhos[1] = r;
    split(s,1,r);
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
  if(T->raiz->numChaves == 0) {
    printf("vazia \n");
    return;
  }
  printf("(");
  for(int j = 1; j < T->raiz->numChaves+1;j++) {
    if(!T->raiz->folha) {
      printf("(");
      for(int i = 1; i < T->raiz->filhos[j]->numChaves+1; i++) {
        printf("%i", T->raiz->filhos[j]->chave[i]); // filhos esquerda
        if(i < T->raiz->filhos[j]->numChaves) printf(" ");
      }
      printf(") ");
    }
    printf("%i", T->raiz->chave[j]); // imprime a raiz
    if(!T->raiz->folha) {
      printf(" (");
      for(int k = 1; k < T->raiz->filhos[j+1]->numChaves+1; k++) { 
        printf("%i", T->raiz->filhos[j+1]->chave[k]); //filhos direita
        if(k < T->raiz->filhos[j+1]->numChaves) printf(" ");
      }
      printf(")"); 
    }
  }
  printf(")\n"); 
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
