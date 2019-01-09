#include <stdio.h>
#define MAX 100
//multiplicacao de matrizes.
//funcao so para ler matriz
void lermatriz(int matriz[MAX][MAX],int linhas,int colunas){
	int i,j;
	for (i = 0; i < linhas; i++){
		for (j = 0; j < colunas; j++){
			scanf("%d",&matriz[i][j]);
		}
	}
}
//funcao so para printar a matriz
void pritmatriz(int matriz[MAX][MAX],int linhas, int colunas){
	int i,j;
	for (i = 0; i < linhas; i++){
		for (j = 0; j < colunas; j++){
			printf("%d ", matriz[i][j]);
		}
		printf("\n");
	}
}

int main(){
	int n,m,q,i,j,k,soma;
	int matrizA[MAX][MAX], matrizB[MAX][MAX], matrizC[MAX][MAX];

	scanf("%d %d %d",&n,&m,&q); 
	
	lermatriz(matrizA,n,m);
	lermatriz(matrizB,m,q);
	// multiplicacao a main pq passando por funcao
	// vai receber muitos parametos.
	for (i = 0; i < n; i++){
		for (j = 0; j < q; j++){
			soma = 0;
			for (k = 0; k < m; k++){
				soma += matrizA[i][k]*matrizB[k][j];
			}
			matrizC[i][j] = soma;
		}
	}

	pritmatriz(matrizC,n,q);

	return 0;
}