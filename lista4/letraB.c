#include <stdio.h>

int main(){
	long long int N,I,quant,i;
	long long int aux = 0;
	scanf("%lld %lld",&N,&I);
	for (i = 0; i < N; ++i){
		scanf("%lld",&quant);
		aux = aux + quant;
	}
	aux = aux/N;
	if (aux > I){
		printf("media : %lld\n", aux);
		printf("o rock reinara mais uma vez\n");
	}
	else{
		printf("media : %lld\n", aux);
		printf("rockeiros trabalhando ja\n");
	}
	return 0;
}