/* ######## Thais de Souza Rodrigues ###########
   ########   Número USP: 11796941   ###########
   ########        AED - EP2         ###########
   ########        Arvore B+         ########### */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define t 3 //grau minimo

typedef int TipoChave;

typedef struct str_no {
  TipoChave chave[2*t]; //max chaves do no. Onde eh 2t-1, mas considerando a posicao 0 = 2t
  struct str_no* filhos[2*t+1]; //max filhos do no. Onde eh 2t, mas considerando a posicao 0 = 2t+1
  int numChaves; //num chaves armazenadas
  bool folha; //folha ou no interno
  struct str_no* prox;
} NO;

typedef struct {
  NO* raiz;
} ArvB;

// Métodos implementados 
ArvB* criaArvoreB(ArvB* T);
NO* alocaNo();
NO* busca(NO* raiz,int chave);
bool split(NO* x,int indice, NO* y);
void insercaoNaoCheia(NO* x,int k);
bool insercao(ArvB* T, int k);
bool juntar(NO* x,char caso, int i, NO* y,int c);
bool arrumaProx(ArvB* T);
bool remover(ArvB* T,NO* raiz, int chave); 
void imprimir(FILE* f,ArvB* T, NO* raiz);

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

NO* alocaNo() { //aloca memoria para o no
  NO* x = (NO*) malloc(sizeof(NO));
  return x;
}

NO* busca(NO* raiz,int chave) { //busca elementos na arvore
  int i = 1;
  NO* achado = NULL;

  while(i <= raiz->numChaves && chave >= raiz->chave[i]) i++;
  i--;
  if(i <= raiz->numChaves+1 && chave >= raiz->chave[i]) {
    if(raiz->folha) {
      for(int i = 1; i <= raiz->numChaves;i++) {
        if(raiz->chave[i] == chave) return raiz;
      }
      return NULL;
    }
    else if(!raiz->folha) achado = busca(raiz->filhos[i+1],chave);
  }
  else if(i == 0 && !(raiz->folha)) achado = busca(raiz->filhos[1],chave);
  return achado;
} 

bool split(NO* x,int i, NO* y) { //realiza a separacao dos elementos, caso necessario
  NO* z = alocaNo();
  z->folha = y->folha;

  if(y->folha) { 
    z->numChaves = t;
    for(int j = 1; j <= t;j++) z->chave[j] = y->chave[j+t-1]; //pegar a mediana também 
    if(!y->folha) for(int j = 1; j <= t+1;j++) z->filhos[j] = y->filhos[j+t]; 
    y->numChaves = t-1; 
  }
  else {
    z->numChaves = t-1;
    for(int j = 1; j <=t-1;j++) z->chave[j] = y->chave[j+t];
    if(!y->folha) for(int j = 1; j <= t;j++) z->filhos[j] = y->filhos[j+t]; 
    y->numChaves = t-1; 
  }

  for(int j = x->numChaves+1; j >= i+1;j--) x->filhos[j+1] = x->filhos[j];
  x->filhos[i+1] = z; 
  for(int j = x->numChaves; j >= i; j--) x->chave[j+1] = x->chave[j];  
  x->chave[i] = y->chave[t]; 
  x->numChaves++;

  y->prox = z;
  z->prox = NULL;
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
    if((x->filhos[i])->numChaves == 2*t-1) { 
      split(x,i,x->filhos[i]);
      if(k > x->chave[i]) i++;
    }
    insercaoNaoCheia(x->filhos[i],k);
  }
}

