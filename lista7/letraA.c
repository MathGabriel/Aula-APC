#include <stdio.h>
void maimen(int n){
	long long int a=1e9,aux,b=-1e9,i;
	for (i = 0; i < n; i++){
		scanf("%lld", &aux);
		if (aux > b){
			b = aux;
		}
		if (aux < a){
			a = aux;
		}
	}
	printf("min = %lld; max = %lld\n", a,b);
}
int main(){
	int n;
	scanf("%d", &n);
	maimen(n);
	return 0;
}