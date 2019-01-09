#include <stdio.h>
int main(){
	int a,b,c;
	scanf("%d %d %d",&a,&b,&c);
	if (a+b <= c || a+c <= b || b+c <= a){
		printf("deivis sendo deivis\n");
	}
	else{
			printf("triangulo\n");
		if (a != b && c != b && c != a){
			printf("escaleno\n");
		}
		else{
			if ((a == b) || (a == c) || (b == c)){
				printf("isosceles\n");
			}
			else{
				if (a == b && a == c && b == c){
					printf("equilatero\n");
				}
			}
		}
	}
	if (a*a == b*b +c*c || b*b == a*a + c*c || c*c == a*a + b*b){
		printf("retangulo\n");
	}
	return 0;
}