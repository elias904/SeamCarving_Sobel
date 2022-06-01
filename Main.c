#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Dinamica.h"
#include "Matriz.h"
#include "Lista.h"

typedef struct
{

    int R, G, B;

} Pixel;

void main()
{

    float pixel_x;
    float pixel_y;

    int Linhas = 3;
    int Colunas = 3;

    Pixel Matriz[3][3];
    // Primeira Linha
    Matriz[0][0].R = 255, Matriz[0][0].G = 0, Matriz[0][0].B = 0;     // Elemento [0,0]
    Matriz[0][1].R = 0, Matriz[0][1].G = 0, Matriz[0][1].B = 0;       // Elemento [0,1]
    Matriz[0][2].R = 255, Matriz[0][2].G = 255, Matriz[0][2].B = 255; // Elemento [0,2]

    // Segunda Linha
    Matriz[1][0].R = 0, Matriz[1][0].G = 0, Matriz[1][0].B = 0;   // Elemento [1,0]
    Matriz[1][1].R = 0, Matriz[1][1].G = 255, Matriz[1][1].B = 0; // Elemento [1,1]
    Matriz[1][2].R = 0, Matriz[1][2].G = 0, Matriz[1][2].B = 0;   // Elemento [1,2]

    // Terceira Linha
    Matriz[2][0].R = 128, Matriz[2][0].G = 128, Matriz[2][0].B = 128; // Elemento [2,0]
    Matriz[2][1].R = 0, Matriz[2][1].G = 0, Matriz[2][1].B = 0;       // Elemento [2,1]
    Matriz[2][2].R = 0, Matriz[2][2].G = 0, Matriz[2][2].B = 255;     // Elemento [2,2]

    // Cria a matriz de intensidade luminosa
    float *MatrizIL = CriaMatriz(Linhas, Colunas);

    // Coloca os valores dentro da matriz de intensidade luminosa
    for (int i = 0; i < Colunas; i++)
    {
        for (int j = 0; j < Linhas; j++)
        {

            float Valor = 0.3 * Matriz[i][j].R + 0.59 * Matriz[i][j].G + 0.11 * Matriz[i][j].B;
            MatrizIL[i * Linhas + j] = Valor;
        }
    }

    /*Cria matriz extendida para poder aplicar o filtro de Sobel corretamente. Ou seja,
   extendemos a Matriz de Intensidade Luminosa
   */
    int LinhasExtend = Linhas + 2;
    int ColunasExtend = Colunas + 2;
    float *MatrizExtendida = CriaMatriz(LinhasExtend, ColunasExtend);

    int indice = 0;

    // Preenchendo a MatrizExtend com a MatrizDeIntensidadeLuminosa
    for (int x = 1; x < ColunasExtend - 1; x++)
    {
        for (int y = 1; y < LinhasExtend - 1; y++)
        {
            MatrizExtendida[x * LinhasExtend + y] = MatrizIL[indice];
            indice++;
        }
    }

    //Liberando memoria da Matriz de Intensidade Luminosa
    free(MatrizIL);

    /////////////////////////////////////////////////////////////////////////////////////////
    // Duplicando os valores de linhas e colunas.

    // Fazendo a Linha 0 duplicar os elementos da linha 1(Excluindo as kinas da esquerda, superiro e inferiors);
    int x = 0;
    while (x == 0)
    {
        for (int y = 1; y < LinhasExtend - 1; y++)
        {
            MatrizExtendida[x * ColunasExtend + y] = MatrizExtendida[1 * ColunasExtend + y];
        }
        x++;
    }
    // Fazendo a linha da esquerda duplicar(Excluindo as Kinas da esquerda, superior e inferior)
    for (int x = 1; x < ColunasExtend - 1; x++)
    {
        for (int y = 0; y < 1; y++)
        {
            MatrizExtendida[x * LinhasExtend + y] = MatrizExtendida[x * LinhasExtend + 1];
        }
    }

    // Duplica a ultima linha
    for (int j = 1; j < LinhasExtend - 1; j++)
    {
        MatrizExtendida[(ColunasExtend - 1) * ColunasExtend + j] = MatrizExtendida[(ColunasExtend - 2) * ColunasExtend + j];
    }

    // Duplica a linha na direita
    for (int x = 0; x < ColunasExtend - 2; x++)
    {
        for (int y = LinhasExtend; y < LinhasExtend + 1; y++)
        {
            MatrizExtendida[x * ColunasExtend + LinhasExtend + y - 1] = MatrizExtendida[x * ColunasExtend + LinhasExtend + y - 2];
        }
    }

    // Resolvendo o Problema das 4 quinas;
    MatrizExtendida[0] = MatrizExtendida[1 * ColunasExtend + 1]; // Superior(Esquerda)

    MatrizExtendida[ColunasExtend - 1] = MatrizExtendida[1 * ColunasExtend + ColunasExtend - 2]; // Superior(Direita)

    MatrizExtendida[LinhasExtend * (ColunasExtend - 1)] = MatrizExtendida[(ColunasExtend - 2) * ColunasExtend + 1]; // Inferior Esquerda

    MatrizExtendida[(LinhasExtend * ColunasExtend) - 1] = MatrizExtendida[(ColunasExtend - 2) * ColunasExtend + LinhasExtend - 1]; // Inferior Direita

    /////////////////////////////////////////////////////////////////////////////////////////

    // Criamos a matriz para receber os valores do gradiente.
    float *MatrizWPesos = CriaMatriz(Linhas, Colunas);

    float sobel_x[3][3] =
        {{-1, 0, 1},
         {-2, 0, 2},
         {-1, 0, 1}};

    float sobel_y[3][3] =
        {{-1, -2, -1},
         {0, 0, 0},
         {1, 2, 1}};

    int idx = 0;

    for (int x = 1; x < ColunasExtend - 1; x++)
    {
        for (int y = 1; y < LinhasExtend - 1; y++)
        {

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    pixel_x += sobel_x[i][j] * MatrizExtendida[ColunasExtend * (y - 1 + i) + (x - 1 + j)];
                    pixel_y += sobel_y[i][j] * MatrizExtendida[ColunasExtend * (y - 1 + i) + (x - 1 + j)];
                }
            }
            float e = (float)sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y));
            // printf("\nValor e = %.2f\n",e);

            MatrizWPesos[idx] = e;
            idx++;
        }
    }

    //Liberando o espaço da Matriz de Intensidade Luminosa Extendida
    free(MatrizExtendida);

    // ImprimeMatriz(MatrizWPesos, Linhas, Colunas);
    float *MatrizDinamica = CriaMatriz(Linhas, Colunas);

    // Parei aqui, agora so falta achar o menor caminho e depois fazer funções retira linhas e retira coluna

    // Produzindo a MatrizDinamica, pesos devidamente somados ao longo das colunas para poder encontrar o melhor caminho
    for (int x = 0; x < Colunas; x++)
    {
        for (int y = 0; y < Linhas; y++)
        {
            if (x * Linhas == 0)
            {
                // Copiando a primeira linha da MatrizWPesos para a matriz Dinamica.
                MatrizDinamica[x * Linhas + y] = MatrizWPesos[x * Linhas + y];
            }
            else
            {
                if (y == 0) // Borda Esquerda
                {

                    float A = 0;
                    A = MatrizWPesos[(x - 1) * Linhas + y];
                    float B = 0;
                    B = MatrizWPesos[(x - 1) * Linhas + y + 1];
                    MatrizDinamica[x * Linhas + y] = MatrizWPesos[x * Linhas + y] + ValorDinamicoBorda(A, B);

                    // printf("\nIndice: %d", x * Linhas + y);
                    // printf("\nCaminhos Anteriores: %d, %d\n", (x - 1) * Linhas + y, (x - 1) * Linhas + y + 1);
                }
                else if (y == Linhas - 1) // Borda Direita
                {
                    float C = 0;
                    C = MatrizWPesos[(x - 1) * Linhas + (Colunas - 1)];
                    float D = 0;
                    D = MatrizWPesos[(x - 1) * Linhas + (Colunas - 2)];
                    MatrizDinamica[x * Linhas + (Colunas - 1)] = MatrizWPesos[x * Linhas + (Colunas - 1)] + ValorDinamicoBorda(C, D);

                    // printf("\nIndice: %d", x * Linhas + (Colunas - 1));
                    // printf("\nCaminhos Anteriores: %d, %d\n", (x - 1) * Linhas + (Colunas - 1), (x - 1) * Linhas + (Colunas - 2));
                }
                else // Meio
                {

                    float E = 0;
                    E = MatrizWPesos[(x - 1) * Linhas + (y - 1)];
                    float F = 0;
                    F = MatrizWPesos[(x - 1) * Linhas + y];
                    float G = 0;
                    G = MatrizWPesos[(x - 1) * Linhas + (y + 1)];

                    MatrizDinamica[x * Linhas + y] = MatrizWPesos[x * Linhas + y] + ValorDinamicoMid(E, F, G);

                    // printf("\nIndice: %d", x * Linhas + y);
                    // printf("\nCaminhos Anteriores: %d, %d e %d\n", (x - 1) * Linhas + (y - 1), (x - 1) * Linhas + y, (x - 1) * Linhas + (y + 1));
                }
            }
        }
    }

    //Liberando a memoria da Matriz com as "Importâncias" de cada pixel
    free(MatrizWPesos);

    

    printf("\n-------------------------------\n");
    printf("\nImagem 3 x 3\n");
    ImprimeMatriz(MatrizDinamica, Linhas, Colunas);
    printf("\n-------------------------------\n");

    int k = 0;
    float * List = malloc(Linhas * sizeof(float));
    int y = 0;
    while (k == 0)
    {

        for (y = 0; y < Colunas; y++)
        {
            List[y] = MatrizDinamica[y];
        } 

        k++;
    }
    int PosMenorNmr = PosicaoMenor(List,Colunas);
    int *Lista = (int *)malloc(Linhas * sizeof(int));
    PosicaoDosMenoresCaminhos(MatrizDinamica, PosMenorNmr, Linhas, Colunas, Lista);
    //ImprimeListaIndices(Lista,Linhas);
    //Remove menor caminho da dado pela Lista
    RemoveCaminho(MatrizDinamica,Lista,Linhas,Colunas);

    //Transpoe Matriz
    //MatrizTransposta(MatrizDinamica,Linhas,Colunas);
}