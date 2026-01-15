#include "huffman.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoHeapMin{
    NoHuffman* no;
    int freq;
} NoHeapMin;

typedef struct HeapMin{
    int tamanho;
    int capacidade;
    NoHeapMin **array;
} HeapMin;

NoHeapMin* novo_no_heap_min(NoHuffman* no, int freq){
    NoHeapMin* noHeapMin = (NoHeapMin*)malloc(sizeof(NoHeapMin));

    if (!noHeapMin){
        fprintf(stderr, "Erro de alocação de memória para NoHeapMin.\n");
        exit(1);
    }
    noHeapMin->no = no;
    noHeapMin->freq = freq;
    return noHeapMin;
}

HeapMin* criar_heap_min(int capacidade){
    HeapMin* heapMin = (HeapMin*)malloc(sizeof(HeapMin));

    if (!heapMin){
        fprintf(stderr, "Erro de alocação de memória para HeapMin.\n");
        exit(1);
    }

    heapMin->tamanho = 0;
    heapMin->capacidade = capacidade;
    heapMin->array = (NoHeapMin**)malloc(capacidade * sizeof(NoHeapMin*));

    if (!heapMin->array){
        free(heapMin);
        fprintf(stderr, "Erro de alocação de memória para array de HeapMin.\n");
        exit(1);
    }
    return heapMin;
}

void trocar_nos_heap_min(NoHeapMin** a, NoHeapMin** b){
    NoHeapMin* t = *a;
    *a = *b;
    *b = t;
}

void heapify_min(HeapMin* heapMin, int idx){

    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;

    if (esquerda < heapMin->tamanho && heapMin->array[esquerda]->freq < heapMin->array[menor]->freq)
        menor = esquerda;

    if (direita < heapMin->tamanho && heapMin->array[direita]->freq < heapMin->array[menor]->freq)
        menor = direita;

    if (menor != idx){
        trocar_nos_heap_min(&heapMin->array[menor], &heapMin->array[idx]);
        heapify_min(heapMin, menor);
    }
}

int eh_tamanho_um(HeapMin* heapMin){
    return (heapMin->tamanho == 1);
}

NoHuffman* extrair_min(HeapMin* heapMin){
    NoHuffman* temp = heapMin->array[0]->no;
    free(heapMin->array[0]);
    
    heapMin->array[0] = heapMin->array[heapMin->tamanho - 1];
    heapMin->tamanho--;
    heapify_min(heapMin, 0);
    
    return temp;
}

void inserir_heap_min(HeapMin* heapMin, NoHuffman* no, int freq){
    heapMin->tamanho++;
    int i = heapMin->tamanho - 1;

    while (i > 0 && freq < heapMin->array[(i - 1) / 2]->freq){
        heapMin->array[i] = heapMin->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heapMin->array[i] = novo_no_heap_min(no, freq);
}

void construir_heap_min(HeapMin* heapMin){
    int n = heapMin->tamanho - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; i--)
        heapify_min(heapMin, i);
}

int* calcular_frequencia(const char* texto){

    int* freq = (int*)calloc(256, sizeof(int));

    if (!freq){
        fprintf(stderr, "Erro de alocação de memória para frequência.\n");
        exit(1);
    }

    for (int i = 0; texto && texto[i] != '\0'; i++){
        freq[(unsigned char)texto[i]]++;
    }
    return freq;
}

NoHuffman* novo_no(unsigned char dado, int freq){

    NoHuffman* no = (NoHuffman*)malloc(sizeof(NoHuffman));

    if (!no){
        fprintf(stderr, "Erro de alocação de memória para NoHuffman.\n");
        exit(1);
    }

    no->dado = dado;
    no->freq = freq;
    no->esquerda = no->direita = NULL;
    return no;
}

