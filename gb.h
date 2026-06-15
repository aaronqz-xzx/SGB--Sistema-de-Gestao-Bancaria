#ifndef GB_H
#define GB_H

#define HASH_SIZE 23

typedef enum{
    VAZIO,
    OCUPADO,
    REMOVIDO
} Estado;

typedef struct {
    int chave;
    Estado estado;
} Entrada;

typedef struct {
    Entrada entradas[HASH_SIZE];
    int quantidade;
} hash;

void inicializar(hash *h);
int inserir(hash *h, int chave);
int buscar(hash *h, int chave);
int remover(hash *h, int chave);
int pesquisar(hash *h, int chave);
void imprimir(hash *h);


#endif