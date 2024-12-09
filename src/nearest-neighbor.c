#include <stdio.h>
#include <math.h>
#include "nearest-neighbor.h"

int calculaTamanhoVetor(int sequencia, int tamanhoVetorValores){
  return (tamanhoVetorValores - sequencia) + 1;
}

double calculaRaiz(double *vetor, int i, int j, int sequencia, int debug){
	double valorSoma = 0;
	double raiz = 0;
	int l;


	#pragma omp parallel private(l) shared(i,j,sequencia,vetor)
	{
		#pragma omp for
		for (int l = 0; l < sequencia; l++) {
	  		if (debug == 1)
	  			printf("Valor I %.2f valor J %.2f\n", vetor[l], vetor[j]);
	  		
	  		valorSoma = valorSoma + pow((vetor[i + l] - vetor[j +l]),2);
		}
	}
	
	raiz = sqrt(valorSoma);


  	if (debug)
  		printf("%.1f\n", raiz);
  	
  	return raiz;
}

void extraiVizinhoMaisProximo(
	int tamanho,
	double **vetorResultadoValoresDistancia,
	float *vetorVizinhoMaisProximo,
	int *vetorIndicesDistancia,
	int debug
){
	int i, j;
	#pragma omp parallel private(i,j) shared(vetorResultadoValoresDistancia, vetorVizinhoMaisProximo,vetorIndicesDistancia)
	{
		#pragma omp for
		for (int i = 0; i  < tamanho; i++) {
  			float valorMaisProximo = vetorResultadoValoresDistancia[i][0];
   	    	int indice = 0;		
        
			for(int j = 1; j  < tamanho; j++) {
				if(valorMaisProximo < vetorResultadoValoresDistancia[i][j]){
			    	if (valorMaisProximo == 0){
			    		if (vetorResultadoValoresDistancia[i][j] != 0) {
			        		valorMaisProximo = vetorResultadoValoresDistancia[i][j];
			       			indice = j + 1;
			      		}
			    	}
			  	} else {
			    	if (vetorResultadoValoresDistancia[i][j] != 0) {
			       		valorMaisProximo = vetorResultadoValoresDistancia[i][j];
			       		indice = j + 1;
			    	}
			  	}
			}
       
        	vetorVizinhoMaisProximo[i] = valorMaisProximo;
        	vetorIndicesDistancia[i] = indice;
       
       		if (debug == 1) {
        		printf("Valor raiz vizinho mais proximo: %.1f\n", vetorVizinhoMaisProximo[i]);
        		printf("Valor indice vizinho mais proximo:  %d\n", vetorIndicesDistancia[i]);	
       		}
    	}
  	}
}
