#include <stdio.h>
int main()
{

	int n,a=0,b=0,i,x,y,sair;
	int elevai=0;
	scanf("%d",&n);
	
	for(i=1;i<=n;i++)
	{
		scanf("%d %d",&a,&b);
		
		elevai=elevai-a;
		elevai=elevai+b;
		
		if(elevai>15)
		{
			sair=elevai-15;
			printf("Excesso de Passageiros. Devem sair %d\n",sair);
			elevai=15;
		}
		
		

	}
		printf("%d pessoas vao descer",elevai);
		
	return 0;
}