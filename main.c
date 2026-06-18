#include <stdio.h>
#include <stdlib.h>
#include "gb.h"
#include "banco.h"


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

static void criar_conta(Banco *banco) {
    char titular[NAME_SIZE];
    int numero, nif;
    double saldo;

    printf("Digite o nome do titular: ");
    scanf("%49s", titular);
    if (!ler_inteiro("Numero da conta: ", &numero) ||
        !ler_inteiro("NIF: ", &nif) ||
        !ler_double("Saldo inicial: ", &saldo)) {
        printf("Entrada invalida. Conta nao criada.\n");
        return;
    }

    if (banco_criar_conta(banco, titular, numero, nif, saldo)) {
        printf("Conta criada com sucesso!\n");
    } else {
        printf("Erro ao criar conta. Verifique os dados e tente novamente.\n");
    }
}

static void consultar_por_numero(Banco *banco) {
    int numero;
    Conta *conta;

    if (!ler_inteiro("Digite o numero da conta: ", &numero)) {
        printf("Entrada invalida.\n");
        return;
    }

    conta = banco_buscar_por_numero(banco, numero);
    if (conta == NULL) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("Titular: %s | NIF: %d | Saldo: %.2f AOA\n",
        conta->titular,
        conta->nif,
        conta->saldo);
}

static void pesquisar_por_nif(Banco *banco) {
    int nif;
    Conta *conta;

    if (!ler_inteiro("NIF: ", &nif)) {
        printf("Entrada invalida.\n");
        return;
    }

    conta = banco_buscar_por_nif(banco, nif);
    if (conta == NULL) {
        printf("Conta nao encontrada.\n");
        return;
    }

    printf("Numero: %d | Titular: %s | Saldo: %.2f AOA\n",
        conta->numero,
        conta->titular,
        conta->saldo);
}

static void actualizar_conta(Banco *banco) {
    int numero;
    int nif;
    char titular[NAME_SIZE];

    if (!ler_inteiro("Numero da conta: ", &numero)) {
        printf("Entrada Invalida.\n");
        return;
    }

    printf("Novo titular (Sem espacos): ");
    scanf("%49s", titular);

    if(!ler_inteiro("Novo NIF: ", &nif)) {
        printf("Entrada Invalida.\n");
        return;
    }

    if (banco_actualizar_conta(banco, numero, titular, nif)) {
        printf("Conta atualizada com sucesso.\n");
    } else {
        printf("Nap foi possivel actualizar.Conta inexistente ou NIF duplicado.\n");
    }
}

static void operar_valor(Banco *banco, int deposito) {
    int numero;
    double valor;
    int sucesso;

    if (!ler_inteiro("Numero da conta: ", &numero) ||
        !ler_double("Valor: ", &valor)) {
        printf("Entrada invalida.\n");
        return;
    }

    sucesso = deposito ? banco_depositar(banco, numero, valor) : banco_levantar(banco, numero, valor);

    if (sucesso) {
        printf("Operacao realizada com sucesso.\n");
    } else {
        printf("Operacao recusada. Verifique conta, valor e saldo disponivel.\n");
    }
}

static void transferir(Banco *banco) {
    int origem;
    int destino;
    double valor;

    if (!ler_inteiro("Conta de origem: ", &origem) ||
        !ler_inteiro("Conta de destino: ", &destino) ||
        !ler_double("Valor: ", &valor)) {
        printf("Entrada invalida.\n");
        return;
    }

    if (banco_transferir(banco, origem, destino, valor)) {
        printf("Transferencia realizada com sucesso.\n");
    } else {
        printf("Transferencia recusada. Verifique contas, valor e saldo.\n");
    }
}

static void carregar_ficheiro(Banco *banco) {
    char caminho[120];
    int inseridas;

    printf("Caminho do ficheiro: ");
    scanf("%119s", caminho);

    inseridas = banco_carregar_contas(banco, caminho);
    if (inseridas < 0) {
        printf("Nao foi possivel abrir o ficheiro.\n");
    } else {
        printf("%d conta(s) carregada(s).\n", inseridas);
    }
}

static void guardar_relatorio(Banco *banco) {
    char caminho[120];

    printf("Caminho do relatorio: ");
    scanf("%119s", caminho);

    if (banco_guardar_relatorio(banco, caminho)) {
        printf("Relatorio guardado com sucesso.\n");
    } else {
        printf("Nao foi possivel guardar o relatorio.\n");
    }
}

int main(void) {
    Banco banco;
    int opcao = -1;

    banco_inicializar(&banco);

    do {
        menu();

        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            printf("Opcao invalida.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                criar_conta_manual(&banco);
                break;
            case 2:
                carregar_ficheiro(&banco);
                break;
            case 3:
                actualizar_conta(&banco);
                break;
            case 4:
                consultar_por_numero(&banco);
                break;
            case 5:
                operar_valor(&banco, 1);
                break;
            case 6:
                operar_valor(&banco, 0);
                break;
            case 7:
                transferir(&banco);
                break;
            case 8:
                pesquisar_por_nif(&banco);
                break;
            case 9:
                banco_listar_contas(&banco);
                break;
            case 10: {
                int numero;
                if (ler_inteiro("Numero da conta: ", &numero)) {
                    banco_mostrar_extrato(&banco, numero);
                } else {
                    printf("Entrada invalida.\n");
                }
                break;
            }
            case 11:
                banco_mostrar_estatisticas(&banco);
                break;
            case 12:
                guardar_relatorio(&banco);
                break;
            case 0:
                printf("A terminar o sistema...\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    banco_liberar(&banco);
    return 0;
}
