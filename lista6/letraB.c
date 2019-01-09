#include <stdio.h>

int main(){
	int x,ms = -10000;
	int atual,s;
	scanf("%d", &x);
	do{
		scanf("%d", &atual);
		s = x + atual;
		if (s > ms){
			ms = s;
		}
		x = atual;
	}while(x!=0);

	printf("%d\n", ms);
	return 0;
}