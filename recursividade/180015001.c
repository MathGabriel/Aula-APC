/*Universidade de Brasilia

Instituto de Ciencias Exatas

Departamento de Ciencia da Computacao

Algoritmos e Programacao de Computadores 2/2018

Aluno(a): Claudio Gabriel Duarte Brandao

Matricula: 180015001

Descricao: O proposito deste trabalho e criar um jogo do tipo

           Tetris. Algumas especificacoes sobre o funcionamento estao

           descritas sob a forma de comentarios ao longo do programa.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#define CLEAR "clear"
#define VERMELHO "\033[31m"
#define VERDE "\033[32m"
#define AMARELO "\033[33m"
#define AZUL "\033[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define NEGRITO "\033[1m"
#define RESETCOLOR "\033[0m"

#ifndef _WIN32

int kbhit() {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, & oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, & newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int getch(void) {
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, & oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, & newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, & oldt);
    return ch;
}
#else
    #include <conio.h>
    #endif
char altura = 13;
char largura = 8;
int velocidade=500000,placar=0;
char p1, p2;
int linha1, coluna1, linha2, coluna2;
char mat[13][8], aux[13][8];

//Funcao que mostra a tela de encerramento do jogo
void gameover() {
    system(CLEAR);
    printf(VERDE"Voce perdeu, mas nao desanime.\n");
    printf("Aperta enter pra voltar ao menu. \n"RESETCOLOR);
}

//Funcao que cria o char do jogo
void criarchar(char mat[][largura]) {
    int i;
    srand(time(NULL));
    coluna1 = (rand()%(largura-3))+1;
    coluna2 = coluna1+1;
    mat[0][coluna1] = 'A' + rand() % 5;
    mat[0][coluna2] = 'A' + rand() % 5;
    p1 = mat[0][(coluna1)];
    p2 = mat[0][coluna2];
    linha1 = 0;
    linha2 = 0;
}

//Funcao que imprime na tela a borda do jogo
int printarmatriz() {
    int i, j;
    system(CLEAR);
    printf("\n");
    printf("Placar: %d\n",placar);
    printf("\n");
    for (i = 0; i < altura; i++){
        for (j = 0; j < largura; j++){
            if (j==0||j==7||i==12){
                printf("%c", mat[i][j]);
            }
            else{
                printf("\033[%dm%c"RESETCOLOR,(30+(mat[i][j]%6+1)),mat[i][j]);
            }
        }
        printf("\n");
    }
    return 1;
}

void arrumartudo(){
    int i,j;
    char aux;
    for ( i = 0; i < 12; i++){
        for (j = 1; j<7 ; j++){
            if(mat[i][j] != ' ' && mat[i][j] != '#'){
                while(mat[i+1][j] == ' '){
                    printf("%c\n",mat[i][j]);
                    aux = mat[i][j];
                    mat[i][j] = ' ';
                    mat[i + 1][j] = aux;
                    i++;
                }
            }
        }
    }
}

void apagar() {

    int i, j;

    for (i = 0; i < 13; i++) {

        for (j = 0; j < 8; j++) {

            if (aux[i][j] == '*') {

                mat[i][j] = ' ';

            }

        }

    }
    arrumartudo();

}

int verificaraxu() {
    int i, j, cont=0;
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 8; j++) {
            if (aux[i][j] == '*') {
                cont++;
            }
        }
    }
    return cont;
}

void explosao(char letra, int i, int j) {
    if(mat[i][j] == letra){
        aux[i][j] = '*';
    }    
    if (letra == aux[i][j + 1]) {
        explosao(letra, i, j + 1);
    }
    if (letra == aux[i + 1][j] ) {
        explosao(letra, i + 1, j);
    }
    if (i != 0 && letra == aux[i - 1][j]) {
        explosao(letra, i - 1, j);
    }
    if (letra == aux[i][j - 1]) {
        explosao(letra, i, j - 1);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
//Inicio das funcoes que fazem os movimentos nas pecas na vertical
void voltarnormal(char mat[][largura]) {
    char aux;
    aux = mat[linha1 - 1][coluna1 + 1];
    mat[linha1 - 1][coluna1 + 1] = ' ';
    mat[linha1][coluna1] = aux;
    return;
}

void alternarVert(char mat[][largura]) {
    int aux;
    aux = mat[linha1 - 1][coluna1 + 1];
    mat[linha1 - 1][coluna1 + 1] = mat[linha2][coluna2];
    mat[linha2][coluna2] = aux;
}

//Funcao que desce as pecas vericais mais rapida
int descerapidoVert(char mat[][largura]) {
    char aux1, aux2;
    int x;
    if (mat[linha2 + 1][coluna2] == ' ') {
        aux2 = mat[linha2][coluna2];
        mat[linha2 + 1][coluna2] = aux2;
        aux1 = mat[linha1 - 1][coluna1 + 1];
        mat[linha2][coluna2] = aux1;
        mat[linha1 - 1][coluna1 + 1] = ' ';
        linha1++;
        linha2++;
        x = 1;
    } else
        x = 0;
    return x;
}

//Funcao que move pecas verticais para a esquerda
void movEsquerdaVert(char mat[][largura]) {
    char aux;
    if (mat[linha2][coluna2 - 1] == ' ' && mat[linha1 - 1][coluna1] == ' ') {
        aux = mat[linha2][coluna2];
        mat[linha2][coluna2 - 1] = aux;
        mat[linha2][coluna2] = ' ';
        mat[linha1 - 1][coluna1] = mat[linha1 - 1][coluna1 + 1];
        mat[linha1 - 1][coluna1 + 1] = ' ';
        coluna1--;
        coluna2--;
    }
}

//Funcao que move pecas verticais para a direita
void movDireitaVert(char mat[][largura]) {
    char aux;
    if (mat[linha2][coluna2 + 1] == ' ' && mat[linha1 - 1][coluna1 + 2] == ' ') {
        aux = mat[linha2][coluna2];
        mat[linha2][coluna2 + 1] = aux;
        mat[linha2][coluna2] = ' ';
        mat[linha1 - 1][coluna1 + 2] = mat[linha1 - 1][coluna1 + 1];
        mat[linha1 - 1][coluna1 + 1] = ' ';
        coluna1++;
        coluna2++;
    }
}

/*Funcao que meio que cria uma 'menuzinho' para as acoes do usurio, so que dessa
vez com pessas verticais, como ir pra direita, esquerda, girar a peca, etc. */
int clicandoVert(char mat[][largura]) {
    char mov;
    int x;

    if(kbhit()){
        mov=getch();

        if (mov == 'd' || mov == 'D') {
            movDireitaVert(mat);
        }  if (mov == 'a' || mov == 'A') {
            movEsquerdaVert(mat);
        }  if (mov == 's' || mov == 'S') {
            x = descerapidoVert(mat);
            while (x == 1)
            {
                x = descerapidoVert(mat);
                printarmatriz();
            }
        } if (mov == 'q' || mov == 'Q') {
            alternarVert(mat);
        } if (mov == 'w' || mov == 'W') {
            voltarnormal(mat);
            return 1;
        }
    }
}

