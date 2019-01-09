#include <stdio.h>
#include <math.h>
int main(){
	int a,b,i;
	long long int cont=1;
	int aux = 1000000000+7;
	scanf("%d %d",&a,&b);
	for (i = 0; i < b; i++){
		cont = cont * a;
		cont = cont % aux;
	}
	printf("%lld\n", cont);
	return 0;
}