#include <stdio.h>

int main(){
	long long int a,b,p,T,result;
	scanf("%lld %lld %lld %lld",&T,&a,&b,&p);
	if (T == 1){
		result = ((a % p) + (b % p)) % p;
	}
	else if (T == 2){
		result =((a%p)+(p-(b%p)))%p;
	}
	else if (T == 3){
		result = ((a % p) * (b % p)) % p;
	}
	printf("%lld\n", result);
	return 0;
}