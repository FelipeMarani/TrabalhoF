#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define degree 2 // Definindo o grau da árvore B+

typedef struct no // Definição da estrutura de um nó
{
    int *chave;        // Chave do nó
    struct no **filho; // Ponteiro para os filhos do nó
    int n;             // Número de filhos
    bool folha;        // Flag para indicar se o nó é folha
    struct no *prox;   // Ponteiro para o próximo nó
} no;

typedef struct arvore // Definição da estrutura da árvore B+
{
    int t;  // Grau minimo da árvore
    no *rz; // Ponteiro para a raiz da árvore
    int n;  // Número de nós na árvore
} arv;

no *createNo(int t, bool f)
{                                   // função para a criação de um novo nó
    no *newNo = malloc(sizeof(no)); // Aloca memória para o novo nó
    if (newNo == NULL)
    {
        printf("Erro ao alocar memória para o nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória
    }
    newNo->folha = f;                                 // Define se o nó é folha ou não
    newNo->chave = malloc((2 * t - 1) * sizeof(int)); // Aloca memória para as chaves do nó
    if (newNo->chave == NULL)
    {
        printf("Erro ao alocar memória para as chaves do nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória para as chaves
    }
    newNo->filho = malloc((2 * t + 1) * sizeof(no *)); // Aloca memória para os filhos do nó
    if (newNo->filho == NULL)
    {
        printf("Erro ao alocar memória para os filhos do nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória para os filhos
    }
    newNo->prox = NULL; // Inicializa o ponteiro para o proximo nó como NULL
    newNo->n = 0;       // Inicializa o número de chaves no nó como 0
    return newNo;       // Retorna o novo nó criado
}

arv *createArvore(int g) // Função para criar uma nova árvore B+
{
    arv *newArv = (arv *)malloc(sizeof(arv)); // Aloca memória para a nova árvore
    if (newArv == NULL)
    {
        printf("Erro ao alocar memória para a árvore.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória
    }
    newArv->t = g;                  // Define o grau mínimo da árvore
    newArv->n = 1;                  // inicializa o numero de nós como 1
    newArv->rz = createNo(g, true); // Cria a raiz da árvore como um nó folha
    if (newArv->rz == NULL)
    {
        printf("Erro ao criar a raiz da árvore.\n");
        exit(1); // Finaliza o programa se não conseguir criar a raiz
    }
    return newArv; // Retorna a nova árvore criada
}

void divFilho(arv *A, no *pai, int idx, no *folha)
{
    int t = A->t;
    no *new = createNo(t, true); // Cria o novo nó folha

    new->n = t - 1; // O novo nó (da direita) receberá t-1 chaves

    for (int j = 0; j < t - 1; j++) // Copia as últimas t-1 chaves do nó cheio para o novo nó
    {
        new->chave[j] = folha->chave[j + t];
    }

    folha->n = t; // O nó antigo (da esquerda) ficará com t chaves

    new->prox = folha->prox; // Ajusta os ponteiros da lista ligada de folhas
    folha->prox = new;

    for (int j = pai->n; j >= idx + 1; j--) // Prepara para inserir a nova chave de separação no pai
    {
        pai->filho[j + 1] = pai->filho[j];
    }

    pai->filho[idx + 1] = new;

    for (int j = pai->n - 1; j >= idx; j--)
    {
        pai->chave[j + 1] = pai->chave[j];
    }

    // A chave de separação é a primeira chave do novo nó folha
    pai->chave[idx] = new->chave[0]; // Move as chaves do pai para a direita para abrir espaço

    pai->n++; // Incrementa o contador de chaves no pai
}

void divInterna(arv *A, no *pai, int idx, no *intr) // Função para realizar a divisão interna
{
    int t = A->t;                 // Obtém o grau mínimo da árvore
    no *new = createNo(t, false); // Cria um novo nó interno
    for (int j = 0; j < t - 1; j++)
    {
        new->chave[j] = intr->chave[j + t]; // Copia as chaves do nó interno para o novo nó
    }
    for (int j = 0; j < t; j++)
    {
        new->filho[j] = intr->filho[j + t]; // Copia os filhos do nó interno para o novo nó
    }
    int sep = intr->chave[t - 1]; // Obtém a chave de separação
    intr->n = t - 1;              // Atualiza o numero de chaves no nó interno
    new->n = t - 1;               // Atualiza o número de chaves no novo nó
    for (int j = pai->n; j > idx; j--)
    {
        pai->filho[j + 1] = pai->filho[j]; // Move os filhos para a direita
        pai->chave[j] = pai->chave[j - 1]; // Move as chaves para a direita
    }
    pai->filho[idx + 1] = new; // Define o novo nó como filho do pai
    pai->chave[idx] = sep;     // Define a chave de separação no pai
    pai->n++;                  // Incrementa o número de chaves no pai
}

void insereNotFull(arv *A, no *r, int k) // Função para inserir um elemento em um nó não cheio
{
    int i = r->n - 1; // Obtém o número de chaves no nó
    if (r->folha)     // Verifica se o nó é uma folha
    {
        while (i >= 0 && r->chave[i] > k) // Percorre as chaves do nó
        {
            r->chave[i + 1] = r->chave[i]; // Move as chave para a direita
            i--;                           // Decrementa o índice
        }
        r->chave[i + 1] = k; // Insere a nova chave na posição correta
        r->n++;              // Incrementa o número de chaves no nó
    }
    else
    {
        while (i >= 0 && r->chave[i] > k) // Percorre as chaves do nó
        {
            i--; // Decrementa o índice
        }
        i++; // Incrementa o índice para a posição correta
        if (r->filho[i]->n == 2 * A->t - 1)
        {
            if (r->filho[i]->folha) // Verifica se o filho é uma folha
            {
                divFilho(A, r, i, r->filho[i]); // Divide a folha
            }
            else
            {
                divInterna(A, r, i, r->filho[i]); // Divide o nó interno
            }
            if (r->chave[i] < k) // Verifica se a nova chave é maior que a chave de separação
            {
                i++; // Incrementa o indice para a posição correta
            }
        }
        insereNotFull(A, r->filho[i], k); // Chama a função de forma recursiva para inserir a chave no filho correto
    }
}

void insereArvore(arv *A, int k) // Função para inserir um elemento na árvore B+
{
    no *r = A->rz;            // Obtém a raiz da árvore
    if (r->n == 2 * A->t - 1) // Verifica se a raiz está cheia
    {
        no *new = createNo(A->t, false); // Cria um novo nó interno
        A->rz = new;                     // Define o novo nó como a nova raiz
        new->filho[0] = r;               // Define a antiga raiz como o primeiro filho do novo nó
        if (r->folha)                    // Verifica se a antiga raiz é uma folha
        {
            divFilho(A, new, 0, r); // Divide a folha
        }
        else
        {
            divInterna(A, new, 0, r); // Divide o nó interno
        }
        A->n++;                   // incrementa o número de nós na árvore
        insereNotFull(A, new, k); // Insere a nova chave no novo nó
    }
    else
    {
        insereNotFull(A, r, k); // Insere a nova chave na raiz
    }
}

void printArvore(no *r, int nl) // Função para imprimir a árvore B+
{
    
    if (r == NULL)// Se o nó é nulo, não há nada a fazer.
    {
        return;
    }

    
    if (r->folha) // Se for um nó folha, imprime suas chaves com o prefixo de folha.
    {
        
        for (int i = 0; i < nl; i++) // Imprime a indentação com espaços, baseada no nível.
        {
            printf("  "); // Adiciona dois espaços para cada nível de profundidade
        }

        
        printf("|-- "); // Imprime o prefixo de nó folha e as chaves.
        for (int i = 0; i < r->n; i++)
        {
            printf("%d ", r->chave[i]);
        }
        printf("\n");
    }
    else // Se for um nó interno, faz o percurso em ordem.
    {
        for (int i = 0; i < r->n; i++)
        {
           
            printArvore(r->filho[i], nl + 1);  // Chamada recursiva para o filho à esquerda da chave.

            
            
            for (int j = 0; j < nl; j++) // Imprime a indentação com espaços.
            {
                printf("  ");
            }

           
            if (nl > 0)  // Imprime o prefixo de nó interno ou raiz.
            {
                printf("|-"); // Para nós internos.
            }
            else
            {
                printf("| "); // Para a chave da raiz.
            }
            printf("%d\n", r->chave[i]);
        }
        
        printArvore(r->filho[r->n], nl + 1); // Chamada recursiva para o último filho.
    }
}

void freeNo(no *r, bool f) // Função para liberar a memória de um nó
{
    if (r == NULL) // Verifica se o nó é nulo
    {
        return; // Retorna se o nó for nulo
    }
    if (!r->folha) // Verifica se o nó não é uma folha
    {
        for (int i = 0; i <= r->n; i++)
        {
            freeNo(r->filho[i], r->filho[i]->folha); // Chama a função recursivamente para liberar os filhos
        }
    }
    free(r->chave); // Libera a memória das chaves do nó
    free(r->filho); // Libera a memória dos filhos do nó
    free(r);        // Libera a memória do nó
}

void freeArvore(arv *r) // Função para liberar a memória da árvore B+
{
    if (r == NULL) // Verifica se o nó é nulo
    {
        return; // Retorna se o nó for nulo
    }
    freeNo(r->rz, r->rz->folha); // Chama a função para liberar a memória do nó raiz
    free(r);                     // Libera a memória da árvore
}

int main()
{

    int num = 0, ch;

    FILE *fp = fopen("/home/fels/Documentos/POD/TrabalhoF/dados.txt", "r"); // Abre o arquivo para leitura
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Retorna erro se não conseguir abrir o arquivo
    }
    else
    {
        printf("Arquivo aberto com sucesso. \n");
    }

    arv *arvore = createArvore(degree); // Cria uma nova árvore B+ com o grau definido

    while ((ch = fgetc(fp)) != EOF) // Le os caracteres do arquivo até o final
    {
        if (ch >= '0' && ch <= '9') // Verifica se o caractere é um digito
        {
            num = num * 10 + (ch - '0'); // Converte o caractere para inteiro
        }
        else if (ch == ';')
        {
            insereArvore(arvore, num); // Insere o número na árvore B+
            num = 0;                   // Reseta o número para o próximo dígito
        }
    }
    if (num != 0) // Verifica se o último número foi lido
    {
        insereArvore(arvore, num); // Insere o último número na árvore B+
    }

    fclose(fp); // Fecha o arquivo
    printf("Dados inseridos na árvore B+ com sucesso.\n");
    printArvore(arvore->rz, 0); // Imprime a árvore B+
    printf("\n");
    freeArvore(arvore); // Libera a memória da árvore B+
}
