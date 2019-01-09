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
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

/* kbhit e getch para linux */
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

    int getch(void){
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
    #define CLEAR "clear"
#else
    #include <conio.h>
    #define CLEAR "cls"
#endif
/*Declaracoes de cores*/
#define VERDE   "\x1b[32m"
#define VERDECLARO   "\x1b[92m"
#define AMARELO  "\x1b[33m"
#define AMARELOCLARO   "\x1b[93m"
#define AZUL    "\x1b[34m"
#define AZULCLARO    "\x1b[94m"
#define MAGENTA "\x1b[35m"
#define MAGENTACLARO   "\x1b[95m"
#define CYAN    "\x1b[36m"
#define CYANCLARO   "\x1b[96m"
#define VERMELHO     "\x1b[31m"
#define VERMELHOCLARO   "\x1b[91m"
#define NEGRITO "\033[1m"
#define RESETCOLOR   "\x1b[0m"

/*Declaracao de termos importantes*/
#define Clear() system(CLEAR);
#define TRUE 1
#define FALSE 0
#define SUMIR -1000
/*Variaveis Globais*/
char Mat[13][8];
int Altura = 13, Largura = 8;
int linha, coluna;
int velocidade;
char Letra1, Letra2;
int pontuacao;
int Combo;

/* funcao que cria a letra
 de forma aleatoria */
int Aleatorio_Letra(){
    int num;
    num = rand()%5;
    return num;
}
/* Funcao que deixa a posicao 
aparecer em lugaresaleatorios */
int Aleatorio_Posicao(){
    int num;
    num = (rand()%(Largura-3))+1;
    return num;
}
/*Funcao que manda a letra de forma aleatoria
e com isso ele verifica e passa a cor
para a letra.*/
char EscolheLetra(){
    int cor = Aleatorio_Letra();
    switch(cor){
        case 0 : return 'A';
                 break;
        case 1 : return 'B';
                 break;
        case 2 : return 'C';
                 break;
        case 3 : return 'D';
                 break;
        case 4 : return 'E';
                 break;
        default : break;    
    }
}
/*Funcao que vai crirar a letra e ser chamada acima*/
void criaChar(){
    Letra1 = EscolheLetra();
    Letra2 = EscolheLetra();
}
/*Funcao que Cria a matriz delimitando
as bordas e no interior os espaços
em branco ou seja espaco vazio*/
void Inicializar_matriz(){
    int i, j;
    for(i = 0; i < Altura; i++){
        for(j = 0; j < Largura; j++){
            if(j == 0 || j == Largura-1 || i == Altura-1){
                Mat[i][j] = '#';
            }
            else{
                Mat[i][j] = ' ';
            }
        }
    }
}

/* Na funcao PrePrint eh onde tem 
primeira tela do jogo
ou seja a tela de boas vindas*/
void Preprint(){
    int i, idxlaser=0;
    Clear();
    char laser[15];
    laser[14]='\0';
    while(1==1) {
        if (kbhit()){
            getch();
          return;
        }
        else{
            for (i = 0; i < 13; i++){
                if (i==idxlaser){
                    laser[i] = '_';
                    laser[i+1]='_';  
                    i++;
                }
                else{
                    laser[i] = ' ';
                    laser[i+1] = ' ';
                    
                }
            }
            idxlaser = idxlaser+1;
            if (idxlaser == 14){
                idxlaser=0;
            }
            Clear();
            printf(VERMELHO NEGRITO"Seja Bem vindo!!\n"RESETCOLOR);
            printf(NEGRITO"Ao Jogo "VERDE"'DIY Puyo Puyo'\n"RESETCOLOR);
            printf("           ________\n");
            printf("      _,.-Y  |  |  Y-._\n");
            printf("  .-~¹   ||  |  |  |   ¹-.\n");
            printf("  I¹ ¹¹==¹|¹ !¹¹! ¹|¹[]¹¹|     _____\n");
            printf("  L__  [] |,,------|:   _[----I¹ .-{¹-.\n");
            printf(" I___|  ,,| l______|l_ [__L]_[I_/r(=}=-P");printf(VERMELHO"%s\n"RESETCOLOR, laser);
            printf("[L______L_[________]______j~  ¹-=c_]/=-^\n");
            printf(" |_I_j.--.|==I|I==_/.--L_]\n");
            printf("   [_((==)[`-----¹](==)j\n");
            printf("      I--I¹~~¹¹¹~~¹I--I\n");
            printf("      |[]|         |[]|\n");
            printf("      l__j         l__j\n");
            printf("      |!!|         |!!|\n");
            printf("      |..|         |,,|\n");
            printf("      ([])         ([])\n");
            printf("      ]--[         ]--[\n");
            printf("      [_L]         [_L]\n");
            printf("     /|..|\\        /|,,|\\ \n");
            printf("    `=}--{='     `=}--{='\n");
            printf("   .-^--r-^-.   .-^--r-^-.\n");
            printf(NEGRITO"Para Iniciar o Jogo tecle enter: \n"RESETCOLOR);

            usleep(100000);
        }
    }
}

/* Funcao que vai imprimir a matriz
e verifica se tem algum ponto vai ser uma letra
dependendo de qual letra for
ele printa de diferente  cor cada letra*/
void Imprimir_matriz(){
    int i, j;
    Clear();
    /* Onde se atualiza a pontuacao do usuario */
    printf("Pontos: %d\n\n", pontuacao);
    for(i = 0; i < Altura; i++){
        for (j = 0; j < Largura; j++){
            if(Mat[i][j] == 'A'){
                printf(VERMELHO"%c "RESETCOLOR, Mat[i][j]);
            }
            else if(Mat[i][j] == 'B'){
                printf(VERDE"%c "RESETCOLOR, Mat[i][j]);
            }
            else if(Mat[i][j] == 'C'){
                printf(AMARELO"%c "RESETCOLOR, Mat[i][j]);
            }
            else if(Mat[i][j] == 'D'){
                printf(AZUL"%c "RESETCOLOR, Mat[i][j]);
            }
            else if(Mat[i][j] == 'E'){
                printf(MAGENTA"%c "RESETCOLOR, Mat[i][j]);
            }
            else{
                printf("%c ", Mat[i][j]);
            }
        }
        printf("\n");
    }
}

/* Funcao para pausa o jogo*/
void Pausa_tempo(){
    int i;

    for(i = 0; i < 80; i++){
        if(kbhit()){
            switch(getch()){
                case 'p':
                    Imprimir_matriz();

                    printf(AMARELOCLARO "\nPressione Enter para continuar" RESETCOLOR);
                    getch();
                break;

                default:
                break;
            }
        }
        usleep(1000);
    }
}
/* Funcao que vai Verificar se tem pontos que sao iguais
e se forem ele vai fazer a explosao dos pontos para que
assim possa se pontuar e jogar o jogo. */
void Verifica_explosao(int Verifica_peca[Altura-1][Largura-2], int i, int j){
    int deu = FALSE;

    while(deu == FALSE){
        deu = TRUE;
        for(i = 0; i < 12; i++){
            for(j = 0; j < 6; j++){
                if(Verifica_peca[i][j] == SUMIR){

                    if(Verifica_peca[i+1][j] > 0 && Verifica_peca[i+1][j] != SUMIR && i < 11){ /*Inferior*/
                        Verifica_peca[i+1][j] = SUMIR;
                        deu = FALSE;
                    }
                    if(Verifica_peca[i-1][j] > 0 && Verifica_peca[i-1][j] != SUMIR && i > 0){ /*Superior*/
                        Verifica_peca[i-1][j] = SUMIR;
                        deu = FALSE;
                    }
                    if(Verifica_peca[i][j+1] > 0 && Verifica_peca[i][j+1] != SUMIR && j < 5){ /*Direita*/
                        Verifica_peca[i][j+1] = SUMIR;
                        deu = FALSE;
                    }
                    if(Verifica_peca[i][j-1] > 0 && Verifica_peca[i][j-1] != SUMIR && j > 0){ /*Esquerda*/
                        Verifica_peca[i][j-1] = SUMIR;
                        deu = FALSE;
                    }
                }
            }
        }
    }
}

/* Funcao que vai verificar se pode descer e assim vai descer
os pontos. */
int Descer_pecas(){
    int deu = FALSE;
    int troca = FALSE;

    while(deu == FALSE){
        deu = TRUE;
        for(coluna = 1; coluna < 7; coluna++){
            for(linha = 11; linha > 0; linha--){
                if(Mat[linha][coluna] == ' ' && Mat[linha-1][coluna] != ' '){
                    deu = FALSE;
                    troca = TRUE;
                    Mat[linha][coluna] = Mat[linha-1][coluna];
                    Mat[linha-1][coluna] = ' ';
                }
            }
        }
        if(troca){
            Imprimir_matriz();
            Pausa_tempo();
        }
    }
    return troca;
}
/*Funcao que printa as intruçoes de movimentaçao do jogo e como ele "funciona"*/
void instrucoes(){
    Clear();
    printf(NEGRITO"\tCONTROLES:\n\n"RESETCOLOR);
    printf("\t Movimentar a peca para esquerda\n\n");
    printf("\t[a] Movimentar a peca pra esquerda\n\n");
    printf("\t[d] Movimentar a peca pra direita\n\n");
    printf("\t[s] Acelerar a queda da peca\n\n");
    printf("\t[z] Girar a peca no sentido anti-horario\n\n");
    printf("\t[x] Girar a peca no sentido horario\n\n");
    printf("\t[q] Sair do jogo\n\n");
    printf("\t[p] Pausar o jogo\n\n");
    printf(
        "\n\n\
        "CYAN"Funcionamento do Jogo:\n\
        "AZUL"voce pordera mudar ela para esquerda e direita pra que\n\
        fique do jeito que achar melhor.\n\
        O objetivo eh que voce organize melhor as peças a fim de juntar\n\
        4 letras aleatorias de cor iguais, logo faz elas explodirem\n\
        e ganhando pontos a partir disso.\n\n\
        "RESETCOLOR"Pressione Enter para retornar ao Menu\n\
        ");
    getchar();
    getchar();
}
/* Funcao que vai pegar os pontos que anteriormente foram identificados
como iguais um proximo do outro e apartir disso ele vai remover-los*/
int RemocaoPontos(){
    int Verifica_peca[12][6];
    int peca, i;
    int troca = FALSE;
    char c;

    for(peca = 0; peca < 5; peca++){
        c = 'A' + peca;
        for(linha = 0; linha < 12; linha++){
            for(coluna = 1; coluna < 7; coluna++){
                if(Mat[linha][coluna] == c){
                    Verifica_peca[linha][coluna-1] = 0;
                    if(Mat[linha+1][coluna] == c){
                        Verifica_peca[linha][coluna-1]++;
                    }
                    if(Mat[linha-1][coluna] == c){
                        Verifica_peca[linha][coluna-1]++;
                    }
                    if(Mat[linha][coluna+1] == c){
                        Verifica_peca[linha][coluna-1]++;
                    }
                    if(Mat[linha][coluna-1] == c){
                        Verifica_peca[linha][coluna-1]++;
                    }
                }
                else {
                    Verifica_peca[linha][coluna-1] = -1;
                }
            }
        }
        for(linha = 0; linha < 12; linha++){
            for(coluna = 0; coluna < 6; coluna++){
                if( Verifica_peca[linha][coluna] != SUMIR && (Verifica_peca[linha][coluna] >= 3 || /* ocorrera desaparecimento se, uma peca tiver 3 ou 4 vizinhos semelhantes */
                (Verifica_peca[linha][coluna] == 2 && /* ou se ela tiver 2 vizinhos semelhantes e um desses vizinho tiver 2 ou mais vizinhos iguais */
                ( (Verifica_peca[linha+1][coluna] >= 2 && linha < 11) || /* vizinho inferior */
                (Verifica_peca[linha-1][coluna] >= 2 && linha > 0) ||   /* vizinho superior */
                (Verifica_peca[linha][coluna+1] >= 2 && coluna < 5) ||  /* vizinho a direita */
                (Verifica_peca[linha][coluna-1] >= 2 && coluna > 0) ) ) ) ){   /* vizinho a esquerda */
                    troca = TRUE;
                    Combo++;
                    Verifica_peca[linha][coluna] = SUMIR;
                    Verifica_explosao(Verifica_peca, linha, coluna); /* marcamos a primeira peca a desaparecer e chamamos a funcao Verifica_explosao para "espalhar" o desaparecimento */
                }
            }
        }

        if(troca){ /* se houve mudanca, removemos as pecas, atualizamos o tabuleiro e somamos os pontuacao */
            for(i = 0; i < 2; i++){ /* ciclo duplo para atualizarmos como '*' uma vez, dando efeito ao desaparecimento */
                for(linha = 0; linha < 12; linha++){
                    for(coluna = 0; coluna < 6; coluna ++){
                        if(Verifica_peca[linha][coluna] == SUMIR){
                            if(i == 0){
                                Mat[linha][coluna+1] = '*';
                            }
                            else{
                                Mat[linha][coluna+1] = ' ';
                                pontuacao += Combo;
                            }
                        }
                    }
                }
                Imprimir_matriz();
                Pausa_tempo();
            }
        }
    }
    return troca;
}
/* Funcao onde entra o jogo e logo chama a fucao pra printar a matriz pra gerar os termos
para fazer com que eles descam e assim o jogo flua. */
void Jogar(){
    int Letra1linha, Letra1coluna, Letra2linha, Letra2coluna;
    int tecla, i, pos, deu, ciclo;
    Combo = 0;

    int Sair = FALSE;
    int PERDEU = FALSE;
    pontuacao = 0;

    while(PERDEU == FALSE && Sair == FALSE){

        Imprimir_matriz();
        usleep(1000);
        criaChar();
        Letra1linha = 0;
        Letra1coluna = Aleatorio_Posicao();
        Letra2linha = 0;
        Letra2coluna = Letra1coluna+1;

        if(Mat[Letra1linha][Letra1coluna] == ' ' && Mat[Letra2linha][Letra2coluna] == ' '){
            Mat[Letra1linha][Letra1coluna] = Letra1;
            Mat[Letra2linha][Letra2coluna] = Letra2;
        }
        else {
            PERDEU = TRUE;
        }
        Imprimir_matriz();
        pos = 3;
        deu = FALSE;

        ciclo = 180;/*Ciclo de queda das pecas, 180 * 4000 microsegundos = 720 milisegundos*/
        while(deu == FALSE && Sair == FALSE && PERDEU == FALSE){
            for(i = 0; i < ciclo; i++){
                if(kbhit()){
                    tecla = getch();
                    switch (tecla){
                        case 'q': Sair = TRUE;
                                  return;
                                  break;
                        case 'p': Imprimir_matriz();
                        printf(VERMELHO"\nVoce acaba de pausar o jogo...");
                                  printf(CYAN "\nPressione 'Enter' para continuar" RESETCOLOR);
                                  getch();
                                  break;
                        case 's': ciclo = 20; /* aceleramento da queda da peca */
                                  break;
                        case 'd': if(Mat[Letra1linha][Letra1coluna+1] == ' ' || Mat[Letra2linha][Letra2coluna+1] == ' '){
                                    Letra1coluna++;
                                    Letra2coluna++;
                                    Mat[Letra1linha][Letra1coluna-1] = ' ';
                                    Mat[Letra2linha][Letra2coluna-1] = ' ';
                                    Mat[Letra1linha][Letra1coluna] = Letra1;
                                    Mat[Letra2linha][Letra2coluna] = Letra2;
                                  }
                                  Imprimir_matriz();
                                  break;
                        case 'a': if(Mat[Letra1linha][Letra1coluna-1] == ' ' || Mat[Letra2linha][Letra2coluna-1] == ' '){
                                    Letra1coluna--;
                                    Letra2coluna--;
                                    Mat[Letra1linha][Letra1coluna+1] = ' ';
                                    Mat[Letra2linha][Letra2coluna+1] = ' ';
                                    Mat[Letra1linha][Letra1coluna] = Letra1;
                                    Mat[Letra2linha][Letra2coluna] = Letra2;
                                  }
                                  Imprimir_matriz();
                                  break;
                        case 'z': switch(pos % 7){
                                case 0: if(Mat[Letra2linha][Letra2coluna+1] == ' '){
                                            Mat[Letra1linha][Letra1coluna] = ' '; 
                                            Letra1linha = Letra2linha;
                                            Letra2coluna = Letra2coluna + 1;
                                            pos = 3;
                                        }
                                        break;
                                case 1: if(Mat[Letra2linha-1][Letra2coluna] == ' '){
                                            Mat[Letra1linha][Letra1coluna] = ' ';
                                            Letra1coluna = Letra2coluna;
                                            Letra1linha = Letra2linha - 1;
                                            pos--;
                                        }
                                        break;
                                case 2: if(Mat[Letra1linha][Letra1coluna+1] == ' '){
                                            Mat[Letra2linha][Letra2coluna] = ' ';
                                            Letra2linha = Letra1linha;
                                            Letra1coluna = Letra2coluna + 1;
                                            pos--;
                                        }
                                        break;
                                case 3: if(Mat[Letra1linha-1][Letra1coluna] == ' '){
                                            Mat[Letra2linha][Letra2coluna] = ' ';
                                            Letra2linha = Letra1linha - 1;
                                            Letra2coluna = Letra1coluna;
                                            pos--;
                                        }
                                        break;

                                case 4: if(Mat[Letra2linha][Letra2coluna-1] == ' '){
                                            Mat[Letra1linha][Letra1coluna] = ' ';
                                            Letra1linha = Letra2linha;
                                            Letra1coluna--;
                                            pos--;
                                        }
                                        break;
                                case 5: if(Mat[Letra2linha-1][Letra2coluna] == ' '){
                                            Mat[Letra1linha][Letra1coluna] = ' ';
                                            Letra1linha = Letra2linha - 1;
                                            Letra1coluna = Letra2coluna;
                                            pos = 0;
                                        }
                                        break;
                                case 6: if(Mat[Letra1linha][Letra1coluna-1] == ' '){
                                            Mat[Letra2linha][Letra2coluna] = ' ';
                                            Letra2linha = Letra1linha;
                                            Letra2coluna--;
                                            pos = 1;
                                        }
                                        break;
                                default: break;
                            }

                            Mat[Letra1linha][Letra1coluna] = Letra1;
                            Mat[Letra2linha][Letra2coluna] = Letra2;

                            Imprimir_matriz();
                        break;
                        case 'x': switch(pos % 7){
                                  case 0: if(Mat[Letra2linha][Letra2coluna+1] == ' '){
                                              Mat[Letra1linha][Letra1coluna] = ' ';
                                              Letra1linha = Letra2linha;
                                              Letra1coluna = Letra2coluna + 1;
                                              pos = 5;
                                          }
                                          break;
                                  case 1: if(Mat[Letra1linha-1][Letra1coluna] == ' '){
                                              Mat[Letra2linha][Letra2coluna] = ' ';
                                              Letra2coluna = Letra1coluna;
                                              Letra2linha = Letra1linha - 1;
                                              pos = 6;
                                          }
                                          break;
                                  case 2: if(Mat[Letra1linha][Letra1coluna+1] == ' '){
                                              Mat[Letra2linha][Letra2coluna] = ' ';
                                              Letra2linha = Letra1linha;
                                              Letra2coluna = Letra1coluna + 1;
                                              pos++;
                                          }
                                          break;
                                  case 3: if(Mat[Letra2linha-1][Letra2coluna] == ' '){
                                              Mat[Letra1linha][Letra1coluna] = ' ';
                                              Letra1linha = Letra2linha - 1;
                                              Letra1coluna = Letra2coluna;
                                              pos++;
                                          }
                                          break;
                                  case 4: if(Mat[Letra2linha][Letra2coluna-1] == ' '){
                                              Mat[Letra1linha][Letra1coluna] = ' ';
                                              Letra1linha = Letra2linha;
                                              Letra2coluna--;
                                              pos++;
                                          }
                                          break;
                                  case 5: if(Mat[Letra1linha-1][Letra1coluna] == ' '){
                                              Mat[Letra2linha][Letra2coluna] = ' ';
                                              Letra2linha = Letra1linha - 1;
                                              Letra2coluna = Letra1coluna;
                                              pos++;
                                          }
                                          break;
                                  case 6: if(Mat[Letra1linha][Letra1coluna-1] == ' '){
                                              Mat[Letra2linha][Letra2coluna] = ' ';
                                              Letra2linha = Letra1linha;
                                              Letra1coluna--;
                                              pos = 3;
                                          }
                                          break;
                                  default: break;
                        }
                        Mat[Letra1linha][Letra1coluna] = Letra1;
                        Mat[Letra2linha][Letra2coluna] = Letra2;
                        Imprimir_matriz();
                        break;
                    default: break;
                    }
                    Imprimir_matriz();
                }
                usleep(4000); /* usleep de 4000 microsegundos */
            }
            if( (Mat[Letra1linha+1][Letra1coluna] == ' ' || (Letra1linha == Letra2linha - 1 && Letra1coluna == Letra2coluna) ) &&
            (Mat[Letra2linha+1][Letra2coluna] == ' ' || (Letra2linha == Letra1linha -1 && Letra2coluna == Letra1coluna) ) ){
                Letra1linha++;
                Letra2linha++;
                Mat[Letra1linha-1][Letra1coluna] = ' ';
                Mat[Letra2linha-1][Letra2coluna] = ' ';
                Mat[Letra1linha][Letra1coluna] = Letra1;
                Mat[Letra2linha][Letra2coluna] = Letra2;
            }
            else { /* caso contrario, chamamos a funcao de queda onde nao ha controle do jogador e apos a queda marcamos o fim de um ciclo de pecas */
                Descer_pecas();
                deu = TRUE;
            }
            Imprimir_matriz();
        }
        Combo = 0;
        do {
            RemocaoPontos();
            Descer_pecas();
        } while (RemocaoPontos() || Descer_pecas());
        Combo = 0;
        if(PERDEU){
            printf(VERMELHOCLARO "\t\n  Fim da linha meu caro  " RESETCOLOR);
            printf(CYAN"\t\tPor favor coroado ou princesa \n"RESETCOLOR);
            printf(AMARELOCLARO"\t Aperte enter para voltar ao meu humilde menu.. \n");
            getchar();
        }
    }
}

int MenuPrincipal(){
    int opcao;
    Clear();
    printf(VERDECLARO"1 - Jogar\n"RESETCOLOR);
    printf(AMARELOCLARO"2 - Instrucoes\n"RESETCOLOR);
    printf("3 - Configuracoes\n");
    printf("4 - Ranking\n");
    printf(VERMELHOCLARO"5 - Sair\n\n"RESETCOLOR);
    printf("Digite o numero correspondente a opcao desejada: ");
    scanf("%d", &opcao);
    switch(opcao){
        case 1: Inicializar_matriz();
                Jogar();
                break;
        case 2: instrucoes();
                break;
        case 3: printf("Configuracoes ainda nao implementado\n");
                break;
        case 4: printf("Ranking ainda nao implementado\n");
                break;
        case 5: return opcao;
                break;
        default: break;
    }
    return opcao;
}

/*Funcao principal*/
int main(){
    int escolha_menu;
    srand(time(0));
    Inicializar_matriz();
    Preprint(); /* tela de boas vindas */
    do{
        escolha_menu = MenuPrincipal();
    }while(escolha_menu != 5);

    return 0;
}