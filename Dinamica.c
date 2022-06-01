#include <stdio.h>
#include <string.h>


// Retorna o valor do menor caminho anterior de um pixel na borda da imagem
float ValorDinamicoBorda(float A, float B)
{
    if (A > B)
        return B;
    else
        return A;
}

// Retorna o valor do menor caminho anterior de um pixel no meio da imagem
float ValorDinamicoMid(float A, float B, float C)
{
    float Lista[3] = {A, B, C};
    int Menor = Lista[0];
    for (int x = 0; x < 3; x++)
    {
        if (Lista[x] < Menor)
            Menor = Lista[x];
    }
    return Menor;
}

