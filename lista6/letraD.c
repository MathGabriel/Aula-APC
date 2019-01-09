#include <stdio.h>

int main(){
	long long int g,j,gus=140,jul=110;
	int cont=0;
	scanf("%lld %lld", &g,&j);
	while(jul<=gus){
		jul = jul+j;
		gus=gus+g;
		cont++;
	}
	printf("%d\n", cont);
	return 0;
}