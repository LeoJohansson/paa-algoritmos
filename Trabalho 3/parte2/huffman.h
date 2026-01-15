#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct HuffmanNode{
    unsigned char dado;
    int freq;
    struct HuffmanNode *esquerda;
    struct HuffmanNode *direita;
} NoHuffman;

typedef struct HuffmanCode{
    unsigned char caractere;
    char *codigo;
} CodigoHuffman;

int* calcular_frequencia(const char* texto);
NoHuffman* novo_no(unsigned char dado, int freq);
NoHuffman* construir_arvore_huffman(const char* texto);
CodigoHuffman* gerar_codigos_huffman(NoHuffman* raiz, int* numCodigos);
unsigned char* comprimir_huffman(const char* texto, CodigoHuffman* codigos, int numCodigos, int* tamanho_comprimido_bytes);
char* descomprimir_huffman(const unsigned char* texto_comprimido, NoHuffman* raiz, int tamanho_comprimido_bytes, int tamanho_original);
void liberar_arvore_huffman(NoHuffman* raiz);
void liberar_codigos_huffman(CodigoHuffman* codigos, int numCodigos);
unsigned char* comprimir_padrao(const char* padrao, CodigoHuffman* codigos, int numCodigos, int* comprimento_bits_padrao_comprimido);

#endif