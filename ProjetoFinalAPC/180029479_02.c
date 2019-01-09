/*	Universidade de Brasilia
	Instituto de Ciencias Exatas
	Departamento de Ciencia da Computacao
	Algoritmos e Programação de Computadores – 1/2018
	Aluno(a): Yuri Crystian Ribeiro e Silva
	Matricula: 180029479
	Turma: A
	Versão do compilador: gcc 5.4.0 20160609
	Descricao: eh um jogo no qual existem diversos objetos diferentes, sendo eles o X que eh o inimigo, o F que eh o combustivel, e o B que eh o bonus
			   o bonus se divide em tres tipos diferentes e o objetivo do jogador eh conseguir a maior quantia de pontos sem que seu combustivel chegue
			   a 0 ou que seja atingido por um inimigo dessa forma encerrando sua partida.
	*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#define RAND rand()%100+1
#ifdef _WIN32
	#define CLEAR system("cls")
	#define VERMELHO "\033[31m"
	#define VERDE "\033[32m"
	#define AMARELO "\033[33m"
	#define AZUL "\033[34m"
	#define ROXO "\033[35m"
	#define CIANO "\033[36m"
	#define BRANCO "\033[37m"
	#define PRETO "\033[30m"
	#define NEGRITO "\033[1m"
	#define RESETCOLOR "\033[0m"
	#define BOOM printf("\033[%dm", rand()%7+31);
	#define STOP getchar();getchar();
#else
	#include <termios.h>
	#define CLEAR printf("\033c")
	#define VERMELHO "\033[31m"
	#define VERDE "\033[32m"
	#define AMARELO "\033[33m"
	#define AZUL "\033[34m"
	#define ROXO "\033[35m"
	#define CIANO "\033[36m"
	#define BRANCO "\033[37m"
	#define PRETO "\033[30m"
	#define NEGRITO "\033[1m"
	#define RESETCOLOR "\033[0m"
	#define BOOM printf("\033[%dm", rand()%7+31);
	#define STOP getchar();
#endif
#ifndef _WIN32
int kbhit(){
	struct termios oldt, newt;
	int ch, oldf;
	tcgetattr(STDIN_FILENO,&oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF){
		ungetc(ch,stdin);
		return 1;
	}
	return 0;
}
int getch(void) {
	int ch;
	struct termios oldt;
	struct termios newt;
	tcgetattr(STDIN_FILENO,&oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#else
	#include <conio.h>
#endif
typedef struct{
	char tipo;
	int vida;
}objeto;
typedef struct{
	char nick[11];
	int score;
}player;
/*	variaveis globais	*/
/*	combustivel recarregado por F	*/
int fuelrecharge=40;
/*	pontuacao por inimigo 	*/
int score_enemy = 50;
/*	combustivel inicial	*/
int start_fuel = 400;
/*	fuel combustivel da nave, quando chega a 0 = game over	*/
int fuel;
/*	score= pontuacao	*/
int score=0;
/*	life= vida extra, vida 0 e morrer game over	*/
int life=1;
/*	probabilidade de aparecer um inimigo	*/
int probX= 25;
/*	probabilidade de aparecer um combustivel	*/
int probF= 10;
/*	probabilidade de aparecer um bonus	*/
int probB= 2;
/* probabilidade de aparecer um O */
int probO= 1;
/* probabilidade de aparecer um T */
int probT=4;
/* vida do inimigo O */
int vidaO=10;
/* tiros do inimigo T */
int tirosT=5;
int probAT=2;
/*	bonus ativo ou nao	*/
int Bonus_Ativo=0;
/*	usos restantes do bonus atual*/
int buffuses=0;
/*	quantidade de usos ganhos por bonus*/
int bfrech=10;
/*	altura e largura do tabuleiro	*/
int altura=10;
int largura=135;
/* define se vai usar ou nao as configuracoes do arquivo */
int modorank = 0;
/*	velocidade do jogo em microsegundos	*/
int velocidade=60000;
/*	tamanho do tabuleiro base do jogo, diretamente alteravel atraves das variaveis altura e largura	*/
objeto mat[10][135];
/*	opcao seleiconada no menu*/
char opc=0;
char config=0;
int pausar=0; /*	se estiver pausado ou nao 	*/
/*	cheats	*/
/*	qualquer cheat ativado torna o combustivel infinito, menos invencibilidade e todos precisam de espaço para andar	*/
int super; /*	combo dos cheats	*/
int infbomb=0; /*	direita cima baixo direita, coisas usando essa variavel no codigo estao relacionados ao cheat*/
int infquad=0; /*	direita esquerda direita esquerda cima	*/
int invencibilidade=0; /* cima cima baixo baixo esquerda direita esquerda direita A B */
int airstrike=0; /* airstrike, juncao de tudo */
char c; /*	utilizado para verificacao de movimento	*/
/*	so pra parar os warnings*/
int menu();
int usleep();
int padraoconfig();
/* funcao que le o que estah guardado no arquivo	*/
int pegaconfig(){
	char conf[] = "config.txt";
	FILE *fd;
	fd = fopen(conf,"r");
	if(fd==NULL){
		padraoconfig();
	}
	else{	
		fscanf(fd,"Altura = %d\n",&altura);
		fscanf(fd,"Largura = %d\n",&largura);
		fscanf(fd,"ProbX = %d\n",&probX);
		fscanf(fd,"ProbF = %d\n",&probF);
		fscanf(fd,"ProbB = %d\n",&probB);
		fscanf(fd,"ProbO = %d\n",&probO);
		fscanf(fd,"ProbT = %d\n",&probT);
		fscanf(fd,"ProbAT = %d\n",&probAT);
		fscanf(fd,"Velocidade = %d\n",&velocidade);
		fscanf(fd,"VidaO = %d\n",&vidaO);
		fscanf(fd,"TirosT = %d\n",&tirosT);
		fscanf(fd,"ModoRankeado = %d\n",&modorank);
		fclose(fd);
	}
	return 1; /*	funcao responsavel por recuperar os valores salvos no arquivo*/
}
/* funcao responsavel por printar as mensagens da configuracao	*/
void printconfig(int tip){
	pegaconfig();
	if(tip==1){
		printf("[1] - "NEGRITO BRANCO"Tabuleiro\n"RESETCOLOR"[2] - ");
		BOOM;
		printf(NEGRITO"NPC\n"RESETCOLOR"[3]");
		if(modorank==0){
			printf(" - "NEGRITO VERDE"Ativar Modo Rankeado"RESETCOLOR);
		}
		else if(modorank==1){
			printf(" - "NEGRITO VERMELHO"Desativar Modo Rankeado"RESETCOLOR);
		}
		printf("\n[4] - "NEGRITO CIANO"Retornar Para o Padrao"RESETCOLOR"\n[5] - "AMARELO NEGRITO"Voltar"RESETCOLOR"\nEscolha sua opcao e aperte enter: ");
	}
	else if(tip==12){
		printf("[1] - "NEGRITO AZUL"Altura"RESETCOLOR"\n[2] - "NEGRITO ROXO"Largura"RESETCOLOR"\n[3] - "AMARELO NEGRITO"Voltar"RESETCOLOR"\nEscolha sua opcao e aperte enter: ");
	}
	else if(tip==13){
		printf(NEGRITO AZUL"Altura:\n"NEGRITO VERDE"Min [5]\n"NEGRITO VERMELHO"Max [20]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite a Altura Desejada:", altura);
	}
	else if(tip==14){
		printf(NEGRITO ROXO"Largura:\n"NEGRITO VERDE"Min[73]\n"NEGRITO VERMELHO"Max[135]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite a Largura Desejada:",largura);
	}
	else if(tip==2){
		printf("Chance de Nascer:\n[1] - "VERMELHO"X"RESETCOLOR"\n[2] - "AMARELO"F"RESETCOLOR"\n[3] - "NEGRITO VERDE"B"RESETCOLOR"\n[4] - "NEGRITO CIANO"O"RESETCOLOR"\n[5] - "ROXO"T"RESETCOLOR"\nDiferenciais:\n[6] - "VERDE"velocidade"RESETCOLOR"\n[7] - "VERMELHO NEGRITO"Vidas O"RESETCOLOR"\n[8] - "ROXO NEGRITO"Tiros T"RESETCOLOR"\n[9] - "VERDE NEGRITO"ChanceTatirar"RESETCOLOR"\n[q] - "AMARELO NEGRITO"Voltar"RESETCOLOR"\nEscolha sua opcao e aperte enter:");
	}
	else if(tip==21){
		printf("Chance de Nascer "VERMELHO"X"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[100]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probX);
	}
	else if(tip==22){
		printf("Chance de Nascer "AMARELO"F"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[100]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probF);
	}
	else if(tip==23){
		printf("Chance de Nascer "NEGRITO VERDE"B"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[10]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probB);
	}
	else if(tip==24){
		printf("Chance de Nascer "NEGRITO CIANO"O"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[10]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probO);
	}
	else if(tip==25){
		printf("Chance de Nascer "ROXO"T"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[10]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probT);
	}
	else if(tip==26){
		printf(VERDE"Velocidade"RESETCOLOR":\n"NEGRITO VERDE"Min[50000]\n"NEGRITO VERMELHO"Max[100000]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nQuanto Maior, Mais lento fica\nDigite o valor desejado: ",velocidade);
	}
	else if(tip==27){
		printf(NEGRITO VERMELHO"Vidas O"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[20]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",vidaO);
	}
	else if(tip==28){
		printf(NEGRITO ROXO"Tiros T"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[20]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",tirosT);
	}
	else if(tip==29){
		printf(VERDE NEGRITO"Chance T Atirar"RESETCOLOR":\n"NEGRITO VERDE"Min[1]\n"NEGRITO VERMELHO"Max[100]\n"NEGRITO AMARELO"Atual[%d]"RESETCOLOR"\nDigite o valor desejado: ",probAT);
	}
}
/*  funcao para alterar as configuracoes do Tabuleiro*/
int tabuleiro(){
	char tab=0;
	int valor;
	char conf[] = "config.txt";
	FILE *fd;
	while(tab!='3'){
		pegaconfig();
		CLEAR;
		printconfig(12);
		scanf(" %c", &tab);
		switch(tab){
			case '1':
				CLEAR;
				pegaconfig();
				printconfig(13);
				scanf("%d", &valor);
				while(valor<5 || valor>20){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(13);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '2':
				CLEAR;
				pegaconfig();

				printconfig(14);
				scanf("%d", &valor);
				while(valor<73 || valor>135){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(14);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			default:
			break;
		}
	}
	return 0;
}
/* funcao para mexer nas configuracoes dos inimigos */
int npc(){
	char tab=0;
	int valor;
	char conf[] = "config.txt";
	FILE *fd;
	while(tab!='q'){
		pegaconfig();
		CLEAR;
		printconfig(2);
		scanf(" %c", &tab);
		switch(tab){
			case '1':
				CLEAR;
				printconfig(21);
				scanf("%d", &valor);
				while(valor<1 || valor>100){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(21);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '2':
				CLEAR;
				printconfig(22);
				scanf("%d", &valor);
				while(valor<1 || valor>100){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(22);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '3':
				CLEAR;
				printconfig(23);
				scanf("%d", &valor);
				while(valor<1 || valor>10){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(23);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '4':
				CLEAR;
				printconfig(24);
				scanf("%d", &valor);
				while(valor<1 || valor>10){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(24);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '5':
				CLEAR;
				printconfig(25);
				scanf("%d", &valor);
				while(valor<1 || valor>10){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(25);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '6':
				CLEAR;
				printconfig(26);
				scanf("%d", &valor);
				while(valor<50000 || valor>100000){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(26);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '7':
				CLEAR;
				printconfig(27);
				scanf("%d", &valor);
				while(valor<1 || valor>20){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(27);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",tirosT);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '8':
				CLEAR;
				printconfig(28);
				scanf("%d", &valor);
				while(valor<1 || valor>20){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(28);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",probAT);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case '9':
				CLEAR;
				printconfig(29);
				scanf("%d", &valor);
				while(valor<1 || valor>100){
					CLEAR;
					printf("Valor Invalido, Digite Novamente\n");
					printconfig(29);
					scanf("%d", &valor);
				}
				fd = fopen(conf,"w");
				fprintf(fd, "Altura = ");
				fprintf(fd, "%d\n",altura);
				fprintf(fd, "Largura = ");
				fprintf(fd, "%d\n",largura);
				fprintf(fd, "ProbX = ");
				fprintf(fd, "%d\n",probX);
				fprintf(fd, "ProbF = ");
				fprintf(fd, "%d\n",probF);
				fprintf(fd, "ProbB = ");
				fprintf(fd, "%d\n",probB);
				fprintf(fd, "ProbO = ");
				fprintf(fd, "%d\n",probO);
				fprintf(fd, "ProbT = ");
				fprintf(fd, "%d\n",probT);
				fprintf(fd, "ProbAT = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "Velocidade = ");
				fprintf(fd, "%d\n",velocidade);
				fprintf(fd, "VidaO = ");
				fprintf(fd, "%d\n",vidaO);
				fprintf(fd, "TirosT = ");
				fprintf(fd, "%d\n",valor);
				fprintf(fd, "ModoRankeado = ");
				fprintf(fd, "%d\n",modorank);
				fclose(fd);
			break;
			case 'q':
			break;
			default:
			break;
		}
	}
	return 1;
}
/* funcao para ativar o modo rankeado */
int rank(){
	FILE *fd;
	char conf[] = "config.txt";
	pegaconfig();
	fd = fopen(conf,"w");
	if(modorank==1){
		fprintf(fd, "Altura = ");
		fprintf(fd, "%d\n",altura);
		fprintf(fd, "Largura = ");
		fprintf(fd, "%d\n",largura);
		fprintf(fd, "ProbX = ");
		fprintf(fd, "%d\n",probX);
		fprintf(fd, "ProbF = ");
		fprintf(fd, "%d\n",probF);
		fprintf(fd, "ProbB = ");
		fprintf(fd, "%d\n",probB);
		fprintf(fd, "ProbO = ");
		fprintf(fd, "%d\n",probO);
		fprintf(fd, "ProbT = ");
		fprintf(fd, "%d\n",probT);
		fprintf(fd, "ProbAT = ");
		fprintf(fd, "%d\n",probAT);
		fprintf(fd, "Velocidade = ");
		fprintf(fd, "%d\n",velocidade);
		fprintf(fd, "VidaO = ");
		fprintf(fd, "%d\n",vidaO);
		fprintf(fd, "TirosT = ");
		fprintf(fd, "%d\n",tirosT);
		fprintf(fd, "ModoRankeado = ");
		fprintf(fd, "0\n");
	}
	if(modorank==0){
		fprintf(fd, "Altura = ");
		fprintf(fd, "%d\n",altura);
		fprintf(fd, "Largura = ");
		fprintf(fd, "%d\n",largura);
		fprintf(fd, "ProbX = ");
		fprintf(fd, "%d\n",probX);
		fprintf(fd, "ProbF = ");
		fprintf(fd, "%d\n",probF);
		fprintf(fd, "ProbB = ");
		fprintf(fd, "%d\n",probB);
		fprintf(fd, "ProbO = ");
		fprintf(fd, "%d\n",probO);
		fprintf(fd, "ProbT = ");
		fprintf(fd, "%d\n",probT);
		fprintf(fd, "ProbAT = ");
		fprintf(fd, "%d\n",probAT);
		fprintf(fd, "Velocidade = ");
		fprintf(fd, "%d\n",velocidade);
		fprintf(fd, "VidaO = ");
		fprintf(fd, "%d\n",vidaO);
		fprintf(fd, "TirosT = ");
		fprintf(fd, "%d\n",tirosT);
		fprintf(fd, "ModoRankeado = ");
		fprintf(fd, "1\n");
	}
	fclose(fd);
	return 0;
}
/*funcao para fazer as configuracoes retornar ao padrao*/
int padraoconfig(){
	FILE *fd;
	char conf[]="config.txt";
	fd = fopen(conf,"w");
	fprintf(fd, "Altura = ");
	fprintf(fd, "10\n");
	fprintf(fd, "Largura = ");
	fprintf(fd, "135\n");
	fprintf(fd, "ProbX = ");
	fprintf(fd, "25\n");
	fprintf(fd, "ProbF = ");
	fprintf(fd, "10\n");
	fprintf(fd, "ProbB = ");
	fprintf(fd, "2\n");
	fprintf(fd, "ProbO = ");
	fprintf(fd, "1\n");
	fprintf(fd, "ProbT = ");
	fprintf(fd, "4\n");
	fprintf(fd, "ProbAT = ");
	fprintf(fd, "20\n");
	fprintf(fd, "Velocidade = ");
	fprintf(fd, "60000\n");
	fprintf(fd, "VidaO = ");
	fprintf(fd, "10\n");
	fprintf(fd, "TirosT = ");
	fprintf(fd, "5\n");
	fprintf(fd, "ModoRankeado = ");
	fprintf(fd, "0\n");
	fclose(fd);
	pegaconfig();
	return 0;
}
int configuracoes(){
	while(config!='5'){
		CLEAR;

		printconfig(1);
		scanf(" %c", &config);
		switch(config){
			case '1':
				tabuleiro();
			break;
			case '2':
				npc();
			break;
			case '3':
				rank();
			break;
			case '4':
				padraoconfig();
			break;
			case '5':
			break;
			default:
			break;
		}
	}
	return 0;
}

void gameover(int tip){
	CLEAR;
	printf("\t\t\t\t\t\t\t\t\t\t" NEGRITO CIANO "|" BRANCO "|" VERDE "|" VERMELHO "GAME OVER" VERDE "|" BRANCO "|" CIANO "|" RESETCOLOR "\n");
	if(tip==1){
		printf("\t\t\t\t\t\t\t\t\t\t" "Voce se Explodiu\n" "\t\t\t\t\t\t\t\t\t\t" "Sua pontuacao foi de:" CIANO "%d" RESETCOLOR , score);
	}
	else if(tip==2){
		printf("\t\t\t\t\t\t\t\t\t\t" "Voce foi atingido pelo INIMIGO\n" "\t\t\t\t\t\t\t\t\t\t" "Sua pontuacao foi de:" CIANO "%d" RESETCOLOR , score);
	}
	else if(tip==3){
		printf("\t\t\t\t\t\t\t\t\t\t" "Seu Combustivel acabou\n" "\t\t\t\t\t\t\t\t\t\t" "Sua pontuacao foi de:" CIANO "%d" RESETCOLOR , score);
	}
	else if(tip==4){
		printf("\t\t\t\t\t\t\t\t\t\t" "Voce foi atingido pelo tiro inimigo\n" "\t\t\t\t\t\t\t\t\t\t" "Sua pontuacao foi de:" CIANO "%d" RESETCOLOR , score);
	}
	printf("\n\n\n\nPressione Enter para Continuar:");
	STOP
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n" "\t\t\t\t\t\t\t\t\t\t" AMARELO "QUE A FORCA ESTEJA COM VOCE, Tente Novamente"  RESETCOLOR  "\n");
	usleep(25000);
}
/* funcao para fazer o efeito do O */
void matatuto(objeto mat[altura][largura]){
	int i,j;
	for(i=0;i<altura;i++){
		for(j=0;j<largura;j++){
			if(mat[i][j].tipo=='X'){
				mat[i][j].tipo='%';
				score+=10;
			}
		}
	}	
}

	/*	funcoes relacionadas a bonus	*/
void quadshot(objeto mat[altura][largura], int i, int j){
	if(super==1){
		if(mat[i][j+1].tipo=='X' || mat[i][j+2].tipo=='X' || mat[i+1][j+1].tipo=='X' || mat[i-1][j+1].tipo=='X'){
			/*	verificacoes se os proximos espacos a serem ocupados pelo tiro sao inimigos, td essa parte relacionado ao buff	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='*';
			}
			else if(mat[i][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='*';
			}
			else if(mat[i][j+2].tipo=='X'){
				score+=score_enemy;
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo=='F' || mat[i][j+2].tipo=='F' || mat[i+1][j+1].tipo=='F' || mat[i-1][j+1].tipo=='F'){
			/*	apenas verificacoes se os proximos espacos ocupados pelo tiro sao combustiveis	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='*';
			}
			else if(mat[i][j+1].tipo=='F'){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='*';
			}
			else if(mat[i][j+2].tipo=='F'){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='F'){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='F'){
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo=='B' || mat[i][j+2].tipo=='B' || mat[i+1][j+1].tipo=='B' || mat[i-1][j+1].tipo=='B'){
			/*	apenas verificacoes se os espacos que vao ser ocupados pelo tiro sao bonus	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='*';
			}
			else if(mat[i][j+1].tipo=='B'){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='*';
			}
			else if(mat[i][j+2].tipo=='B'){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='B'){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo=='B'){
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo==' ' || mat[i][j+2].tipo==' ' || mat[i+1][j+1].tipo==' ' || mat[i-1][j+1].tipo==' '){
			/*	apenas verificacoes se os espacos que vao ser ocupados pelo tiro sao vazios	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='*';
			}
			else if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='*';
			}
			else if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='*';
			}
			else if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo=' ';
			}
		}
	}
	else{
		if(mat[i][j+1].tipo=='X' || mat[i][j+2].tipo=='X' || mat[i+1][j+1].tipo=='X' || mat[i-1][j+1].tipo=='X'){
			/*	verificacoes se os proximos espacos a serem ocupados pelo tiro sao inimigos	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='>';
			}
			else if(mat[i][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='>';
			}
			else if(mat[i][j+2].tipo=='X'){
				score+=score_enemy;
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='X'){
				score+=score_enemy;
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo=='F' || mat[i][j+2].tipo=='F' || mat[i+1][j+1].tipo=='F' || mat[i-1][j+1].tipo=='F'){
			/*	apenas verificacoes se os proximos espacos ocupados pelo tiro sao combustiveis	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='>';
			}
			else if(mat[i][j+1].tipo=='F'){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='>';
			}
			else if(mat[i][j+2].tipo=='F'){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='F'){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='F'){
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo=='B' || mat[i][j+2].tipo=='B' || mat[i+1][j+1].tipo=='B' || mat[i-1][j+1].tipo=='B'){
			/*	apenas verificacoes se os espacos que vao ser ocupados pelo tiro sao bonus	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='>';
			}
			else if(mat[i][j+1].tipo=='B'){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='>';
			}
			else if(mat[i][j+2].tipo=='B'){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='B'){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo=='B'){
				mat[i+1][j+1].tipo=' ';
			}
		}
		else if(mat[i][j+1].tipo==' ' || mat[i][j+2].tipo==' ' || mat[i+1][j+1].tipo==' ' || mat[i-1][j+1].tipo==' '){
			/*	apenas verificacoes se os espacos que vao ser ocupados pelo tiro sao vazios	*/
			if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo='>';
			}
			else if(mat[i][j+1].tipo==' '){
				mat[i][j+1].tipo=' ';
			}
			if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo='>';
			}
			else if(mat[i][j+2].tipo==' '){
				mat[i][j+2].tipo=' ';
			}
			if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo=' ';
			}
			if(mat[i-1][j+1].tipo==' '){
				mat[i-1][j+1].tipo='>';
			}
			else if(mat[i+1][j+1].tipo==' '){
				mat[i+1][j+1].tipo=' ';
			}
		}
	}
}	/*	funcao para gerar o bonus de tiro quadruplo*/

void explosion(objeto mat[altura][largura], int i, int j){
	/*	funcao relacionada ao buff de tiros explosivos*/
	/*	verificacoes com relacao a pontuacoes e objetos	*/
	/*	quantidade imensa de verificacoes para impedir bugs	*/
	if(mat[i][j+1].tipo=='X'){
		score+=score_enemy;
		mat[i][j+1].tipo=' ';
		mat[i][j].tipo=' ';
	}
	else if(mat[i][j+2].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i+1][j+1].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i+1][j-1].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i-1][j+1].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i-1][j-1].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i+1][j].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i+2][j].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i-1][j].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i-2][j].tipo=='X'){
		score+=score_enemy;
	}
	else if(mat[i][j+1].tipo=='F'){
		mat[i][j+1].tipo=' ';
		mat[i][j].tipo=' ';
	}
	else if(mat[i][j+1].tipo=='B'){
		mat[i][j+1].tipo=' ';
		mat[i][j].tipo=' ';
	}
	mat[i][j].tipo='%';
	if (j+1!=largura-1){
		if(mat[i][j+1].tipo!='*'){
			mat[i][j+1].tipo='%';
		}
		if(mat[i][j+2].tipo!='*'){
			mat[i][j+2].tipo='%';
		}
		if(mat[i][j-1].tipo!='+'){
			mat[i][j-1].tipo='%';
		}
		else{
			life--;
			if(life<=0){
				gameover(1);
			}
		}
		if(mat[i][j-2].tipo!='+'){
			mat[i][j-2].tipo='%';
		}
		else{
			life--;
			if(life<=0){
				gameover(1);
			}
		}
		if(mat[i+1][j].tipo!='#'){
			if(mat[i+1][j].tipo!='+'){
				mat[i+1][j].tipo='%';
			}
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i+2][j].tipo!='#' && mat[i+1][j].tipo!='#'){
			if(mat[i+2][j].tipo!='+'){
				mat[i+2][j].tipo='%';
			}
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i-1][j].tipo!='#'){
			if(mat[i-1][j].tipo!='+')
				mat[i-1][j].tipo='%';
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i-2][j].tipo!='#' && mat[i-1][j].tipo!='#'){
			if(mat[i-2][j].tipo!='+'){
				mat[i-2][j].tipo='%';
			}
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i+1][j+1].tipo!='#'){
			if(mat[i+1][j+1].tipo!='*'){
				mat[i+1][j+1].tipo='%';
			}
			if(mat[i+1][j+1].tipo=='+'){
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i+1][j-1].tipo!='#'){
			if(mat[i+1][j-1].tipo!='*'){
				mat[i+1][j-1].tipo='%';
			}
			if(mat[i+1][j-1].tipo=='+'){
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i-1][j-1].tipo!='#'){
			if(mat[i-1][j-1].tipo!='+'){
				mat[i-1][j-1].tipo='%';
			}
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
		if(mat[i-1][j+1].tipo!='#'){
			if(mat[i-1][j+1].tipo!='+'){
				mat[i-1][j+1].tipo='%';
			}
			else{
				life--;
				if(life<=0){
					gameover(1);
				}
			}
		}
	}
	if(buffuses<=0){
		Bonus_Ativo=0;
	}
}

	/*	funcoes relacionadas a criacao e atualizacao da matriz	*/

