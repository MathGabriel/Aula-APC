#include <stdio.h>

int main(){
	int a,b;
	scanf("%d %d",&a,&b);
	if (a > b){
		printf("%d\n", a);
		printf("%d\n", b);
		printf("diferentes\n");
	}
	else{
		if (b > a){
			printf("%d\n", b);
			printf("%d\n", a);
			printf("diferentes\n");
		}
		else{
			if (a == b){
			printf("%d\n", a);
			printf("%d\n", b);
			printf("iguais\n");
			}
		}
	} 
	return 0;
}