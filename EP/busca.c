//Thais de Souza Rodrigues 11796941
#include "busca.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

Grafo* inicializaGrafo(int numVert, int numArest) { 
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));  //Aloca memoria para o grafo
    grafo->numVertices = numVert;
    grafo->numArestas = numArest;
    grafo->tempo = 0;
    grafo->listaAdj = (Apontador*) malloc(MAXNUMVERTICES+1*sizeof(Apontador)); 
    for(int i=0;i<grafo->numVertices;i++) grafo->listaAdj[i] = NULL; 
    return grafo;
}

bool insereAresta(Grafo *grafo,int vPart,int vDest,Peso p){
	if(!grafo) return false; //Retorna falso quando eh inserido um valor invalido
	if(vPart < 0 || vPart >= grafo->numVertices) return false;
	if(vDest < 0 || vDest >= grafo->numVertices) return false;

	Apontador atual1 = (Apontador) malloc(sizeof(Aresta));
    Apontador atual2 = (Apontador) malloc(sizeof(Aresta));
    // Como eh nao direcionado, deve ser adicionado de duas formas:
    atual1->vdest = vPart; 
    atual2->vdest = vDest; 
  	
  	atual1->peso = p;
  	atual2->peso = p;

    atual1->prox = grafo->listaAdj[atual2->vdest];
    atual2->prox = grafo->listaAdj[atual1->vdest];

    grafo->listaAdj[atual2->vdest] = atual1; 
    grafo->listaAdj[atual1->vdest] = atual2; 

    return true;
}

void imprimeEntrada(Grafo* grafo, FILE* fout) {
//imprimeEntrada irá imprimir a entrada em ordem crescente, em relação ao vertice origem
	Apontador p;
	int visitado[grafo->numVertices][grafo->numVertices];
	memset (visitado, false, sizeof(visitado)); //verifica se ja passou pela posicao

    fprintf(fout,"%i %i \n",grafo->numVertices,grafo->numArestas); 
    for(int i = 0; i < grafo->numVertices; i++){
        p = grafo->listaAdj[i];
        while (p){
        	if(visitado[i][p->vdest] == false && visitado[p->vdest][i] == false) {
	           	fprintf(fout,"%i %i %i \n", i, p->vdest, p->peso);
	           	visitado[i][p->vdest] = true;
	           	visitado[p->vdest][i] = true;
	        }
	        p = p->prox;
        } 
    }
    fprintf(fout,"\n");
}

void pegaRaiz(Grafo* grafo, FILE* fout,int* raiz,int* antecessor,int j) { 
//O array Raiz começa com o valor -10. Troca-se por -1 ao visitar
//Depois, ao chegar a raiz, todos que eram -1, trocam-se pelo valor da raiz
	if(antecessor[j] != -1) {
		raiz[j] = -1;
		pegaRaiz(grafo,fout,raiz,antecessor,antecessor[j]);
	}
	else {
		raiz[j] = j;
		for(int i = 0; i < grafo->numVertices;i++) if(raiz[i] == -1) raiz[i] = j;
	}
	return;
}

void imprimeAntecessor(Grafo* grafo, FILE* fout,int* antecessor,int j) {
// Apenas a raiz tem um valor de antecessor igual a -1, logo utilizo isso para imprimir
	if(antecessor[j] != -1) {
		imprimeAntecessor(grafo,fout,antecessor,antecessor[j]);
		fprintf(fout,"%i ",j);
	}
	else fprintf(fout,"%i ",j);
	return;
}

void visitaBP(Grafo* grafo, int j, int* antecessor, int* cor, int* tempoDesc,int* tempoTerm) {
//Ira realizar a busca em profundidade, aproveitando-se do uso das cores, aprendido em aula
	cor[j] = cinza;
	tempoDesc[j] = ++grafo->tempo;
	Apontador v = grafo->listaAdj[j];
	while (v) {
		if (cor[v->vdest] == branca) {
			antecessor[v->vdest] = j;
			visitaBP(grafo,v->vdest,antecessor,cor,tempoDesc,tempoTerm);
		}
		v = v->prox;
	}
	tempoTerm[j] = ++grafo->tempo;
	cor[j] = preta;
}


