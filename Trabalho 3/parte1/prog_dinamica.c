#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "prog_dinamica.h"

long long contagem_comparacoes_prog_dinamica = 0;

#define TAMANHO_MAX_BUFFER_OCORRENCIAS (10000)

char* buscar_prog_dinamica(const char *texto, const char *padrao, int k){
    int tamanho_texto = strlen(texto);
    int tamanho_padrao = strlen(padrao);

    contagem_comparacoes_prog_dinamica = 0;

    int **matriz_dp = malloc((tamanho_padrao + 1) * sizeof(int *));

    if (!matriz_dp){
        exit(1);
    }

    for (int i = 0; i <= tamanho_padrao; i++){

        matriz_dp[i] = malloc((tamanho_texto + 1) * sizeof(int));

        if (!matriz_dp[i]){
            for (int l = 0; l < i; l++) free(matriz_dp[l]);
            free(matriz_dp);
            exit(1);
        }
    }

    for (int j = 0; j <= tamanho_texto; j++)
        matriz_dp[0][j] = 0;
    
    for (int i = 1; i <= tamanho_padrao; i++)
        matriz_dp[i][0] = i;

    char *buffer_saida = malloc(TAMANHO_MAX_BUFFER_OCORRENCIAS);

    if (!buffer_saida){
        for (int i = 0; i <= tamanho_padrao; i++) free(matriz_dp[i]);
        free(matriz_dp);
        exit(1);
    }
    
    snprintf(buffer_saida, TAMANHO_MAX_BUFFER_OCORRENCIAS, "%s", padrao);
    int comprimento_atual = strlen(buffer_saida);

    for (int j = 1; j <= tamanho_texto; j++){

        for (int i = 1; i <= tamanho_padrao; i++){

            contagem_comparacoes_prog_dinamica++;
            
            int custo_substituicao;
            if (padrao[i - 1] == texto[j - 1]){
                custo_substituicao = 0;
            } else{
                custo_substituicao = 1;
            }

            matriz_dp[i][j] = fmin(fmin(
                matriz_dp[i - 1][j] + 1,
                matriz_dp[i][j - 1] + 1),
                matriz_dp[i - 1][j - 1] + custo_substituicao
            );
        }

        if (matriz_dp[tamanho_padrao][j] <= k){

            int escrito = snprintf(buffer_saida + comprimento_atual, TAMANHO_MAX_BUFFER_OCORRENCIAS - comprimento_atual, " %d", j - tamanho_padrao + 1);

            if (escrito < 0 || escrito >= TAMANHO_MAX_BUFFER_OCORRENCIAS - comprimento_atual){
                break;
            }
            comprimento_atual += escrito;
        }
    }

    for (int i = 0; i <= tamanho_padrao; i++)
        free(matriz_dp[i]);
    free(matriz_dp);

    return buffer_saida;
}