#include <banco.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static NoB *btree_criar_no(int folha) {
    NoB *no = (NoB *) malloc(sizeof(NoB));
    if (no == NULL) {
        return  NULL;
    }

    no->quantidade = 0;
    no->follha = folha;

    for (int i = 0; i < MAX_CHAVES; i++) {
        no->contas[i] = NULL;
    }

    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }

    return no;
}

static int hash_indice(int nif) {
    int indice = nif % HASH_SIZE;
    return indice < 0 ? indice + HASH_SIZE : indice;
}

static void obter_data_hora(char destino[20]) {
    time_t agora = time(NULL);
    struct tm *tempo = localtime(&agora);

    if (tempo == NULL) {
        strcpy(destino, "0000-00-00 00:00");
        return;
    }

    strftime(destino, 20, "%Y-%m-%d %H:%M", tempo);
}

 const char *banco_tipo_movimento_texto(TipoMovimento tipo) {
    switch (tipo) {
        case MOVIMENTO_DEPOSITO:
            return "Deposito";
        case MOVIMENTO_LEVANTAMENTO:
            return "Levantamento";
        case MOVIMENTO_TRANSFERENCIA_ENTRADA:
            return "Transferencia recebida";
        case MOVIMENTO_TRANSFERENCIA_SAIDA:
            return "Transferencia enviada";
        default:
            return "Desconhecido";
    }
}

static int hash_inserir(Banco *banco, Conta *conta){
    int indice = hash_indice(conta->nif);
    NoHash *novo = (NoHash *)malloc(sizeof(NoHash));

    if (novo == NULL) {
        return 0;
    }
}