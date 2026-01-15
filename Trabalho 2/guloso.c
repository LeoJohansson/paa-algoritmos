#include <stdlib.h>
#include "guloso.h"

Solucao resolverGuloso(Grafo *g, int D, int W, int *pesos, int *habilidades){

    Solucao sol = {0, NULL, 0, NULL};
    if (!g || g->n <= 0) return sol;

    int melhor = -1;
    double razaoMax = -1.0;
    for (int i = 0; i < g->n; i++){

        if (pesos[i] > 0){
            double r = (double)habilidades[i] / pesos[i];

            if (r > razaoMax){
                razaoMax = r;
                melhor = i;
            }
        }
    }
    if (melhor < 0) return sol;

    int qtd = W / pesos[melhor];
    long long pesoUsado = (long long)qtd * pesos[melhor];

    int extra = -1;
    long long habExtra = 0;

    for (int e = g->cabeca[melhor]; e != -1; e = g->arestas[e].proxima){
        int v = g->arestas[e].destino;
        int d = g->arestas[e].distancia;

        if (d <= D && pesoUsado + pesos[v] <= W && habilidades[v] > habExtra){
            habExtra = habilidades[v];
            extra = v;
        }
    }

    int tam;

    if (extra >= 0){
        tam = 2;
    } else{
        tam = 1;
    }

    int *caminho = malloc(tam * sizeof(int));
    long long *quant = malloc(tam * sizeof(long long));

    caminho[0] = melhor;
    quant[0] = qtd;

    if (extra >= 0){
        caminho[1] = extra;
        quant[1] = 1;
    }

    sol.totalHabilidade = (long long)qtd * habilidades[melhor];

    if (extra >= 0){
        sol.totalHabilidade = sol.totalHabilidade + habilidades[extra];
    }

    sol.tamCaminho = tam;
    sol.caminho = caminho;
    sol.quantidades = quant;
    return sol;
}