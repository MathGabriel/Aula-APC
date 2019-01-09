/*Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programação de Computadores – 1/2018
Aluno(a): < Jose Eduardo Lindoso da Fonseca >
Matricula: < 160152658 >
Turma: B
Versao do Compilador: < gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.5) >
Descricao: O proposito deste trabalho e criar um jogo do tipo
		   Tetris. Algumas especificacoes sobre o funcionamento estao 
	       descritas sob a forma de comentarios ao longo do programa.*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void clear(){
	
	system("clear || cls");
}

/*apaga a peça anterior*/
void limpa(char game[][20]){
	
	int i, j;
	
	/*avalia toda parte de manipular a peça*/
	for(i = 0; i < 6; i++){
		
		for(j = 0; j < 20; j++){
			
			game[i][j] = ' ';
		}
	}
}

/*funçao para girar a peça*/
void roda(char game[][20], char *seq, int *vertical, int *coluna){
	
	int tam = strlen(seq);
	int i;
	if(*vertical){
		/*executa o giro da peça em caso de peça na posiçao horizontal*/
		if(((*coluna)+tam) <= 20){	
			/*chama a funçao que limpa a peça anterior*/
			limpa(game);
			for(i = 0; i < tam; i++){
				
				game[0][*coluna+i] = seq[i];
			}
			if(*vertical == 1){
				/*muda a flag pra saber se a peça ta na horizontal ou na vertical*/
				*vertical = 0;
			}
			else{
				
				*vertical = 1;
			}
		}
	}
	/*executa o giro quando a peça esta na vertical*/
	else{
		
		limpa(game);
		for(i = 0; i < tam; i++){
			
			game[i][*coluna] = seq[i];
		}
		if(*vertical == 1){
			
			/*muda a flag para informar a posiçao da peça*/
			*vertical = 0;
		}
		else{
			
			*vertical = 1;
		}
	}
}

void cair(char game[][20],char *seq,int vertical,int *coluna){
	
    int i, tam = strlen(seq), flag = 0, j;
    if(vertical){
		
        j = 6;
        while(j < 15){
            if(game[j][*coluna] != ' '){
				
                flag = j;
                break;
    		}
            j++;
	    }
		/*imprime a peça que caiu*/
        for(i = 0; i < tam; i++){
			
            game[j-i-1][*coluna] = seq[i];
        }
    }
	/*entra no else pra descer peças na horizontal*/
    else{
		
        j = 6;
        flag = 0;
		/*restringe a açao ao tamanho da matriz*/
        while(j < 15){
			
        	for(i = 0; i < tam; i++){
				
        		if (game[j][*coluna+i] != ' '){
					
        	   		flag = j;
        		}
			}
        		if(flag != 0){
					
        	    	break;
				}
        	j++;
		}
        for(i = 0; i < tam; i++){
			
    	    game[j-1][*coluna+i] = seq[i];
		}
	}
}

/*movimenta a peça para a esquerda*/
void esquerda(char game[][20], char *seq, int vertical, int *coluna){
	
	int tam = strlen(seq);
	int i;
	
	if(*coluna > 0){
		
		limpa(game);
		*coluna = *coluna - 1;
		
		if( vertical ) {
			
			for(i = 0; i < tam; i++){
				
				game[i][*coluna] = seq[i];
			}
		} 
		else{
			
			for(i = 0; i < tam; i++){
				
				game[0][(*coluna)+i] = seq[i];
			}
		}
	}
}

/*move a peça para a direita*/
void direita(char game[][20], char *seq, int vertical, int *coluna){
	
	int tam = strlen(seq);
	int i;
	
	if(*coluna < 19){
		
		if(vertical){
			
			*coluna = *coluna + 1;
			limpa(game);
			for(i = 0; i < tam; i++){
				
				game[i][*coluna] = seq[i];
			}
		}
		else{
			
			*coluna = *coluna + 1;
			if(((*coluna)+tam) <= 20){
				
				limpa(game);
				for(i = 0; i < tam; i++){
					
					game[0][(*coluna)+i] = seq[i];
				}
			}
			else{
				
				*coluna = *coluna - 1;
			}
		}
	}
}

