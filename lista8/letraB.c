#include <stdio.h>
#include <math.h>
void bhaskara(int a, int b, int c, double* delta, double* x1, double* x2){
	*delta = pow(b,2) - (4*a*c);
	*x1 = (-b + sqrt(*delta))/(2*a);
	*x2 = (-b - sqrt(*delta))/(2*a);
}

void menormaior(double* x1, double* x2){
	int j1 = (int)round(*x1);
	int j2 = (int)round(*x2);
	if (j1 > j2){
		printf("{%d,%d}\n", j2,j1);
	}
	else
		printf("{%d,%d}\n", j1,j2);
}

int main(){
	int a,b,c;
	double delta,x1,x2;
	
	scanf("%d %d %d", &a,&b,&c);
	bhaskara(a,b,c,&delta,&x1,&x2);

	int l1 = (int)round(x1);
	if (delta < 0){
		printf("{}\n");	
	}
	else if (delta == 0){
		printf("{%d}\n", l1);
	}
	else if (delta > 0){
		menormaior(&x1,&x2);
	}

	return 0;
}
