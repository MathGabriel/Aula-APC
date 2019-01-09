#include <stdio.h>

int main(){
	int vetor[10];
	int i,x;

	for(i = 0; i < 10; i++){
		scanf("%d",&vetor[i]);
		if (vetor[i] <= 0)
		{
			vetor[i] = 1;
			printf("X[%d] = %d\n",i,vetor[i]);
		}
		else
			printf("X[%d] = %d\n",i,vetor[i]);
	}

	return 0;
}