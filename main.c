#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define degree 4 // Definindo o grau da árvore B+

typedef struct no // Definição da estrutura de um nó
{
    int *chv;        // Chave do nó
    int v;           // Valor do nó
    struct no **fh;  // Ponteiro para os filhos do nó
    int n;           // Número de filhos
    bool f;          // Flag para indicar se o nó é folha
    struct no *prox; // Ponteiro para o próximo nó
} no;

typedef struct arvore // Definição da estrutura da árvore B+
{
    no *rz; // Ponteiro para a raiz da árvore
    int n;  // Número de nós na árvore
} arv;

no *createNo(int v, bool f) // função para criar um novo nó
{
    no *newNo = (no *)malloc(sizeof(no)); // Aloca memória para o novo nó
    if (newNo == NULL)
    {
        printf("Erro ao alocar memória para o nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória
    }
    newNo->v = v;                                          // Atribui o valor ao nó
    newNo->f = f;                                          // Define se o nó é folha
    newNo->chv = (int *)malloc((2 * v - 1) * sizeof(int)); // Aloca memória para as chaves do nó
    if (newNo->chv == NULL)
    {
        printf("Erro ao alocar memória para as chaves do nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória
    }
    newNo->fh = (no **)malloc((2 * v) * sizeof(no *)); // Aloca memória para os filhos do nó
    if (newNo->fh == NULL)
    {
        printf("Erro ao alocar memória para os filhos do nó.\n");
        exit(1); // Finaliza o programa se não conseguir alocar memória
    }
    newNo->n = 0;       // Inicializa o número de filhos com 0
    newNo->prox = NULL; // Inicializa o ponteiro para o próximo nó como NULL
    return newNo;       // Retorna o novo nó criado
}

arv *createArvore(int n) // função para criar uma nova árvore B+
{
    arv *newArv = (arv *)malloc(sizeof(arv)); // Aloca memória para a nova árvore
    if (newArv == NULL)
    {
        printf("Erro ao alocar memória para a árvore.\n");
        exit(1); // Finaliza o programa caso não consiga alocar memória
    }
    newArv->n = n;                  // Atribui o número de nós à árvore
    newArv->rz = createNo(n, true); // Cria a raiz da árvore como um nó folha
    if (newArv->rz == NULL)
    {
        printf("Erro ao criar a raiz da árvore.\n");
        exit(1); // Finaliza o programa caso não consiga criar a raiz
    }
    return newArv; // retorna a nova árvore criada
}

void dvFilhos(no *pai, int i, no *filho) // Função para dividir os filhos de um nó
{
    int n = filho->n;                  // Obtém o número de filhos do nó filho
    no *newFh = createNo(n, filho->f); // Cria um novo nó para os filhos
    if (newFh == NULL)
    {
        printf("Erro ao criar novo nó para os filhos.\n");
        exit(1); // Finaliza o programa caso não consiga criar o novo nó
    }
    newFh->n = n - 1; // Define o número de filhos do novo nó
    for (int i = 0; i < n; i++)
    {
        newFh->chv[i] = filho->chv[i + 1]; // copia as chaves do nó filho para o novo nó
    }
    if (!filho->f) // se o nó filho não for folha
    {
        for (int i = 0; i < n; i++)
        {
            newFh->fh[i] = filho->fh[i + n]; // copia os filhos do nó para o novo nó
        }
    }

    filho->n = n - 1; // Atualiza o número de filhos do nó filho

    for (int i = pai->n - 1; n >= i; i--)
    {
        pai->fh[i + 1] = pai->fh[i]; // Move as chaves do pai para a direita
    }

    pai->fh[i + 1] = newFh; // Insere o novo nó de filhos no pai

    for (int i = pai->n - 1; n >= i; i--)
    {
        pai->chv[i + 1] = pai->chv[i]; // Move as chaves do pai para a direita
    }

    pai->chv[i] = filho->chv[n - 1]; // Insere a chave do nó filho no pai
    pai->n++;                        // Incrementa o número de filhos do pai
}

void insereNotFull(no *no, int chv)
{
    int i = no->n - 1; // Obtém o índice do último filho
    if (no->f)         // se o nó for folha
    {
        while (i >= 0 && chv < no->chv) // Enquanto o índice for maior ou igual a 0 e a chave for menor que a  chave do nó
        {
            no->chv[i + 1] = no->chv[i]; // Move as chaves para a direita
            i--;                         // Decrementa o índice
        }
        no->chv[i + 1] = chv; // Insere a nova chave na posição correta
        no->n++;              // Incrementa o número de chaves no nó
    }
    else
    {
        while (i >= 0 && no->chv[i] > chv) // Enqaunto o índice for maior ou igual a 0 e a chave for maior que a chave do nó
        {
            i--; // Decrementa o índice
        }
        i++;                               // Incrementa o índice para a posição correta
        if (no->fh[i]->n == 2 * no->v - 1) // Se o filho estiver cheio
        {
            dvFilhos(no, i, no->fh[i]); // Divide os
            if (no->chv[i] < chv)       // Se a chave for menor que a chave do nó
            {
                i++; // Incrementa o índice
            }
        }
        insereNotFull(no->fh[i], chv); // Insere a chave no filho
    }
}

void insereArvore(arv *arv, int chv)
{
    no *rz = arv->rz;            // Obtém a raiz da árvore
    if (rz->n == 2 * arv->n - 1) // Se a raiz estiver cheia
    {
        no *newRz = createNo(arv->n, false); // Cria um novo nó raiz
        if (newRz == NULL)
        {
            printf("Erro ao criar nova raiz.\n");
            exit(1); // Finaliza o programa caso não consiga criar a nova raiz
        }
        newRz->fh[0] = rz;        // Define o filho da nova raiz como a antiga raiz
        dvFilhos(newRz, 0, rz);   // Divide os filhos da nova raiz
        insereArvore(newRz, chv); // Insere a chave na nova raiz
        arv->rz = newRz;          // Atualiza a raiz da árvore
    }
    else
    {
        insereNotFull(rz, chv); // Insere a chave na raiz se ela não estiver cheia
    }
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
}
