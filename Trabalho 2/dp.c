#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dp.h"
#include "mochila.h"

#define MAXP 1000

static Grafo *G;
static int distanciaMax;
static int pesoMax;
static int *vetPesos, *vetHabi;
static int visitado[MAXP], caminhoAtual[MAXP], tamAtual;
static long long melhorHabi;
static int melhorCaminho[MAXP], tamMelhor;
static long long quantMelhor[MAXP];

static void refazerQuantidades(long long *dp, int pesoUsado){

    for (int i = 0; i < tamMelhor; i++) quantMelhor[i] = 0;
    int w = pesoUsado, somaP = 0;

    while (w > 0){

        int achou = 0;

        for (int i = 0; i < tamMelhor; i++){

            int nodo = melhorCaminho[i];
            int p = vetPesos[nodo];
            long long h = vetHabi[nodo];

            if (w >= p && dp[w] == dp[w - p] + h && somaP + p <= pesoMax){
                quantMelhor[i]++;
                w -= p;
                somaP += p;
                achou = 1;
                break;
            }
        }
        if (!achou) break;
    }
}

static void dfs(int distPerc, long long *dpCur){

    long long maxH = 0;
    int melhorW = 0;

    for (int w = 0; w <= pesoMax; w++){
        if (dpCur[w] > maxH){
            maxH = dpCur[w];
            melhorW = w;
        }
    }
    if (maxH > melhorHabi){

        melhorHabi = maxH;
        tamMelhor = tamAtual;
        memcpy(melhorCaminho, caminhoAtual, tamAtual * sizeof(int));
        refazerQuantidades(dpCur, melhorW);
    }

    int u = caminhoAtual[tamAtual - 1];

    for (int e = G->cabeca[u]; e != -1; e = G->arestas[e].proxima){
        int v = G->arestas[e].destino;
        int d = G->arestas[e].distancia;

        if (!visitado[v] && distPerc + d <= distanciaMax){
            visitado[v] = 1;
            caminhoAtual[tamAtual++] = v;

            long long *dpNovo = calloc(pesoMax + 1, sizeof(long long));
            if (!dpNovo) exit(1);
            memcpy(dpNovo, dpCur, (pesoMax + 1) * sizeof(long long));
            atualizarMochilaIlimitada(pesoMax, vetPesos[v], vetHabi[v], dpNovo);

            dfs(distPerc + d, dpNovo);
            free(dpNovo);

            visitado[v] = 0;
            tamAtual--;
        }
    }
}

Solucao resolverDp(Grafo *g, int D, int W, int *pesos, int *habilidades){
    Solucao sol = {0, NULL, 0, NULL};
    if (!g || g->n <= 0) return sol;

    G = g; distanciaMax = D; pesoMax = W;
    vetPesos = pesos; vetHabi = habilidades;
    melhorHabi = 0; tamMelhor = 0;
    memset(visitado, 0, g->n * sizeof(int));

    for (int i = 0; i < g->n; i++){

        visitado[i] = 1;
        caminhoAtual[0] = i; tamAtual = 1;
        long long *dpLocal = calloc(pesoMax + 1, sizeof(long long));

        if (!dpLocal) exit(1);
        atualizarMochilaIlimitada(pesoMax, pesos[i], habilidades[i], dpLocal);
        dfs(0, dpLocal);
        free(dpLocal);
        visitado[i] = 0;
    }

    sol.totalHabilidade = melhorHabi;
    sol.tamCaminho = tamMelhor;

    if (tamMelhor > 0){
        sol.caminho = malloc(tamMelhor * sizeof(int));
        sol.quantidades = malloc(tamMelhor * sizeof(long long));
        memcpy(sol.caminho, melhorCaminho, tamMelhor * sizeof(int));
        memcpy(sol.quantidades, quantMelhor, tamMelhor * sizeof(long long));
    }
    return sol;
}