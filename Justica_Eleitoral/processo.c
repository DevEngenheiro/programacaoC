#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

// Função para remover os caracteres especiais (como {, }) de um campo, se necessário.
void limparCampo(char *campo) {
    char temp[50];
    int j = 0;
    for (int i = 0; campo[i] != '\0'; i++) {
        if (campo[i] != '{' && campo[i] != '}')
            temp[j++] = campo[i];
    }
    temp[j] = '\0';
    strcpy(campo, temp);
}

// Função para ler o arquivo CSV
int lerProcessos(const char *nomeArquivo, Processo processos[], int max) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return 0;
    }
    
    char linha[256];
    int count = 0;
    
    // Ignorar o cabeçalho
    fgets(linha, sizeof(linha), fp);
    
    while (fgets(linha, sizeof(linha), fp) && count < max) {
        // Remover a quebra de linha
        linha[strcspn(linha, "\n")] = '\0';
        
        // Exemplo de linha:
        // 638633058,"00000103020166070018",2016-04-20 15:03:40.000,{12554},{11778},0
        char *token = strtok(linha, ",");
        if (token) processos[count].id = atol(token);
        
        token = strtok(NULL, ",");
        if (token) {
            // Remove as aspas, se presentes
            token[strcspn(token, "\"")] = '\0';
            strcpy(processos[count].numero, token);
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strcpy(processos[count].data_ajuizamento, token);
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strcpy(processos[count].id_classe, token);
            limparCampo(processos[count].id_classe);
        }
        
        token = strtok(NULL, ",");
        if (token) {
            strcpy(processos[count].id_assunto, token);
            limparCampo(processos[count].id_assunto);
        }
        
        token = strtok(NULL, ",");
        if (token) {
            processos[count].ano_eleicao = atoi(token);
        }
        
        count++;
    }
    
    fclose(fp);
    return count;
}

// Função de ordenação por id (crescente) usando bubble sort simples
void ordenarPorId(Processo processos[], int n) {
    Processo temp;
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (processos[j].id > processos[j+1].id) {
                temp = processos[j];
                processos[j] = processos[j+1];
                processos[j+1] = temp;
            }
        }
    }
}

// Função de ordenação por data_ajuizamento (decrescente)
// Aqui, para simplificar, utilizamos strcmp, assumindo que as datas estão no formato "YYYY-MM-DD HH:MM:SS.xxx"
void ordenarPorData(Processo processos[], int n) {
    Processo temp;
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            // Para ordem decrescente, se processos[j] é menor que processos[j+1], troca
            if (strcmp(processos[j].data_ajuizamento, processos[j+1].data_ajuizamento) < 0) {
                temp = processos[j];
                processos[j] = processos[j+1];
                processos[j+1] = temp;
            }
        }
    }
}

// Conta quantos processos possuem um id_classe específico
int contarPorIdClasse(Processo processos[], int n, const char *idClasse) {
    int count = 0;
    for (int i = 0; i < n; i++){
        if (strcmp(processos[i].id_classe, idClasse) == 0) {
            count++;
        }
    }
    return count;
}

// Conta quantos id_assuntos únicos existem nos processos
int contarIdAssuntosUnicos(Processo processos[], int n) {
    char assuntos[100][20]; // Supondo no máximo 100 assuntos diferentes
    int total = 0;
    
    for (int i = 0; i < n; i++){
        int existe = 0;
        for (int j = 0; j < total; j++){
            if (strcmp(processos[i].id_assunto, assuntos[j]) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            strcpy(assuntos[total], processos[i].id_assunto);
            total++;
        }
    }
    return total;
}

// Lista os processos que possuem mais de um id_assunto (aqui, se o campo tiver vírgula)
void listarProcessosMultiAssunto(Processo processos[], int n) {
    for (int i = 0; i < n; i++){
        if (strchr(processos[i].id_assunto, ',') != NULL) {
            printf("Processo %ld, numero: %s, possui múltiplos assuntos: %s\n",
                   processos[i].id, processos[i].numero, processos[i].id_assunto);
        }
    }
}

// Função que simula o cálculo dos dias em tramitação para um processo
// Para simplificar, vamos assumir que a função converte a data_ajuizamento para um número de dias
// e retorna a diferença até uma data fixa (por exemplo, 04/04/2025), mas aqui usaremos um valor fixo.
int calcularDiasTramitacao(const char *dataAjuizamento) {
    // Em um caso real, converteríamos a string para uma struct tm, converter para time_t e calcularíamos a diferença.
    // Para simplificar, vamos retornar um valor arbitrário, por exemplo, 100 dias.
    return 100;
}
// Função auxiliar para limpar o campo de id_assunto