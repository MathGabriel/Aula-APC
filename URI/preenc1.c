#include <stdio.h>

int main(){
	int N[10],resp,i;
	/*o primeiro termo do vetor*/
	scanf("%d",&N[1]);
	printf("N[0] = %d\n", N[1]);
	/*um for que dobra os termos*/
	for (i = 1; i < 10; ++i){
		resp = N[i]*2;
		printf("N[%d] = %d\n", i,resp);
		N[i+1] = resp;
	}
	return 0;
}