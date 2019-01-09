#include<stdio.h>

int main()
{
	int n,k,horas=0,resto=0,m;

	scanf("%d %d",&n,&k);

	horas=n;
	while(n>=k)
	{
		m=n%k;
		n=n/k;
		horas=horas+n;
		n+=m;
	}

	printf("%d\n",horas);

	return 0;
}