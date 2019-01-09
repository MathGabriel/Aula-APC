/*
Universidade de Brasilia
Departamento de Ciencia da Computacao
Instituto de Ciencias Exatas
Algoritmos e Programação de Computadores - 2/2018
Aluno: Matheus Gabriel da Silva Rodrigues
Matricula: 180025031
Curso: Computacao(Licenciatura)
Versão do compilador: <versao utilizada>
Descricao:  Implementar um jogo similar ao jogo "Tetris", que vamos chamar de "DIY Puyo Puyo".
*/
/*Declaracao de bibliotecas*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
/*alguns termos que usarei bastante ao longo do codigo*/
#define CLEAR printf("\033c");
#define VERMELHO "\033[31m"
#define VERDE "\033[32m"
#define AMARELO "\033[33m"
#define AZUL "\033[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define NEGRITO "\033[1m"
#define RESETCOLOR "\033[0m"
#define STOP getchar();getchar();
/*A seguir a funcao que me deram nas 
especificacoes para quando for windows*/
#ifndef _WIN32
#include <termios.h>
#include <fcntl.h>
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
/*
VARIAVEIS GLOBAIS
*/
char largura=8;
char altura=12;
int velocidade=720000;
int x1,x2,y1,y2;
int verifverti=0;
/* Para usar a velocidade 
sem aparecer um warning*/
void usleep();

/* Na funcao PrePrint eh onde tem 
primeira tela do jogo
ou seja a tela de boas vindas*/
void PrePrint(){
	printf(VERMELHO NEGRITO"\t\tSeja Bem vindo!!\n"RESETCOLOR);
	printf("\t\t"NEGRITO"Ao Jogo "VERDE"'DIY Puyo Puyo'\n"RESETCOLOR);
	printf(NEGRITO"\t\tPara Iniciar o Jogo tecle enter: \n"RESETCOLOR);
	getchar();
}
/* Na funcao  Print eh o menu onde ele 
digita para qual funcao deseja ir*/
void Print(){
	printf("1 - "VERDE "Jogar\n"RESETCOLOR);
	printf("2 - "AMARELO"Instrucoes\n"RESETCOLOR);
	printf("3 - "NEGRITO"Configuracoes\n"RESETCOLOR);
	printf("4 - "NEGRITO"Ranking\n"RESETCOLOR);
	printf("5 - "CYAN "Sair\n"RESETCOLOR);
	printf(""NEGRITO"Digite o numero correspondente a opcao desejada: \n"RESETCOLOR);
}
/*Caso o usuario digite "D"
a funcao move as pecas para a direita*/
void andarDireita(char matriz[][largura]){
	int aux,aux2;
	if (verifverti==0){
		aux2 = matriz[y2][x2+1]; 
		matriz[y2][x2+1] = matriz[y2][x2];
		matriz[y2][x2] = aux2;
		x2++;
		aux = matriz[y1][x1+1]; 
		matriz[y1][x1+1] = matriz[y1][x1];
		matriz[y1][x1] = aux;
		x1++;
	}
	else{
		aux2 = matriz[y2][x2+1]; 
		matriz[y2][x2+1] = matriz[y2][x2];
		matriz[y2][x2] = aux2;
		x2++;
		aux = matriz[y1-1][x1+2]; 
		matriz[y1-1][x1+2] = matriz[y1-1][x1+1];
		matriz[y1-1][x1+1] = aux;
		x1++;
	}
}
/*Caso o usuario digite "A"
a funcao move as pecas para a esquerda*/
void andarEsquerda(char matriz[][largura]){
	int aux,aux2;
	if (verifverti==0){
		aux = matriz[y1][x1-1];
		matriz[y1][x1-1] = matriz[y1][x1];
		matriz[y1][x1] = aux;
		x1--;
		aux2 = matriz[y2][x2-1];
		matriz[y2][x2-1] = matriz[y2][x2];
		matriz[y2][x2] = aux2;
		x2--;
	}
	else{
		aux = matriz[y1-1][x1];
		matriz[y1-1][x1] = matriz[y1-1][x1+1];	
		matriz[y1-1][x1+1] = aux;
		x1--;
		aux2 = matriz[y2][x2-1];
		matriz[y2][x2-1] = matriz[y2][x2];
		matriz[y2][x2] = aux2;
		x2--;		
	}
}
/*Caso o usuario digite "S"
a funcao move as pecas mais rapido para baixo*/
void DescerMaisRapido(char matriz[][largura]){
	int aux,aux2;
	aux = matriz[y1+1][x1];
	matriz[y1+1][x1] = matriz[y1][x1];
	matriz[y1][x1] = aux;
	y1++;
	aux2 = matriz[y2+1][x2];
	matriz[y2+1][x2] = matriz[y2][x2];
	matriz[y2][x2] = aux2;
	y2++;
}
/*Caso o usuario digite "S" e esteja na vertical
a funcao move as pecas mais rapido para baixo*/
void DescerMaisRapidoVertical(char matriz[][largura]){
	int aux,aux2;
	aux2 = matriz[y2+1][x2];
	matriz[y2+1][x2] = matriz[y2][x2];
	matriz[y2][x2] = aux2;
	y2++;
	aux = matriz[y1][x1+1];
	matriz[y1][x1+1] = matriz[y1-1][x1+1];
	matriz[y1-1][x1+1] = aux;
	y1++;
}
/*Caso o usuario digite "L"
a funcao faz com que as peças gire*/
void GirandoPontos(char matriz[][largura]){
	if(matriz[y1-1][x1+1] == ' '){
		matriz[y1-1][x1+1] = matriz[y1][x1];
		matriz[y1][x1]=' ';
	}
}
/*Caso o usuario digite "R"
a funcao faz com uma peça troca com a outra*/
void MudarOrdem(char matriz[][largura]){
	int aux;
	if (verifverti == 0){
		aux = matriz[y1][x1];
		matriz[y1][x1]= matriz[y2][x2];
		matriz[y2][x2]= aux;
	}
	else if (verifverti == 1){
		aux = matriz[y1-1][x1+1];
		matriz[y1-1][x1+1]= matriz[y2][x2];
		matriz[y2][x2]= aux;
	}
}

