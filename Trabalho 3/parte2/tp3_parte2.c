#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "bmh.h"
#include "huffman.h"

#define TAMANHO_BUFFER_IO 5000

char *ler_arquivo_texto(const char *nome_arquivo){

    FILE *f = fopen(nome_arquivo, "rb");

    if (!f){
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tamanho_arq = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *texto = malloc(tamanho_arq + 1);
    if (!texto){
        fclose(f);
        return NULL;
    }

    size_t bytes_lidos = fread(texto, 1, tamanho_arq, f);

    if (bytes_lidos != tamanho_arq){
        free(texto);
        fclose(f);
        return NULL;
    }
    texto[tamanho_arq] = '\0';

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

    char buffer[TAMANHO_BUFFER_IO];
    while (fgets(buffer, TAMANHO_BUFFER_IO, f) != NULL){
        buffer[strcspn(buffer, "\n\r")] = 0;

        if (strlen(buffer) == 0) continue;

        (*num_padroes)++;
        *padroes = realloc(*padroes, (*num_padroes) * sizeof(char *));
        if (!(*padroes)) {
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

int obter_bit(const unsigned char* dados, int indice_bit){
    int indice_byte = indice_bit / CHAR_BIT;
    int bit_no_byte = indice_bit % CHAR_BIT;
    return (dados[indice_byte] >> (CHAR_BIT - 1 - bit_no_byte)) & 1;
}

int main(int argc, char *argv[]){
    if (argc != 3) {
        
        return 1;
    }

    char *caminho_arquivo_texto = argv[1];
    char *caminho_arquivo_padroes = argv[2];

    char *texto_original = ler_arquivo_texto(caminho_arquivo_texto);

    if (!texto_original){
        return 1;
    }

    NoHuffman* raiz_huffman = construir_arvore_huffman(texto_original);

    if (!raiz_huffman){
        free(texto_original);
        return 1;
    }

    int num_codigos_huffman;
    CodigoHuffman* codigos_huffman = gerar_codigos_huffman(raiz_huffman, &num_codigos_huffman);

    if (!codigos_huffman){
        free(texto_original);
        liberar_arvore_huffman(raiz_huffman);
        return 1;
    }
    int tamanho_texto_comprimido_bytes;
    unsigned char* texto_comprimido = comprimir_huffman(texto_original, codigos_huffman, num_codigos_huffman, &tamanho_texto_comprimido_bytes);

    if (!texto_comprimido){
        free(texto_original);
        liberar_arvore_huffman(raiz_huffman);
        liberar_codigos_huffman(codigos_huffman, num_codigos_huffman);
        return 1;
    }
    
    long long total_bits_texto = 0;
    
    for (int k = 0; texto_original[k] != '\0'; k++){

        for (int l = 0; l < num_codigos_huffman; l++){

            if (codigos_huffman[l].caractere == texto_original[k]){
                total_bits_texto += strlen(codigos_huffman[l].codigo);
                break;
            }
        }
    }
    
    FILE *arquivo_saida = fopen("Arquivo Saida", "w");

    if (arquivo_saida == NULL){
        free(texto_original);
        liberar_arvore_huffman(raiz_huffman);
        liberar_codigos_huffman(codigos_huffman, num_codigos_huffman);
        if (texto_comprimido) free(texto_comprimido);
        return 1;
    }

    char **padroes = NULL;
    int num_padroes = 0;
    ler_arquivo_padroes(caminho_arquivo_padroes, &padroes, &num_padroes);

    if (!padroes){
        free(texto_original);
        liberar_arvore_huffman(raiz_huffman);
        liberar_codigos_huffman(codigos_huffman, num_codigos_huffman);
        if (texto_comprimido) free(texto_comprimido);
        fclose(arquivo_saida);
        return 1;
    }

    printf("--- Resultados da Busca BMH ---\n");
    printf("Arquivo Texto: %s\n", caminho_arquivo_texto);
    printf("Arquivo Padrões: %s\n", caminho_arquivo_padroes);
    printf("-----------------------------------------\n");

    for (int i = 0; i < num_padroes; i++){
        char* padrao_atual = padroes[i];
        
        printf("Padrão: '%s'\n", padrao_atual);

        int contagem_nao_comprimido;
        long long comparacoes_nao_comprimido = 0;
        clock_t inicio_nao_comprimido = clock();
        int* ocorrencias_nao_comprimido = buscar_bmh(texto_original, padrao_atual, &contagem_nao_comprimido, &comparacoes_nao_comprimido);
        clock_t fim_nao_comprimido = clock();
        double tempo_nao_comprimido = (double)(fim_nao_comprimido - inicio_nao_comprimido) * 1000.0 / CLOCKS_PER_SEC;

        printf("  Não Comprimido: Ocorrências: ");
        fprintf(arquivo_saida, "%s\n", padrao_atual);
        char buffer_ocorrencias_str[5000];
        int pos_atual = 0;

        if (ocorrencias_nao_comprimido && contagem_nao_comprimido > 0){

            for (int j = 0; j < contagem_nao_comprimido; j++){
                printf("%d ", ocorrencias_nao_comprimido[j]);
                pos_atual += snprintf(buffer_ocorrencias_str + pos_atual, sizeof(buffer_ocorrencias_str) - pos_atual, "%d ", ocorrencias_nao_comprimido[j]);
            }

            if (pos_atual > 0){ 
                 buffer_ocorrencias_str[pos_atual-1] = '\0';
            } else{
                buffer_ocorrencias_str[0] = '\0';
            }
            fprintf(arquivo_saida, "%s\n", buffer_ocorrencias_str);
        } else{
            printf("Nenhuma\n");
            fprintf(arquivo_saida, "Nenhuma\n");
        }

        printf("(Comparações: %lld, Tempo: %.4f ms)\n", comparacoes_nao_comprimido, tempo_nao_comprimido);

        if (ocorrencias_nao_comprimido) free(ocorrencias_nao_comprimido);

        int comprimento_bits_padrao_comprimido;
        unsigned char* padrao_comprimido = comprimir_padrao(padrao_atual, codigos_huffman, num_codigos_huffman, &comprimento_bits_padrao_comprimido);

        if (padrao_comprimido && comprimento_bits_padrao_comprimido > 0){

            int contagem_comprimido = 0;
            long long comparacoes_comprimido = 0;
            clock_t inicio_comprimido = clock();

            for (long long bit_offset = 0; bit_offset <= total_bits_texto - comprimento_bits_padrao_comprimido; bit_offset++){

                int match = 1;

                for (int k = 0; k < comprimento_bits_padrao_comprimido; k++){
                    comparacoes_comprimido++;

                    if (obter_bit(texto_comprimido, bit_offset + k) != obter_bit(padrao_comprimido, k)){
                        match = 0;
                        break;
                    }
                }
                if (match){
                    contagem_comprimido++;
                }
            }
            
            clock_t fim_comprimido = clock();
            double tempo_comprimido = (double)(fim_comprimido - inicio_comprimido) * 1000.0 / CLOCKS_PER_SEC;

            printf("  Comprimido (busca bit a bit): Ocorrências Encontradas: %d (Comparações: %lld, Tempo: %.4f ms)\n", 
                        contagem_comprimido, comparacoes_comprimido, tempo_comprimido);
            
            free(padrao_comprimido);
        } else{
            printf("  Comprimido: Falha ao comprimir padrão para busca ou padrão vazio/sem caracteres nos códigos Huffman.\n");
        }
        printf("-----------------------------------------\n");
    }

    fclose(arquivo_saida);
    free(texto_original);
    liberar_arvore_huffman(raiz_huffman);
    liberar_codigos_huffman(codigos_huffman, num_codigos_huffman);
    if (texto_comprimido) free(texto_comprimido);
    for (int i = 0; i < num_padroes; i++) {
        free(padroes[i]);
    }
    free(padroes);

    return 0;
}