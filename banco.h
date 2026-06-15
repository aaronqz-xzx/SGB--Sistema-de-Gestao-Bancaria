#ifndef BANCO_H
#define BANCO_h


#define NAME_SIZE 50
#define HASH_TAMANHO 101
#define GRAU_MINIMO 3
#define MAX_CHAVES (2 * GRAU_MINIMO - 1)
#define MAX_FILHOS (2 * GRAU_MINIMO)

typedef enum {
    MOVIMENTO_DEPOSITO,
    MOVIMENTO_LEVANTAMENTO,
    MOVIMENTO_TRABSFERENCIA_ENTRADA,
    MOVIMENTO_TRANSFERENCIA_SAIDA
}TipoMovimento;

typedef struct Movimento {
    char dataHora[20];
    TipoMovimento tipo;
    double valor;
    double saldosApos;
    struct Movimento *proximo;
}Movimento;

typedef struct Conta {
    int numero;
    int nif;
    char titular[NAME_SIZE];
    double saldo;
    Movimento *movimento;
} Conta;

typedef struct NoB {
    int quantidade;
    int follha;
    Conta *contas[MAX_CHAVES];
    struct NoB *filhos[MAX_FILHOS];
}NoB;

typedef struct NoHash {
    int nif;
    Conta *conta;
    struct NoHash *proximo;
} NoHash;

typedef struct Banco {
    NoB *raiz;
    NoHash *hash[HASH_TAMANHO];
    int totalContas;
} Banco;


void inicializarBanco(Banco *banco);
void liberarBanco(Banco *banco);

#endif