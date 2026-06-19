#include <stdio.h>
#include "gb.h"

static int hashFunction(int chave) {
    int indice = chave % GB_HASH_SIZE;

    if (indice < 0) {
        indice += GB_HASH_SIZE;
    }

    return indice;
}

static int pesquisarIndice(hash *h, int chave) {
    int InitialIndex = hashFunction(chave);

    for (int i = 0 ; i < GB_HASH_SIZE; i++){
        int indice = (InitialIndex + i) % GB_HASH_SIZE;

        if (h->entradas[indice].estado == VAZIO) {
            return -1; // Chave não encontrada
        }
        if (h->entradas[indice].estado == OCUPADO && h->entradas[indice].chave == chave) {
            return indice; // Chave encontrada
        }
    }
    return -1; // Chave não encontrada
}

int buscar(hash *h, int chave) {
    return pesquisarIndice(h, chave);
}

void inicializar(hash *h) {
    for (int i = 0; i < GB_HASH_SIZE; i++) {
        h->entradas[i].estado = VAZIO;
        h->entradas[i].chave = 0;
    }
    h->quantidade = 0;
}

int inserir(hash *h, int chave) {
    if (h->quantidade == GB_HASH_SIZE) {
        return 0; // Tabela cheia
    }

    if (pesquisarIndice(h, chave) != -1) {
        return 0; // Chave já existe
    }

    int InitialIndex = hashFunction(chave);
    int primeiroRemovido = -1;

    for (int i = 0; i < GB_HASH_SIZE; i++) {
        int indice = (InitialIndex + i) % GB_HASH_SIZE;

        if (h->entradas[indice].estado == REMOVIDO && primeiroRemovido == -1) {
            primeiroRemovido = indice;
        }

        if (h->entradas[indice].estado == VAZIO) {
            int indiceInsercao = primeiroRemovido != -1 ? primeiroRemovido : indice;

            h->entradas[indiceInsercao].chave = chave;
            h->entradas[indiceInsercao].estado = OCUPADO;
            h->quantidade++;

            return 1;
        }
    }

    if (primeiroRemovido != -1) {
        h->entradas[primeiroRemovido].chave = chave;
        h->entradas[primeiroRemovido].estado = OCUPADO;
        h->quantidade++;

        return 1;
    }

    return 0;
}

int remover(hash *tabela, int chave) {
    int indice = pesquisarIndice(tabela, chave);

    if (indice == -1) {
        return 0;
    }

    tabela->entradas[indice].estado = REMOVIDO;
    tabela->quantidade--;

    return 1;

}

void imprimir(hash *tabela) {
    printf("\nTabela Hash (Tamnho %d, ocupados %d):\n", GB_HASH_SIZE, tabela->quantidade);

    for (int i = 0; i < GB_HASH_SIZE; i++) {
        printf("[%02d] ", i);
        
        if (tabela->entradas[i].estado == OCUPADO) {
            printf("%d\n", tabela->entradas[i].chave);
        } else if (tabela->entradas[i].estado == REMOVIDO) {
            printf("REMOVIDO\n");
        } else {
            printf("VAZIO\n");
        }
    }
}