//Funcao que faz as pecas verticais descerem
void movimentovertical(char mat[][largura]) {
    int voltoutd = 0;
    while (mat[linha2 + 1][coluna2] == ' ' || voltoutd == 1)
    {
        mat[linha2 + 1][coluna2] = mat[linha2][coluna2];
        mat[linha2][coluna2] = mat[linha1 - 1][coluna1 + 1];
        mat[linha1 - 1][coluna1 + 1] = ' ';
        linha2++;
        linha1++;
        usleep(velocidade);
        printarmatriz();
        voltoutd = clicandoVert(mat);
        if (voltoutd == 1){
            return;
        }
    }
}

//Fim das funcoes que fazem os movimentos nas pecas na vertical
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Funcao que faz as pecas ficarem verticalmente, e dentro dela
chamando outro 'menu de acoes do usuario' com propsito de fazer as pecas horizontais irem pra
esquerda, direita, trocar, etc*/
void vertical(char mat[][largura]) {
    char aux;
    if (mat[linha1][coluna1 + 1] == ' ' || mat[linha2][coluna2 - 1] == ' ') {
        return;
    }
    aux = mat[linha1][coluna1];
    mat[linha1][coluna1] = ' ';
    mat[linha1 - 1][coluna1 + 1] = aux;
    movimentovertical(mat);
}

