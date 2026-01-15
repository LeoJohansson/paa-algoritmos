#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shift_and.h"

#define MAX_CARACTER 1000
#define MAX_PADRAO 100
#define TAMANHO_MAX_BUFFER_OCORRENCIAS (10000)


long long contagem_comparacoes_shift_and = 0;

char* buscar_shift_and(const char *texto, const char *padrao, int k){
    int tamanho_padrao = strlen(padrao);
    int tamanho_texto = strlen(texto);

    if (tamanho_padrao == 0){
        char *buffer_saida = strdup(padrao);
        if (!buffer_saida) { exit(1); }
        return buffer_saida;
    }

    if (tamanho_padrao > MAX_PADRAO){

        char *str_erro = malloc(256);
        snprintf(str_erro, 256, "%s ERRO_PADRAO_MUITO_LONGO", padrao);
        return str_erro;
    }

    contagem_comparacoes_shift_and = 0;

    uint64_t mascara_caracter[MAX_CARACTER];
    for (int i = 0; i < MAX_CARACTER; i++){
        mascara_caracter[i] = 0;
    }

    for (int i = 0; i < tamanho_padrao; i++){
        mascara_caracter[(unsigned char)padrao[i]] |= ((uint64_t)1 << i);
    }

    uint64_t estado_R[k + 1];
    for (int e = 0; e <= k; e++){
        estado_R[e] = (uint64_t)-1;
    }

    char *buffer_saida = malloc(TAMANHO_MAX_BUFFER_OCORRENCIAS);
    if (!buffer_saida){
        exit(1);
    }

    snprintf(buffer_saida, TAMANHO_MAX_BUFFER_OCORRENCIAS, "%s", padrao);
    int comprimento_atual = strlen(buffer_saida);

    for (int j = 0; j < tamanho_texto; j++){

        uint64_t estado_R_passo_anterior[k + 1];

        for (int e = 0; e <= k; e++){
            estado_R_passo_anterior[e] = estado_R[e];
        }

        estado_R[0] = ((estado_R_passo_anterior[0] << 1) | 1) & mascara_caracter[(unsigned char)texto[j]];
        contagem_comparacoes_shift_and++;

        for (int e = 1; e <= k; e++){

            estado_R[e] = ((estado_R_passo_anterior[e] << 1) | 1) & mascara_caracter[(unsigned char)texto[j]];
            contagem_comparacoes_shift_and++;

            estado_R[e] |= ((estado_R[e-1] << 1) | 1);

            estado_R[e] |= estado_R[e-1];

            estado_R[e] |= (estado_R_passo_anterior[e-1] << 1);
        }

        if ((estado_R[k] & ((uint64_t)1 << (tamanho_padrao - 1))) != 0){

            int indice_inicio_base_zero = j - tamanho_padrao + 1;

            if (indice_inicio_base_zero >= 0){
                int escrito = snprintf(buffer_saida + comprimento_atual, TAMANHO_MAX_BUFFER_OCORRENCIAS - comprimento_atual, " %d", indice_inicio_base_zero + 1);
                if (escrito < 0 || escrito >= TAMANHO_MAX_BUFFER_OCORRENCIAS - comprimento_atual) {

                    break;
                }
                comprimento_atual += escrito;
            }
        }
    }

    return buffer_saida;
}