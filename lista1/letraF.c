#include <stdio.h>

int main(){
	float N1,N2,N3;
	float P1,P2,P3;
	float NT,PT,mda;
	scanf("%f %f %f",&N1,&N2,&N3);
	scanf("%f %f %f",&P1,&P2,&P3);
	N1 = N1*P1;
	N2 = N2*P2;
	N3 = N3*P3;
	NT = N1+N2+N3;
	PT = P1+P2+P3;
	mda = NT/PT;
	printf("%f\n", mda);
	return 0;
}