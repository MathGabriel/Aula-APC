#include <stdio.h>

void levetor(int vetor[],int n){
	int i;
	for (i = 0; i < n; i++){
		scanf("%d",&vetor[i]);
	}
}

void BubbleSort(int vetor[],int tamanho){
	int i,ii,aux;
	for (i = 0; i < tamanho; i++){
		for (ii = 0; ii < tamanho-1 ; ii++){
			if (vetor[ii]>vetor[ii+1]){
				aux = vetor[ii];
				vetor[ii] = vetor[ii+1];
				vetor[ii+1] = aux;
			}
		}
	}
}

char buscaBinaria(int vetor[],int n,int Q){
	int inicio=0,fim=n-1;
	int meio;

	while( inicio <= fim ){
		meio = (inicio+fim)/2;
		if (vetor[meio] == Q){
			return 's';
		}
		else if (Q > vetor[meio]){
			inicio = meio + 1;
		}
		else if (Q < vetor[meio]){
			fim = meio - 1;
		}
	}
	return 'n';
}

int main(){
	int n,q,i,Q;
	char resbuscabi;

	scanf("%d %d",&n,&q);

	int vetor[n];
	levetor(vetor,n);
	BubbleSort(vetor,n);
	for (i = 0; i < q; i++){
		scanf("%d",&Q);
		resbuscabi = buscaBinaria(vetor,n,Q);
		printf("%c\n", resbuscabi);
	}
	return 0;
}