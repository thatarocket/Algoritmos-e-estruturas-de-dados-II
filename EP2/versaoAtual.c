// Thais de Souza Rodrigues - 11796941
// AED - EP2
// Arvore B+

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define t 3 //grau minimo

typedef int TipoChave;

typedef struct str_folha {
  Tipochave chave[2*t-1];
  int numChaves;
  struct str_folha* prox;
  bool folha;
} FOLHA;

typedef struct str_no {
  TipoChave chave[2*t-1];
  struct str_no* filhos[2*t]; //filhos que sao nos internos
  FOLHA* filhosFolha; //pra quando o filho for folha, que usa uma struct diferente
  int numChaves;
  bool folha;
} NO;


typedef struct {
  NO* raiz;
  FOLHA* folhas;
} ArvB;

bool criaArvoreB(ArvB* T) {
  T = (ArvB*) malloc(sizeof(ArvB));
  NO* x;
  if(!(x = (NO*) malloc(sizeof(NO)))) return false;
  FOLHA* folhas;
  if(!(folhas = (NO*) malloc(sizeof(NO)))) return false;

  x->filhosFolha = NULL;
  x->numChaves = 0;
  x->folha = false; //Pra garantir que vai verificar certinho

  folha->numChaves = 0;
  folhas->prox = NULL;
  folhas->folha = true; 

  T->raiz = x;
  T->folhas = folhas;
  return true;
}

NO* alocaNo() {
  NO* x;
  if(!(x = (NO*) malloc(sizeof(NO)))) return false;
  return x;
}

FOLHA* alocaFolha() {
  FOLHA* x;
  if(!(x = (FOLHA*) malloc(sizeof(FOLHA)))) return false;
  return x;
}

NO* busca(FOLHA* raiz,int chave) {
  int i = 1;
  while(raiz) { //enquanto houver proximos
    while(i <= raiz->numChaves && chave > raiz->chave[i]) i++;
    if(chave > raiz->chave[i]) raiz->prox; //Eh maior que esse bloco atual, olha o proximo bloco
    else return raiz; //Nao atingiu o maximo de chaves, entao encontrou
  }
  return NULL;
} 

void split(NO* x, int i, NO* y) {
  NO* z = alocaNo();
  z->folha = y->folha;
  z->numChaves = t; //igual a t para pegar a mediana
  for(int j = 1; j < t-1;j++) z->chave[j] = y->chave[j+t];
  if(!y->folha) for(int j = 1; j < t; j++) z->filhos[j] = y->filhos[j+t];

  y->numChaves = t-1;

  for(int j = x->numChaves+1; j > i+1;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[i+1] = z;

  for(int j = x->numChaves; j > i;j--) x->chave[j+1] = x->chave[j];
  x->chave[i] = z->chave[0];
  x->numChaves++;
}

void insercaoNaoCheia(NO* r,int k) {
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
  printf("INSERCAO do %i \n",k);
  if(busca(T->folhas,k)) { //tirar depois, usar so para testar se funciona
    printf("REPETIDO \n");
    return false;
  }

  NO* r = T->raiz;
  NO* f = T->folhas;

  if(r->numChaves == (2*t-1) && (!r->folha)) { //está cheio na raiz
    NO* s = alocaNo();
    T->raiz = s;
    s->folha = false;
    s->numChaves = 0;
    s->filhos[1] = r;
    split(s,r); //split quando nao envolve folhas
    insercaoNaoCheia(s,k); //adicionar no indice
    insercaoNaoCheiaF(r,f,k); //adicionar na folha
  }
  else if(r->numChaves == (2*t-1)) { // eh folha
    splitFolha(r,f); //split envolvendo folhas
    insercaoNaoCheia(r,k); //adicionar no indice
    insercaoNaoCheiaF(r,f,k); //adicionar na folha
  }
  else { //nao ta cheio
    insercaoNaoCheia(r,k); //adicionar no indice
    insercaoNaoCheiaF(r,f,k); //adicionar na folha
  }
  return true;
}

bool remover(NO* raiz,int chave) {
  printf("REMOVER \n");
}
void removerDaRaiz(ArvB* T, int k) {}
void imprimir(ArvB* T, NO* raiz) {

}


int main(int argc,char *argv[]) {
  FILE *f,*fsaida;
  if(!(f = fopen(argv[1],"r"))) return 0;
  else {
    fsaida = fopen(argv[2],"w+");
    ArvB* arvore;
    criaArvoreB(arvore);
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
