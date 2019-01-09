#include <stdio.h>
int main(){
	long long int a,b,c,aux,m,u,l;
	long long int k = 1000000007;
	scanf("%lld %lld %lld",&a,&b,&c);
	m = ((a % k) * (b % k)) % k;
	u = ((m % k) * (c % k)) % k;
	l = ((b%k)+(k-(c%k)))%k;
	aux = ((u % k) + (l % k)) % k;
	printf("%lld\n", aux);
	return 0;
}