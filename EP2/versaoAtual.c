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
  TipoChave chave[2*t-1];
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
  if(!(folhas = (FOLHA*) malloc(sizeof(FOLHA)))) return false;

  x->filhosFolha = NULL;
  x->numChaves = 0;
  x->folha = false; //Pra garantir que vai verificar certinho

  folhas->numChaves = 0;
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

FOLHA* busca(FOLHA* raiz,int chave) {
  int i = 1;
  while(raiz) { //enquanto houver proximos
    while(i <= raiz->numChaves && chave > raiz->chave[i]) i++;
    if(chave > raiz->chave[i]) raiz->prox; //Eh maior que esse bloco atual, olha o proximo bloco
    else return raiz; //Nao atingiu o maximo de chaves, entao encontrou
  }
  return NULL;
} 

void split(NO* x, NO* y) { //lidando com o split que é do no interno
  NO* z = alocaNo(); //lado direito
  z->folha = y->folha;
  z->numChaves = t-1;

  for(int j = 1; j < t;j++) z->chave[j] = y->chave[j+t];
  if(!y->folha) { //se nao eh folha, tem filhos. Se tem filhos, o que o filho eh?
    if(!y->filhosFolha) for(int j = 1; j < t+1; j++) z->filhos[j] = y->filhos[j+t];
    else { //Se eh folha, eu tenho que fazer o z apontar para eles
      FOLHA* dividir = y->filhosFolha;
      for(int i = 1; i < t;i++) dividir = dividir->prox; //passando até depois da mediana
      z->filhosFolha = alocaFolha();
      z->filhosFolha = dividir; //passo os filhos para o z apontar
    }
  }
  y->numChaves = t-1;

  for(int j = x->numChaves+1; j > 2;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[2] = z;

  for(int j = x->numChaves; j > 1;j--) x->chave[j+1] = x->chave[j];
  x->chave[1] = y->chave[t]; //mediana pro pai
  x->numChaves++;
}

void splitFolha(NO* x, FOLHA* y) { //lidando com o split que é do no interno
  FOLHA* z = alocaFolha(); //lado direito
  z->folha = y->folha;
  z->numChaves = t-1;

  for(int j = 1; j < t;j++) z->chave[j] = y->chave[j+t]; 
  FOLHA* dividir = y->filhosFolha;
  for(int i = 1; i < t;i++) dividir = dividir->prox; //passando até depois da mediana
  z->filhosFolha = alocaFolha();
  z->filhosFolha = dividir; //passo os filhos para o z apontar

  y->numChaves = t-1;

  for(int j = x->numChaves+1; j > 2;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[2] = z;

  for(int j = x->numChaves; j > 1;j--) x->chave[j+1] = x->chave[j];
  x->chave[1] = y->chave[t]; //mediana pro pai
  x->numChaves++;
}

void insercaoNaoCheia(NO* x,int k,FOLHA* f) {
  int i = x->numChaves;
  FOLHA* atual = x->filhosFolha;
  FOLHA* prox = x->filhosFolha;
  if(x->filhosFolha) {
    while(i >= 1 && k < x->chave[i]) {
      atual = atual->prox;
      i--;
    }
    atual->prox = x->filhosFolha->prox; 
    x->filhosFolha->prox = atual;
    atual->chave[i+1] = k;
    atual->numChaves++;
  }
  else {
    while(i >= 1 && k < x->chave[i]) i--;
    i++;
    if((x->filhos[i])->numChaves == 2*t-1) {
      split(x,x->filhos[i]);
      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k,f);
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
    insercaoNaoCheia(s,k,f); //adicionar no indice
  }
  else if(f->numChaves == (2*t-1)) { // eh folha
    NO* s = alocaNo();
    T->raiz = s;
    s->folha = false;
    s->numChaves = 0;
    s->filhosFolha = f;
    splitFolha(s,f); //split envolvendo folhas
    insercaoNaoCheia(r,k,f); //adicionar no indice
  }
  else { //nao ta cheio
    insercaoNaoCheia(r,k,f); //adicionar no indice
  }
  return true;
}

bool remover(NO* raiz,int chave) {
  printf("REMOVER \n");
}
void removerDaRaiz(ArvB* T, int k) {}
void imprimir(ArvB* T, NO* raiz) {
  printf("nós folhas: \n (");
  FOLHA* f = T->folhas;
  int i = 1;
  while(f) {
    f = f->prox;
    printf("%i ",f->chave[i]);
    i++;
  }
  printf(")\n");
  //depois printar em in ordem

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