//Funcao que trocas as pecas horizontais de lugares
void alternar(char mat[][largura]) {
    int aux;
    if (mat[linha1][coluna1 + 1] == mat[linha2][coluna2]) {
        aux = mat[linha1][coluna1];
        mat[linha1][coluna1] = mat[linha2][coluna2];
        mat[linha2][coluna2] = aux;
    }
}

//Funcao que move as pecas horizontais descerem mais rapidas
int descerapido(char mat[][largura]) {
    char aux1, aux2;
    int x;
    if (mat[linha2 + 1][coluna2] == ' ') {
        aux2 = mat[linha2][coluna2];
        mat[linha2][coluna2] = ' ';
        mat[linha2 + 1][coluna2] = aux2;
        linha2++;
        x = 1;
    }
    if (mat[linha1 + 1][coluna1] == ' ') {
        aux1 = mat[linha1][coluna1];
        mat[linha1][coluna1] = ' ';
        mat[linha1 + 1][coluna1] = aux1;
        linha1++;
        x = 1;
    } else
        x = 0;
    return x;
}

//Funcao que move as pecas horizontais para a esquerda
void movEsquerda(char mat[][largura]) {
    char aux;
    if (mat[linha1][coluna1 - 1] == ' ' && mat[linha2 + 1][coluna2] == ' ' && mat[linha2][coluna2 - 1] == mat[linha1][coluna1]) {
        aux = mat[linha1][coluna1];
        mat[linha1][coluna1 - 1] = aux;
        mat[linha1][coluna1] = mat[linha2][coluna2];
        mat[linha2][coluna2] = ' ';
        coluna1--;
        coluna2--;
    }
}

//Funcao que move as pecas horizontais para a direita
void movDireita(char mat[][largura]) {
    char aux;
    if (mat[linha2][coluna2 + 1] == ' ' && mat[linha1 + 1][coluna1] == ' ' && mat[linha1][coluna1 + 1] == mat[linha2][coluna2]) {
        aux = mat[linha2][coluna2];
        mat[linha2][coluna2 + 1] = aux;
        mat[linha2][coluna2] = mat[linha1][coluna1];
        mat[linha1][coluna1] = ' ';
        coluna1++;
        coluna2++;
    }
}

/*Funcao que meio que cria uma 'menuzinho' para as acoes do usurio,
como ir pra direita, esquerda, girar a peca, etc. */
void printaraux() {
    int i, j;
    system(CLEAR);
    printf("\n");
    printf("Placar: %d\n",placar);
    printf("\n");
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            printf("%c", aux[i][j]);
        }
        printf("\n");
    }
}

void auxiliar() {
    int i, j;
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 8; j++) {
            aux[i][j] = mat[i][j];
        }
    }
}
void clicando(char mat[][largura]) {
    char mov;
    int x;
    if (kbhit()) {
        mov = getch();
        if (mov == 'd' || mov == 'D') {
            movDireita(mat);
        } 
        else if (mov == 'a' || mov == 'A') {
            movEsquerda(mat);
        } 
        else if (mov == 's' || mov == 'S') {
            x = descerapido(mat);
            while (x == 1)
            {
                x = descerapido(mat);
                printarmatriz();
            }
        } 
        else if (mov == 'q' || mov == 'Q') {
            alternar(mat);
        } 
        else if (mov == 'w' || mov == 'W') {
            vertical(mat);
        }
        else if( mov == 'r' || mov=='R'){
            printarmatriz();
            printf(AMARELO "\nPressione Enter para continuar" RESETCOLOR);
            getch();
        }
    }
}

