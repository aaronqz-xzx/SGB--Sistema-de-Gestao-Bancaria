#include <stdio.h>
#include "gb.h"

static int hashFunction(int chave) {
    int indice = chave % HASH_SIZE;

    if (indice < 0) {
        indice += HASH_SIZE;
    }

    return indice;
}

static int pesquisarIndice(hash *h, int chave) {
    int InitialIndex = hashFunction(chave);

    for (int i = 0 ; i < HASH_SIZE; i++){
        int indice = (InitialIndex + i) % HASH_SIZE;

        if (h->entradas[indice].estado == VAZIO) {
            return -1; // Chave não encontrada
        }
        if (h->entradas[indice].estado == OCUPADO && h->entradas[indice].chave == chave) {
            return indice; // Chave encontrada
        }
    }
    return -1; // Chave não encontrada
}

void inicializar(hash *h) {
    for (int i = 0; i < HASH_SIZE; i++) {
        h->entradas[i].estado = VAZIO;
        h->entradas[i].chave = 0;
    }
    h->quantidade = 0;
}

int inserir(hash *h, int chave) {
    if (h->quantidade == HASH_SIZE) {
        return 0; // Tabela cheia
    }

<<<<<<< HEAD
    if (pesquisarIndice(h, chave) != -1) {
=======
    if (pesquisar(h, chave) != -1) {
>>>>>>> 90e812f (primeiras adicioes)
        return 0; // Chave já existe
    }

    int InitialIndex = hashFunction(chave);
    int primeiroRemovido = -1;

    for (int i = 0; i < HASH_SIZE; i++) {
        int indice = (InitialIndex + i) % HASH_SIZE;
<<<<<<< HEAD

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

int remover (hash *tabela, int chave) {
    int indice = pesquisarIndice(tabela, chave);
    
    if (indice == -1) {
        return 0;
    }

    tabela->entradas[indice].estado = REMOVIDO;
    tabela->quantidade--;

    return 1;
}

int pesquisar(hash *tabela) {
    printf("\nTabela Hash (Tamnho %d, ocupados %d):\n", HASH_SIZE, tabela->quantidade);

    for (int i = 0; i < HASH_SIZE; i++) {
        printf("[%02d] ", i);
        
        if (tabela->entradas[i].estado == OCUPADO) {
            printf("%d\n", tabela->entradas[i].chave);
        } else if (tabela->entradas[i].estado == REMOVIDO) {
            printf("REMOVIDO\n");
        } else {
            printf("VAZIO\n");
        }
=======
>>>>>>> 90e812f (primeiras adicioes)
    }
}

