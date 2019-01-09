#include <stdio.h>

int main(){
	double s,ajust=0.015;
	int ano,i;
	scanf("%lf %d",&s,&ano);
	for (i = 2006; i <= ano; i++){
		s = s + s*ajust;
		ajust = ajust * 2;
	}
	printf("%.15lf\n", s);
	return 0;
}