void buscaProfundidade(Grafo* grafo,FILE* fsaida) {
// Ira chamar o visitaBP que ira realizar a busca em si e depois imprimir
	grafo->tempo = -1;
	int antecessor[grafo->numVertices];
	int cor[grafo->numVertices];
	int tempoDesc[grafo->numVertices];
	int tempoTerm[grafo->numVertices];

	memset (antecessor, -1, sizeof(antecessor)); 
	memset (cor, branca, sizeof(cor)); 
	memset (tempoDesc, 0, sizeof(tempoDesc)); 
	memset (tempoTerm, 0, sizeof(tempoTerm)); 

	for (int j = 0; j < grafo->numVertices;j++) if (cor[j] == branca) visitaBP(grafo,j,antecessor,cor,tempoDesc,tempoTerm);

	fprintf(fsaida,"BP: \n");
	for(int k = 0; k < grafo->tempo;k++) { //Esse eh do antecessor
		for(int i = 0; i < grafo->numVertices;i++) { //Esse representa os adjacente
			if(tempoDesc[i] == k) fprintf(fsaida,"%i ",i); 
		}
	}

	
	fprintf(fsaida,"\r\n");
	fprintf(fsaida,"Caminhos BP: \n");
	for(int i = 0; i < grafo->numVertices;i++) {
		imprimeAntecessor(grafo,fsaida,antecessor,i);
		fprintf(fsaida,"\n");
	}
	fprintf(fsaida,"\n");
}

Fila* inicializaFila() { //Inicializara a fila que sera usada no busca em largura
	Fila* atual = (Fila*) malloc(sizeof(Fila));
	atual->inicio = NULL;
	atual->fim = NULL;
	return atual;
}
void insereFila(Fila* fila, int i) {
	Apontador2 atual = (Apontador2) malloc(sizeof(Item));
	atual->id = i;
	atual->prox = NULL;
	if(!fila->inicio) fila->inicio = atual;
  	else fila->fim->prox = atual;
  	fila->fim = atual;
}
int removeFila(Fila* fila) {
	int eliminado;
	Apontador2 atual = fila->inicio;
	eliminado = atual->id;
	fila->inicio = atual->prox;
	free(atual);
	if(!fila->inicio) fila->fim = NULL;
	return eliminado;
}

void visitaLargura(Grafo* grafo, int s, int* cor,int* antecessor, int* distancia,int* ordem,int* cont) {
// Ira realizar a busca em largura, utilizando fila e cor, aprendidos em aula
	cor[s] = cinza;
	ordem[*cont] = s;
	(*cont)++;
	distancia[s] = 0;
	Fila* fila;
	fila = inicializaFila();
	insereFila(fila,s);
		
	while(fila->inicio) {
		int w = removeFila(fila);
		for(Apontador u = grafo->listaAdj[w]; u != NULL; u = u->prox) {
			if (cor[u->vdest] == branca) {
				cor[u->vdest] = cinza;
				antecessor[u->vdest] = w;
				distancia[u->vdest] = distancia[w] + 1;
				ordem[*cont] = u->vdest;
				(*cont)++;
				insereFila(fila,u->vdest);
			}
		}
		cor[w] = preta;
	}
}

void buscaLargura(Grafo* grafo,FILE* fsaida) {
//Ira chamar o visitaLargura e depois imprimir como foi a busca em largura
	int cor[grafo->numVertices];
	int antecessor[grafo->numVertices];
	int distancia[grafo->numVertices];
	int ordem[grafo->numVertices];

	memset (cor, branca, sizeof(cor)); 
	memset (antecessor, -1, sizeof(antecessor)); 
	memset (distancia, -10, sizeof(distancia));
	memset (ordem, -10, sizeof(ordem));
	int cont = 0;
	for(int i = 0; i  < grafo->numVertices;i++) if(cor[i] == branca) visitaLargura(grafo,i,cor,antecessor,distancia,ordem,&cont);

	int raiz[grafo->numVertices]; 
	bool verif[grafo->numVertices];
	memset(raiz,-10,sizeof(raiz));
	memset(verif,false,sizeof(verif));

	for(int i = 0; i < grafo->numVertices;i++) if(!verif[i]) pegaRaiz(grafo, fsaida,raiz,antecessor,i); //Buscar as raizes dos componentes
	
	
	fprintf(fsaida,"BL: \n");
	for(int t = 0; t < grafo->numVertices;t++) fprintf(fsaida,"%i ",ordem[t]);
	fprintf(fsaida,"\n\n");

	fprintf(fsaida,"Caminhos BL: \n");
	for(int i = 0; i < grafo->numVertices;i++) {
		imprimeAntecessor(grafo,fsaida,antecessor,i);
		fprintf(fsaida,"\n");
	}
	fprintf(fsaida,"\n");
}

