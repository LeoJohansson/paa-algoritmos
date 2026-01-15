#ifndef DP_H
#define DP_H

#include "grafo.h"

typedef struct{
    long long totalHabilidade;
    int *caminho;
    int tamCaminho;
    long long *quantidades;
} Solucao;

Solucao resolverDp(Grafo *g, int D, int W, int *pesos, int *habilidades);

#endif // DP_H
