#include <stdio.h>

int main(){
	int n,i,j;
	int m,prim;
	scanf("%d",&n);
	/*Diagonal principal crescente*/
	for (i = 0; i < n ; i++){
		for (j = 0; j < n ; j++){
			scanf("%d",&m);
			if (i == j){
				if (i == 0 && j== 0){
					prim = m;
				}
				else if (i > 0 && j > 0){
					if (prim < m){
						prim = m;
					}
					else{
						i = n+1;
					}
				}
			}
			
		}
	}

	/*print para crescente*/
	if (i>n){
		printf("nada de especial\n");
	}
	else {
		printf("diagonal invertida\n");
	}


	return 0;
}