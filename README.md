# Sistema-de-Gestao-# Sistema de Gestão Bancária

Sistema de gestão bancária desenvolvido em **C**, no âmbito da disciplina de **Estrutura de Dados II**. O projeto combina três estruturas de dados clássicas para gerir contas bancárias, processar operações financeiras e manter o histórico de movimentos de cada conta.

> ⚠️ **Estado do projeto: em desenvolvimento.** As estruturas de dados e o menu já estão definidos, mas a implementação das funcionalidades em `banco.c` ainda está em curso (ficheiro por adicionar/completar no repositório).

## Objetivos

- Aplicar conceitos fundamentais de estruturas de dados a um problema com requisitos reais e distintos
- Justificar tecnicamente a escolha de cada estrutura
- Implementar um sistema funcional em C com múltiplas estruturas de dados interligadas
- Praticar análise de complexidade algorítmica

## Estruturas de Dados Utilizadas

|Estrutura                                           |Papel no Sistema                     |Justificação                                                                                                                         |
|----------------------------------------------------|-------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|
|**Árvore B** (grau t = 3)                           |Índice principal de contas por número|Pesquisa, inserção e listagem ordenada em O(log n), com nós equilibrados — semelhante aos índices de bases de dados relacionais reais|
|**Tabela Hash** (encadeamento externo, 101 posições)|Acesso rápido por NIF do titular     |Tabela com tamanho primo para minimizar colisões, garantindo acesso em O(1) médio                                                    |
|**Lista Ligada** (simples)                          |Histórico de movimentos por conta    |Inserção em O(1) na cabeça, sem limite de tamanho, mantendo a ordem cronológica inversa das transações                               |

A Árvore B e a Tabela Hash indexam os mesmos objetos `Conta` em memória — não há duplicação de dados. Ao criar uma conta, esta é inserida simultaneamente nas duas estruturas, garantindo acesso eficiente por número de conta e por NIF sem overhead de sincronização.

## Funcionalidades

O programa disponibiliza um menu interativo em ciclo com as seguintes opções:

1. **Criar conta** — número único, nome do titular, NIF e depósito inicial
- **Modo automático**: carrega contas a partir de um ficheiro de texto
- **Modo manual**: introdução via teclado, com validação de NIF duplicado, número único e saldo inicial não negativo
1. **Atualizar conta** — edita nome e/ou NIF do titular, com atualização consistente nas duas estruturas de indexação
1. **Consultar saldo** — pesquisa por número de conta na Árvore B
1. **Depositar** — adiciona fundos e regista o movimento (data, hora, tipo, saldo resultante)
1. **Levantar** — retira fundos com verificação de saldo suficiente
1. **Transferir** — movimenta fundos entre duas contas de forma atómica, registando o movimento em ambas
1. **Pesquisar por NIF** — acesso direto via Tabela Hash, em O(1) médio
1. **Listar todas as contas** — travessia in-ordem da Árvore B (ordem crescente por número de conta)
1. **Extrato** — apresenta o histórico de movimentos de uma conta (data, tipo, valor, saldo após cada operação)
1. **Estatísticas** — total de contas, saldo médio, conta com maior/menor saldo e total de movimentos
1. **Guardar em ficheiro** — exporta o estado atual de todas as contas (saldo e nº de movimentos) para um ficheiro de relatório
1. **Sair** — termina o programa libertando toda a memória alocada

## Formato dos Ficheiros

### Ficheiro de entrada (modo automático)

Uma conta por linha, no formato:

```
NomeDoTitular NumeroConta NIF SaldoInicial
```

Exemplo:

```
Maria-Fernandes 1001 123456789 100000.00
Carlos-Neto 1002 987654321 50000.00
Ana-Paula 1003 111222333 75000.00
```

> Nota: o nome do titular não deve conter espaços (usar `-` como separador).

### Ficheiro de saída (opção 11 — Guardar em ficheiro)

```
=== RELATORIO DO SISTEMA BANCARIO ===
Data de geração: 2026-06-02 14:30
Total de contas: 5

Conta | Titular | NIF | Saldo (AOA) | Movimentos
1001 | Maria-Fernandes | 123456789 | 105000.00 | 3
1002 | Carlos-Neto | 987654321 | 60000.00 | 2
```

## Estrutura do Projeto

```
.
├── main.c        # Ponto de entrada e menu interativo
├── banco.c       # Implementação das operações bancárias (em desenvolvimento)
├── banco.h       # Estruturas de dados principais (Conta, Árvore B, Tabela Hash, Lista de Movimentos)
├── gb.c          # Implementação de uma tabela hash auxiliar/de testes
├── gb.h          # Definições da tabela hash auxiliar (HASH_SIZE = 23)
└── README.md
```

> Nota: `gb.h`/`gb.c` contêm uma implementação independente e mais simples de tabela hash (provavelmente usada para testes/protótipo inicial), distinta da tabela hash de 101 posições definida em `banco.h` para o sistema final.

## Compilação e Execução

> Atualmente o `banco.c` ainda não está completo/disponível no repositório, pelo que a compilação completa ainda não é possível.

```bash
gcc -o sistema main.c banco.c gb.c -Wall
./sistema
```

## Tecnologias

- Linguagem: **C**
- Sem dependências externas (apenas biblioteca padrão)

## Autores

Trabalho académico desenvolvido para a disciplina de Estrutura de Dados II.
