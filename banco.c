#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "banco.h"

static NoB *btree_criar_no(int folha) {
    NoB *no = (NoB *) malloc(sizeof(NoB));
    if (no == NULL) {
        return  NULL;
    }

    no->quantidade = 0;
    no->folha = folha;

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

 const char *banco_tipo_movimentos_texto(TipoMovimento tipo) {
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

    novo->nif = conta->nif;
    novo->conta = conta;
    novo->proximo = banco->hash[indice];
    banco->hash[indice] = novo;
    return 1;
}

static int hash_remover(Banco *banco, int nif) {
    int indice = hash_indice(nif);
    NoHash *atual = banco->hash[indice];
    NoHash *anterior = NULL;

    while (atual != NULL) {
        if (atual->nif == nif) {
            if (anterior == NULL) {
                banco->hash[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

Conta *banco_buscar_por_nif(Banco *banco, int nif) {
    int indice = hash_indice(nif);
    NoHash *atual = banco->hash[indice];

    while (atual != NULL) {
        if (atual->nif == nif) {
            return atual->conta;
        }
        atual = atual->proximo;
    }
    return NULL;
}

static Conta *btree_buscar(NoB *no, int numero) {
    int i = 0;

    if (no == NULL) {
        return NULL;
    }

    while (i < no->quantidade && numero > no->contas[i]->numero) {
        i++;
    }
    if (i < no->quantidade && numero == no->contas[i]->numero) {
        return no->contas[i];
    }
    if (no->folha) {
        return NULL;
    }
    return btree_buscar(no->filhos[i], numero);
}

Conta *banco_buscar_por_numero(Banco *banco, int numero) {
    return btree_buscar(banco->raiz, numero);
}

static int btree_dividir_filho(NoB *pai, int indice, NoB *filho) {
    NoB *novo = btree_criar_no(filho->folha);
    NoB *cheio = pai->filhos[indice];

    if (novo == NULL) {
        return 0;
    }

    novo->quantidade = GRAU_MINIMO - 1;

    for (int j = 0; j < GRAU_MINIMO - 1; j++) {
        novo->contas[j] = filho->contas[j + GRAU_MINIMO];
        cheio->contas[j + GRAU_MINIMO] = NULL;
    }

    if (!cheio->folha) {
        for(int j = 0; j < GRAU_MINIMO; j++) {
            novo->filhos[j] = cheio->filhos[j + GRAU_MINIMO];
            cheio->filhos[j + GRAU_MINIMO] = NULL;
        }
    }

    cheio->quantidade = GRAU_MINIMO - 1;

    for (int j = pai->quantidade; j >= indice + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[indice + 1] = novo;
    
    for (int j = pai->quantidade - 1; j >= indice; j--) {
        pai->contas[j + 1] = pai->contas[j];
    }

    pai->contas[indice] = cheio->contas[GRAU_MINIMO - 1];
    cheio->contas[GRAU_MINIMO - 1] = NULL;
    pai->quantidade++;

    return 1;
}

static int btree_inserir_nao_cheio(NoB *no, Conta *conta) {
    int i = no->quantidade - 1;

    if (no->folha) {
        while (i >= 0 && conta->numero < no->contas[i]->numero){
            no->contas[i + 1] = no->contas[i];
            i--;
        }

        no->contas[i + 1] = conta;
        no->quantidade++;
        return 1;
    }

    while (i >= 0 && conta->numero < no->contas[i]->numero) {
        i--;
    }
    i++;

    if (no->filhos[i]->quantidade == MAX_CHAVES) {
        if (!btree_dividir_filho(no, i, no->filhos[i])) {
            return 0;
        }

        if (conta->numero > no->contas[i]->numero) {
            i++;
        }
    }
    return btree_inserir_nao_cheio(no->filhos[i], conta);
}


static int btree_inserir(Banco *banco, Conta *conta) {
    if (banco->raiz == NULL) {
        banco->raiz = btree_criar_no(1);
        if (banco->raiz == NULL) {
            return 0;
        }
    }

    if (banco->raiz->quantidade == MAX_CHAVES) {
        NoB *novaRaiz = btree_criar_no(0);
        if (novaRaiz == NULL) {
            return 0;
        }
        novaRaiz->filhos[0] = banco->raiz;
        if (!btree_dividir_filho(novaRaiz, 0, banco->raiz)) {
            free(novaRaiz);
            return 0;
        }
        banco->raiz = novaRaiz;
    }
    return btree_inserir_nao_cheio(banco->raiz, conta);
}

static void movimentos_adicionar(Conta *conta, TipoMovimento tipo, double valor) {
    Movimento *novo = (Movimento *) malloc(sizeof(Movimento));
    if (novo == NULL) {
        return;
    }

    obter_data_hora(novo->dataHora);
    novo->tipo = tipo;
    novo->valor = valor;
    novo->saldosApps = conta->saldo;
    novo->proximo = conta->movimentos;
    conta->movimentos = novo;
}

void banco_inicializar(Banco *banco) {
    banco->raiz = NULL;
    banco->totalContas = 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        banco->hash[i] = NULL;
    }
}

int banco_criar_conta(Banco *banco, const char *titular, int numero, int nif, double saldoInicial) {
    Conta *conta;

    if (saldoInicial < 0 || banco_buscar_por_numero(banco, numero) != NULL || banco_buscar_por_nif(banco, nif) != NULL) {
        return 0;
    }

    conta = (Conta *) malloc(sizeof(Conta));
    if (conta == NULL) {
        return 0;
    }

    conta->numero = numero;
    conta->nif = nif;
    strncpy(conta->titular, titular, NAME_SIZE - 1);
    conta->titular[NAME_SIZE - 1] = '\0';
    conta->saldo = saldoInicial;
    conta->totalMovimentos = 0;
    conta->movimentos = NULL;

    if (!btree_inserir(banco, conta)) {
        free(conta);
        return 0;
    }

    if (!hash_inserir(banco, conta)){
        free(conta);
        return 0;
    }

    banco->totalContas++;

    if (saldoInicial > 0) {
        movimentos_adicionar(conta, MOVIMENTO_DEPOSITO, saldoInicial);
    }

    return 1;
}


int banco_actualizar_conta(Banco *banco, int numero, const char * novoTitular, int novoNif) {
    Conta *conta = banco_buscar_por_numero(banco, numero);
    Conta *donoNif;
    int nifAntigo;

    if (conta == NULL) {
        return 0;
    }

    donoNif = banco_buscar_por_nif(banco, novoNif);
    if (donoNif != NULL && donoNif != conta) {
        return 0;
    }

    nifAntigo = conta->nif;
    hash_remover(banco, nifAntigo);

    strncpy(conta->titular, novoTitular, NAME_SIZE -1);
    conta->titular[NAME_SIZE - 1] = '\0';
    conta->nif = novoNif;

    return hash_inserir(banco, conta);
}

int banco_depositar(Banco *banco, int numero, double valor) {
    Conta *conta = banco_buscar_por_numero(banco, numero);

    if (conta == NULL || valor <= 0) {
        return 0;
    }

    conta->saldo += valor;
    movimentos_adicionar(conta, MOVIMENTO_DEPOSITO, valor);
    return 1;
}

int banco_levantar(Banco *banco, int numero, double valor) {
    Conta *conta = banco_buscar_por_numero(banco, numero);

    if (conta == NULL || valor <= 0 || conta->saldo < valor) {
        return 0;
    }

    conta->saldo -= valor;
    movimentos_adicionar(conta, MOVIMENTO_LEVANTAMENTO, valor);
    return 1;
}

int banco_transferir(Banco *banco, int origem, int destino, double valor) {
    Conta *contaOrigem = banco_buscar_por_numero(banco, origem);
    Conta *contaDestino = banco_buscar_por_numero(banco, destino);

    if (contaOrigem == NULL || contaDestino == NULL || contaOrigem == contaDestino || valor <= 0 || contaOrigem->saldo < valor) {
        return 0;
    }
    contaOrigem->saldo -= valor;
    contaDestino->saldo += valor;

    movimentos_adicionar(contaOrigem, MOVIMENTO_TRANSFERENCIA_SAIDA, valor);
    movimentos_adicionar(contaDestino, MOVIMENTO_TRANSFERENCIA_ENTRADA, valor);
    return 1;
}

static void imprimir_conta(Conta *conta) {
    printf("%-8d | %-20s | %-9d | %12.2f | %d\n",
    conta->numero,
    conta->titular,
    conta->nif,
    conta->saldo,
    conta->totalMovimentos);
}

static void btree_listar(NoB *no) {
    if (no == NULL) {
        return;
    }

    for (int i = 0; i < no->quantidade; i++) {
        if (!no->folha) {
            btree_listar(no->filhos[i]);
        }
        imprimir_conta(no->contas[i]);
    }

    if (!no->folha) {
        btree_listar(no->filhos[no->quantidade]);
    }
}

void banco_listar_contas(Banco *banco) {
    printf("\nConta    | Titular              | NIF       | Saldo (AOA)  | movimentoss\n");
    printf("---------+----------------------+-----------+--------------+------------\n");
    btree_listar(banco->raiz);
}

void banco_mostrar_extrato(Banco *banco, int numero) {
    Conta *conta = banco_buscar_por_numero(banco, numero);
    Movimento *atual;

    if (conta == NULL) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("\nExtrato da conta %d - %s\n", conta->numero, conta->titular);
    printf("Data/Hora        | Tipo                    | Valor       | Saldo apos\n");
    printf("-----------------+-------------------------+-------------+------------\n");

    atual = conta->movimentos;
    while (atual != NULL) {
        printf("%-16s | %-23s | %11.2f | %10.2f\n",
        atual->dataHora,
        banco_tipo_movimentos_texto(atual->tipo),
        atual->valor,
        atual->saldosApps);
        atual = atual->proximo;
    }
}

typedef struct Estatisticas {
    double somaSaldos;
    int totalMovimentos;
    Conta *maiorSaldo;
    Conta *menorSaldo;
} Estatisticas;

static void btree_estatisticas(NoB *no, Estatisticas *estatisticas) {
    if (no == NULL) {
        return;
    }

    for (int i = 0; i < no->quantidade; i++) {
        Conta *conta;
        if (!no->folha) {
            btree_estatisticas(no->filhos[i], estatisticas);
        }

        conta = no->contas[i];
        estatisticas->somaSaldos += conta->saldo;
        estatisticas->totalMovimentos += conta->totalMovimentos;

        if (estatisticas->maiorSaldo == NULL || conta->saldo > estatisticas->maiorSaldo->saldo) {
            estatisticas->maiorSaldo = conta;
        }

        if (estatisticas->menorSaldo == NULL || conta->saldo < estatisticas->menorSaldo->saldo) {
            estatisticas->menorSaldo = conta;
        }
    }

    if(!no->folha) {
        btree_estatisticas(no->filhos[no->quantidade], estatisticas);
    }
}

void banco_mostrar_estatisticas(Banco *banco) {
    Estatisticas estatisticas = {0, 0, NULL, NULL};

    btree_estatisticas(banco->raiz, &estatisticas);

    printf("\nTotal de contas: %d\n", banco->totalContas);
    if (banco->totalContas == 0) {
        return;
    }

    printf("Saldo medio: %.2f AOA\n", estatisticas.somaSaldos / banco->totalContas);
    printf("Maior saldo: conta %d (%.2f AOA)\n", estatisticas.maiorSaldo->numero, estatisticas.maiorSaldo->saldo);
    printf("Menor saldo: conta %d (%.2f AOA)\n", estatisticas.menorSaldo->numero, estatisticas.menorSaldo->saldo);
    printf("Total de movimentos: %d\n", estatisticas.totalMovimentos);
}

int banco_carregar_contas(Banco *banco, const char *caminho) {
        FILE *ficheiro = fopen(caminho, "r");
        char titular[NAME_SIZE];
        int numero;
        int nif;
        double saldo;
        int inseridas = 0;

        if (ficheiro == NULL) {
            printf("Erro ao abrir o ficheiro.\n");
            return -1;
        }

        while (fscanf(ficheiro, "%49s %d %d %lf", titular, &numero, &nif, &saldo) == 4) {
            if (banco_criar_conta(banco, titular, numero, nif, saldo)) {
                inseridas++;
            }
        }

        fclose(ficheiro);
        return inseridas;
}

static void relatorio_contas(NoB *no, FILE *ficheiro) {
    if (no == NULL) {
        return;
    }

    for (int i = 0; i < no ->quantidade; i++) {
        if (!no->folha) {
            relatorio_contas(no->filhos[i], ficheiro);
        }

        fprintf(ficheiro, "%-6d | %-20s | %-9d | %12.2f | %d\n",
        no->contas[i]->numero,
        no->contas[i]->titular,
        no->contas[i]->nif,
        no->contas[i]->saldo,
        no->contas[i]->totalMovimentos);
    }

    if (!no->folha) {
        relatorio_contas(no->filhos[no->quantidade], ficheiro);
    }
}

int banco_guardar_relatorio(Banco *banco, const char *caminho) {
    FILE *ficheiro = fopen(caminho, "w");
    char dataHora[20];

    if (ficheiro == NULL) {
        return 0;
    }

    obter_data_hora(dataHora);

    fprintf(ficheiro, "=== RELATORIO DO SISTEMA BANCARIO ===\n");
    fprintf(ficheiro, "Data de geracao: %s\n", dataHora);
    fprintf(ficheiro, "Total de contas: %d\n\n", banco->totalContas);
    fprintf(ficheiro, "Conta  | Titular              | NIF       | Saldo (AOA) | Movimentos\n");

    relatorio_contas(banco->raiz, ficheiro);
    fclose(ficheiro);
    return 1;
}


static void movimentos_liberar(Movimento *movimento) {
    while (movimento != NULL) {
        Movimento *proximo = movimento->proximo;
        free(movimento);
        movimento = proximo;
    }
}

static void btree_liberar(NoB *no) {
    if (no == NULL) {
        return;
    }

    for (int i = 0; i < no->quantidade; i++) {
        if(!no->folha) {
            btree_liberar(no->filhos[i]);
        }
        movimentos_liberar(no->contas[i]->movimentos);
        free(no->contas[i]);
    }

    if (!no->folha) {
        btree_liberar(no->filhos[no->quantidade]);
    }

    free(no);
}

void banco_liberar(Banco *banco) {
    btree_liberar(banco->raiz);
    banco->raiz = NULL;

    for (int i = 0; i < HASH_SIZE; i++) {
        NoHash *atual = banco->hash[i];
        while (atual != NULL) {
            NoHash *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        banco->hash[i] = NULL;
    }

    banco->totalContas = 0;
}