int lerSeq(char *seq, char game[][20]){
	
	int i, tam = strlen(seq);
	
	for(i = 0; i < tam; i++){
		
		game[i][9] = seq[i];
	}
	return 1;
}

/*funçao que imprime a matriz do jogo*/
void imprimir(char game[][20], int tot_pontos){
	
	printf("\n\n");	
	clear();
	int i, j;
	printf("  +--------------------+ PONTUACAO: %d\n",tot_pontos);
	
	for (i = 0; i < 15; i++){
		
		if (i==6){
			
			printf("=>+");
		}
		else{
		
			printf("  |");
		}
		for(j = 0; j < 20; j++){
			
			printf("%c",game[i][j]);
		}
		if(i==6){
			printf("+<=\n");
		}
		else{
			printf("|\n");
		}
	}
	printf("  +--------------------+\n");
	printf("R-rotar D-direita E-esquerda C-cair\n");
	printf(": ");
}

/*função verifica se a linha esta completamente preenchida*/
void verifica_linha(char game[][20],int *pontos){
	
   int i,j,cont,k;
	/*observa o preenchimento da linha*/
    for (i = 6; i < 15; i++){
		
        cont = 0;
        for(j = 0; j < 20; j++){
			
            if(game[i][j] == ' '){
				
                cont++;
            }
        }
		/*se algum espaço nao estiver preenchido o laço eh interrompido*/
        if(cont == 0){
            break;
        }
    }
	/*a linha esta cheia*/
    if(cont == 0){
		/*preenche a linha com x*/
        for(j = 0; j < 20; j++){
			
            game[i][j] = 'X';
        }
		/*soma os pontos do jogador*/
		*pontos = *pontos + 100;
        limpa(game);
        imprimir(game,*pontos);
        getchar();
        k=i;
        while(k > 6){
			
            for(j = 0; j < 20; j++){
				
                game[k][j] = game[k-1][j];
            }
            k--;
        }
    }
    imprimir(game,*pontos);
    getchar();
}

/*mostra que o jogo acabou*/
void fimDoJogo(int fim, int pontos){
	
	int i, j;
	char game[15][20];
	
	/*se a flag de fim for modificada, exibe a mensagem de derrota*/
	if(fim){
		clear();
		/*preenche todo o jogo com x*/
		for(i = 0; i < 15; i++){
			
			for(j = 0; j < 20; j++){
				
				game[i][j] = 'X';
			}
		}
		/*imprime uma nova matriz de fim de jogo*/
		imprimir(game,pontos);
		printf(" YOU LOSE!!!Tecle <enter> para voltar para o menu.");
		getchar();
		getchar();
	}
	else{
		
		clear();
		/*exibe a mensagem de final de jogo e a pontuaçao obtida*/
		printf("*** PARABENS!!! VOCE CHEGOU AO FIM DO JOGO COM %d PONTOS!!!\nTecle <enter> para sair\n", pontos);
		getchar();
	}
}

/*verifica se o jogo terminou*/
int terminaJogo(char game[][20], int pontos){
	
	int i;
	
	for(i = 0; i < 20; i++){
		
		if(game[6][i] != ' '){
			
			fimDoJogo(1, pontos);
		return 0;
		}
	}
	return 1;
}

/*entrada do jogo para o usuario*/
void inicializacao(){
	
	printf("__________________________________________________\n");
	printf("_________´$$$$`_____________________________,,,___\n");
	printf("________´$$$$$$$`_________________________´$$$`___\n");
	printf("_________`$$$$$$$`______,,________,,_______´$$$$´_\n");
	printf("__________`$$$$$$$`____´$$`_____´$$`____´$$$$$´___\n");
	printf("___________`$$$$$$$`_´$$$$$`_´$$$$$`__´$$$$$$$´___\n");
	printf("____________`$$$$$$$_$$$$$$$_$$$$$$$_´$$$$$$$´____\n");
	printf("_____________`$$$$$$_$$$$$$$_$$$$$$$`´$$$$$$´_____\n");
	printf("____,,,,,,______`$$$$$$_$$$$$$$_$$$$$$$_$$$$$$´___\n");
	printf("__´$$$$$`____`$$$$$$_$$$$$$$_$$$$$$$_$$$$$$´______\n");
	printf("_´$$$$$$$$$`´$$$$$$$_$$$$$$$_$$$$$$$_$$$$$´_______\n");
	printf("_´$$$$$$$$$$$$$$$$$$_$$$$$$$_$$$$$$$_$$$$$´_______\n");
	printf("____`$$$$$$$$$$$$$$$_$$$$$$$_$$$$$$_$$$$$$´_______\n");
	printf("_______`$$$$$$$$$$$$$_$$$$$__$$_$$$$$$_$$´________\n");
	printf("________`$$$$$$$$$$$$,___,$$$$,_____,$$$$$´_______\n");
	printf("__________`$$$$$$$$$$$$$$$$$$$$$$$$$$$$$´_________\n");
	printf("___________`$$$$$$$$$$$$$$$$$$$$$$$$$$$´__________\n");
	printf("_____________`$$$$$$$$$$$$$$$$$$$$$$$$´___________\n");
	printf("________________`$$$$$$$$$$$$$$$$$$$$´____________\n");
	printf("__________________________________________________\n");
	printf("                  TETRIS ENJJJJJUADO              \n");
	
	
	getchar();
}