bool insercao(ArvB* T, int k) { //insere elementos na arvore
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

bool juntar(NO* x,char caso, int i, NO* y,int c) { //junta os elementos da arvore, caso necessario
 
  if(caso == 'e') { //esquerda empresta
    y->numChaves++; //aumento mais um nas chaves, para poder passar 
    for(int j = y->numChaves; j >= 1;j--) y->chave[j+1] = y->chave[j]; //pega o da esquerda e coloca na direita
    y->chave[1] = x->filhos[i-1]->chave[x->filhos[i-1]->numChaves]; //passo o emprestado pra direita
    x->filhos[i-1]->numChaves--;
    x->chave[i-1] = y->chave[1];
  }
  else if(caso == 'd') { //direita empresta
    y->numChaves++;
    y->chave[y->numChaves+1] = x->filhos[i+1]->chave[1];
      
    for(int j = 1; j <= y->numChaves;j++) x->filhos[i+1]->chave[j] = x->filhos[i+1]->chave[j+1];
    x->filhos[i+1]->numChaves--;

    int menor;
    int primeiro = i;
    for(int v = i; v <= x->numChaves+1;v++) {
      if(v == primeiro) menor = x->filhos[i+1]->chave[v];
      else if(menor == c) { //pode acontecer da primeira chaver ser a que vai ser eliminada
        menor = x->filhos[i+1]->chave[v];
        break;
      }
    } 
    x->chave[i] = menor;
  }
  else if(caso == 'n') { //nao eh necessario ninguem emprestar
    int menor;
    int indice;
    for(int v = 1; v <= y->numChaves;v++) {
      if(v == 1) {
        menor = y->chave[v];
        indice = v;
      }
      else if(menor == c) {
        menor = y->chave[v];
        indice = v;
      }
    }
    x->chave[i] = y->chave[indice];
  }
}

bool arrumaProx(ArvB* T) { //corrige os ponteiros das folhas
  NO* arrumando = T->raiz;
  int i = 1;
  while(i <= arrumando->numChaves+1) { //cada filho deve apontar para a proxima folha, caso tenha
    if(i <= arrumando->numChaves) arrumando->filhos[i]->prox = arrumando->filhos[i+1];
    if(i == arrumando->numChaves+1) arrumando->filhos[i]->prox = NULL;
    i++;
  }
  return true;
}

bool remover(ArvB* T,NO* raiz,int chave) { //remove um elemento, caso exista
  if(!busca(raiz,chave)) return false; //se nao tem o valor para eliminar
  int i = raiz->numChaves;
  while(i >= 1 && chave < raiz->chave[i]) i--;
  i++;
  if(!(raiz->folha) && raiz->filhos[i]->folha) {
    if(raiz->filhos[i]->numChaves >= t) { // o lugar do removido da folha ja tem mais que o minimo
      if(raiz->chave[i-1] == chave) {
        int k = i;
        char caso = 'n';
        juntar(raiz,caso,k-1,raiz->filhos[k],chave);
      }
      remover(T,raiz->filhos[i],chave);
    }
    else if(raiz->filhos[i-1] && raiz->filhos[i-1]->numChaves >= t) { // o irmao da esquerda tem como emprestar
      char caso = 'e';
      int s = 1;
      juntar(raiz,caso,i,raiz->filhos[i],chave);
      remover(T,raiz->filhos[i],chave);
    }
    else if(raiz->filhos[i+1] && raiz->filhos[i+1]->numChaves >= t) { // o irmao da direita tem como emprestar
      char caso = 'd';
      juntar(raiz,caso,i,raiz->filhos[i],chave);
      remover(T,raiz->filhos[i],chave);
    }
    else { //os irmaos nao podem ajudar, entao junta os irmaos

      if(i > 1) { //se tem elemento na esquerda(nao eh o primeiro)
        int tamAnt = raiz->filhos[i-1]->numChaves; //tamanho anterior da esquerda
        raiz->filhos[i-1]->numChaves += raiz->filhos[i]->numChaves; //acerta o numChaves 

        for(int k = 1; k <= raiz->filhos[i]->numChaves;k++) //acrescenta os novos itens
          raiz->filhos[i-1]->chave[tamAnt+k] = raiz->filhos[i]->chave[k];

        for(int j = i; j <= raiz->numChaves;j++) raiz->filhos[j] = raiz->filhos[j+1]; //arrumando os filhos da raiz
        for(int j = i-1; j <=raiz->numChaves;j++) raiz->chave[j] = raiz->chave[j+1]; //arrumando as chaves da raiz
        raiz->numChaves--;
        remover(T,raiz->filhos[i-1],chave); //agora pra eliminar da folha
      }
      else if(i == 1) { //nao tem elemento da esquerda(eh o primeiro)
        int tamAnt = raiz->filhos[i+1]->numChaves;
        raiz->filhos[i+1]->numChaves += raiz->filhos[i]->numChaves; //acerta o numChaves 

        for(int k = 1; k <= raiz->filhos[i]->numChaves;k++) 
          raiz->filhos[i+1]->chave[k+1] = raiz->filhos[i+1]->chave[k]; //passa para o lado para dar espaco para o da esquerda entrar
    
        for(int m = 1; m <= raiz->filhos[i]->numChaves;m++) raiz->filhos[i+1]->chave[m] = raiz->filhos[i]->chave[m]; //insere o da esquerda

        for(int j = i; j <= raiz->numChaves;j++) raiz->filhos[j] = raiz->filhos[j+1]; //arrumando os filhos da raiz
        for(int j = i-1; j <=raiz->numChaves;j++) raiz->chave[j] = raiz->chave[j+1];
        raiz->numChaves--;
        remover(T,raiz->filhos[i],chave); //agora pra eliminar da folha
      }
    }
  }
  else if(raiz->folha) { //Se eh folha, eh o momento para eliminar
    i--;
    int posic = -1;
    for(int j = 1; j <= raiz->numChaves;j++) {
      if(raiz->chave[j] == chave) {
        posic = j;
        break;
      }
    }
    for(int j = posic; j <= raiz->numChaves; j++) raiz->chave[j] = raiz->chave[j+1]; //caso seja o primeiro da lista
    raiz->numChaves--;
    arrumaProx(T); //arruma os ponteiros das folhas
  }
  else if(!raiz->folha) { 
    
    if(raiz->chave[i-1] == chave) {
      NO* andando = raiz;

      while(!andando->filhos[i-1]->folha) andando = andando->filhos[i-1];
      andando = andando->filhos[andando->numChaves+1];
  
      raiz->chave[i-1] = andando->chave[andando->numChaves];
      andando->numChaves--;
      insercao(T,raiz->chave[i-1]);
      remover(T,raiz->filhos[i],chave); //ainda nao chegou nas folhas
    }
    remover(T,raiz->filhos[i],chave); //ainda nao chegou nas folhas
  }
}

void imprimir(FILE* f, ArvB* T, NO* raiz) { //imprime a arvore atual
  if(T->raiz->numChaves == 0) {
    fprintf(f,"vazia \n");
    return;
  }
  NO* atual = raiz;
  fprintf(f,"(");
  for(int j = 1; j <= atual->numChaves+1;j++) {
    if(atual->folha) { 
      if(j <= atual->numChaves) {
        if(atual->chave[j]) {
          if(j == 1) fprintf(f,"%i", atual->chave[j]);
          else if(j == atual->numChaves && j != 1) fprintf(f," %i", atual->chave[j]);
          else if(j != atual->numChaves) fprintf(f," %i", atual->chave[j]);
        }
      }
    }
    else {
      imprimir(f,T,atual->filhos[j]);
      if(j <= atual->numChaves) fprintf(f," %i ", atual->chave[j]); // imprime a raiz
    }  
  }
  if(T->raiz->chave[raiz->numChaves] == raiz->chave[raiz->numChaves]) fprintf(f,") \n"); 
  else fprintf(f,")");
}

int main(int argc,char *argv[]) {
  FILE *f,*fsaida;
  if(!(f = fopen(argv[1],"r"))) return 0;
  else {
    fsaida = fopen(argv[2],"w+");
    ArvB* arvore = criaArvoreB(arvore);
    char comando;
    while (comando != 'f'){
      int num;
      switch (comando) {
        case 'i': {
          fscanf(f,"%i",&num);
          insercao(arvore,num); 
          break;
        } 
        case 'r' : {
          fscanf(f,"%i",&num);
          remover(arvore,arvore->raiz,num);
          break;
        }
        case 'p' : imprimir(fsaida,arvore,arvore->raiz); break;
        case 'f' : break;
        case EOF: break;
        default: break;
      }
      fscanf(f,"%c ",&comando);
    }
  }
  fclose(f);
  return 0;
}