#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int compararCidades(const void *a, const void *b) {
    Cidade *c1 = (Cidade *)a;
    Cidade *c2 = (Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *e = (Estrada *)malloc(sizeof(Estrada));
    if (!e) {
        fclose(arquivo);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &(e->T)) != 1 || e->T < 3 || e->T > 1000000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &(e->N)) != 1 || e->N < 2 || e->N > 10000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    e->C = (Cidade *)malloc(e->N * sizeof(Cidade));
    if (!e->C) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    for (int i = 0; i < e->N; i++) {
        if (fscanf(arquivo, "%d %[^\n]", &(e->C[i].Posicao), e->C[i].Nome) != 2) {
            free(e->C);
            free(e);
            fclose(arquivo);
            return NULL;
        }

        if (e->C[i].Posicao <= 0 || e->C[i].Posicao >= e->T) {
            free(e->C);
            free(e);
            fclose(arquivo);
            return NULL;
        }

        // Vê se a posição já existe
        for (int j = 0; j < i; j++) {
            if (e->C[i].Posicao == e->C[j].Posicao) {
                free(e->C);
                free(e);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return e;
}

void calcularVizinhanças(Estrada *e, double *vizinhas) {
    qsort(e->C, e->N, sizeof(Cidade), compararCidades);

    for (int i = 0; i < e->N; i++) {
        double inicio, fim;
        if (i == 0) {
            inicio = 0;
        } else {
            inicio = (e->C[i].Posicao + e->C[i - 1].Posicao) / 2.0;
        }

        if (i == e->N - 1) {
            fim = e->T;
        } else {
            fim = (e->C[i].Posicao + e->C[i + 1].Posicao) / 2.0;
        }

        vizinhas[i] = fim - inicio;
    }
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1.0;

    double *vizinhas = (double *)malloc(sizeof(double) * e->N);
    if (!vizinhas) {
        free(e->C);
        free(e);
        return -1.0;
    }

    calcularVizinhanças(e, vizinhas);

    double menor = vizinhas[0];
    for (int i = 1; i < e->N; i++) {
        if (vizinhas[i] < menor) {
            menor = vizinhas[i];
        }
    }

    free(vizinhas);
    free(e->C);
    free(e);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    double *vizinhas = (double *)malloc(sizeof(double) * e->N);
    if (!vizinhas) {
        free(e->C);
        free(e);
        return NULL;
    }

    calcularVizinhanças(e, vizinhas);

    double menor = vizinhas[0];
    int idx = 0;
    for (int i = 1; i < e->N; i++) {
        if (vizinhas[i] < menor) {
            menor = vizinhas[i];
            idx = i;
        }
    }

    char *resultado = (char *)malloc(strlen(e->C[idx].Nome) + 1);
    if (resultado) {
        strcpy(resultado, e->C[idx].Nome);
    }

    free(vizinhas);
    free(e->C);
    free(e);
    return resultado;
}