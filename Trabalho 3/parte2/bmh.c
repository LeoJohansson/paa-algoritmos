#include "bmh.h"

int* pre_processar_bmh(const char* padrao){

    int m = strlen(padrao);
    int* tabela = (int*)malloc(sizeof(int) * 256);

    if (!tabela){
        exit(1);
    }

    for (int i = 0; i < 256; i++){
        tabela[i] = m;
    }

    for (int i = 0; i < m - 1; i++){
        tabela[(unsigned char)padrao[i]] = m - 1 - i;
    }

    return tabela;
}

int* buscar_bmh(const char* texto, const char* padrao, int* num_ocorrencias, long long* comparacoes){

    int n = strlen(texto);
    int m = strlen(padrao);
    int* ocorrencias = NULL;
    *num_ocorrencias = 0;
    *comparacoes = 0;

    if (m == 0 || m > n){
        return NULL;
    }

    int* tabela = pre_processar_bmh(padrao);

    if (!tabela){
        return NULL;
    }

    int i = m - 1;

    while (i < n){
        int j = m - 1;
        int k = i;

        while (j >= 0 && k >= 0 && texto[k] == padrao[j]){
            (*comparacoes)++;
            k--;
            j--;
        }

        if (j < 0){
            (*num_ocorrencias)++;
            ocorrencias = (int*)realloc(ocorrencias, sizeof(int) * (*num_ocorrencias));

            if (!ocorrencias){
                liberar_tabela_bmh(tabela);
                exit(1);
            }
            ocorrencias[*num_ocorrencias - 1] = k + 2;
            i += 1;
        } else{
            i += tabela[(unsigned char)texto[i]];
        }
    }

    liberar_tabela_bmh(tabela);
    return ocorrencias;
}

void liberar_tabela_bmh(int* tabela){
    free(tabela);
}