//Thais de Souza Rodrigues 11796941
#include "busca.c"

void visitaConexo(Grafo* grafo, int j, int i, int* conect) {
//Procurara pelos componenentes conexos
	conect[i] = j;
	Apontador v = grafo->listaAdj[i];
	while (v) {
		if (conect[v->vdest] == -1) visitaConexo(grafo,j,v->vdest,conect);
		v = v->prox;
	}
}

void componenteConexo(Grafo* grafo,FILE* fsaida) {
//Ira chamar o visitaConexo para encontrar os componentes conexos e depois imprimir
	int conect[grafo->numVertices];
	int cont = 0;
	int inicio = 1;
	bool entrou = false;
	bool imprimiu[grafo->numVertices];

	memset(conect,-1,sizeof(conect));
	memset(imprimiu,false,sizeof(imprimiu));

	for(int i = 0; i  < grafo->numVertices;i++) {
		if(conect[i] == -1) {
			visitaConexo(grafo,i,i,conect);
			cont++;
		}
	}

	fprintf(fsaida,"Componentes Conectados: \n");
	for(int j = 0; j < grafo->numVertices; j++) {
		for(int k = 0; k < grafo->numVertices;k++) {
			if(conect[k] == j) {
				if(inicio <= cont) {
					if(!imprimiu[j]) {
						fprintf(fsaida,"C%i: ",inicio);
						imprimiu[j] = true;
					}
					fprintf(fsaida,"%i ",k);
					entrou = true;
				}
			}
		}
		if(inicio <= cont && entrou == true) {
			fprintf(fsaida,"\n");
			inicio++;
		}
		entrou = false;	
	}
	fprintf(fsaida,"\n");
		
} 

int menor(int num1,int num2) { //Ira procurar o menor valor entre dois numeros
	if(num1 < num2) return num1;
	else return num2;
}

int filhoRaiz(int u, Grafo* grafo) { //Ira ver quantos filhos tem certo vertice
	int filhos = 0;
	Apontador v = grafo->listaAdj[u];
	while(v) {
		filhos++;
		v = v->prox;
	}
	return filhos;
}


void visitaVertice(Grafo* grafo, FILE* fout, int* tempoDesc,int* tempoMin,int* cont,int u,int* visitado,bool* vertice,int* antecessor,int raiz) {
	tempoDesc[u] = tempoMin[u] =(*cont)++; //Inicialmente, sao iguais ao cont
	visitado[u] = cinza; // ja foi visitado 
	Apontador v = grafo->listaAdj[u];

	while(v) {
		if(visitado[v->vdest] == branca) { // se o adj ainda nao foi visitado
			antecessor[v->vdest] = u; // guarda quem eh o pai de cada adjacente
			visitaVertice(grafo,fout,tempoDesc,tempoMin,cont,v->vdest,visitado,vertice,antecessor,raiz);

			if(tempoMin[v->vdest] >= tempoDesc[u] && u != raiz) vertice[u] = true;
			else if(u == raiz && tempoMin[v->vdest] > tempoDesc[u]) {
				if(filhoRaiz(u,grafo) >= 2) vertice[u] = true;
			}
			tempoMin[u] = menor(tempoMin[u],tempoMin[v->vdest]);
		}
		else {
			if(antecessor[u] != v->vdest) tempoMin[u] = menor(tempoMin[u],tempoDesc[v->vdest]);
		}
		v = v->prox;
	}

	visitado[u] = preta;
	return;
}

void buscaVertice(Grafo* grafo, FILE* fout) {
	int antecessor[grafo->numVertices]; 
	int visitado[grafo->numVertices]; // 0 branca 1 cinza 2 preta
	int tempoDesc[grafo->numVertices];
	int tempoMin[grafo->numVertices];
	bool vertice[grafo->numVertices]; //Registra quais sao os vertices de articulacao
	int cont = 0; //Conta cada chamada do metodo

	memset(antecessor,-1,sizeof(antecessor));
	memset(visitado,branca,sizeof(visitado));
	memset(tempoDesc,-1,sizeof(tempoDesc)); //Tempo de descoberta de cada vertice
	memset(tempoMin,-1,sizeof(tempoMin));
	memset(vertice,false,sizeof(vertice));

	int filhoRaiz = 0; //Conta quantos filhos tem a raiz
	int raiz; // vai trocando por quem eh a raiz

	for(int i = 0; i < grafo->numVertices;i++) {
		raiz = i;
		if(visitado[i] == branca) visitaVertice(grafo,fout,tempoDesc,tempoMin,&cont,i,visitado,vertice,antecessor,raiz);
		filhoRaiz = 0;
	}

	fprintf(fout,"Vertices de articulacao: \n");
	for(int i = 0; i < grafo->numVertices;i++) if(vertice[i]) fprintf(fout,"%i ",i);
	fprintf(fout,"\n");
}

int main() {
	FILE *f,*fsaida;
	Grafo* grafo;
	int numVertices,numArestas, vPart, vdest, peso;

	if((f = fopen("entrada.txt","r")) == NULL) return 0; //Se nao houver um arquivo	
    else {
    	fsaida = fopen("saida.txt","w+");
    	if(fscanf(f, "%i %i", &numVertices, &numArestas) != EOF) { 
    		grafo = inicializaGrafo(numVertices,numArestas);
    		while(fscanf(f, "%i %i %i", &vPart,&vdest,&peso) != EOF) {
    			insereAresta(grafo,vPart,vdest,peso);
    		}
    		imprimeEntrada(grafo,fsaida);
    		buscaLargura(grafo,fsaida);
	    	buscaProfundidade(grafo,fsaida);
	    	componenteConexo(grafo,fsaida);
	    	buscaVertice(grafo,fsaida); //vertices de articulacao
    	}
    	
	}
	fclose(f);
	return 0;
}