NoHuffman* construir_arvore_huffman(const char* texto){

    int* freq = calcular_frequencia(texto);
    int capacidade = 0;

    for (int i = 0; i < 256; i++){
        if (freq[i] > 0)
            capacidade++;
    }

    if (capacidade == 0){
        free(freq);
        return NULL;
    }

    if (capacidade == 1){

        for (int i = 0; i < 256; i++){

            if (freq[i] > 0){
                NoHuffman* raiz = novo_no((unsigned char)i, freq[i]);
                free(freq);
                return raiz;
            }
        }
    }


    HeapMin* heapMin = criar_heap_min(capacidade);

    for (int i = 0; i < 256; i++){
        if (freq[i] > 0) {
            inserir_heap_min(heapMin, novo_no((unsigned char)i, freq[i]), freq[i]);
        }
    }

    while (!eh_tamanho_um(heapMin)){
        NoHuffman* esquerda = extrair_min(heapMin);
        NoHuffman* direita = extrair_min(heapMin);

        NoHuffman* topo = novo_no('$', esquerda->freq + direita->freq);
        topo->esquerda = esquerda;
        topo->direita = direita;

        inserir_heap_min(heapMin, topo, topo->freq);
    }

    NoHuffman* raiz = extrair_min(heapMin);
    free(heapMin->array);
    free(heapMin);
    free(freq);
    return raiz;
}

void gerar_codigos_recursivo(NoHuffman* raiz, char* codigo_atual, int indice, CodigoHuffman* codigos, int* numCodigos){

    if (raiz->esquerda == NULL && raiz->direita == NULL){
        codigos[*numCodigos].caractere = raiz->dado;
        codigo_atual[indice] = '\0';
        codigos[*numCodigos].codigo = strdup(codigo_atual);

        if (!codigos[*numCodigos].codigo){
            fprintf(stderr, "Erro de alocação de memória para código Huffman.\n");
            exit(1);
        }

        (*numCodigos)++;
        return;
    }

    if (raiz->esquerda){
        codigo_atual[indice] = '0';
        gerar_codigos_recursivo(raiz->esquerda, codigo_atual, indice + 1, codigos, numCodigos);
    }

    if (raiz->direita){
        codigo_atual[indice] = '1';
        gerar_codigos_recursivo(raiz->direita, codigo_atual, indice + 1, codigos, numCodigos);
    }
}

CodigoHuffman* gerar_codigos_huffman(NoHuffman* raiz, int* numCodigos){
    if (!raiz) return NULL;

    CodigoHuffman* codigos = (CodigoHuffman*)malloc(sizeof(CodigoHuffman) * 256);

    if (!codigos){
        fprintf(stderr, "Erro de alocação de memória para array de códigos Huffman.\n");
        exit(1);
    }

    char codigo_atual[257];
    *numCodigos = 0;
    gerar_codigos_recursivo(raiz, codigo_atual, 0, codigos, numCodigos);
    return codigos;
}

unsigned char* comprimir_huffman(const char* texto, CodigoHuffman* codigos, int numCodigos, int* tamanho_comprimido_bytes){
    if (!texto || !codigos) return NULL;

    long long comprimento_bits = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        int encontrado = 0;
        for (int j = 0; j < numCodigos; j++) {
            if (codigos[j].caractere == (unsigned char)texto[i]){
                comprimento_bits += strlen(codigos[j].codigo);
                encontrado = 1;
                break;
            }
        }
        if (!encontrado){
            return NULL;
        }
    }

    *tamanho_comprimido_bytes = (int)((comprimento_bits + CHAR_BIT - 1) / CHAR_BIT);
    unsigned char* texto_comprimido = (unsigned char*)calloc(*tamanho_comprimido_bytes, sizeof(unsigned char));

    if (!texto_comprimido){
        fprintf(stderr, "Erro de alocação de memória para texto comprimido.\n");
        exit(1);
    }

    int bit_atual = 0;

    for (int i = 0; texto[i] != '\0'; i++){

        for (int j = 0; j < numCodigos; j++){

            if (codigos[j].caractere == (unsigned char)texto[i]){

                for (int k = 0; codigos[j].codigo[k] != '\0'; k++){

                    if (codigos[j].codigo[k] == '1'){
                        texto_comprimido[bit_atual / CHAR_BIT] |= (1 << (CHAR_BIT - 1 - (bit_atual % CHAR_BIT)));
                    }
                    bit_atual++;
                }
                break;
            }
        }
    }
    return texto_comprimido;
}

