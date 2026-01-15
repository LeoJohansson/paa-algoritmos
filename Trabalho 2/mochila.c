#include "mochila.h"

void atualizarMochilaIlimitada(int W, int peso, int habilidade, long long *dp) {
    for (int w = peso; w <= W; w++) {
        long long valor = dp[w - peso] + habilidade;
        if (valor > dp[w]) {
            dp[w] = valor;
        }
    }
}
