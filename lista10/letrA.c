#include <stdio.h>
#define MAX 1000
void lematriz(int matriz[MAX][MAX],int n,int m){
	int i,j;
	for (i = 0; i < n; i++){
		for (j = 0; j < m; j++){
			scanf("%d",&matriz[i][j]);
		}
	}
}

void transmatriz(int matriz[MAX][MAX],int n,int m){
	int i,j,aux;

	for (j = 0; j < m ; j++){
		for (i = 0; i < n; i++){
			printf("%d ",matriz[i][j]);
		}
		printf("\n");
	}
}

int main(){
	int n,m,matriz[MAX][MAX];
	scanf("%d %d",&n,&m);
	lematriz(matriz,n,m);
	transmatriz(matriz,n,m);
	return 0;
}