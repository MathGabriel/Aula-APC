#include <stdio.h>
#include <math.h>
int main(){
	double a,b,c;
	double delta,x1,x2;
	scanf("%lf %lf %lf",&a,&b,&c);
	delta = pow(b,2)-(4*a*c);
	x1 = (b) + sqrt(delta);
	x1 = x1/(2*a);
	x2 = (b) - sqrt(delta);
	x2 = x2/(2*a);
	printf("%lf\n", x1);
	printf("%lf\n", x2);
	return 0;
}