/*menu do jogo onde o usuario escolhe o que fazer*/
void menu_principal(int* p){
	
	int opcao;
	
	printf("1 - Jogar\n");
	printf("2 - Ranking\n");
	printf("3 - Instrucoes\n");
	printf("4 - Sair\n");
	printf("Escolha uma opcao: ");
	
	scanf("%d", &opcao);
	
	getchar();
	
	*p = opcao;
}

/*funcao do jogo tetris em si*/
int jogar(){
	
	FILE *fp;
	char game[15][20], seq[70][7], op;
	int i = 0, vertical, coluna, pontos = 0, j;
	fp = fopen("sequencias.txt", "r");
	/*laço que permite que o jogo recomece*/
	while(fscanf(fp, "%s", seq[i] ) > 0){
		
		i++;
	}
	/*laço que deixa a tela em branco*/
	for(i = 0; i < 15; i++){
		
		for(j = 0; j < 20; j++){
			
			game[i][j] = ' ';
		}
	}
	imprimir(game, pontos);
	i = 0;
	/*esse laço eh sobre as peças do jogo*/
	while(i < 70 && lerSeq(seq[i], game)){
		
		vertical = 1;
		coluna = 9;
		imprimir(game, pontos);
		do{
			
			scanf("%c", &op);
			fflush(stdout);
			switch(op){
				case 'r': /*roda a peça*/
					roda(game, seq[i], &vertical, &coluna);
					imprimir(game, pontos);
					break;
				case 'd': /*move a peça pra direita*/
					direita(game, seq[i], vertical, &coluna);
					imprimir(game, pontos);
					break;
				case 'e': /*move a peça pra esquerda*/
					esquerda(game, seq[i], vertical, &coluna);
					imprimir(game, pontos);
					break;
				default:
					break;
			}
		}
		/*enquanto o mvimento nao for 'cair' faca:*/
		while(op != 'c');
		
		cair(game, seq[i], vertical, &coluna);
		limpa(game);
		if(terminaJogo(game, pontos)){
			
			imprimir(game, pontos);
			verifica_linha(game, &pontos);
			imprimir(game, pontos);
			limpa(game);
			i++;
		}
		else{
			
			return 0;
		}	
	}
	fimDoJogo(0, pontos);
return 0;
}

/*funcao que mostra os ranking dos jogadores*/
void ranking(){
	
	printf("Indisponivel no momento!");
	getchar();
}

/*instrucoes para jogar*/
void instrucoes(){
	
	printf("Para se movimentar eh necessario digitar uma tecla.\n\n");
	printf("Controles:\n c - cair\n e - esquerda\n d - direita\n r - rataciona\n");
	
	getchar();
}

/*funcao que ordena os processos do jogo*/
int main(){
	
	int opcao = 0;
	
	system("clear");
	inicializacao();
	system("clear");
		
	while(opcao != 4){
		
		system("clear");
		menu_principal(&opcao);
		system("clear");
		
		switch (opcao){
			
			case 1: jogar(); break;
			case 2: ranking(); break;
			case 3: instrucoes(); break;
				
		}
	}
	
	printf("Volte Sempre!!\n");
	getchar();
	system("clear");
	
	
return 0;	
}
	