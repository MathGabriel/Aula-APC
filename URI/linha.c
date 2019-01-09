#include <stdio.h>
int main(){
	char T;
	int i,j;
	float L,res=0,matriz[12][12];
	scanf("%f",&L);
	scanf(" %c",&T);

	for (i = 0; i < 12; i++){
		for (j = 0; j < 12; j++){
			if (j == L){
				scanf("%f", &matriz[i][j]);
				res = res + matriz[i][j];
			}
			else{
				scanf("%f", &matriz[i][j]);
			}
		}
	}
	if (T == 'S'){
		printf("%.1f\n", res);
	}
	else if (T== 'M'){
		res = res/12;
		printf("%.1f\n", res);
	}
	return 0;
}