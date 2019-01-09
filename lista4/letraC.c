#include <stdio.h>
int main(){
	int N;
	long long int a=0,b=1,c=1e9+7;
	long long int aux;
	scanf("%d",&N);
	printf("1\n");
	printf("2\n");
	for (int i = 2; i < N; i++){
		aux = (a+b)%(c);
		a=b;
		b = aux;
		printf("%lld\n", aux+1);
	}
	return 0;
}