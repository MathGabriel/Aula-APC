#include <stdio.h>
#include <math.h>
int main(){
	double r,h,V;
	float PI = 3.14;
	scanf("%lf %lf",&r,&h);
	V = PI*(r*r)*h;
	printf("%.6lf\n", V);
	return 0;
}