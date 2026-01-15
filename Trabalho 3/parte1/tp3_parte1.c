#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prog_dinamica.h"
#include "shift_and.h"

char* ler_arquivo_texto(const char* caminho_arquivo);
void ler_arquivo_padroes(const char* caminho_arquivo, char*** padroes, int* num_padroes);

extern long long contagem_comparacoes_prog_dinamica;
extern long long contagem_comparacoes_shift_and;

int main(int argc, char *argv[]){

    if (argc != 5){
        printf("Uso: %s <algoritmo> <arquivo_texto> <arquivo_padroes> <k>\n", argv[0]);
        return 1;
    }

    int algoritmo = atoi(argv[1]);
    char *caminho_arquivo_texto = argv[2];
    char *caminho_arquivo_padroes = argv[3];
    int valor_k = atoi(argv[4]);

    if (valor_k < 0){
        return 1;
    }

    char *texto = ler_arquivo_texto(caminho_arquivo_texto);
    if (!texto){
        return 1;
    }

    char **padroes = NULL;
    int num_padroes = 0;
    ler_arquivo_padroes(caminho_arquivo_padroes, &padroes, &num_padroes);

    if (!padroes){
        free(texto);
        return 1;
    }

    FILE *arquivo_saida = fopen("Arquivo Saida", "w");

    if (arquivo_saida == NULL){
        free(texto);
        for (int i = 0; i < num_padroes; i++) free(padroes[i]);
        free(padroes);
        return 1;
    }

    printf("--- Iniciando Busca ---\n");

    if (algoritmo == 1){
        printf("Algoritmo selecionado: Programação Dinâmica\n");
    } else if (algoritmo == 2){
        printf("Algoritmo selecionado: Shift-And\n");
    } else{
        printf("Algoritmo selecionado: Inválido\n");
    }

    printf("Com k = %d\n", valor_k);
    printf("-----------------------------------------\n");

    for (int i = 0; i < num_padroes; i++){

        clock_t tempo_inicio, tempo_fim;
        double tempo_cpu_usado;
        long long num_compara_alg;
        char *str_ocorrencias = NULL;

        tempo_inicio = clock();

        if (algoritmo == 1){
            str_ocorrencias = buscar_prog_dinamica(texto, padroes[i], valor_k);
            num_compara_alg = contagem_comparacoes_prog_dinamica;
        } else if (algoritmo == 2){
            str_ocorrencias = buscar_shift_and(texto, padroes[i], valor_k);
            num_compara_alg = contagem_comparacoes_shift_and;
        } else{
            break;
        }

        tempo_fim = clock();
        tempo_cpu_usado = ((double)(tempo_fim - tempo_inicio)) * 1000.0 / CLOCKS_PER_SEC;

        printf("Padrão '%s': Tempo: %.2f ms | Comparações: %lld\n", padroes[i], tempo_cpu_usado, num_compara_alg);

        if (str_ocorrencias){

            fprintf(arquivo_saida, "%s\n", str_ocorrencias);
            free(str_ocorrencias);
            str_ocorrencias = NULL;
        } else{
            fprintf(arquivo_saida, "%s (Erro na busca ou alocação de ocorrências)\n", padroes[i]);
        }

    }

    fclose(arquivo_saida);

    free(texto);
    for (int i = 0; i < num_padroes; i++){
        free(padroes[i]);
    }
    free(padroes);

    return 0;
}

#define TAMANHO_BUFFER 5000

char *ler_arquivo_texto(const char *nome_arquivo){
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long tamanho_arquivo = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *texto = malloc(tamanho_arquivo + 1);
    if (!texto) {
        fclose(f);
        return NULL;
    }
    size_t bytes_lidos = fread(texto, 1, tamanho_arquivo, f);
    if (bytes_lidos != tamanho_arquivo) {
        free(texto);
        fclose(f);
        return NULL;
    }
    texto[tamanho_arquivo] = '\0';
    fclose(f);
    return texto;
}

void ler_arquivo_padroes(const char *nome_arquivo, char ***padroes, int *num_padroes){
    FILE *f = fopen(nome_arquivo, "r");

    if (!f){
        *padroes = NULL;
        *num_padroes = 0;
        return;
    }
    *num_padroes = 0;
    *padroes = NULL;
    char buffer[TAMANHO_BUFFER];

    while (fgets(buffer, TAMANHO_BUFFER, f) != NULL){

        buffer[strcspn(buffer, "\n\r")] = 0;

        if (strlen(buffer) == 0) continue;
        (*num_padroes)++;
        *padroes = realloc(*padroes, (*num_padroes) * sizeof(char *));

        if (!(*padroes)){
            for (int i = 0; i < (*num_padroes) - 1; i++){
                free((*padroes)[i]);
            }
            free(*padroes);
            *padroes = NULL;
            *num_padroes = 0;
            fclose(f);
            return;
        }
        (*padroes)[(*num_padroes) - 1] = strdup(buffer);
        
        if (!(*padroes)[(*num_padroes) - 1]){
            for (int i = 0; i < (*num_padroes); i++){
                free((*padroes)[i]);
            }

            free(*padroes);
            *padroes = NULL;
            *num_padroes = 0;
            fclose(f);
            return;
        }
    }
    fclose(f);
}