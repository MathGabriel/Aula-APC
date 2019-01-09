#include<stdio.h>

int main()
{
	int n,t=0,aux=0;

	scanf("%d",&n);

	do{

		aux++;
		t=t+aux;
		
	}while(t<n);

	printf("%d\n",t );
	printf("%d\n",aux );

	return 0;
}