//Funcao que faz pecas horizontais cairem
int movimento(char mat[][largura]) {
    int i = 0, j = 0, x = 0;
    char aux1, aux2;
    x = 0;
    while (mat[linha1 + 1][coluna1] == ' ' || mat[linha2 + 1][coluna2] == ' ') {
        if (mat[linha1 + 1][coluna1] == ' ') {
            aux1 = mat[linha1][coluna1];
            mat[linha1][coluna1] = ' ';
            mat[linha1 + 1][coluna1] = aux1;
            linha1++;
            x = 1;
        }
        if (mat[linha2 + 1][coluna2] == ' ') {
            aux2 = mat[linha2][coluna2];
            mat[linha2][coluna2] = ' ';
            mat[linha2 + 1][coluna2] = aux2;
            x = 1;
            linha2++;
        }
        usleep(velocidade);
        clicando(mat);
        x = printarmatriz();
    }
    arrumartudo();

    return x;
}

//Funcao que coloca novas pecas no jogo
void acao(char mat[][largura]) {
    int i = 0, resul = 0, cont;
        criarchar(mat);
        mat[linha1][coluna1] = p1;
        mat[linha2][coluna2] = p2;
        printarmatriz();
        resul = movimento(mat);
        while (resul == 1) {
            resul = 0;
            criarchar(mat);
            mat[linha1][coluna1] = p1;
            mat[linha2][coluna2] = p2;
            printarmatriz();
            resul = movimento(mat);
            usleep(750000);
            auxiliar();
            for (int i = 0; i < 12; i++) {
                for (int j = 1; j < 7; j++) {
                    if(mat[i][j] != ' ' && mat[i][j] != '#'){
                    explosao(mat[i][j], i, j);
                    cont = verificaraxu();
                        if (cont >= 4) {
                            printaraux();
                            usleep(750000);
                            apagar();
                            placar+=cont;
                            arrumartudo();
                        }
                    auxiliar();
                    }
                }
            }
            if (resul == 0) {
                return;
            }
        }
    }


//Funcao que cria a borda do jogo
void bordamatriz() {
    int i, j;
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            if (j == 0 || j == largura - 1 || i == altura - 1) {
                mat[i][j] = '#';
            } else {
                mat[i][j] = ' ';
            }
        }
    }
}

//Funcao que fara a borda do jogo e ira mostrar na tela
void jogo() {
    system(CLEAR);
    bordamatriz(mat);
    printarmatriz();
    acao(mat);
    gameover();
    getchar();
}

//funcao que mostra o menu inicial do jogo
char MostrarMenu() {

    char opcao;
    printf(AZUL"Seja Bem vindo!!\n");
    printf("1 - Jogar \n");
    printf("2 - Intrucoes \n");
    printf("3 - Configuracoes\n");
    printf("4 - Ranking\n");
    printf("5 - Sair\n");
    printf("Digite o numero correspondente a opocao desejada: \n"RESETCOLOR);
    scanf("%c", & opcao);
    return opcao;
}

