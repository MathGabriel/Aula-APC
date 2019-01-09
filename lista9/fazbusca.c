#include <stdio.h>
#define MAX 100000

void levetor(long long int vetor[],int tamanho){
	int i;
	for (i = 0; i < tamanho; i++){
		scanf("%lld",&vetor[i]);
	}
}

int verificaprim(long long int vetor[],int tamanho,int numescolhido){
	int i,meio=0,inicio=0,fim=tamanho-1,aux=-1;

	while(inicio<=fim){
		meio = (inicio+fim)/2;
		if (numescolhido < vetor[meio]){
			fim = meio - 1;
		}
		else if (numescolhido > vetor[meio]){
			inicio = meio + 1;
		}
		else{
			fim = meio-1;
			aux = meio;
		}
	}
	return aux;
}

int verificaultm(long long int vetor[],int tamanho,int numescolhido){
	int i,meio=0,inicio=0,fim=tamanho-1,aux=-1;

	while(inicio<=fim){
		meio = (inicio+fim)/2;
		if (numescolhido < vetor[meio]){
			fim = meio - 1;
		}
		else if (numescolhido > vetor[meio]){
			inicio = meio + 1;
		}
		else{
			inicio = meio + 1;
			aux = meio;
		}
	}
	return aux;
}

int main(){
	int n=0,quant=0,j=0,result=0,x=0,result2=0,res=0;
	long long int vetor[MAX];

	scanf("%d %d", &n,&quant);

	levetor(vetor,n);

	for (j = 0; j < quant; j++){
		scanf("%d", &x);
		result = verificaprim(vetor,n,x);
		result2 = verificaultm(vetor,n,x);
		if (result!=-1){
			res = (result2 - result)+1;
			printf("%d\n", res);
			result=0;
			result2=0;
			res=0;
		}
		else{
			printf("%d\n", res);
			result=0;
			result2=0;
			res=0;
		}
	}
	return 0;
}