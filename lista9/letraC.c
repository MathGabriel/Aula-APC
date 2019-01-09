#include <stdio.h>

int main(){
	long long int n,valor,q,vetor[100000],substitui;
	int i;
	char operacao;

	scanf("%lld %lld", &n,&q);

	for (i = 0; i < n; i++){
		scanf("%lld",&vetor[i]);
	}

	for (i = 0; i < q ; i++){
		scanf(" %c",&operacao);
		scanf("%lld", &valor);
		if (operacao == 'R'){
			printf("%lld\n", vetor[valor-1]);
		}
		
		if (operacao =='U'){
			scanf("%lld",&substitui);
			vetor[valor-1] = substitui;
		}

	}


	return 0;
}