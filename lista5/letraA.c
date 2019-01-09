#include <stdio.h>

int main(){
	
	int n,x,cont=0;
	int soma=0;

	do{

		scanf("%d",&x);

		if (x!= -1)
		{
		
			soma=soma+x;
			cont++;
		}

	}while(x!=-1);

	n=soma/cont;

	printf("%d\n",n );

	return 0;
}