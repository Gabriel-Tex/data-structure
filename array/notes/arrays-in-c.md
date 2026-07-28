# Anotações — Arrays em C (Modern C, seção 6.1)

## Arrays não são ponteiros

Muita gente que aprende C confunde arrays com ponteiros, porque os dois estão bastante relacionados na linguagem — mas são conceitos diferentes. Para entender arrays de verdade, o ideal é estudá-los isoladamente, sem pensar neles como "ponteiros disfarçados": um array é um objeto que agrupa vários elementos do mesmo tipo, guardados lado a lado na memória.

## Declarando um array

Um array é declarado colocando um `[N]` depois do tipo, indicando quantos elementos ele vai ter:

```c
double a[4];   // array com 4 elementos do tipo double
signed b[N];   // array com N elementos do tipo signed
```

## Arrays multidimensionais

Um array também pode guardar, como elemento, outro array — isso forma um array multidimensional (tipo uma matriz). Como os colchetes `[]` são lidos da esquerda pra direita, uma declaração como essa:

```c
double C[M][N];
```

deve ser lida "de dentro para fora": `C` é um array de M posições, e cada uma dessas posições é, por sua vez, um array de N valores `double`. Ou seja, uma matriz nada mais é do que um "array de arrays".

## Acessando e inicializando elementos

Elementos são acessados com colchetes, como `a[0]`. Também é possível inicializar um array indicando explicitamente qual posição está recebendo qual valor (os chamados inicializadores designados) — isso deixa o código mais claro e menos propenso a erro se o tamanho do array mudar no futuro.

## O que não se pode fazer com um array

Existem algumas limitações importantes sobre como arrays podem ser usados em C:

- Um array não tem "valor" por si só — só existe como objeto. Por isso, operações que dependem de um valor não funcionam diretamente sobre um array inteiro.
- **Arrays não podem ser comparados** diretamente com `==` ou `<`.
- **Arrays não podem receber atribuição** com `=` depois de já declarados.
- O único operador que realmente funciona diretamente sobre um array é o de indexação, `[]`.

## Tamanho fixo vs. tamanho variável

Arrays em C podem ter o tamanho definido de duas formas:

- **Tamanho fixo (constante)** — o tamanho é conhecido em tempo de compilação, como em `int arr[10];`. Essa é a forma tradicional, presente em C desde sempre.
- **Tamanho variável (VLA — Variable-Length Array)** — o tamanho é decidido apenas em tempo de execução, algo como `int arr[n];` onde `n` é uma variável. Esse recurso foi introduzido no C99 e tem restrições: só pode ter valor inicial padrão (não dá pra inicializar com valores específicos), e não pode ser declarado fora de uma função.

Também é possível declarar um array sem especificar o tamanho, desde que ele já venha com valores iniciais — nesse caso, o compilador deduz o tamanho automaticamente a partir da quantidade de valores fornecidos:

```c
double E[] = { 22.0, 17.0, 1, 0.5 };  // o compilador entende que o tamanho é 4
```

**Calculando o tamanho de um array já existente:** já que arrays "esquecem" seu próprio tamanho em vários contextos (como veremos a seguir), uma forma de descobrir quantos elementos ele tem é dividir o tamanho total em bytes pelo tamanho de um elemento:

```c
tamanho = sizeof(A) / sizeof(A[0]);
```

## Um problema importante: arrays como parâmetro de função

Esse é um dos pontos que mais gera confusão para quem está aprendendo C. Quando você passa um array como parâmetro para uma função, ele **perde a informação do seu tamanho** — a função recebe, na prática, apenas o endereço do primeiro elemento, sem saber quantas posições o array tem.

Duas consequências práticas:

- **Nunca use `sizeof` dentro da função para tentar descobrir o tamanho do array recebido como parâmetro** — o resultado vai estar errado, porque a função não tem mais essa informação.
- Como a função recebe apenas o endereço, qualquer alteração feita no array **dentro da função afeta o array original** — é como se o array fosse passado por referência, mesmo sem você usar ponteiro explicitamente.

Exemplo: essa função troca os dois primeiros elementos de um array `double`, e a troca realmente afeta o array original passado por quem chamou a função:

```c
void swap_double(double a[static 2]) {
    double tmp = a[0];
    a[0] = a[1];
    a[1] = tmp;
}
```

## Strings: um caso especial de array

Em C não existe um tipo "string" de verdade — uma string é, na prática, apenas um **array de `char` que termina com um caractere especial de valor zero**, chamado de terminador nulo (`'\0'`). Por isso, a palavra `"hello"`, que tem 5 letras, na verdade ocupa 6 posições de memória: as 5 letras mais o `'\0'` no final.

Isso traz uma consequência importante: **nem todo array de caracteres é uma string**. Um array de `char` só é considerado uma string de verdade se realmente terminar com esse `'\0'`. Um array que guarda exatamente as letras "jay" sem espaço para o terminador, por exemplo, parece uma string, mas não é uma — e usar funções de string nele vai causar problemas.

## Funções da biblioteca padrão para lidar com arrays de caracteres

A biblioteca `<string.h>` oferece duas famílias de funções, com propósitos diferentes:

**Funções que começam com `mem`** — funcionam sobre qualquer array de bytes, não exigem que seja uma string terminada em `'\0'`, mas em compensação sempre pedem o tamanho como parâmetro explícito:

- `memcpy(destino, origem, tamanho)` — copia um bloco de memória para outro
- `memcmp(a, b, tamanho)` — compara dois blocos de memória byte a byte
- `memchr(array, caractere, tamanho)` — procura um caractere dentro do array

**Funções que começam com `str`** — exigem que os argumentos sejam strings de verdade (terminadas em `'\0'`), e por isso não precisam receber o tamanho como parâmetro — elas simplesmente andam pelo array até encontrar o terminador:

- `strlen(s)` — retorna o tamanho da string até encontrar o `'\0'` (não é o tamanho do array, é o tamanho até o terminador)
- `strcpy(destino, origem)` — copia uma string inteira até seu `'\0'`
- `strcmp(a, b)` — compara duas strings, parando no primeiro `'\0'` encontrado
- `strchr(s, caractere)` — procura um caractere dentro de uma string

**Ponto de atenção crítico:** usar uma função da família `str` em um array de `char` que não é uma string de verdade (ou seja, que não termina em `'\0'`) é um erro grave e comum. Os sintomas típicos são: a função demora um tempo anormalmente longo (porque continua lendo memória à procura de um `'\0'` que nunca aparece), o programa trava com erro de segmentação (por tentar acessar memória fora dos limites do array), ou dados são corrompidos silenciosamente sem gerar erro nenhum na hora.

## Ponteiro genérico (`void*`)

As funções da família `mem` (como `memcpy`) recebem seus parâmetros como `void*` — um tipo especial de ponteiro que não indica qual é o tipo do dado apontado. Isso é o que permite essas funções funcionarem com qualquer tipo de array (não só `char`), mas o entendimento completo desse mecanismo depende de conhecer ponteiros com mais profundidade.
