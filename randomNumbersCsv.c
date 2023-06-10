#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void criarArquivoCSV(const char* nomeArquivo, int quantidadeRegistros) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.");
        return;
    }

    srand(time(NULL)); // Inicializa a semente para geração de números aleatórios


    for (int i = 0; i < quantidadeRegistros; i++) {
        int campo1 = rand() % 91 + 10; // Gera um número aleatório entre 10 e 100
        int campo2 = rand() % 1001 + 500; // Gera um número aleatório entre 500 e 1500

        fprintf(arquivo, "%d,%d\n", campo1, campo2);
    }

    fclose(arquivo);
    printf("Arquivo CSV criado com sucesso.\n");
}

int main() {
    const char* nomeArquivo = "dados.csv";
    int quantidadeRegistros = 10;

    criarArquivoCSV(nomeArquivo, quantidadeRegistros);

    return 0;
}
