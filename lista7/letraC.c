#include <stdio.h>
long long int fatn(long long int n){
	long long int fat = 1,i;
	for (i = 2; i <= n; i++){
		fat = (fat*i);
	}
	return fat;
}

long long int denom(long long int n, long long int k){
	long long int nfatn,kfatn,dnominador;
	dnominador = (n - k);
	dnominador = fatn(k)*fatn(dnominador);
	return dnominador;
}
long long inverso(long long x){
	long long primo= 1e6+3;
	long long resultado =1, expoente = primo -2;
	while(expoente--)
		resultado = (resultado*x)% primo;
	return resultado;
}
int main(){
	long long int n,k,mod=1e6+3;
	long long int rfatn,rdenom,res;
	scanf("%lld %lld", &n,&k);
	rfatn = fatn(n);
	rdenom= denom(n,k);
	res = rfatn*inverso(rdenom);
	res = res%mod;
	printf("%lld\n", res);
	return 0;
}