char* descomprimir_huffman(const unsigned char* texto_comprimido, NoHuffman* raiz, int tamanho_comprimido_bytes, int tamanho_original){
    if (!texto_comprimido || !raiz) return NULL;

    char* texto_descomprimido = (char*)malloc(sizeof(char) * (tamanho_original + 1));
    if (!texto_descomprimido) {
        fprintf(stderr, "Erro de alocação de memória para texto descomprimido.\n");
        exit(1);
    }

    NoHuffman* atual = raiz;
    int indice_descomprimido = 0;
    int indice_bits = 0;
    
    while (indice_descomprimido < tamanho_original){
        if (!atual->esquerda && !atual->direita) {
            texto_descomprimido[indice_descomprimido++] = atual->dado;
            atual = raiz;
            continue;
        }

        if (indice_bits / CHAR_BIT >= tamanho_comprimido_bytes){
            fprintf(stderr, "ERRO: Fim inesperado do texto comprimido durante a descompressão.\n");
            free(texto_descomprimido);
            return NULL;
        }

        unsigned char byte_atual = texto_comprimido[indice_bits / CHAR_BIT];
        int bit_no_byte = indice_bits % CHAR_BIT;
        int bit = (byte_atual >> (CHAR_BIT - 1 - bit_no_byte)) & 1;

        if (bit == 0){
            atual = atual->esquerda;
        } else{
            atual = atual->direita;
        }
        indice_bits++;
    }
    texto_descomprimido[indice_descomprimido] = '\0';
    return texto_descomprimido;
}

void liberar_arvore_huffman(NoHuffman* raiz){

    if (raiz){
        liberar_arvore_huffman(raiz->esquerda);
        liberar_arvore_huffman(raiz->direita);
        free(raiz);
    }
}

void liberar_codigos_huffman(CodigoHuffman* codigos, int numCodigos){
    if (codigos){
        for (int i = 0; i < numCodigos; i++){
            free(codigos[i].codigo);
        }
        free(codigos);
    }
}

unsigned char* comprimir_padrao(const char* padrao, CodigoHuffman* codigos, int numCodigos, int* comprimento_bits_padrao_comprimido){
    if (!padrao || !codigos) return NULL;

    *comprimento_bits_padrao_comprimido = 0;
    for (int i = 0; padrao[i] != '\0'; i++){
        int encontrado = 0;
        for (int j = 0; j < numCodigos; j++) {
            if (codigos[j].caractere == (unsigned char)padrao[i]){
                *comprimento_bits_padrao_comprimido += strlen(codigos[j].codigo);
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            return NULL;
        }
    }

    if (*comprimento_bits_padrao_comprimido == 0 && strlen(padrao) > 0){
        return NULL;
    }
    
    int tamanho_bytes_padrao_comprimido = (*comprimento_bits_padrao_comprimido + CHAR_BIT - 1) / CHAR_BIT;
    unsigned char* padrao_comprimido = (unsigned char*)calloc(tamanho_bytes_padrao_comprimido + 1, sizeof(unsigned char));

    if (!padrao_comprimido){
        fprintf(stderr, "Erro de alocação de memória para padrão comprimido.\n");
        exit(1);
    }

    int bit_atual = 0;

    for (int i = 0; padrao[i] != '\0'; i++){

        for (int j = 0; j < numCodigos; j++){

            if (codigos[j].caractere == (unsigned char)padrao[i]){

                for (int k = 0; codigos[j].codigo[k] != '\0'; k++){

                    if (codigos[j].codigo[k] == '1'){
                        padrao_comprimido[bit_atual / CHAR_BIT] |= (1 << (CHAR_BIT - 1 - (bit_atual % CHAR_BIT)));
                    }
                    bit_atual++;
                }
                break;
            }
        }
    }
    return padrao_comprimido;
}