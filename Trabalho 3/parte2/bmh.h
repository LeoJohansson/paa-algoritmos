#ifndef BMH_H
#define BMH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int* pre_processar_bmh(const char* padrao);
int* buscar_bmh(const char* texto, const char* padrao, int* num_ocorrencias, long long* comparacoes);
void liberar_tabela_bmh(int* tabela);

#endif