void VoltaPontos(char matriz[][largura]){
	if(matriz[y1][x1] == ' '){
		matriz[y1][x1] = matriz[y1-1][x1+1];
		matriz[y1-1][x1+1]=' ';
	}
}
/* Aqui o se verifica se o usuario 
apertou o botao "D","A","S",W",E" ou "Q"
e dependendo de qual clicar ele leva pras
3 funcoes acima dessa.*/
void AndandoClique(char matriz[][largura]){
	char mvm;	
	if (kbhit()){
		mvm=getch();
		if (mvm =='d' || mvm =='D'){
			if (matriz[y2][x2+1] != '#' && matriz[y2][x2+1] == ' ' && matriz[y2+1][x2] == ' ' &&matriz[y1+1][x1] == ' '){
				andarDireita(matriz);
			}
		}
		else if (mvm =='a' || mvm =='A'){
			if (matriz[y1][x1-1] != '#' && matriz[y1][x1-1] == ' ' ){
				andarEsquerda(matriz);
			}
		}
		else if (mvm =='s' || mvm =='S'){
			if (verifverti==0){
				if (matriz[y1+1][x1] == ' ' || matriz[y2+1][x2] == ' ' || matriz[y1+2][x1] == ' ' || matriz[y2+2][x2] == ' '){			
					DescerMaisRapido(matriz);
				}
			}
			else{
				if (matriz[y1+1][x1] == ' ' || matriz[y1+2][x1] == ' '){			
					DescerMaisRapidoVertical(matriz);
				}
			}
		}
		else if (mvm == 'w' || mvm == 'W'){
			verifverti = 1;
			GirandoPontos(matriz);
		}
		else if (mvm == 'e' || mvm == 'E'){
			MudarOrdem(matriz);
		}
		else if(mvm == 'q' || mvm == 'Q'){
			verifverti=0;
			VoltaPontos(matriz);
		}
	}
}
/* Nessa funcao eh onde se cria as letras*/
int CriaLetra(char matriz[][largura]){
	velocidade=720000;
	/*Aqui ele so vai verificar se os primeiros
	pontos estao vazios ou nao e se tiver ele coloca
	os 2 pontos nele*/
	if(matriz[0][largura/2-1] != ' ' || matriz[0][largura/2] != ' ')
		return 0;
	matriz[0][(largura/2)-1] = 'A' + (rand()%5);
	matriz[0][(largura/2)] = 'A' + (rand()%5);
	y1=0;
	y2=0;
	x1=(largura/2)-1;
	x2=largura/2;
	return 1;
}
/*Aqui Se ouver letras ele faz
a troca fazendo-as descer*/
int DescerLetra(char matriz[][largura]){
	int achou=0,achoumelhor=0;
	/*verifica se o termo depois dele eh um
	espaco vazio e se for faz ela descer*/
	if (y1==0 || y2==0){
		verifverti=0;
	}
	if (verifverti == 0){  // horizontal
		if (matriz[y1+1][x1] == ' '){
			matriz[y1+1][x1] = matriz[y1][x1];
			matriz[y1][x1] = ' ';
			y1++;
		}		
		else
			achou=1;
		if (matriz[y2+1][x2] == ' '){
			matriz[y2+1][x2] = matriz[y2][x2];
			matriz[y2][x2] = ' ';
			y2++;
		}
		else
			achoumelhor=1;
		if (achou && achoumelhor){
			if(!CriaLetra(matriz)){
				return 0;
			}
			return 1;
		}
		return 1;
	}
	else{
		if (matriz[y2+1][x2] == ' '){
			matriz[y2+1][x2] = matriz[y2][x2];
			matriz[y2][x2] = ' ';
			y2++;
		}
		else
			achou=1;
		if (matriz[y1][x1+1] == ' '){
			matriz[y1][x1+1] = matriz[y1-1][x1+1];
			matriz[y1-1][x1+1] = ' ';
			y1++;
		}
		else
			achoumelhor=1;
		if (achou && achoumelhor){
			if(!CriaLetra(matriz)){
				return 0;
			}
			return 1;
		}
		return 1;
	}
}
/*Aqui ele verifica se a Letra desceu
se tiver descido ele volta 
a andar com os pontos*/
int atualizaTudo(char matriz[][largura]){
	if(!DescerLetra(matriz))
		return 0;
	if (y1!= 0 || y2 != 0){
		AndandoClique(matriz);
	}
	return 1;
}
/*Aqui eu simplesmente
criei a borda da matriz
e coloquei espacos em branco
dentro da matriz*/
void bordamatr(char matriz[][largura]){
	int i,j;
	/* os 2 fors para ele fazer
	somente as bordas da matriz*/
	for (i = 0; i < altura; i++){
		for (j = 0; j < largura; j++){
			if (j==0 || j==largura-1 || i==altura-1){
				matriz[i][j]='#';
			}
			else{
				matriz[i][j]=' ';
			}
		}
	}
}
/*Aqui eu printo os termos 
ja lidos anteriormente
e os que nao forem a borda
recebem cores no caso os 2 pontos*/
void printmatr(char matriz[][largura]){
	int i,j;
	/*os 2 fors pra rodar a matriz
	printando a borda e os novos elementos
	de cores aleatorias
	*/
	for (i = 0; i < altura; i++){
		for (j = 0; j < largura; j++){
			if (j==0||j==7||i==11){
				printf("%c", matriz[i][j]);
			}
			else{
				printf("\033[%dm%c"RESETCOLOR,(30+(matriz[i][j]%6+1)),matriz[i][j]);
			}
		}
		printf("\n");
	}
}