//Funcao que se chamada mostra as intrucoes do jogo
void instructions() {
    printf(AMARELO"\tO jogo se se assemelha muito ao Tetris, onde voce tem duas\n");
    printf("\tpecas de letras e voce precisa sumir com elas juntando 4 letras iguais ou mais.\n");
    printf("\n");
    printf("\t Teclas:\n");
    printf("\t ___________________________________\n");
    printf("\t|A: Move a peca para a esquerda     |\n");
    printf("\t|D: Move a peca para a direta       |\n");
    printf("\t|S: Desce a peca mais rapida        |\n");
    printf("\t|W: Gira e peca vertical/horizontal |\n");
    printf("\t|R: Pausa o jogo                    |\n");
    printf("\t|Q: Troca as letras de lugar        |\n");
    printf("\t|___________________________________|\n");
    printf("\n");
    printf("\tPara voltar ao menu aperte 'Enter': \n"RESETCOLOR);
    getchar();
}

/*Funcao que podera alterar algumas configuracoes do jogo caso  o
usuario queira */
void configuracoes() {
    printf("Vai vim junto com a proxima DLC\n");
    getchar();
}

//Funcao que mostra o ranking(ainda nao feita).
void ranking() {
    printf("Na proxima atualizacao chega.\n");
    getchar();
}

//Funcao para chamar o menu do jogo e escolher a opcao desejada.
void opcao() {
    char opcao = 0;
    while (opcao != '5') {
        system(CLEAR);
        opcao = MostrarMenu();
        getchar();
        switch (opcao) {
        case '1':
            jogo();
            break;
        case '2':
            instructions();
            break;
        case '3':
            configuracoes();
            break;
        case '4':
            ranking();
            break;
        case '5':
            break;
        default:
            printf("Valor incorreto, informe novamente.\n");
        }
    }
}

//Funcao que mostra a tela de boas-vindas ao usuario
void telainicial() {
    system(CLEAR);
    printf(CYAN"\t\tBEM VINDO AO JOGO\t\t\t\t(perdi)\n"RESETCOLOR);
    printf("\n");
    printf(VERMELHO"__________________________________________________\n");
    printf("_________A $$$$ _____________________________,,,___\n");
    printf("________A $$$$$$$ _________________________A $$$ ___\n");
    printf("_________ $$$$$$$ ______,,________,,_______A $$$$A _\n");    
    printf("__________ $$$$$$$ ____A $$ _____A $$ ____A $$$$$A ___\n");
    printf("___________ $$$$$$$ _A $$$$$ _A $$$$$ __A $$$$$$$A ___\n");
    printf("____________ $$$$$$$_$$$$$$$_$$$$$$$_A $$$$$$$A ____\n");
    printf("_____________ $$$$$$_$$$$$$$_$$$$$$$ A $$$$$$A _____\n");
    printf("____,,,,,,______ $$$$$$_$$$$$$$_$$$$$$$_$$$$$$A ___\n");
    printf("__A $$$$$ ____ $$$$$$_$$$$$$$_$$$$$$$_$$$$$$A ______\n");
    printf("_A $$$$$$$$$ A $$$$$$$_$$$$$$$_$$$$$$$_$$$$$A _______\n");
    printf("_A $$$$$$$$$$$$$$$$$$_$$$$$$$_$$$$$$$_$$$$$A _______\n");
    printf("____ $$$$$$$$$$$$$$$_$$$$$$$_$$$$$$_$$$$$$A _______\n");
    printf("_______ $$$$$$$$$$$$$_$$$$$__$$_$$$$$$_$$A ________\n");
    printf("________ $$$$$$$$$$$$,___,$$$$,_____,$$$$$A _______\n");
    printf("__________ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$A _________\n");
    printf("___________ $$$$$$$$$$$$$$$$$$$$$$$$$$$A __________\n");
    printf("_____________ $$$$$$$$$$$$$$$$$$$$$$$$A ___________\n");
    printf("________________ $$$$$$$$$$$$$$$$$$$$A ____________\n");
    printf("__________________________________________________\n");
    printf("                  TETRIS ENJ'UADO                  \n"RESETCOLOR);
    printf("\n\n");
    printf(CYAN"\t\tAperta enter pra continuar.\n"RESETCOLOR  );
    getchar();
}

//Funcao principal
int main() {
    telainicial();
    opcao();
    return 0;
}