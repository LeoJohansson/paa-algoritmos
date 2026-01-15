#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grafo.h"
#include "dp.h"
#include "guloso.h"

int main(int argc, char *argv[]){

    if (argc != 3 || strcmp(argv[1], "-i") != 0){
        fprintf(stderr, "Uso: %s -i <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *fin = fopen(argv[2], "r");
    FILE *foutO = fopen("saida_otima.txt", "w");
    FILE *foutG = fopen("saida_heuristica.txt", "w");

    if (!fin){
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }
    if (!foutO){
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }
    if (!foutG) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }


    int K;
    fscanf(fin, "%d", &K);
    int instancia = 1;

    while (K--){
        int P, D, W, C;
        fscanf(fin, "%d %d %d %d", &P, &D, &W, &C);

        int *pesos = malloc(P * sizeof(int));
        int *habil = malloc(P * sizeof(int));
        for (int i = 0; i < P; i++) {
            int id;
            fscanf(fin, "%d %d %d", &id, &pesos[i], &habil[i]);
        }

        Grafo g;
        criarGrafo(&g, P, C);

        for (int i = 0; i < C; i++){
            int u, v, d;
            fscanf(fin, "%d %d %d", &u, &v, &d);
            adicionarAresta(&g, u - 1, v - 1, d);
        }

        clock_t inicioOtima = clock();
        Solucao so = resolverDp(&g, D, W, pesos, habil);
        clock_t fimOtima = clock();
        double tempoOtima = (double)(fimOtima - inicioOtima) / CLOCKS_PER_SEC;

        fprintf(foutO, "%lld", so.totalHabilidade);

        for (int i = 0; i < so.tamCaminho; i++){
            fprintf(foutO, " %d %lld", so.caminho[i] + 1, so.quantidades[i]);
        }
        fprintf(foutO, "\n");

        clock_t inicioHeur = clock();
        Solucao sg = resolverGuloso(&g, D, W, pesos, habil);
        clock_t fimHeur = clock();
        double tempoHeur = (double)(fimHeur - inicioHeur) / CLOCKS_PER_SEC;

        fprintf(foutG, "%lld", sg.totalHabilidade);
        
        for (int i = 0; i < sg.tamCaminho; i++){
            fprintf(foutG, " %d %lld", sg.caminho[i] + 1, sg.quantidades[i]);
        }
        fprintf(foutG, "\n");

        printf("Instância %d:\n", instancia++);
        printf("  Tempo solução ótima     : %.6f segundos\n", tempoOtima);
        printf("  Tempo solução heurística: %.6f segundos\n", tempoHeur);

        free(pesos);
        free(habil);
        if (so.caminho) free(so.caminho);
        if (so.quantidades) free(so.quantidades);
        if (sg.caminho) free(sg.caminho);
        if (sg.quantidades) free(sg.quantidades);
        liberarGrafo(&g);
    }

    fclose(fin);
    fclose(foutO);
    fclose(foutG);
    return 0;
}
