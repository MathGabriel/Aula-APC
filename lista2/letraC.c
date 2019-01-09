#include <stdio.h>

int main(){
	long long int a,b,c,delta;
	scanf("%lld %lld %lld", &a,&b,&c);
	delta = (b*b) -(4*a*c);
	if (delta < 0)
	{
		printf("complexas\n");
	}
	else{
		printf("reais\n");
	}
	return 0;
}