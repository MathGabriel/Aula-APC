#include <stdio.h>

void Lermatriz(int n,char matriz[][n],int linhas,int colunas){
	int i,j;
	for (i = 0; i < linhas; i++){
		for (j = 0; j < colunas; j++){
			scanf(" %c",&matriz[i][j]);
		}
	}
}

void Espalha(int n, char matriz[][n], int x, int y){
    matriz[x][y] = 'x';
    if(matriz[x+1][y] == '.'){
        Espalha(n,matriz,x+1,y);
    }
    if(matriz[x-1][y] == '.'){
        Espalha(n,matriz,x-1,y);
    }
    if(matriz[x][y-1] == '.'){
        Espalha(n,matriz,x,y-1);
    }
    if(matriz[x][y+1] == '.'){
        Espalha(n,matriz,x,y+1);
    }
}

void PrintaMatriz(int n,char matriz[][n],int x,int y){
	int i,j;
	for(i = 0; i < n; i++){
        for(j = 0; j < n; j++)
            printf("%c ", matriz[i][j]);
        printf("\n");
    }
}

int main(){
	int n,x,y;

	scanf("%d", &n);	
	char matriz[n][n];
	Lermatriz(n,matriz,n,n);

	scanf("%d %d", &x,&y);
	Espalha(n,matriz,x,y);
	PrintaMatriz(n,matriz,x,y);
	return 0;
}