#include <stdlib.h>
#include "grafo.h"

void criarGrafo(Grafo *g, int n, int C){
    g->n = n;
    g->cabeca = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        g->cabeca[i] = -1;
    }

    g->arestas = malloc(2 * C * sizeof(Aresta));
    g->qtArestas = 0;
}

void adicionarAresta(Grafo *g, int u, int v, int distancia){

    g->arestas[g->qtArestas].destino = v;
    g->arestas[g->qtArestas].distancia = distancia;
    g->arestas[g->qtArestas].proxima = g->cabeca[u];
    g->cabeca[u] = g->qtArestas++;
    g->arestas[g->qtArestas].destino = u;
    g->arestas[g->qtArestas].distancia = distancia;
    g->arestas[g->qtArestas].proxima = g->cabeca[v];
    g->cabeca[v] = g->qtArestas++;
}

void liberarGrafo(Grafo *g){
    free(g->cabeca);
    free(g->arestas);
}
