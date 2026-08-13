# **Pilhas (Stacks)**

*Baseado em Cormen, Leiserson, Rivest & Stein — **Algoritmos: Teoria e Prática**, 3ª ed., Seção 10.1.*

---

## 1. Introdução

Uma **pilha** é um conjunto dinâmico que implementa a política **LIFO** — *Last-In, First-Out* (último a entrar, primeiro a sair). Junto com a **fila** (FIFO — primeiro a entrar, primeiro a sair), a pilha é uma das estruturas de dados mais elementares usadas para representar conjuntos dinâmicos.

> **Analogia clássica:** pense em uma pilha de pratos empilhados em uma mola (como em restaurantes self-service). Você só consegue colocar (empilhar) ou retirar (desempilhar) o prato que está no **topo**. A ordem de retirada é sempre o inverso da ordem de inserção.

---

## 2. Operações fundamentais

A pilha suporta duas operações principais de manipulação, ambas variações das operações genéricas `INSERT` e `DELETE` de conjuntos dinâmicos:

| Operação         | Descrição                                                                                       |
| ---------------- | ----------------------------------------------------------------------------------------------- |
| `PUSH(S, x)`     | Insere o elemento `x` no topo da pilha `S` (equivale ao `INSERT`).                              |
| `POP(S)`         | Remove e retorna o elemento do topo da pilha `S` (equivale ao `DELETE`, sem receber argumento). |
| `STACK-EMPTY(S)` | Consulta que testa se a pilha está vazia.                                                       |

### Condições de erro

* **Estouro negativo (*underflow*):** ocorre quando tentamos executar `POP` em uma pilha vazia. É considerado um erro.
* **Estouro (*overflow*):** ocorre quando tentamos executar `PUSH` em uma pilha que já atingiu sua capacidade máxima, no caso de uma implementação com arranjo de tamanho fixo.

---

## 3. Implementação com arranjo

Cormen implementa a pilha usando um arranjo `S[1..n]`, com um atributo adicional `S.topo` que indica o índice do elemento mais recentemente inserido, isto é, o **topo da pilha**.

* A pilha ocupa as posições `S[1..S.topo]`.
* `S[1]` é o elemento na **base** da pilha.
* `S[S.topo]` é o elemento no **topo** da pilha.
* Quando `S.topo = 0`, a pilha está **vazia**.

### Pseudocódigo (Cormen)

```text
STACK-EMPTY(S)
1  if S.topo == 0
2      return TRUE
3  else return FALSE

PUSH(S, x)
1  S.topo = S.topo + 1
2  S[S.topo] = x

POP(S)
1  if STACK-EMPTY(S)
2      error "estouro negativo"
3  else S.topo = S.topo - 1
4      return S[S.topo + 1]
```

**Observação importante:** o `POP` não remove fisicamente o elemento do arranjo — apenas decrementa o índice `topo`. O elemento permanece "logicamente fora" da pilha, mesmo que continue armazenado na posição do arranjo.

---

## 4. Análise de complexidade

Todas as três operações — `PUSH`, `POP` e `STACK-EMPTY` — têm tempo de execução:

**O(1)**

Ou seja, tempo **constante**, independentemente do número de elementos armazenados. Isso torna a pilha extremamente eficiente para as operações que suporta.

---

## 5. Implementação alternativa: lista ligada

Embora Cormen use arranjos como exemplo didático principal, a pilha também pode ser implementada com **lista simplesmente ligada** (Seção 10.2, Exercício 10.2-2), inserindo e removendo sempre pelo **início** da lista:

* `PUSH` = inserir no início da lista → **O(1)**
* `POP` = remover do início da lista → **O(1)**

**Vantagem sobre o arranjo:** não há limite fixo de capacidade, portanto não sofre *overflow* por atingir uma capacidade pré-determinada (embora ainda possa ocorrer falha por falta de memória do sistema).

**Desvantagem:** há *overhead* de ponteiros e de alocação dinâmica de memória.

---

## 6. Aplicações e relação com outras estruturas

* **Duas pilhas em um único arranjo:** é possível implementar duas pilhas dentro de um mesmo arranjo `A[1..n]`, crescendo uma a partir da extremidade esquerda e a outra da extremidade direita, uma em direção à outra. Nenhuma sofre *overflow*, a menos que o total de elementos das duas pilhas ultrapasse `n` (Exercício 10.1-2).
* **Implementação de fila usando duas pilhas** e vice-versa (Exercícios 10.1-6 e 10.1-7) — mostra a relação estrutural entre essas duas estruturas de dados.
* **Chamadas de sub-rotinas e recursão:** o uso de pilhas para gerenciar o encadeamento de sub-rotinas remonta a A. M. Turing em 1947 (nota histórica do capítulo).
* Pilhas são usadas em **avaliação de expressões**, **backtracking**, **percursos não recursivos de árvores** (Exercício 12.1-3), **verificação de balanceamento de parênteses**, **algoritmos de compiladores**, entre outros.

