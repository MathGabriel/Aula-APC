#include <stdio.h>

int main(){
	int x,y,w,h;
	int a,b;
	scanf("%d %d %d %d", &x,&y,&w,&h);
	scanf("%d %d", &a,&b);
	if (a > x+w || a < x || b > y+h || b < y){
		printf("fora\n");
	}
	else{
		if (a > x && a < x+w && b > y && b < y+h){
			printf("dentro\n");
		}
		else{
			printf("sobre a borda\n");
		}
	}
	return 0;
}