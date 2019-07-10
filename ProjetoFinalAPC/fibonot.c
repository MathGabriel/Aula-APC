#include <stdio.h>

int main(){
	int a0 = 1,a1 = 1,a2 = 0;
	int nmr,cont=0,i;

	scanf("%d", &nmr);
	for (i;i;i){
		i=1;
		a2= a1+a0;
		while (a2 > a1 + i){
			cont += 1;
			if (cont == nmr){
				printf("%d\n", a1+i);
				return 0;
			}
			i++;
		}
		a0 = a1;
		a1 = a2;
	}

	return 0;
}