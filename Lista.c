#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"

int PosicaoMenor(float *Lista, int Colunas)
{
    // Retorna a posição do menor termo da linha passada;
    int x = 0;
    float Menor = Lista[0];

    for (x = 1; x < Colunas; x++)
    {
        if (Lista[x] < Menor)
            Menor = Lista[x];
    }
    return RetornaPosicao(Menor, Lista);
}

//Retorna a posição de um elemento dentro de uma lista de floats
int RetornaPosicao(float P, float *Lista)
{
    int i = 0;
    while (Lista[i] != P)
        i++;

    return i;
}

//Imprime uma lista
void ImprimeListaIndices(int *List, int Tamanho)
{
    int i = 0;
    for (int i = 0; i < Tamanho; i++)
        printf("[%d]", List[i]);
    printf("\n");
}

int Achou(int Busca, int *Lista, int Tamanho)
{
    int x = 0;
    int Achou = 0;
    while(Tamanho)
    {
        if(Lista[x] == Busca )
        {
            Achou = 1;
            break;
        }
        x++;
        Tamanho--;
    }
    return Achou;
}