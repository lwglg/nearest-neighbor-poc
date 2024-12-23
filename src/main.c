#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nearest-neighbor.h"

void exportaVizinhosMaisProximos(char *linha, int tamanho,
                                 int *vetorIndicesDistancia,
                                 float *vetorVizinhoMaisProximo) {
  FILE *arqout = fopen("resources/vizinho-mais-proximo.csv", "wb");
  char cabecalho[] = "LINHA,TAMANHO\n";

  if (arqout == NULL) {
    printf("\nErro ao abrir o arquivo para escrita!\n");
    exit(1);
  }

  fwrite(cabecalho, strlen(cabecalho), 1, arqout);

  for (int p = 0; p < tamanho; p++) {
    sprintf(linha, "%d,%.15f\n", vetorIndicesDistancia[p],
            vetorVizinhoMaisProximo[p]);
    fwrite(linha, strlen(linha), 1, arqout);
  }

  fclose(arqout);
}

double *realizaLeituraSequencias(char *linha, int tamanhoVetor, int debug) {
  char *result;
  FILE *arqin = fopen("resources/dados-entrada.txt", "rt");

  double *numbers = (double *)malloc(100 * sizeof(double));

  if (arqin == NULL) {
    printf("Erro na abertura do arquivo\n");
    return numbers;
  }

  tamanhoVetor = 0;

  while (!feof(arqin)) {
    // Lê uma linha (com o '\n')
    result = fgets(linha, 100, arqin); // Lê até 99 caracteres ou até o '\n'

    if (result) {
      if (debug)
        printf("%s\n", linha);

      numbers[tamanhoVetor] = atof(linha);
    }

    tamanhoVetor++;

    if ((tamanhoVetor % 100) == 0)
      numbers = (double *)realloc(numbers, ((tamanhoVetor / 100) * 100 + 100) *
                                               sizeof(double));

    numbers[tamanhoVetor] = atof(linha);
  }

  return numbers;
}

double **calculaValoresDistancia(int tamanho, int sequencia, double *numbers,
                                 int debug) {
  int k;

  // Matriz que armazena o cálculo das raízes com base na comparação com
  // vetorVizinhoMaisProximo
  double **vetorResultadoValoresDistancia = malloc(tamanho * sizeof(double *));

#pragma omp parallel private(k)
  {
#pragma omp for
    for (k = 0.; k < tamanho; k++) {
      vetorResultadoValoresDistancia[k] = malloc(tamanho * sizeof(double));
    }
  }

  int t, u;
#pragma omp parallel private(t, u) shared(numbers, sequencia)
  {
#pragma omp for
    for (t = 0; t < tamanho; t++) {
      for (u = 0; u < tamanho; u++) {
        vetorResultadoValoresDistancia[t][u] =
            calculaRaiz(numbers, t, u, sequencia, debug);

        if (debug)
          printf("%.1f\n", vetorResultadoValoresDistancia[t][u]);
      }
    }
  }

  return vetorResultadoValoresDistancia;
}

void main(int argc, char *argv[]) {
  FILE *arqout;
  char *result;
  char linha[100];
  int tamanhoVetor, sequencia;
  int debug = 1;

  printf("Informe a sequencia: ");
  scanf("%d", &sequencia);
  printf("----------------------------------------------------------\n ");

  double *numbers;
  numbers = realizaLeituraSequencias(linha, tamanhoVetor, debug);

  tamanhoVetor = 0;
  int tamanho = calculaTamanhoVetor(
      sequencia, tamanhoVetor); // Calcula o tamanho do vetor com base na
                                // sequência informada
  int vetorResultadosParaSoma[sequencia]; // Vetor que receberá as somas das
                                          // raízes
  float *vetorVizinhoMaisProximo = malloc(
      tamanho *
      sizeof(
          float *)); // Vetor que armazenará as raízes do vizinho mais próximo

  // Calcula raízes e popula vetorResultadoValoresDistancia
  double **vetorResultadoValoresDistancia;
  vetorResultadoValoresDistancia =
      calculaValoresDistancia(tamanho, sequencia, numbers, debug);

  // Calcula vizinho mais próximo, extraindo o seu índice
  int *vetorIndicesDistancia;
  vetorIndicesDistancia = extraiVizinhoMaisProximo(
      tamanho, vetorResultadoValoresDistancia, vetorVizinhoMaisProximo, debug);

  // Exportação dos NN em um arquivo CSV
  exportaVizinhosMaisProximos(linha, tamanho, vetorIndicesDistancia,
                              vetorVizinhoMaisProximo);
}
