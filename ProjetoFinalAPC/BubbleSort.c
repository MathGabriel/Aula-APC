#include <stdio.h>
#define MAX 100000
#define CLEAR printf("\033c")

//funcao so para leitura do vetor
void LerVetor(long long int vetor[],int tamanho){
	int i;
	printf("Agora digite os nmeros que compoe o vetor: \n");
	for (i = 0; i < tamanho; i++){
		scanf("%lld",&vetor[i]);
	}
}

//funcao pra organizar em ordem crescente o vetor
void BubbleSort(long long int vetor[],int tamanho) {
    int aux,i,j;
    for(i = 0; i < tamanho; i++){
        for(j = 0; j < tamanho-1; j++){
            if(vetor[j] > vetor[j+1]){
                aux = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = aux;
            }
        }
    }
}

void PrintOrdenado(long long int vetor[],int tamanho){
	int i;	
	for (i = 0; i < tamanho; i++){
		printf("vetor ordenado: %lld\n", vetor[i]);
	}
}

int BuscaBinaria(long long int vetor[],int tamanho,int NmrProcurado){
	int inicio = 0,fim = tamanho - 1;
	int meio,achou=0;
	while((inicio<=fim) && (!achou)){
		meio = (inicio+fim)/2;
		if (NmrProcurado < vetor[meio]){
			fim = meio - 1;
		}
		else if (NmrProcurado > vetor[meio]){
			inicio = meio + 1;
		}
		else if (NmrProcurado == vetor[meio]){
			achou = 1;
		}
	}
	return achou;
}

int main(){
	int n,i,x,resdivbin;
	long long int vetor[MAX];
	CLEAR;
	printf("Digite o tamanho do vetor: \n");
	scanf("%d",&n);
	CLEAR;
	LerVetor(vetor,n);
	CLEAR;
	BubbleSort(vetor,n);
	PrintOrdenado(vetor,n);
	printf("Agora podemos fazer a busca binaria com o vetor ja ordenado...\n");
	printf("Digite o termo que deseja procurar: \n");
	scanf("%d",&x);
	CLEAR;
	resdivbin=BuscaBinaria(vetor,n,x);
	if (resdivbin){
		printf("O valor %d pertence ao vetor digitado!\n", x);
	}
	else{
		printf("Desculpa, mas esse valor nao pertence a esse vetor.\n");
	}
	return 0;
}