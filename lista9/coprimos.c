#include <stdio.h>
#include <math.h>

void max(long long int* a,long long int* b) {
	long long int temporaria;
	if(*b > *a) {
		temporaria = *b;
		*b = *a;
		*a = temporaria;
	}
}

long long int ehprimo(long long int x) {
	long long int i;
	if(x == 1) {
		return 0;
	}
	for(i = 2; i < x; i++) {
		if(x % i == 0) {
			return 0;
		}
	}
	return 1;
}

long long int divisoresPrimos(long long int x){
	long long int i,quant = 0;
	for (i = 2; i <= x ; i++){
		if (x % i == 0 && ehprimo(i)){
			quant++;
		}
	}
	return quant;
}

long long int ff(long long int x,long long int a,long long int b){
	long long int f, g;
	g = divisoresPrimos(x);
	f = a * g * g * g - (b * x);

	return f;
}

int main(){
	long long int a,b,l,r, maior, f,i; 
	scanf("%lld %lld %lld %lld",&a,&b,&l,&r);

	maior = ff(l, a, b);
	for (i = l+1; i <= r ; i++){
		f = ff(i, a, b);
		max(&maior, &f);
	}

	printf("%lld\n", maior);

	return 0;
}