void cria_mat(objeto mat[][largura]){
	/*	mostra a matriz inicial */
	int i, j;
	for(i=0;i<altura;i++){
		for(j=0;j<largura;j++){
			mat[i][j].tipo= ' ';
			if(i==altura/2 && j==0){
				mat[i][j].tipo= '+';
			}
			if(i==0 || i==altura-1){
				mat[i][j].tipo= '#' ;
			}
		}
	}
}

void updade_mat(objeto mat[][largura]){
	/*	atualiza a matriz com os comandos fornecidos, alem de gerar inimigos e combustivel	*/
	if(buffuses<=0){
		Bonus_Ativo=0;
	}
	int i, j, cont=0, linx, linf, linb,lino,lint, probx, probf, probb,probo,probt,probat, tipbuff;
	char aux;
	/*	linf eh uma variavel randomica para ver onde o f vai nascer(o mesmo para as variaveis linx e linb para x, b, o e t*/
	linf = rand()%altura+1;
	linx = rand()%altura+1;
	linb = rand()%altura+1;
	lino = rand()%altura+1;
	lint = rand()%altura+1;

	/*	variavel randomica para ver o tipo de bonus que sera recebido ao coletar o B*/
	tipbuff = rand()%3+1;
	probx = RAND;
	probf = RAND;
	probb = RAND;
	probo = RAND;
	probt = RAND;
	probat = RAND;
	c=' ';
	if(infbomb==4){
		/*	se o codigo for digitado corretamente o jogador fica com bombas infinitas	*/
		buffuses=999999;
		Bonus_Ativo=3;
		fuel=999999;
	}
	if(infquad==5){
		/*	se o codigo for digitado corretamente o jogador fica com bombas infinitas	*/
		buffuses=999999;
		Bonus_Ativo=1;
		fuel=999999;
	}
	if((infbomb==4 && infquad==5) || (airstrike==9)){
		infbomb=4;
		infquad=5;
		super=1;
		if(airstrike==9)
			invencibilidade=10;

	}
	if(modorank==1){
		infbomb=0;
		infquad=0;
		airstrike=0;
		invencibilidade=0;;
	}
	if(kbhit()){
		c = getch();
	}
	if(c=='p' || c=='P'){
		if(pausar==0){
			pausar=1;
		}
		else{
			pausar=0;
		}
	}
	if(!pausar){
		score++;
		fuel--;
		for(i=0;i<altura;i++){
			for(j=0;j<largura;j++){
				if(i==0 || i==altura-1){
					mat[i][j].tipo='#';
				}
				if(mat[i][j].tipo=='%'){
					mat[i][j].tipo=' ';
				}
				if(mat[i][j].tipo=='+' && (c=='w' || c=='W') && cont==0 && mat[i-1][j].tipo!='#'){
					/*	movimentacao vertical para cima */
					if(mat[i-1][j].tipo=='F'){
						/*	verificacao se o objeto acima eh um combustivel	*/
						mat[i-1][j].tipo=' ';
						fuel+=fuelrecharge;
					}
					if(mat[i-1][j].tipo=='X' || mat[i-1][j].tipo=='<' || mat[i-1][j].tipo=='T'){
						/*	verificacao se o objeto acima eh um inimigo 	*/
						if(invencibilidade!=10){ 
							life+=-1;
						}
						mat[i-1][j].tipo=' ';
						if(life<=0){
							gameover(2);
						}
					}
					mat[i-1][j].tipo = '+';
					mat[i][j].tipo = ' ';
					cont=1;
					if(Bonus_Ativo==2){
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else{
						fuel-=2;
					}
					if(infbomb==1 && infbomb<4){
						infbomb++;
					}
					else if(infbomb!=1 && infbomb<4){
						infbomb=0;
					}
					if(infquad==4 && infquad<5){
						infquad++;
					}
					else if(infquad!=4 && infquad<5){
						infquad=0;
					}
					if((invencibilidade==0 || invencibilidade==1) && invencibilidade<10){
						invencibilidade++;
					}
					else if((invencibilidade!=0 && invencibilidade!=1) && invencibilidade<10){
						invencibilidade=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='s' || c=='S') && cont==0 && mat[i+1][j].tipo!='#'){

					if(mat[i+1][j].tipo=='F'){
						/*	verificacao se o objeto abaixo eh um combustivel	*/

						mat[i+1][j].tipo=' ';
						fuel+=fuelrecharge;
					}
					if(mat[i+1][j].tipo=='X' || mat[i+1][j].tipo=='<' || mat[i+1][j].tipo=='T'){
						/*	verificacao se o objeto abaixo eh um inimigo	*/
						if(invencibilidade!=10){ 
							life+=-1;
						}
						mat[i+1][j].tipo=' ';
						if(life<=0){
							gameover(2);
						}
					}
					mat[i+1][j].tipo = '+';
					mat[i][j].tipo = ' ';
					cont=1;
					if(Bonus_Ativo==2){
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else{
						fuel-=2;
					}
					if(infbomb==2 && infbomb<4){
						infbomb++;
					}
					else if(infbomb!=2 && infbomb<4){
						infbomb=0;
					}
					if((invencibilidade==2 || invencibilidade==3) && invencibilidade<10){
						invencibilidade++;
					}
					else if((invencibilidade!=2 && invencibilidade!=3) && invencibilidade<10){
						invencibilidade=0;
					}
					if(airstrike==3 && airstrike<9){
						airstrike++;
					}
					else if(airstrike!=3 && airstrike<9){ 
						airstrike=0;
					}

				}
				if(mat[i][j].tipo=='+' && (c=='d' || c=='D') && cont==0 && j!=largura-2){
					/*	movimentacao horizontal para direita */
					if(mat[i][j+1].tipo=='F'){
						/*	verificacao se o objeto a direita eh um combustivel	*/
						mat[i-1][j].tipo=' ';
						fuel+=fuelrecharge;
					}
					if(mat[i][j+1].tipo=='X' || mat[i][j+1].tipo=='<' || mat[i][j+1].tipo=='T'){
						/*	verificacao se o objeto a direita eh um inimigo 	*/
						if(invencibilidade!=10){ 
							life+=-1;
						};
						mat[i][j+1].tipo=' ';
						if(life<=0){
							gameover(2);
						}
					}
					mat[i][j+1].tipo = '+';
					mat[i][j].tipo = ' ';
					cont=1;
					if(Bonus_Ativo==2){
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else{
						fuel-=2;
					}
					if((infbomb==0 || infbomb==3) && infbomb<4){
						infbomb++;
					}
					else if((infbomb!=0 && infbomb!=3) && infbomb<4){
						infbomb=0;
					}
					if((infquad==0 || infquad==2) && infquad<5){
						infquad++;
					}
					else if((infquad!=0 || infquad!=2) && infquad<5){
						infquad=0;
					}
					if((invencibilidade==5 || invencibilidade==7) && invencibilidade<10){
						invencibilidade++;
					}
					else if((invencibilidade!=5 && invencibilidade!=7) && invencibilidade<10){
						invencibilidade=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='a' || c=='A') && cont==0 && j!=0){
					/*	movimentacao horizontal para esquerda */
					mat[i][j-1].tipo = '+';
					mat[i][j].tipo = ' ';
					cont=1;
					if(Bonus_Ativo==2){
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else{
						fuel-=2;
					}
					if(infbomb<4){
						infbomb=0;
					}
					if((infquad==1 || infquad==3) && infquad<5){
						infquad++;
					}
					else if((infquad!=1 || infquad!=3) && infquad<5){
						infquad=0;
					}
					if((invencibilidade==4 || invencibilidade==6 || invencibilidade==9) && invencibilidade<10){
						invencibilidade++;
					}
					else if((invencibilidade!=4 && invencibilidade!=6 && invencibilidade!=9) && invencibilidade<10){
						invencibilidade=0;
					}
					if(airstrike==0 && airstrike<9){
						airstrike++;
					}
					else if(airstrike!=0 && airstrike<9){ 
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='k' || c=='K')){
					if(Bonus_Ativo==1){
						/*	se o buff tiver ativo, exercer seus efeitos de quadruplicar o tiro*/
						quadshot(mat, i, j);
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else if(Bonus_Ativo==3){
						if(mat[i][j+1].tipo=='X' || mat[i][j+1].tipo=='F' || mat[i][j+1].tipo=='O'){
						/*	verificacao se logo a frente do personagem eh ou nao um objeto	*/
							if(mat[i][j+1].tipo=='X'){
								score+=score_enemy;
							}
							if(mat[i][j+1].tipo=='O'){
								matatuto(mat);
							}
							mat[i][j+1].tipo=' ';
						}
						else{
							/*	tiro explosivo propriamente dito	*/
							mat[i][j+1].tipo = '*';
						}
						buffuses--;
						fuel--;
						if(infbomb<4)
							infbomb=0;
						if(infquad<5)
							infquad=0;
						if(invencibilidade<10)
							invencibilidade=0;
						
					}
					else
					/*	verificacao para atirar	*/
					if(mat[i][j+1].tipo=='X' || mat[i][j+1].tipo=='F' || mat[i][j+1].tipo=='O'){
						/*	verificacao se logo a frente do personagem eh ou nao um objeto	*/
					if(mat[i][j+1].tipo=='X'){
							score+=score_enemy;
						}
						if(mat[i][j+1].tipo=='O'){
							mat[i][j+1].vida--;
							if(mat[i][j+1].vida<=0){
								matatuto(mat);
							}
						}
						else{
							mat[i][j+1].tipo=' ';
						}
					}
					else{
						/*	tiro propriamente dito	*/
							mat[i][j+1].tipo = '>';
					}
					if(Bonus_Ativo==2){
						buffuses--;
						if(buffuses<=0){
							Bonus_Ativo=0;
						}
					}
					else{
						fuel-=3;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='b' || c=='B')){
					if(invencibilidade==8 && invencibilidade<10){
						invencibilidade++;
					}
					else if(invencibilidade!=8 && invencibilidade<10){
						invencibilidade=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='i' || c=='I')){
					if((airstrike==1 || airstrike==6) && airstrike<9){
						airstrike++;
					}
					else if((airstrike!=1 && airstrike!=6) && airstrike<9){
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='r' || c=='R')){
					if((airstrike==2 || airstrike==5) && airstrike<9){
						airstrike++;
					}
					else if((airstrike!=2 && airstrike!=5) && airstrike<9){
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='t' || c=='T')){
					if(airstrike==4 && airstrike<9){
						airstrike++;
					}
					else if(airstrike!=4 && airstrike<9){
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='e' || c=='E')){
					if(airstrike==8 && airstrike<9){
						airstrike++;
					}
					else if(airstrike!=8 && airstrike<9){
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='k' || c=='K')){
					if(airstrike==7 && airstrike<9){
						airstrike++;
					}
					else if(airstrike!=7 && airstrike<9){
						airstrike=0;
					}
				}
				if(mat[i][j].tipo=='+' && (c=='l' || c=='L')){
					/*	reset de todos os bonus 	*/
					airstrike=0;
					infbomb=0;
					infquad=0;
					invencibilidade=0;
					Bonus_Ativo=0;
					buffuses=0;
					super=0;
					fuel=start_fuel;
				}
				if((i==linx || i==linf || i==linb) && j==largura-1 && linf!=0 && linf!=altura-1 && linx!=0 && linx!=altura-1 && i!=altura-1 && linb!=altura-1){
					if(probf<=probF && mat[i][j-1].tipo!='F' && mat[linf][largura-2].tipo!='F' && mat[linf][largura-1].tipo!='F'){
						/*	verifica se o F vai nascer ou nao	*/
						mat[linf][j].tipo='F';
					}
					if(probx<=probX && linx!=linf && mat[i][j-1].tipo!='X' && mat[linx][largura-2].tipo!='X' && mat[linf][largura-1].tipo!='X'){
						/*	verifica se o X vai nascer ou nao	*/
						mat[linx][j].tipo='X';
					}
					if(probb<=probB && linb!=linf && linb!=linx && mat[i][j-1].tipo!='B' && mat[linb][largura-2].tipo!='B' && mat[linf][largura-1].tipo!='B'){
						/*	verifica se o B vai nascer ou nao	*/
						mat[linb][j].tipo='B';
					}
					if(probo<=probO && lino!=linf && lino!=linx && lino!=linb && mat[i][j-1].tipo!='O' && mat[linb][largura-2].tipo!='O' && mat[linf][largura-1].tipo!='O'){
						/*	verifica se o O vai nascer ou nao	*/
						mat[linb][j].tipo='O';
						mat[linb][j].vida=vidaO;
					}
					if(probt<=probT && lint!=linf && lint!=linx && lint!=linb && lint!=lino && mat[i][j-1].tipo!='T' && mat[linb][largura-2].tipo!='T' && mat[linf][largura-1].tipo!='T'){
						/*	verifica se o T vai nascer ou nao	*/
						mat[linb][j].tipo='T';
						mat[linb][j].vida=tirosT;
					}
				}
				if(mat[i][j].tipo=='<'){
					if(mat[i][j-2].tipo=='+' || mat[i][j-1].tipo=='+'){
						life--;
						mat[i][j].tipo=' ';
						if(life<=0){
							gameover(4);
						}
					}
					if(mat[i][j-2].tipo=='>'){
						mat[i][j].tipo=' ';
						mat[i][j-2].tipo=' ';
					}
					if(mat[i][j-1].tipo=='>'){
						mat[i][j].tipo=' ';
						mat[i][j-1].tipo=' ';
					}
					if(mat[i][j-2].tipo=='*' || mat[i][j-1].tipo=='*'){
						explosion(mat,i,j);
					}
					if(j-2<=0){
						mat[i][j].tipo=' ';
					}
					else if(mat[i][j].tipo=='<'){
						if(mat[i][j-2].tipo==' '){
							mat[i][j-2].tipo='<';
							mat[i][j].tipo=' ';
						}
						else{
							if(mat[i][j-3].tipo=='+'){
								mat[i][j].tipo=' ';
								life--;
								if(life<=0){
									gameover(4);
								}
							}
							else{
								mat[i][j-3].tipo='<';
								mat[i][j].tipo=' ';
							}
						}
					}
				}
				if(mat[i][j].tipo=='F' && j!=0){
					/*	movimentacao do combustivel	*/
					if(j-1==0){
						/*	verificacao se chegou a esquerda da matriz*/
						mat[i][j].tipo=' ';
					}
					if(mat[i][j-1].tipo=='>' || mat[i][j-1].tipo=='*'){
						/*	verificacao se o combustivel se chocou contra um tiro	*/
						if(mat[i][j-1].tipo=='*' && j!=largura-1){
							explosion(mat, i, j+1);
						}
						else{
							mat[i][j].tipo=' ';
							mat[i][j-1].tipo=' ';
						}
					}
					if(mat[i][j-1].tipo=='+'){
						/*	verificacao se encontrou com o jogador	e incremento de combustivel	*/
						mat[i][j].tipo=' ';
						fuel+=fuelrecharge;
					}
					else{
						mat[i][j-1]=mat[i][j];
						mat[i][j].tipo=' ';
					}
				}
				if(mat[i][j].tipo=='B' && j!=0){
					/*	movimentacao do combustivel	*/
					if(j-1==0){
						/*	verificacao se chegou a esquerda da matriz*/
						mat[i][j].tipo=' ';
					}
					if(mat[i][j-1].tipo=='>' || mat[i][j-1].tipo=='*'){
						/*	verificacao se o bonus se chocou com um tiro	*/
						if(mat[i][j-1].tipo=='*' && j!=largura-1){
							explosion(mat, i, j+1);
						}
						else{
							mat[i][j].tipo=' ';
							mat[i][j-1].tipo=' ';
						}
					}
					if(mat[i][j-1].tipo=='+'){
						/*	verificacao se encontrou com o jogador	e incremento de combustivel	*/
						mat[i][j].tipo=' ';
						buffuses=bfrech;
						Bonus_Ativo=tipbuff;
					}
					else{
						mat[i][j-1]=mat[i][j];
						mat[i][j].tipo=' ';
					}
				}
				if(mat[i][j].tipo=='X' && j!=0){
					/*	movimentacao do inimigo	*/
					if(j-1==0){
						/*	verificacao se chegou a esquerda da matriz	*/
						mat[i][j].tipo=' ';
					}
					if(mat[i][j-1].tipo=='>' || mat[i][j-1].tipo=='*'){
						/*	verificacao se o inimigo se chocou com um tiro em um tiro	*/
						if(mat[i][j-1].tipo=='*' && j!=largura-1){
							explosion(mat, i, j+1);
						}
						else{
							score+=score_enemy;
							mat[i][j].tipo=' ';
							mat[i][j-1].tipo=' ';
						}
					}
					if(mat[i][j-1].tipo=='+'){
						/*	verificacao se o X se encontrou com o jogador morte	*/
						if(invencibilidade!=10){ 
							life+=-1;
						}
						mat[i][j].tipo=' ';
						if(life<=0){
							gameover(2);
						}
					}
					else{
						mat[i][j-1].tipo=mat[i][j].tipo;
						mat[i][j].tipo=' ';
					}
				}
				if(mat[i][j].tipo=='O' && j!=0){
					/* movimentacao do O */
					if(j-1==0){
						/*	verificacao se chegou a esquerda da matriz	*/
						mat[i][j].tipo=' ';
					}
					if(mat[i][j-1].tipo=='>' || mat[i][j-1].tipo=='*'){
						/*	verificacao se o inimigo se chocou com um tiro em um tiro	*/
						if(mat[i][j-1].tipo=='*' && j!=largura-1){
							mat[i][j].tipo=' ';
							matatuto(mat);
						}
						else{
							mat[i][j].vida--;
							mat[i][j-1].tipo=' ';
							if(mat[i][j].vida<=0){
								mat[i][j].tipo=' ';
								matatuto(mat);
							}
						}
					}
					if(mat[i][j-1].tipo=='+'){
						/*	verificacao se o O se encontrou com o jogador	*/
						mat[i][j].tipo=' ';
						mat[i][j-1].tipo='+';
					}
					else{
						if(mat[i][j].tipo=='O'){
							mat[i][j-1].tipo='O';
							mat[i][j-1].vida=mat[i][j].vida;
							mat[i][j].vida=0;
							mat[i][j].tipo=' ';
						}
						else{	
							mat[i][j-1].tipo=mat[i][j].tipo;
							mat[i][j].tipo=' ';
						}
					}
				}
				if(mat[i][j].tipo=='T' && j!=0){
					/* movimentacao do T */
					if(j-1==0){
						/*	verificacao se chegou a esquerda da matriz	*/
						mat[i][j].tipo=' ';
					}
					if(mat[i][j-1].tipo=='F' && mat[i][j-1].tipo=='X' && mat[i][j-1].tipo=='O' && mat[i][j-1].tipo=='<' && mat[i][j-2].tipo=='B' && mat[i][j-2].tipo=='T'){
						if(mat[i][j-2].tipo!='F' && mat[i][j-2].tipo!='X' && mat[i][j-2].tipo!='O' && mat[i][j-2].tipo!='<' && mat[i][j-2].tipo!='B' && mat[i][j-2].tipo!='T'){
							if(probat<=probAT && mat[i][j].vida>0){
								mat[i][j-3].tipo='<';
								mat[i][j].vida--;
							}
						}
					}
					else{
						if(probat<=probAT && mat[i][j].vida>0){
							mat[i][j-2].tipo='<';
							mat[i][j].vida--;
						}
					}
					if(mat[i][j-1].tipo=='>' || mat[i][j-1].tipo=='*'){
						/*	verificacao se o inimigo se chocou com um tiro	*/
						if(mat[i][j-1].tipo=='*' && j!=largura-1){
							explosion(mat,i,j);
						}
						else{
							mat[i][j].tipo=' ';
							mat[i][j-1].tipo=' ';
						}
					}
					if(mat[i][j-1].tipo=='+'){
						/*	verificacao se o T se encontrou com o jogador	*/
						mat[i][j].tipo=' ';
						life--;
						if(life<=0){
							gameover(2);
						}
					}
					else{
						if(mat[i][j].tipo=='T'){
							mat[i][j-1].tipo='T';
							mat[i][j-1].vida=mat[i][j].vida;
							mat[i][j].vida=0;
							mat[i][j].tipo=' ';
						}
						else{	
							mat[i][j-1].tipo=mat[i][j].tipo;
							mat[i][j].tipo=' ';
						}
					}
				}
			}
		}
		for(i=0;i<altura;i++){
			for(j=largura-1;j>0;j--){
				/*	movimentacao do projetil/tiro	*/
				if(mat[i][j].tipo=='>'){
					if(j==largura-1){
						/*	verificacao se chegou a direita da matriz	*/
						mat[i][j].tipo=' ';
					}
					if((mat[i][j+1].tipo=='X' || mat[i][j+1].tipo=='F' || mat[i][j+1].tipo=='B' || mat[i][j+1].tipo=='O' || mat[i][j+1].tipo=='T' || mat[i][j+1].tipo=='<') && j+1!=largura-1){
						/*	verificacao se o tiro se chocou com um combustivel ou inimigo	*/
						if(mat[i][j+1].tipo=='X' || mat[i][j].tipo=='X'){
							score+=score_enemy;
						}
						if(mat[i][j+1].tipo=='O'){
							mat[i][j].tipo=' ';
							mat[i][j+1].vida--;
							if(mat[i][j+1].vida==0){
								mat[i][j+1].tipo=' ';
								matatuto(mat);
							}
						}
						else{
							mat[i][j].tipo=' ';
							mat[i][j+1].tipo=' ';
						}
					}
					if(j!=largura-1 && j!=largura){
						if(mat[i][j+1].tipo=='O'){
							mat[i][j+1].tipo='O';
							mat[i][j].tipo=' ';
						}
						else{
							aux=mat[i][j].tipo;
							mat[i][j].tipo=mat[i][j+1].tipo;
							mat[i][j+1].tipo=aux;
						}
					}
				}
				if(mat[i][j].tipo=='*'){
					if(j==largura-1){
						/*	verificacao se chegou a direita da matriz	*/
						mat[i][j].tipo=' ';
					}
					if((mat[i][j+1].tipo=='X' || mat[i][j+1].tipo=='F' || mat[i][j+1].tipo=='B' || mat[i][j+1].tipo=='T' || mat[i][j+1].tipo=='<') && j+1!=largura-1){
						/*	verificacao se o tiro se chocou com um combustivel ou inimigo	*/
						explosion(mat, i, j+1);
					}
					if(mat[i][j+1].tipo=='O'){
						mat[i][j+1].tipo=' ';
						matatuto(mat);
					}
					if(j!=largura-1 && j!=largura){
						aux=mat[i][j].tipo;
						mat[i][j].tipo=mat[i][j+1].tipo;
						mat[i][j+1].tipo=aux;
					}
				}
			}
		}
	}
	if(fuel<=0){
		/*	se o combustivel acabar	*/
		life=-1;
		if(life<=0){
			gameover(3);
		}
	}
}
void printmat(objeto mat[][largura]){
	/*	printa uma matriz e colore ela: terminados em 1: vermelho, 2: verde, 4: azul, 7: branco, 3:amarelo; iniciados em 4: fundo, 3: caractere	*/
	int i, j;
	CLEAR;
	printf(AMARELO "FUEL:%d"  CIANO  " SCORE:%d " VERMELHO RESETCOLOR , fuel, score);
	if(airstrike==9){
		printf( VERDE  " Buff Ativo:");BOOM
		printf(" AIRSTRIKE    CARGAS, PRA QUE"RESETCOLOR"\n");
	}
	else if(super==1){
		printf( VERDE  " Buff Ativo:");BOOM
		printf(" SUPER  "VERDE"  Cargas do Buff:");BOOM
		printf(" %d"  RESETCOLOR  "\n", buffuses);
	}
	else if (invencibilidade==10){
		printf( VERDE  " Buff Ativo:"VERMELHO" KONAMI:"VERMELHO" %d"  RESETCOLOR  "\n", buffuses);

	}
	else if(Bonus_Ativo==1){
		printf( VERDE  " Buff Ativo: Tiro Quadruplo " " Cargas do Buff: %d"  RESETCOLOR  "\n", buffuses);
	}
	else if(Bonus_Ativo==2){
		printf( VERDE  " Buff Ativo: Nao Gastar Combustivel " " Cargas do Buff: %d"  RESETCOLOR  "\n", buffuses);
	}
	else if(Bonus_Ativo==3){
		printf( VERDE  " Buff Ativo: Tiro Explosivo " " Cargas do Buff: %d"  RESETCOLOR  "\n", buffuses);
	}
	else{
		printf("\n");
	}
	for(i=0;i<altura;i++){
		for(j=0;j<largura;j++){
			if(mat[i][j].tipo=='#'){
				printf( "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== ' '){
				printf("%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== '+'){
				if(invencibilidade==10){
				BOOM
					printf( NEGRITO  "%c"  RESETCOLOR , mat[i][j].tipo);	
				}
				else{
					printf( CIANO  "%c"  RESETCOLOR , mat[i][j].tipo);
				}
			}
			else if(mat[i][j].tipo== '>'){
				printf(NEGRITO CIANO "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== '*'){
				BOOM
				printf(NEGRITO "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== 'F'){
				printf(AMARELO "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== 'X'){
				printf(VERMELHO "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== 'B'){
				printf( VERDE  "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== 'O'){
				printf( NEGRITO AZUL  "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== 'T'){
				printf( ROXO  "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo=='<'){
				printf( NEGRITO ROXO  "%c"  RESETCOLOR , mat[i][j].tipo);
			}
			else if(mat[i][j].tipo== '%'){
				printf(AMARELO "%c"  RESETCOLOR , mat[i][j].tipo);
			}
		}
		printf("\n");
	}
	if(c!='K' && c!='k' && c!='w' && c!='W' && c!='S' && c!='s' && c!='A' && c!='a' && c!='D' && c!='d' && c!=' ' && c!='p' && c!='P' && pausar!=1){
		printf("Movimento Invalido");
	}
	if(pausar==1){
		printf("JOGO PAUSADO");
	}
	printf("\n");
}
	/* funcoes relacionadas ao Ranking	*/
void verificanick(char nome[50]){
	int i;

	printf("Digite Seu Nick, Maximo 10 Letras:\n");
	scanf("%s", nome);
	for(i=0;nome[i]!='\0';i++){
	}
	while(i>10){
		CLEAR;
		printf("Nick Invalido\nDigite Seu Nick, Novamente\n");
		scanf("%s", nome);
		for(i=0;nome[i]!='\0';i++){
		}
	}
}
void ordenarank(player rank[11]){
	int i,j,k;
	player aux;
	for(i=0;i<11;i++){
		for(j=0;j<11-1;j++){
			if(rank[j].score<rank[j+1].score){
				for(k=0;k<11;k++){
					aux.nick[k] = rank[j].nick[k];
					rank[j].nick[k] = rank[j+1].nick[k];
					rank[j+1].nick[k] = aux.nick[k];			
				}
				aux.score = rank[j].score;
				rank[j].score = rank[j+1].score;
				rank[j+1].score = aux.score;
			}
		}
	}
}
/* 	ve se o ranking existe e foi aberto	*/
int verificarank(player rank[11],char nome[50],int score){
	FILE *fd;
	int i;
	int cont=0;
	char ranking[] = "ranking.bin";
	fd = fopen(ranking,"rb");
	if(fd==NULL){
		fd = fopen(ranking,"wb");
		for(i=0;i<11;i++){
			rank[i].score = 0;
			rank[i].nick[0]='\0';
		}
		for(i=0;i<11;i++){
			rank[10].nick[i]=nome[i];
		}
		rank[10].score=score;
		ordenarank(rank);
		fwrite(rank, sizeof(player),11,fd);
		cont=1;
	}
	fclose(fd);
	if(cont==0){
		return 1;
	}
	return 0;
} 
/* recupera do arquivo o ranking ja salvo */   
void pegarank(player rank[11]){
	FILE *fd;
	char ranking[] = "ranking.bin";
	fd = fopen(ranking,"rb");
	fread(rank,sizeof(player),11,fd);
	fclose(fd);
}
/* mostra o ranking na tela */
void mostrarank(){
	FILE *fd;
	int i;
	player rank[11];
	char ranking[]="ranking.bin";
	fd = fopen(ranking,"rb");
	if(fd==NULL){
		printf("Ranking:\n\n\t\tNome:\t\tScore:\n\n");
		for(i=0;i<10;i++){
		printf("\t\t \t\t0\n");
		}
		printf("NINGUEM JOGOU AINDA");
		printf("\nPressione Enter Para Voltar:\n");
		STOP;
		getchar();
	}
	else{	
		fread(rank, sizeof(player),11,fd);
		printf("Ranking:\n\n\t\tNome:\t\tScore:\n\n");
		for(i=0;i<10;i++){
			if(i==0){
				printf(AMARELO NEGRITO);
			}
			if(i==1){
				printf(NEGRITO);
			}
			if(i==2){
				printf(AMARELO);
			}
			if(i==3){
				printf(AZUL NEGRITO);
			}
			if(i==4){
				printf(AZUL);
			}
			if(i==5){
				printf(ROXO NEGRITO);
			}
			if(i==6){
				printf(ROXO);
			}
			if(i==7){
				printf(PRETO NEGRITO);
			}
			if(i==8){
				printf(VERMELHO );
			}
			if(i==9){
				printf(VERMELHO NEGRITO);
			}
			printf("\t\t%s\t\t%d\n"RESETCOLOR,rank[i].nick,rank[i].score);

		}
		fclose(fd);
		printf("\nPressione Enter Para Voltar:\n");
		STOP;
		getchar();
	}
}

/* executa todas as funcoes e comando relacionados ao ranking  */
void registermatch(player rank[11], char nome[50],int score){
	FILE *fd;
	int i;
	char ranking[] = "ranking.bin";
	int c=verificarank(rank,nome,score);
	if(c){
		pegarank(rank);
		rank[10].score = score;
		for(i=0;i<11;i++){
			rank[10].nick[i]=nome[i];
		}
		ordenarank(rank);
		fd = fopen(ranking,"wb");
		fwrite(rank,sizeof(player),11,fd);
		fclose(fd);
	}
}


	/*	funcoes base	*/

void jogar(){
	/*	chama algumas funcoes	*/
	CLEAR;
	pegaconfig();
	fuel=start_fuel;
	srand(time(0));
	objeto mat[altura][largura];
	cria_mat(mat);
	char nome[50];
	player rank[11];
	if(modorank==1){
		altura = 10;
		largura =135;
		cria_mat(mat);
		probX = 25;
		probF = 10;
		probB = 2;
		probO = 1;
		probT = 4;
		probAT= 20;
		velocidade = 60000;
		vidaO = 10;
		tirosT = 5;
		verificanick(nome);
	}
	while(life>0){
		printmat(mat);
	    updade_mat(mat);
	    usleep(velocidade);
	}
	if(modorank==1){
		registermatch(rank,nome,score);
	}
	CLEAR;
	life=1;
	score = 0;
	Bonus_Ativo = 0;
	buffuses = 0;
	fuel=start_fuel;
	infbomb=0;
	infquad=0;
	super=0;
	invencibilidade=0;
	airstrike=0;
}

int menu(){
	/*	menuzin do jogo	*/
	int i;
	opc=1;
	while(opc!='1' && opc!='5'){
		CLEAR;
		printf("[1] - "VERDE NEGRITO"Jogar"RESETCOLOR"\n[2] - "AMARELO"Configuracoes"RESETCOLOR"\n[3] - "AMARELO NEGRITO"Ranking"RESETCOLOR"\n[4] - "NEGRITO"Instrucoes"RESETCOLOR"\n[5] - "VERMELHO NEGRITO"Sair"RESETCOLOR"\nEscolha sua opcao e Pressione Enter: ");
		scanf(" %c", &opc);
		switch (opc)
		{
			case '1' :
				jogar();

			break;
			case '2' :
				config=0;
				configuracoes();
			break;
			case '3' :
				CLEAR;
				mostrarank();
			break;
			case '4' :
				CLEAR;
				printf(					
					"\t\t\t\tINSTRUCOES:\n\n\
					"NEGRITO AZUL"W Mover para Cima\n\
					S Mover para Baixo\n\
					A Mover para a Esquerda\n\
					D Mover para a Direita\n\
					K Atirar\n\
					P Pausar o Jogo\n\
					\n\
					"VERMELHO"Tipos de Inimigo:\n\
					O nao te mata, mas eh necessario varios tiros para matar ele\n\
					e se matah-lo todos os X morrem lhe dando 10 pontos por cada X morto\n\
					caso esteja com o tiro explosivo, o O morre com um tiro\n\
					X te mata, mas morre com apenas um tipo e te da 50 pontos\n\
					T nao te da pontos, te mata e tambem atira projeteis que te mata\n\
					Fim de Jogo: Ser atingido ou ter combustivel zerado ou se explodir\n\
					\n\
					"VERDE"Tipos de bonus:\n\
					1:Tiro quadruplo\n\
					2:Nao gastar Combustivel por 10 acoes\n\
					3:Tiro Explosivo(ele pode te matar), ele mata os inimigos fortalecidos em um tiro\n\
					"CIANO"Bater no "AMARELO"Combustivel"CIANO" ou no "VERDE"Bonus "CIANO"para pegah-los\n\
					\n\
					"RESETCOLOR"Pressione Enter para retornar ao Menu\n\
					");
				for(i=0;i<2;i++){
					getchar();
				}
				return 0;
			break;
			case '5' :
				CLEAR;
				return 0;
			default:
			break;
		}
	}
	return 0;
}

int main(){
	CLEAR;
	printf("\t\t\t\t\t\t\t\t\t\t"NEGRITO"Seja " VERDE "Bem Vindo" RESETCOLOR ""NEGRITO" ao " CIANO "Air "VERMELHO"Strike" RESETCOLOR "\n\n\n\t\t\t\t\t\t\t\t\t\tPressione Enter para continuar:");
	getchar();
	while (opc!='5'){
		menu();
	}
	return 0;
}