// Thais de Souza Rodrigues 11796941
#define MAXNUMVERTICES 100
#define branca 0
#define cinza 1
#define preta 2

typedef int Peso;

typedef struct aux {
	int vdest; //vertice destino
	struct aux* prox;
	Peso peso;
} Aresta;

typedef Aresta* Apontador;

typedef struct {
	Apontador* listaAdj;
	int numVertices;
	int numArestas;
	int tempo;
} Grafo;

typedef struct aux2 { //Para a fila
  int id;
  struct aux2* prox;
} Item;

typedef Item* Apontador2;

typedef struct { //Para ser usado na busca de largura
  Apontador2 inicio;
  Apontador2 fim;
} Fila;
