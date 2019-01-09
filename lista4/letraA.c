#include <stdio.h>

int main(){
	int n,i,a;
	long long int max,min;
	scanf("%d",&n);
	max = -10000000000;
	min = 10000000000;
	for (i = 0; i < n ; ++i){
		scanf("%d",&a);
		if (a > max){
			max = a;
		}
		if (a < min)
		{
			min = a;
		}
	}
	printf("max = %lld; min = %lld\n", max,min);
	return 0;
}