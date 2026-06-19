#ifndef BANCO_H
#define BANCO_H


#define NAME_SIZE 50
#define HASH_SIZE 101
#define GRAU_MINIMO 3
#define MAX_CHAVES (2 * GRAU_MINIMO - 1)
#define MAX_FILHOS (2 * GRAU_MINIMO)

typedef enum {
    MOVIMENTO_DEPOSITO,
    MOVIMENTO_LEVANTAMENTO,
    MOVIMENTO_TRANSFERENCIA_ENTRADA,
    MOVIMENTO_TRANSFERENCIA_SAIDA
}TipoMovimento;

typedef struct Movimento {
    char dataHora[20];
    TipoMovimento tipo;
    double valor;
    double saldosApps;
    struct Movimento *proximo;
}Movimento;

typedef struct Conta {
    int numero;
    int nif;
    char titular[NAME_SIZE];
    double saldo;
    int totalMovimentos;
    Movimento *movimentos;
} Conta;

typedef struct NoB {
    int quantidade;
    int folha;
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
    NoHash *hash[HASH_SIZE];
    int totalContas;
} Banco;


void banco_inicializar(Banco *banco);
void banco_liberar(Banco *banco);
int banco_criar_conta(Banco *banco, const char *titular, int numero, int nif, double saldo);
int banco_actualizar_conta(Banco *banco, int numero, const char *novoTitular, int novoNif);
int banco_depositar(Banco *banco, int numero, double valor);
int banco_levantar(Banco *banco, int numero, double valor);
int banco_transferir(Banco *banco, int origem, int destino, double valor);
Conta *banco_buscar_por_numero(Banco *banco, int numero);
Conta *banco_buscar_por_nif(Banco *banco, int nif);
void banco_listar_contas(Banco *banco);
void banco_mostrar_extrato(Banco *banco, int numero);
void banco_mostrar_estatisticas(Banco *banco);
int banco_carregar_contas(Banco *banco, const char *caminho);
int banco_guardar_relatorio(Banco *banco, const char *caminho);

#endif