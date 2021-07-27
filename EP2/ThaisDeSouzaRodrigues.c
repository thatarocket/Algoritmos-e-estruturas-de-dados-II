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
  TipoChave chave[2*t]; //max chaves do no. 2t-1 com 0 = 2t
  struct str_no* filhos[2*t+1]; //max filhos do no. 2t com 0 = 2t+1
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
bool juntar(NO* x,char caso, int i, NO* y,int c);
bool remover(NO* raiz, int chave); 
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

NO* alocaNo() {
  NO* x = (NO*) malloc(sizeof(NO));
  return x;
}

NO* busca(NO* raiz,int chave) {
  int i = 1;
  NO* achado = NULL;
  //bool encontrei = false
  
  while(i <= raiz->numChaves && chave >= raiz->chave[i]) i++;
  i--;
  printf("sou o %i e i %i \n",raiz->chave[i],i);
  if(i <= raiz->numChaves+1 && chave >= raiz->chave[i]) {
    if(raiz->folha) {
      for(int i = 1; i <= raiz->numChaves;i++) {
        if(raiz->chave[i] == chave) return raiz;
      }
      return NULL;
    }
    else if(!raiz->folha) achado = busca(raiz->filhos[i+1],chave);
  }
  else if(i == 0) achado = busca(raiz->filhos[1],chave);

  return achado;
} 

bool split(NO* x,int i, NO* y) { //lidando com o split que é do no interno
  //printf("split \n");
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

bool insercao(ArvB* T, int k) {
  //printf("inserindo o %i \n",k);
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

bool juntar(NO* x,char caso, int i, NO* y,int c) {
  // y eh o da direita, que recebe 
  // x eh o pai 

  if(caso == 'e') { //esquerda empresta
    //printf("entrei e \n");
    y->numChaves++; //aumento mais um pra passar 
    for(int j = y->numChaves; j >= 1;j--) y->chave[j+1] = y->chave[j]; //pega o da esquerda e coloca na direita
    y->chave[1] = x->filhos[i-1]->chave[x->filhos[i-1]->numChaves]; //passo o emprestado pra direita
    x->filhos[i-1]->numChaves--;
    x->chave[i-1] = y->chave[1];
  }
  else if(caso == 'd') {
    //printf("entrei d \n");
    y->numChaves++;
    y->chave[y->numChaves+1] = x->filhos[i+1]->chave[1];
    x->chave[i] = x->filhos[i+1]->chave[1];
    for(int j = 1; j <= y->numChaves;j++) x->filhos[i+1]->chave[j] = x->filhos[i+1]->chave[j+1];
    x->filhos[i+1]->numChaves--;
    int menor;
    int indice;
    /* for(int v = 1; v <= x->filhos[i+2]->numChaves;v++) {
      if(v == 1) {
        menor = x->filhos[i+2]->chave[v];
        indice = v;
      }
      else if(menor == c) {
        menor = y->chave[v];
        indice = v;
      }
    } 
    x->chave[i] = y->chave[indice]; */
  }
  else if(caso == 'n') {
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

bool remover(NO* raiz,int chave) {
  printf("REMOVER %i \n",chave);
  if(!busca(raiz,chave)) {
    printf("FALSO \n");
    return false; 
  }
  else printf("TRUE \n"); //se nao tem o valor
  int i = raiz->numChaves;
  
  while(i >= 1 && chave < raiz->chave[i]) i--;
  i++;
  if(!(raiz->folha) && raiz->filhos[i]->folha) {
    if(raiz->filhos[i]->numChaves >= t) { // o removido da folha ja tem mais que o minimo
      if(raiz->chave[i-1] == chave) {
        int k = i;
        printf("entrei if 2 de chave %i \n",raiz->chave[k-1]);
        char caso = 'n';
        juntar(raiz,caso,k-1,raiz->filhos[k],chave);
      }
      remover(raiz->filhos[i],chave);
    }
    else if(raiz->filhos[i-1] && raiz->filhos[i-1]->numChaves >= t) { // o irmao da esquerda tem como emprestar
      printf("caso esquerda \n");
      char caso = 'e';
      int s = 1;
      juntar(raiz,caso,i,raiz->filhos[i],chave);
      remover(raiz->filhos[i],chave);
    }
    else if(raiz->filhos[i+1] && raiz->filhos[i+1]->numChaves >= t) { // o irmao da direita tem como emprestar
      printf("caso direita \n");
      char caso = 'd';
      juntar(raiz,caso,i,raiz->filhos[i],chave);
      remover(raiz->filhos[i],chave);
    }
    else { //os irmaos nao podem ajudar, entao junta os irmaos
      printf("entrei else 1 %i e i %i \n",raiz->chave[i],i);
      printf("filho dele %i \n",raiz->filhos[i]->chave[1]);

      if(i > 1) { //se tem elemento na esquerda(nao eh o primeiro)
        int tamAnt = raiz->filhos[i-1]->numChaves; //tamanho anterior da esquerda
        raiz->filhos[i-1]->numChaves += raiz->filhos[i]->numChaves; //acerta o numChaves 

        for(int k = 1; k <= raiz->filhos[i]->numChaves;k++) //acrescenta os novos itens
          raiz->filhos[i-1]->chave[tamAnt+k] = raiz->filhos[i]->chave[k];

        for(int j = i; j <= raiz->numChaves;j++) raiz->filhos[j] = raiz->filhos[j+1]; //arrumando os filhos da raiz
        for(int j = i-1; j <=raiz->numChaves;j++) raiz->chave[j] = raiz->chave[j+1]; //arrumando as chaves da raiz
        raiz->numChaves--;
        printf("raiz->filhos[i-1]: %i \n",raiz->filhos[i-1]->chave[1]);
        remover(raiz->filhos[i-1],chave); //agora pra eliminar da folha
      }
      else if(i == 1) { //nao tem elemento da esquerda(eh o primeiro)
        printf("else if 1 \n");
        int tamAnt = raiz->filhos[i+1]->numChaves;
        raiz->filhos[i+1]->numChaves += raiz->filhos[i]->numChaves; //acerta o numChaves 

        for(int k = 1; k < raiz->filhos[i]->numChaves;k++) 
          raiz->filhos[i+1]->chave[k+1] = raiz->filhos[i+1]->chave[k]; //passa para o lado para dar espaco para o da esquerda entrar
        
        for(int m = 1; m <= raiz->filhos[i]->numChaves;m++) raiz->filhos[i+1]->chave[m] = raiz->filhos[i]->chave[m]; //insere o da esquerda
        for(int j = i; j <= raiz->numChaves;j++) raiz->filhos[j] = raiz->filhos[j+1]; //arrumando os filhos da raiz
        raiz->numChaves--;
        remover(raiz->filhos[i],chave); //agora pra eliminar da folha
      }
    }
  }
  else if(raiz->folha) {
    i--;
    int posic = -1;
    printf("entrei no elimina folha: raiz %i com i:%i \n",raiz->chave[1],i);
    for(int j = 1; j <= raiz->numChaves;j++) {
      if(raiz->chave[j] == chave) {
        posic = j;
        break;
      }
    }
    for(int j = posic; j <= raiz->numChaves; j++) raiz->chave[j] = raiz->chave[j+1]; //caso chega o primeiro da lista
    raiz->numChaves--;
  }
  else {
    printf("entrou no else restante \n");
    remover(raiz->filhos[i],chave);
  }
}

void imprimir(FILE* f, ArvB* T, NO* raiz) {
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
          remover(arvore->raiz,num);
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