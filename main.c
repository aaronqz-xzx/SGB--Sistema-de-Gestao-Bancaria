#include <stdio.h>
#include <stdlib.h>
#include "gb.h"

<<<<<<< HEAD
static int ler_inteiro(const char *mensagem, int valor) {
    printf("%s", mensagem);
    if (scanf("%d", valor) != 1){
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return 0;
    }
    return 1;
}

static int ler_double(const char *mensagem, double *valor) {
    printf("%s", mensagem);
    if (scanf("%lf", valor) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF){
        }
        return 0;
    }
}

static void menu(void){

    printf("====== Sistema de Gestao Bancario ========\n");
    printf("1. Criar Conta\n");
    printf("2. Carregar contas de ficheiro\n");
    printf("3.Atualizar conta\n");
    printf("4. Consultar Saldo por numero\n");
    printf("5. Depositar\n");
    printf("6. Levantar\n");
    printf("7. Transferir\n");
    printf("8. Pesquisar por NIF\n");
    printf("9. Listar todas as contas\n");
    printf("10. Extrato \n");
    printf("11. Estatisitcas\n");
    printf("12. Gaurdar Relatorio em Ficheiros\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

static void criarconta(Banco *banco) {
    
=======
int main(){

    int opcao;

    printf("====== Menu Bancario ========\n");
    printf("1. Criar Conta\n");
    printf("2. Actualizar conta\n");
    printf("3. Consultar Saldo\n");
    printf("4. Depositar\n");
    printf("5. Levantar\n");
    printf("6. Transferir\n");
    printf("7. Pesquisar por NIF\n");
    printf("8. Sair\n");
    scanf("%d", &opcao);

    while(opcao != 8){
        switch(opcao){
            case 1:
                criarConta();
                break;
            case 2:
                actualizarConta();
                break;
            case 3:
                consultarSaldo();
                break;
            case 4:
                depositar();
                break;
            case 5:
                levantar();
                break;
            case 6:
                transferir();
                break;
            case 7:
                pesquisarPorNIF();
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    
    }
    return 0;
>>>>>>> 90e812f (primeiras adicioes)
}