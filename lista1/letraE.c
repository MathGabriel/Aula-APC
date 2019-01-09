#include <stdio.h>
#include <math.h>
int main(){
	double c1,c2,hip,h;
	scanf("%lf %lf",&c1,&c2);
	c1 = (c1*c1);
	c2 = (c2*c2);
	h = c1 + c2;
	hip = sqrt(h);
	printf("%lf\n", hip);
	return 0;
}