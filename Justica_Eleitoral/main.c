#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

#define MAX_PROCESSOS 20000

int main() {
    Processo processos[MAX_PROCESSOS];
    int n = lerProcessos("processo_043_202409032338.csv", processos, MAX_PROCESSOS);
    if (n == 0) {
        printf("Nenhum processo foi lido.\n");
        return 1;
    }
    
    // 1. Ordenar os processos em ordem crescente pelo atributo "id"
    ordenarPorId(processos, n);
    // Aqui, você pode gravar os dados ordenados em um arquivo CSV (não mostrado neste exemplo).

    // 2. Ordenar os processos em ordem decrescente por "data_ajuizamento"
    ordenarPorData(processos, n);
    // Da mesma forma, os dados podem ser salvos em um CSV.

    // 3. Contar quantos processos estão vinculados a um determinado "id_classe"
    char idClasseDesejado[20] = "12377"; // Exemplo
    int contClasse = contarPorIdClasse(processos, n, idClasseDesejado);
    printf("Quantidade de processos com id_classe %s: %d\n", idClasseDesejado, contClasse);

    // 4. Contar quantos id_assuntos únicos existem na base
    int contAssuntos = contarIdAssuntosUnicos(processos, n);
    printf("Quantidade de id_assuntos unicos: %d\n", contAssuntos);

    // 5. Listar todos os processos vinculados a mais de um assunto
    listarProcessosMultiAssunto(processos, n);

    // 6. Calcular quantos dias um processo está em tramitação
    // Apenas um exemplo para o primeiro processo
    int dias = calcularDiasTramitacao(processos[0].data_ajuizamento);
    printf("Processo %ld está em tramitação há %d dias.\n", processos[0].id, dias);

    return 0;
}