---

## 7. Implementação em C

A seguir, uma implementação completa em **C**, seguindo fielmente a lógica do pseudocódigo de Cormen, usando um **arranjo estático** de tamanho fixo.

```c
#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_MAX 100

/* Estrutura da pilha, análoga ao objeto S com atributo S.topo */
typedef struct {
    int dados[TAMANHO_MAX]; /* Cormen usa índices de 1 a n; aqui usamos 0 a n-1 */
    int topo;                /* -1 indica pilha vazia (equivalente a S.topo = 0) */
} Pilha;

/* Inicializa a pilha (equivalente a definir S.topo = 0) */
void inicializarPilha(Pilha *S) {
    S->topo = -1;
}

/* STACK-EMPTY(S) */
int pilhaVazia(Pilha *S) {
    return (S->topo == -1);
}

/* Verifica se a pilha está cheia (necessário em implementação estática) */
int pilhaCheia(Pilha *S) {
    return (S->topo == TAMANHO_MAX - 1);
}

/* PUSH(S, x) */
void push(Pilha *S, int x) {
    if (pilhaCheia(S)) {
        fprintf(stderr, "Erro: estouro (overflow) da pilha\n");
        exit(EXIT_FAILURE);
    }

    S->topo = S->topo + 1;
    S->dados[S->topo] = x;
}

/* POP(S) */
int pop(Pilha *S) {
    if (pilhaVazia(S)) {
        fprintf(stderr, "Erro: estouro negativo (underflow) da pilha\n");
        exit(EXIT_FAILURE);
    }

    int x = S->dados[S->topo];
    S->topo = S->topo - 1;

    return x;
}

/* Operação auxiliar: consulta o topo sem remover
   (não faz parte do Cormen, mas é útil na prática) */
int topoPilha(Pilha *S) {
    if (pilhaVazia(S)) {
        fprintf(stderr, "Erro: pilha vazia\n");
        exit(EXIT_FAILURE);
    }

    return S->dados[S->topo];
}

/* Demonstração de uso */
int main(void) {
    Pilha S;
    inicializarPilha(&S);

    push(&S, 4);
    push(&S, 1);
    push(&S, 3);

    printf("Topo atual: %d\n", topoPilha(&S)); /* 3 */

    printf("Removido: %d\n", pop(&S)); /* 3 */
    printf("Removido: %d\n", pop(&S)); /* 1 */

    push(&S, 8);

    printf("Removido: %d\n", pop(&S)); /* 8 */
    printf("Removido: %d\n", pop(&S)); /* 4 */

    if (pilhaVazia(&S))
        printf("Pilha vazia!\n");

    return 0;
}
```

### 7.1 Implementação alternativa: pilha dinâmica com lista ligada

Uma versão sem limite fixo de tamanho, usando ponteiros:

```c
#include <stdio.h>
#include <stdlib.h>

/* Nó da lista ligada usado como pilha */
typedef struct No {
    int dado;
    struct No *proximo;
} No;

typedef struct {
    No *topo; /* ponteiro para o nó do topo; NULL == pilha vazia */
} PilhaDinamica;

void inicializarPilha(PilhaDinamica *S) {
    S->topo = NULL;
}

int pilhaVazia(PilhaDinamica *S) {
    return (S->topo == NULL);
}

/* PUSH: O(1) — insere no início da lista */
void push(PilhaDinamica *S, int x) {
    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente\n");
        exit(EXIT_FAILURE);
    }

    novo->dado = x;
    novo->proximo = S->topo;
    S->topo = novo;
}

/* POP: O(1) — remove do início da lista */
int pop(PilhaDinamica *S) {
    if (pilhaVazia(S)) {
        fprintf(stderr, "Erro: estouro negativo (underflow)\n");
        exit(EXIT_FAILURE);
    }

    No *removido = S->topo;
    int x = removido->dado;

    S->topo = removido->proximo;
    free(removido);

    return x;
}

int main(void) {
    PilhaDinamica S;
    inicializarPilha(&S);

    push(&S, 10);
    push(&S, 20);
    push(&S, 30);

    while (!pilhaVazia(&S)) {
        printf("%d\n", pop(&S)); /* imprime 30, 20, 10 */
    }

    return 0;
}
```

### 7.2 Comparação entre as duas implementações

| Aspecto                   | Arranjo (estático)                    | Lista ligada (dinâmica)                               |
| ------------------------- | ------------------------------------- | ----------------------------------------------------- |
| Complexidade `PUSH`/`POP` | **O(1)**                              | **O(1)**                                              |
| Capacidade                | Fixa, pode sofrer *overflow*          | Limitada apenas pela memória disponível               |
| Uso de memória            | Contígua, sem *overhead* de ponteiros | *Overhead* por ponteiro em cada nó                    |
| Localidade de cache       | Melhor, devido aos dados contíguos    | Geralmente pior, devido aos nós espalhados na memória |
