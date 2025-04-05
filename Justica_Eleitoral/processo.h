#ifndef PROCESSO_H
#define PROCESSO_H

// Definição do TAD Processo
// Observação: Alguns campos são armazenados como strings e outros como inteiros.
// Para os atributos multivalorados (id_classe e id_assunto) você pode utilizar strings
// e, se necessário, implementar funções para extrair os números.
typedef struct {
    long id;                   // Ex: 638633058
    char numero[30];           // Ex: "00000103020166070018"
    char data_ajuizamento[25]; // Ex: "2016-04-20 15:03:40.000"
    char id_classe[20];        // Ex: "{12554}"
    char id_assunto[20];       // Ex: "{11778}"
    int ano_eleicao;           // Ex: 0 ou 2020
} Processo;

// Funções do TAD Processo

// Lê os dados do arquivo CSV e armazena os processos em um vetor (retorna a quantidade lida)
int lerProcessos(const char *nomeArquivo, Processo processos[], int max);

// Ordena os processos em ordem crescente pelo atributo id
void ordenarPorId(Processo processos[], int n);

// Ordena os processos em ordem decrescente pela data_ajuizamento (você pode usar strcmp se armazenar datas como string)
void ordenarPorData(Processo processos[], int n);

// Conta quantos processos estão vinculados a um determinado id_classe
int contarPorIdClasse(Processo processos[], int n, const char *idClasse);

// Identifica quantos id_assunto únicos existem (pode-se usar um vetor auxiliar de strings ou outra abordagem simples)
int contarIdAssuntosUnicos(Processo processos[], int n);

// Lista os processos que possuem mais de um id_assunto (para simplificar, supomos que se o campo contém uma vírgula, é multi-assunto)
void listarProcessosMultiAssunto(Processo processos[], int n);

// Calcula quantos dias um processo está em tramitação (para simplificar, você pode converter a data_ajuizamento para um número ou usar uma função que calcule a diferença de dias)
int calcularDiasTramitacao(const char *dataAjuizamento);

#endif
