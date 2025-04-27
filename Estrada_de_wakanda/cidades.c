#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

static int cmpCidade(const void *a, const void *b) {
    const Cidade *ca = a;
    const Cidade *cb = b;
    return ca->Posicao - cb->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return NULL;

    int T, N;
    if (fscanf(fp, "%d", &T) != 1 || T < 3 || T > 1000000) {
        fclose(fp);
        return NULL;
    }
    if (fscanf(fp, "%d", &N) != 1 || N < 2 || N > 10000) {
        fclose(fp);
        return NULL;
    }

    Estrada *e = malloc(sizeof(Estrada));
    if (!e) { fclose(fp); return NULL; }
    e->T = T;
    e->N = N;
    e->C = malloc(sizeof(Cidade) * N);
    if (!e->C) { free(e); fclose(fp); return NULL; }

    /* descarta o restante da segunda linha */
    char line[1024];
    fgets(line, sizeof(line), fp);

    for (int i = 0; i < N; i++) {
        if (!fgets(line, sizeof(line), fp)) {
            free(e->C);
            free(e);
            fclose(fp);
            return NULL;
        }

        /* lê posição */
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        char *endptr;
        long pos = strtol(p, &endptr, 10);
        if (endptr == p || pos <= 0 || pos >= T) {
            free(e->C);
            free(e);
            fclose(fp);
            return NULL;
        }

        /* lê nome da cidade (resto da linha) */
        p = endptr;
        while (*p == ' ' || *p == '\t') p++;
        char *q = p + strlen(p) - 1;
        while (q >= p && (*q == '\n' || *q == '\r')) *q-- = '\0';

        e->C[i].Posicao = (int)pos;
        strncpy(e->C[i].Nome, p, sizeof(e->C[i].Nome)-1);
        e->C[i].Nome[sizeof(e->C[i].Nome)-1] = '\0';
    }

    fclose(fp);

    /* ordena por posição e verifica duplicatas */
    qsort(e->C, e->N, sizeof(Cidade), cmpCidade);
    for (int i = 1; i < e->N; i++) {
        if (e->C[i].Posicao == e->C[i-1].Posicao) {
            free(e->C);
            free(e);
            return NULL;
        }
    }

    return e;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return 0.0;

    int N = e->N;
    double minViz;
    /* cidade 0 */
    double left = 0.0;
    double right = (e->C[0].Posicao + e->C[1].Posicao) / 2.0;
    minViz = right - left;

    /* cidades intermediárias */
    for (int i = 1; i < N-1; i++) {
        left  = (e->C[i-1].Posicao + e->C[i].Posicao) / 2.0;
        right = (e->C[i].Posicao   + e->C[i+1].Posicao) / 2.0;
        double viz = right - left;
        if (viz < minViz) minViz = viz;
    }

    /* última cidade */
    left  = (e->C[N-2].Posicao + e->C[N-1].Posicao) / 2.0;
    right = (double)e->T;
    {
        double viz = right - left;
        if (viz < minViz) minViz = viz;
    }

    free(e->C);
    free(e);
    return minViz;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    int N = e->N;
    double minViz;
    int idxMin = 0;

    /* cidade 0 */
    double left = 0.0;
    double right = (e->C[0].Posicao + e->C[1].Posicao) / 2.0;
    minViz = right - left;

    /* cidades intermediárias */
    for (int i = 1; i < N-1; i++) {
        left  = (e->C[i-1].Posicao + e->C[i].Posicao) / 2.0;
        right = (e->C[i].Posicao   + e->C[i+1].Posicao) / 2.0;
        double viz = right - left;
        if (viz < minViz) {
            minViz = viz;
            idxMin = i;
        }
    }

    /* última cidade */
    left  = (e->C[N-2].Posicao + e->C[N-1].Posicao) / 2.0;
    right = (double)e->T;
    {
        double viz = right - left;
        if (viz < minViz) {
            minViz = viz;
            idxMin = N-1;
        }
    }

    /* retorna o nome */
    char *res = malloc(strlen(e->C[idxMin].Nome) + 1);
    if (res) strcpy(res, e->C[idxMin].Nome);

    free(e->C);
    free(e);
    return res;
}