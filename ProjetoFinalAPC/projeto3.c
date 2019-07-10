/*
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||    Universidade de Brasilia                                                                          ||
||    Departamento de Ciencia da Computacao                                                             ||
||    Instituto de Ciencias Exatas                                                                      ||
||    Algoritmos e Programação de Computadores - 1/2019                                                 ||
||    Aluno: Matheus Gabriel da Silva Rodrigues                                                         ||
||    Matricula: 180025031                                                                              ||
||    Curso: Computacao(Licenciatura)                                                                   ||
||    Descricao: Implementacao de um codigo de um labirinto, onde mostra o caminho do rato ate o queijo.||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*/
#include <stdio.h>
char matriz[50][50];

int LeituraMatriz(int linha, int coluna){
	int i,j,flag=1;
	for (i = 0; i < linha; i++){
		for (j = 0; j < coluna; j++){
			scanf("%d", &matriz[i][j]);
			if (matriz[i][j] != -1 && matriz[i][j] != 0){
				flag = 0;				
			}
		}
	}
	return flag;
}

int CaminhoDoRato(int linha,int coluna, int x, int y,char aux){
	if (matriz[x][y] != -1){
		matriz[x][y] = aux;
	}
	aux++;
	if ((matriz[x-1][y] != -1) && (x-1 >= 0)){ 				//verifica em a posicao acima do rato se eh uma parede
		if (matriz[x-1][y] == 0){ 							//verifica se esta vazio
			CaminhoDoRato(linha,coluna,x-1,y,aux); 
		}
		else if (matriz[x-1][y] > aux){ 					// se nao tiver vazio ele verifica que numero esta ali
			CaminhoDoRato(linha,coluna,x-1,y,aux);
		}
	}
	if ((matriz[x][y+1] != -1)&& (y+1 < coluna)){ 			// verifica a posicao a direita do rato se eh um aparede
		if(matriz[x][y+1] == 0){							// verifica se esta vazio
			CaminhoDoRato(linha,coluna,x,y+1,aux);
		}
		else if (matriz[x][y+1] > aux){						// se nao tiver vazio ele verifica que numero esta ali
			CaminhoDoRato(linha,coluna,x,y+1,aux);
		}
	}
	if((matriz[x+1][y] != -1) && (x+1 < linha)){ 			// verifica a posicao a baixo do rato se eh uma parede
		if (matriz[x+1][y] == 0){							// verifica se esta vazio
			CaminhoDoRato(linha,coluna,x+1,y,aux);
		}
		else if (matriz[x+1][y]> aux){						// se nao tiver vazio ele verifica que numero esta ali
			CaminhoDoRato(linha,coluna,x+1,y,aux);
		}
	}
	if ((matriz[x][y-1] != -1) &&(y-1 >= 0)){ 				//verifica a posicao a esquerda do rato se eh uma parede
		if (matriz[x][y-1] == 0){							// verifica se esta vazio
			CaminhoDoRato(linha,coluna,x,y-1,aux);
		}
		else if (matriz[x][y-1] > aux){							// se nao tiver vazio ele verifica que numero esta ali
			CaminhoDoRato(linha,coluna,x,y-1,aux);
		}
	}
}

int VerificaCaminho(int linha, int coluna, int aux, int x, int y, int Rx, int Ry){
	if (matriz[Rx][Ry] == aux){
		matriz[Rx][Ry] = 'X';
	}
	else if ((matriz[x-1][y] == aux-1) && (x-1 >= 0)){				//verifica ACIMA
		matriz[x][y] = 'X';
		VerificaCaminho(linha,coluna,aux-1,x-1,y,Rx,Ry);
	}
	else if ((matriz[x][y+1] == aux-1 )&& (y+1 < coluna)){		//verifica a DIREITA
		matriz[x][y] = 'X';
		VerificaCaminho(linha,coluna,aux-1,x,y+1,Rx,Ry);
	}
	else if ((matriz[x+1][y] == aux-1) && (x+1 < linha)){		//verifica ABAIXO 
		matriz[x][y] = 'X';
		VerificaCaminho(linha,coluna,aux-1,x+1,y,Rx,Ry);
	}
	else if ((matriz[x][y-1] == aux-1) && (y-1 >= 0)){			//verifica a ESQUERDA
		matriz[x][y] = 'X';
		VerificaCaminho(linha,coluna,aux-1,x,y-1,Rx,Ry);
	}
}

void MostraLabirintoFinal(int linha, int coluna){
	int i,j;
	printf("O Caminho do rato de acordo com o labirinto ficou: \n\n");
	for (i = 0; i < linha; i++){
		for (j = 0; j < coluna; j++){
			if (matriz[i][j] != 88){
				printf("   ", matriz[i][j]);
			}
			else{
				printf(" X ", matriz[i][j]);
			}
		}
		printf("\n");
	}
	printf("\nAte logo...\n");
}

void MostraLabirintoInicial(int linha,int coluna){
	int i,j;
	printf("Lembrando que '0' = um espaco vazio\n");
	printf("e '-1' = uma parede do labirinto.\n");
	printf("Seu labirinto ficou nesse formato: \n\n");
	for (i = 0; i < linha; i++){
		for (j = 0; j < coluna; j++){
			if (matriz[i][j] == -1){
				printf(" %d", matriz[i][j]);
			}
			else{
				printf("  %d", matriz[i][j]);
			}
		}
		printf("\n");
	}
}

int main(){
	int I,J,X,Y,AUX,res;
	char AUX1;
	int PosQx,PosQy;
	
	printf("Digite o tamanho do labirinto: ");
	scanf("%d %d", &I, &J);
	
	printf("Agora insira os elementos dentro do labirinto:\n");
	res = LeituraMatriz(I,J);
	if (res==0){
		printf("ERRO!! Elemento diferente de 0 e -1 nao pode!\n");
		return 0;
	}
	MostraLabirintoInicial(I,J);
		
	printf("\nDigite a posicao em que \n");
	printf("se encontra o rato( x,y ): ");
	scanf("%d,%d", &X,&Y);
	
	printf("Digite a posicao do QUEIJO( x,y ): ");
	scanf("%d,%d", &PosQx, &PosQy);
	
	AUX=1;
	CaminhoDoRato(I,J,X,Y,AUX);
	AUX1 = matriz[PosQx][PosQy];

	VerificaCaminho(I,J,AUX1,PosQx,PosQy,X,Y);

	MostraLabirintoFinal(I,J);
	return 0;
}