void GameOver(){
	CLEAR;/*limpa o terminal*/
	printf(CYAN"Desculpe, mas sinto lhe informar que perdeu!!\n"RESETCOLOR);
	printf("Nao se preocupe pode tentar novamente.\n");
	printf("Digite enter para voltar ao menu: \n");
	getchar();/*Daqui se volta pra funcao VoltaMenu*/
}

void Jogo(){
	CLEAR;/*limpa o terminal*/
	char matriz[altura][largura];
	bordamatr(matriz);/*Aqui eh criado os elementos da matriz*/
	printmatr(matriz);/*Aqui se printa os elementos da matriz*/
	CriaLetra(matriz);/*Aqui se cria o primeiro par de letra aleatoria do jogo*/
	int ativo = 1;
	while(ativo){	
		CLEAR;/*limpa o terminal*/
		usleep(velocidade);/*velocidade que o jogo ira rodar*/
		printf("Pontos: 0\n");/*Quantidade de pontos acumulados*/
		printf("\n");
		printmatr(matriz);/*printanto uma nova matriz com novo par de letra*/
		ativo = atualizaTudo(matriz);/*Aqui se atualiza a matriz*/
	}
	GameOver();/*Caso perca ira lhe mandar uma mensagem*/
}

void Instrucoes(){
	CLEAR;/*novamente limpando o terminal*/
	printf(
		"\t\t\t\tINSTRUCOES:\n\n\
		"VERDE"D Mover para a Direita\n\
		A - Mover para Esquerda\n\
		S - Mover mais rapido para Baixo\n\
		W - Vira as Peças\n\
		E - Muda a Ordem das Peças\n\
		Q - (Depois de clicar W) voce volta pra posicao inicial\n\
		\n\
		"CYAN"Funcionamento do Jogo:\n\
		"AZUL"voce pordera mudar ela para esquerda e direita pra que\n\
		fique do jeito que achar melhor.\n\
		O objetivo eh que voce organize melhor as peças a fim de juntar\n\
		4 letras aleatorias de cor iguais, logo faz elas explodirem\n\
		e ganhando pontos a partir disso.\n\n\
		"RESETCOLOR"Pressione Enter para retornar ao Menu\n\
		");/*As instrucoes do jogo*/
	STOP;/*Serve para ler o enter e retornar pra funcao VoltaMenu*/
}

void VoltaMenu(){
	char nrm_instrucoes = 0;
	srand((unsigned)time(NULL));
	/*funcao para chamar os primeiros prints*/
	while(nrm_instrucoes!= '5'){
		CLEAR;
		Print();
		/*Aqui voce digita a instrucao que deseja 
		para ser levado a uma funcao respectiva 
		ao valor digitado*/
		scanf("%c",&nrm_instrucoes);
		switch(nrm_instrucoes){
			case '1': Jogo();
			break;
			case '2': Instrucoes();
			break;
			case '5': break;
			default: printf("valor errado cara!\n");
		}
	}
}

int main(){
	CLEAR;/*Primeiramente ele vai limpar a tela*/
	PrePrint();/*Funcao de apresentacao do jogo*/
	CLEAR;
	VoltaMenu();/*Essa eh uma funcao de retorno pro menu*/
	return 0;
}