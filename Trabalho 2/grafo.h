#ifndef GRAFO_H
#define GRAFO_H

typedef struct Aresta{
    int destino;
    int distancia;
    int proxima;
} Aresta;

typedef struct{
    int n;
    int *cabeca;
    Aresta *arestas;
    int qtArestas;
} Grafo;

void criarGrafo(Grafo *g, int n, int C);

void adicionarAresta(Grafo *g, int u, int v, int distancia);

void liberarGrafo(Grafo *g);

#endif // GRAFO_H
