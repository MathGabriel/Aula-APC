#include <stdio.h>
int main(){
	float A[100];
	int i;
	/*leitura das variaveis do vetor*/
	for (i = 0; i < 100; ++i){
		scanf("%f",&A[i]);
	}
	/* printando os termos apos toda leitura*/
	for (i = 0; i < 100; i++){
		if (A[i]<=10){
			printf("A[%d] = %.1f\n", i,A[i]);
		}
	}

	return 0;
}