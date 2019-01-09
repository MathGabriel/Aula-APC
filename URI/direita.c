#include <stdio.h>

void LerMatriz(float matriz[][12]){
	int x,y;
	for (x = 0; x < 12; x++){
		for (y = 0; y < 12; y++){
			scanf("%f", &matriz[x][y]);
		}
	}
}

void SomarTermos(float matriz[][12]){
	int x,y,aux=0;
	int i,j,aux2=0;
	int soma;
	for (x = 5; x < 0 ; x--){
		for (y = 7; y < 12; y++){
			aux = aux + matriz[x][y];
		}
		y--;
	}
	for (i = 6; i < 11; i++){
		for (j = 7; j < 12; j++){
			aux2 = aux2 + matriz[i][j];
		}
		j--;
	}
	soma = aux+aux2;
	printf("%d\n", soma);
}

void MediaTermos(float matriz[][12]){
	int x,y,aux=0;
	int i,j,aux2=0;
	int soma;
	for (x = 5; x < 0 ; x--){
		for (y = 7; y < 12; y++){
			aux = aux + matriz[x][y];
		}
		y--;
	}
	for (i = 6; i < 11; i++){
		for (j = 7; j < 12; j++){
			aux2 = aux2 + matriz[i][j];
		}
		j--;
	}
	soma = (aux+aux2)/10;
	printf("%d\n", soma);
}
int main(){
	float matriz[12][12];
	char O;
	scanf("%c", &O);
	LerMatriz(matriz);
	if (O == 'S'){
		SomarTermos(matriz);
	}
	else if (O == 'M'){
		MediaTermos(matriz);
	}
	return 0;
}