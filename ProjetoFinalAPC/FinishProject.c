/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!    Universidade de Brasilia                                                                        !!
!!    Departamento de Ciencia da Computacao                                                           !!
!!    Instituto de Ciencias Exatas                                                                    !!
!!    Algoritmos e Programação de Computadores - 2/2018                                               !!
!!    Aluno: Matheus Gabriel da Silva Rodrigues                                                       !!
!!    Matricula: 180025031                                                                            !!
!!    Curso: Computacao(Licenciatura)                                                                 !!
!!    Descricao:  Implementar um jogo similar ao jogo "Tetris", que vamos chamar de "DIY Puyo Puyo".  !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
/*Declaracao de bibliotecas*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<fcntl.h>

/* kbhit e getch para linux */
#ifndef _WIN32
    #include <termios.h>
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
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define VERDE         "\x1b[32m"
#define VERDECLARO    "\x1b[92m"
#define AMARELO       "\x1b[33m"
#define AMARELOCLARO  "\x1b[93m"
#define AZUL          "\x1b[34m"
#define AZULCLARO     "\x1b[94m"
#define MAGENTA       "\x1b[35m"
#define MAGENTACLARO  "\x1b[95m"
#define CYAN          "\x1b[36m"
#define CYANCLARO     "\x1b[96m"
#define VERMELHO      "\x1b[31m"
#define VERMELHOCLARO "\x1b[91m"
#define NEGRITO       "\033[1m"
#define RESETCOLOR    "\x1b[0m"
 
int Jogo_rankeado = 0;                                      /*Padrao do jogo fica como nao rankeado*/
char Mat[1000][1000];                                       /*Tamanho maximo da matriz do jogo*/
int Largura = 8, Altura=13;                                 /*Tamanho padrao da matriz do jogo*/
int Velocidade=36000;                                       /*Velocidade padrao do jogo*/
int Numero_pecas=5;                                         /*Numero de letras diferentes no jogo*/
int Pontuacao=4;                                            /*Quantidade de pecas que precisam ser conectadas*/
char Letra1, Letra2;                                        /*Conteudo correspondete a cada letra cirada*/
int posLetra1Lag, posLetra1Col, posLetra2Lag, posLetra2Col; /*Posicao da Letra1 e da Letra2*/
int Orientacao_peca = 0;                                    /*FALSE significa horizontal e TRUE significa vertical*/
int Pontos;                                                 /*Pontuacao do jogador*/
int Letras_adjascentes= 0;                                  /*Contador de pecas adjacentes*/
char nickname[11];                                          /*Nick do jogador para ser gravado no Ranking*/
int replay;                                                 /*Variavel do replay*/
char Nome_arquivo[20];                                      /*Nome do arquivo que deve ser usado no replay*/
int Arquivo_erro;                                           /*Variavel para emitir erro ao nao encontrar o arquivo*/


typedef struct{ /*Struct para armazenar as pecas que serão destruidas*/
    int posl; /*Posicao nas linhas*/
    int posc; /*Posicao nas colunas*/
}Posicao_excluir;
typedef struct{/*struct para as posicoes do Ranking*/
    char rankname[11];/*nome*/
    int rankpont;/*Pontuacao*/
}Posicao_rank;
typedef struct{/*struct para gravar e carregar as pecas que foram utilizadas em jogo e sua posicao inicial*/
    char Letra1, Letra2;
    int posstrt;
}Historico_pecas;

Historico_pecas pecas[10000000];
int replayrecidx=0;
Posicao_excluir pos[100];
Posicao_rank rank[10];

void config_adjust(){/*setando as configuracoes do jogo*/

    FILE* fl=fopen("config.txt", "r");

    if(fl==NULL){
        fl=fopen("config.txt","w");
        fprintf(fl,"%d\n", Largura);
        fprintf(fl,"%d\n", Altura);
        fprintf(fl,"%d\n", Velocidade);
        fprintf(fl,"%d\n", Numero_pecas);
        fprintf(fl,"%d\n", Pontuacao);
        fprintf(fl,"%d\n", Jogo_rankeado);
        fclose(fl);
        fl=fopen("config.txt", "r");
    }
    fscanf(fl,"%d", &Largura);
    fscanf(fl,"%d", &Altura);
    fscanf(fl,"%d", &Velocidade);
    fscanf(fl,"%d", &Numero_pecas);
    fscanf(fl,"%d", &Pontuacao);
    fscanf(fl,"%d", &Jogo_rankeado);
    fclose(fl);

}

void Preprint(){
    int i, idxlaser=0;
    system(CLEAR);
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
            system(CLEAR);
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

void Inicializar(){
    int i=0,j=0;
    char noname[10]={"NONAME"};
    Preprint();
 
    system(CLEAR);
    FILE* fl= fopen("Ranking.bin","rb");
    for(i=0;i<10;i++){
        for(j=0;j<10;j++){
            rank[i].rankname[j]=noname[j];
        }
        rank[i].rankpont=0;
    }
    if(fl==NULL){
        fl= fopen("Ranking.bin", "wb");
        fwrite(&rank,sizeof(Posicao_rank),10,fl);
        fclose(fl);
        fl=fopen("Ranking.bin","rb");
    }
    
    fread(&rank,sizeof(Posicao_rank),10,fl);
    fclose(fl);
}
int Menu(){
 
    int Opcao;
    do{
        printf(VERMELHOCLARO"1 - Jogar\n"RESETCOLOR);
        printf(VERDE"2 - Instrucoes\n"RESETCOLOR);
        printf(AZULCLARO"3 - Configuracoes\n"RESETCOLOR);
        printf(AMARELO"4 - Ranking\n"RESETCOLOR);
        printf(CYANCLARO"5 - Sair\n"RESETCOLOR);
        printf(NEGRITO"Selecione a opcao desejada: "RESETCOLOR);
        if((Opcao<1)&&(Opcao>5)){
            printf("\n");
            printf("Opcao Invalida.");
        }
        scanf("%d", &Opcao);
    }while((Opcao<1)&&(Opcao>5));
    return Opcao;
}

void Gerar_mapa(int carrefararquivo){
    int i, j;
    if (carrefararquivo==0){/*gera o Mat*/
        for(i=0;i<Altura;i++){
            for(j=0;j<Largura;j++){
                if((j==0)||(j==Largura-1)||(i==Altura-1)){
                    Mat[i][j]='#';
                }
                else{
                    Mat[i][j]=' ';
                }
            }
        }
    }
    else{/*carrega o tamuleiro*/
        printf("Pontos : %d\n\n", Pontos);
        for(i=0;i<Altura;i++){
            for(j=0;j<Largura;j++){
                if((j==0)||(j==Largura-1)||(i==Altura-1)){
                    printf(RESETCOLOR"#");
                }
                else{
                    switch(Mat[i][j]){
                        case 'A':printf(VERMELHOCLARO"A");/*peças que vão se movimentar*/
                                break;
                        case 'B':printf(AZULCLARO"B");
                                break;
                        case 'C':printf(VERDECLARO"C");
                                break;
                        case 'D':printf(AMARELOCLARO"D");
                                break;
                        case 'E':printf(MAGENTA"E");
                                break;
                        case 'F':printf(VERDECLARO"F");
                                break;
                        case 'G':printf(AZULCLARO"G");
                                break;
                        case 'H':printf(RESETCOLOR"H");
                                break;
                        case 'I':printf(CYAN"I");
                                break;
                        case 'J':printf(AZUL"J");
                                break;
                        case 'a':printf(VERMELHOCLARO"A");/*peças estáticas*/
                                break;
                        case 'b':printf(AZULCLARO"B");
                                break;
                        case 'c':printf(VERDECLARO"C");
                                break;
                        case 'd':printf(AMARELOCLARO"D");
                                break;
                        case 'e':printf(MAGENTA"E");
                                break;
                        case 'f':printf(VERDECLARO"F");
                                break;
                        case 'g':printf(AZULCLARO"G");
                                break;
                        case 'h':printf(RESETCOLOR"H");
                                break;
                        case 'i':printf(CYAN"I");
                                break;
                        case 'j':printf(AZUL"J");
                                break;
                        default:
                                printf(RESETCOLOR"%c", Mat[i][j]);
                                break;
                    }
                }
            }
            printf("\n");
        }
    }
}
void Gerar_pecas(){/*randomiza as pecas e as posicoes*/
    int Posicao;
    Letra1 = 'A' + rand() % Numero_pecas;
    Letra2 = 'A' + rand() % Numero_pecas;
    Posicao=(rand() % (Largura-3)) + 1;
    
    pecas[replayrecidx].Letra1=Letra1;/*gravando Letra1 para o replay*/
    pecas[replayrecidx].Letra2=Letra2;/*gravando Letra2 para o replay*/
    pecas[replayrecidx].posstrt=Posicao;/*gravando a posicao para o replay*/
    replayrecidx++;

    Mat[0][Posicao]=Letra1;
    posLetra1Lag=0;
    posLetra1Col=Posicao;/*posicao inicial Letra1*/
   
    Mat[0][Posicao+1]=Letra2;
    posLetra2Lag=0;
    posLetra2Col=Posicao+1;/*posicao inicial Letra2*/
    Orientacao_peca=0;/*sempre vai spawnar na horizontal*/
}
void Obtem_teclado(char comando, int TravouLetra1, int TravouLetra2){/*interpreta os comandos do teclado*/
    char aux;
    if((TravouLetra1==0)&&(TravouLetra2==0)){    
        switch(comando){/*interpretação dos comandos do teclado*/
            case 'w':/*girar a peca*/
                    if((Orientacao_peca==0)&&(Mat[posLetra2Lag+1][posLetra1Col]==' ')){
                        Mat[posLetra2Lag+1][posLetra1Col]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Lag=posLetra2Lag+1;
                        posLetra2Col=posLetra1Col;
                        Orientacao_peca=1;
                    }
                    else if((Orientacao_peca==1)&&(Mat[posLetra2Lag-1][posLetra1Col+1]==' ')){
                        Mat[posLetra2Lag-1][posLetra1Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Lag=posLetra2Lag-1;
                        posLetra2Col=posLetra1Col+1;
                        Orientacao_peca=0;
                    }
                    break;
            case 'W':/*girar a peca*/
                    if((Orientacao_peca==0)&&(Mat[posLetra2Lag+1][posLetra1Col]==' ')){
                        Mat[posLetra2Lag+1][posLetra1Col]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Lag=posLetra2Lag+1;
                        posLetra2Col=posLetra1Col;
                        Orientacao_peca=1;
                    }
                    else if((Orientacao_peca==1)&&(Mat[posLetra2Lag-1][posLetra1Col+1]==' ')){
                        Mat[posLetra2Lag-1][posLetra1Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Lag=posLetra2Lag-1;
                        posLetra2Col=posLetra1Col+1;
                        Orientacao_peca=0;
                    }
                    break;
            case 'a':/*mover para a esquerda*/
                if(Orientacao_peca==0){
                    if(Mat[posLetra1Lag][posLetra1Col-1]==' '){    
                        Mat[posLetra1Lag][posLetra1Col-1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col--;
                        Mat[posLetra2Lag][posLetra2Col-1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col--;
                    }
                }
                else if(Orientacao_peca=1){
                    if((Mat[posLetra1Lag][posLetra1Col-1]==' ')&&(Mat[posLetra2Lag][posLetra2Col-1]==' ')){    
                        Mat[posLetra1Lag][posLetra1Col-1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col--;
                        Mat[posLetra2Lag][posLetra2Col-1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col--;
                    }
                }
                break;
            case 'A':/*mover para a esquerda*/
                if(Orientacao_peca==0){
                    if(Mat[posLetra1Lag][posLetra1Col-1]==' '){    
                        Mat[posLetra1Lag][posLetra1Col-1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col--;
                        Mat[posLetra2Lag][posLetra2Col-1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col--;
                    }
                }
                else if(Orientacao_peca=1){
                    if((Mat[posLetra1Lag][posLetra1Col-1]==' ')&&(Mat[posLetra2Lag][posLetra2Col-1]==' ')){    
                        Mat[posLetra1Lag][posLetra1Col-1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col--;
                        Mat[posLetra2Lag][posLetra2Col-1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col--;
                    }
                }
                break;
            case 's':break;/*queda acelerada*/
            case 'S':break;/*queda acelerada*/
            case 'd':/*mover para a direita*/
                if(Orientacao_peca==0){
                    if(Mat[posLetra2Lag][posLetra2Col+1]==' '){
                        Mat[posLetra2Lag][posLetra2Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col++;
                        Mat[posLetra1Lag][posLetra1Col+1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col++;
                        }
                }
                else if(Orientacao_peca=1){
                    if((Mat[posLetra2Lag][posLetra2Col+1]==' ')&&(Mat[posLetra1Lag][posLetra1Col+1]==' ')){
                        Mat[posLetra2Lag][posLetra2Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col++;
                        Mat[posLetra1Lag][posLetra1Col+1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col++;
                    }
                }    
                break;
            case 'D':/*mover para a direita*/
                if(Orientacao_peca==0){
                    if(Mat[posLetra2Lag][posLetra2Col+1]==' '){
                        Mat[posLetra2Lag][posLetra2Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col++;
                        Mat[posLetra1Lag][posLetra1Col+1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col++;
                        }
                }
                else if(Orientacao_peca=1){
                    if((Mat[posLetra2Lag][posLetra2Col+1]==' ')&&(Mat[posLetra1Lag][posLetra1Col+1]==' ')){
                        Mat[posLetra2Lag][posLetra2Col+1]=Letra2;
                        Mat[posLetra2Lag][posLetra2Col]=' ';
                        posLetra2Col++;
                        Mat[posLetra1Lag][posLetra1Col+1]=Letra1;
                        Mat[posLetra1Lag][posLetra1Col]= ' ';
                        posLetra1Col++;
                    }
                }
                break;
            case 'e':/*troca a Letra1 e a Letra2*/
                if((!TravouLetra1)&&(!TravouLetra2)){    
                    Mat[posLetra1Lag][posLetra1Col] = Letra2;
                    Mat[posLetra2Lag][posLetra2Col] = Letra1;
                    aux=Letra1;
                    Letra1=Letra2;
                    Letra2=aux;
                }
                break;
            case 'E':/*troca a  Letra1 e a Letra2*/
                if((!TravouLetra1)&&(!TravouLetra2)){  
                    Mat[posLetra1Lag][posLetra1Col] = Letra2;
                    Mat[posLetra2Lag][posLetra2Col] = Letra1;
                    aux=Letra1;
                    Letra1=Letra2;
                    Letra2=aux;
                }
                break;
            case 'p':/*quando pausar o jogo*/
                    printf(VERMELHO"\nVoce acaba de pausar o jogo...");
                    printf(CYAN "\nPressione 'Enter' para continuar" RESETCOLOR);
                    getch();
                break;
            case 'P':/*quando pausar o jogo*/
                    printf(VERMELHO"\nVoce acaba de pausar o jogo...");
                    printf(CYAN "\nPressione 'Enter' para continuar" RESETCOLOR);
                    getch();
                break;
            default:break;
       
        }
    }
}
int Cair_peca(int* TravouLetra1, int* TravouLetra2){/*movimento de queda das pecas*/
    char aux;/*a funcao tem um conceito simples, mover para baixo se não parou*/
    if((posLetra2Lag==(Altura-2))||((Mat[posLetra2Lag+1][posLetra2Col]<='j')&&(Mat[posLetra2Lag+1][posLetra2Col]>='a'))&&(*TravouLetra2==0)){
        *TravouLetra2=1;
        Mat[posLetra2Lag][posLetra2Col]-='A';
        Mat[posLetra2Lag][posLetra2Col]+='a';
    }
    else{
        if(*TravouLetra2==0){
            Mat[posLetra2Lag+1][posLetra2Col]=Letra2;
            Mat[posLetra2Lag][posLetra2Col]=' ';
            posLetra2Lag++;
        }
    }
    if((posLetra1Lag==(Altura-2))||((Mat[posLetra1Lag+1][posLetra1Col]<='j')&&(Mat[posLetra1Lag+1][posLetra1Col]>='a'))&&(*TravouLetra1==0)){
        *TravouLetra1=1;
        Mat[posLetra1Lag][posLetra1Col]-='A';
        Mat[posLetra1Lag][posLetra1Col]+='a';
    }
    else{    
        if(*TravouLetra1==0){
            Mat[posLetra1Lag+1][posLetra1Col]=Letra1;
            Mat[posLetra1Lag][posLetra1Col]= ' ';
            posLetra1Lag++;
        }
    }
}
void Movimento_peca(){
    int TravouLetra1=0, TravouLetra2=0, i=0, contcomandos=0;/*flags para saber se as pecas pararam, um indice e um contador do numero de comandos*/
    char comando;
 
    while((!TravouLetra1)||(!TravouLetra2)){/*movimento de queda, aquisição e execução de comandos*/
        for(i=0;i<20;i++){
            if(TravouLetra1||TravouLetra2){
                comando='s';
            }
            while((kbhit()==1)&&(contcomandos<20)){/*aquisicao de comandos*/
                contcomandos++;
                comando=getch();
                if(TravouLetra1||TravouLetra2){
                    comando='s';
                }  
                if(comando=='s'){
                    break;
                }
                Obtem_teclado(comando, TravouLetra1, TravouLetra2);
                system(CLEAR);
                Gerar_mapa(1);
                i++;
            }
            if(comando=='s'){
                break;
            }
            usleep(Velocidade);
        }
        contcomandos=0;
        if((comando=='s')||(comando=='S')){/*queda sem interrupção ao apertar o S*/
            while((!TravouLetra1)||(!TravouLetra2)){
                usleep(720);
                Cair_peca(&TravouLetra1, &TravouLetra2);
                system(CLEAR);
                Gerar_mapa(1);
            }
        }
        if((comando!='s')&&(comando!='S')){/*queda padrão*/
            Cair_peca(&TravouLetra1, &TravouLetra2);
        }
 
        system(CLEAR);/*refresh da tela*/
        Gerar_mapa(1);
    }
}
int Verifica(int pospl, int pospc){/*checar se uma peca ja foi citada na struct*/
    int i, ta=0;
    for(i=0;i<Letras_adjascentes; i++){
        if((pospl==pos[i].posl)&&(pospc==pos[i].posc)){
            ta=1;
        }
    }
    return ta;
}
void Encontra(int pospl, int pospc){/*funcao recursiva para procurar as pecas que serão eliminadas*/
   
    if(Letras_adjascentes==0){/*primeira peca*/
        pos[Letras_adjascentes].posl=pospl;
        pos[Letras_adjascentes].posc=pospc;
    }
    if((Mat[pospl][pospc]==Mat[pospl+1][pospc])&&(Verifica(pospl+1,pospc)==0)){/*embaixo*/
        Letras_adjascentes++;
        pos[Letras_adjascentes].posl=pospl+1;
        pos[Letras_adjascentes].posc=pospc;
        Encontra(pospl+1,pospc);
    }
    if((Mat[pospl][pospc]==Mat[pospl-1][pospc])&&(Verifica(pospl-1,pospc)==0)){/*em cima*/
        Letras_adjascentes++;
        pos[Letras_adjascentes].posl=pospl-1;
        pos[Letras_adjascentes].posc=pospc;
        Encontra(pospl-1,pospc);
    }
    if((Mat[pospl][pospc]==Mat[pospl][pospc+1])&&(Verifica(pospl,pospc+1)==0)){/*na direita*/
        Letras_adjascentes++;
        pos[Letras_adjascentes].posl=pospl;
        pos[Letras_adjascentes].posc=pospc+1;
        Encontra(pospl,pospc+1);
    }
    if((Mat[pospl][pospc]==Mat[pospl][pospc-1])&&(Verifica(pospl,pospc-1)==0)){/*na esquerda*/
        Letras_adjascentes++;
        pos[Letras_adjascentes].posl=pospl;
        pos[Letras_adjascentes].posc=pospc-1;
        Encontra(pospl,pospc-1);
    }
}
void Destroi_pecas(){/*pegar as posicoes adjacentes salvas na struct para destruir*/
    int i, j, ok=0;
 
    for(i=0; i<=Letras_adjascentes; i++){/*trocar por asteriscos*/
        Mat[pos[i].posl][pos[i].posc]='*';
    }
    usleep(36000);
    system(CLEAR);
    Gerar_mapa(1);
    for(i=0;i<Altura;i++){/*trocar os asteriscos por espacos*/
        for(j=0;j<Largura;j++){
            if(Mat[i][j]=='*'){
                Mat[i][j]=' ';
            }
        }
    }
    while(!ok){/*checar o Mat enquanto houverem pecas suspensas*/
        ok=1;
        for(i=Altura-1;i>0;i--){
            for(j=0;j<Largura;j++){/*fazer as pecas descerem enquanto houver espaço embaixo delas*/
                if((Mat[i][j]==' ')&&(Mat[i-1][j]!=' ')&&((Mat[i][j]!='#'))){
                    Mat[i][j]=Mat[i][j]^Mat[i-1][j];
                    Mat[i-1][j]=Mat[i][j]^Mat[i-1][j];
                    Mat[i][j]=Mat[i][j]^Mat[i-1][j];
                    ok = 0;
                }
            }
        }
        usleep(36000);
        system(CLEAR);
        Gerar_mapa(1);
    }
}
void Verifica_destricao(){/*checar depois de destruir as pecas se foi gerado um combo e destruir ele*/
    int bonus=2, i, j, ok=0;
    while(!ok){/*enquanto ainda houverem combos a ocorrer*/
        ok=1;
        for(i=Altura-2;i>0;i--){
            for(j=1;j<Largura-1;j++){
                Letras_adjascentes=0;
                if((Mat[i][j]!=' ')&&(Mat[i][j]!='#')){
                    Encontra(i, j);/*procura*/
                    if(Letras_adjascentes>=Pontuacao-1){/*mais do que o necessario para pontuar adjacentes*/
                        ok=0;
                        bonus++;/*soma o bonus*/
                        Destroi_pecas();/*destroi*/
                        Pontos+=(bonus*(Letras_adjascentes+1));
                    }
                }
            }
        }
    }
}
void Incuir_rank(){
    int posnewplayer, demotion, i;
    if(Pontos>rank[9].rankpont){
        for(posnewplayer=9;posnewplayer>=0; posnewplayer--){/*determinando a posicao do novo jogador*/
            if (posnewplayer==0){
                break;
            }
            if (Pontos<rank[posnewplayer-1].rankpont){
                break;
            } 
        }
        for(demotion=posnewplayer+1;demotion<9;demotion++){/*rebaixamento dos que tinham a Pontuacao menor*/
            rank[demotion+1].rankpont=rank[demotion].rankpont;
        }
        for(i=0;i<10; i++){/*gravando os dados do novo jogador*/
            rank[posnewplayer].rankname[i]=nickname[i];
        }
        rank[posnewplayer].rankpont=Pontos;
    }
    FILE* fl= fopen("Ranking.bin", "wb");
    
    fwrite(&rank,sizeof(Posicao_rank),10,fl);
    fclose(fl);
}
void Salvar_replay(){
    int i;
    char txtname[20];
    
    printf(VERDECLARO" Insira o nome do arquivo com (.txt) no final:\n");
    printf("AVISO:Nao inserir o .txt ira resultar na nao leitura do arquivo quando procurado novamente\n");
    printf("OBS:Maximo de 20 caracteres para o nome do arquivo\n");
    printf("OBS2: Nao use espacos para o nome do arquivo\n");
    printf("OBS3: Escrever o nome igual ao de um arquivo anterior, ira resultar no apaJogarnto de seu conteudo anterior\n");
    printf("OBS4: Nao nomear o arquivo como config.txt e Ranking.txt, isso acarretara em danos a execucao do jogo\n "RESETCOLOR);
    scanf(" %s", txtname);

    FILE* fd = fopen(txtname,"w");
    for(i=0;i<replayrecidx;i++){
        fprintf(fd,"%c %c %d\n", pecas[i].Letra1, pecas[i].Letra2, pecas[i].posstrt);
    }
    fclose(fd);

}
void Replay_perdeu(){
    char trash[20];
    printf(VERMELHOCLARO "\t\n  Fim da linha meu caro  " RESETCOLOR);
    printf(CYAN"\t\tPor favor coroado ou princesa \n"RESETCOLOR);
    printf("\tSua Pontuacao Neste Replay Foi Um Total Foi De %d Pontos\n\n\n", Pontos);
    Pontos=0;
    printf(NEGRITO"\tDigite qualquer numero para continuar: \n"RESETCOLOR);
    scanf(" %s", trash);
}
void Perdeu(){
    int ok=0;
    char trash[20];
    char save;
    printf(VERMELHOCLARO "\t\n  Fim da linha meu caro  " RESETCOLOR);
    printf(CYAN"\t\tPor favor coroado ou princesa \n"RESETCOLOR);
    printf("\tSua Pontuacao Total Foi De %d Pontos\n\n\n", Pontos);
    if(Jogo_rankeado){
        Incuir_rank();
    }
    if((!replay)&&(!Jogo_rankeado)){
        printf(AZULCLARO"Voce gostaria de salvar seu replay?(s/n)\n"RESETCOLOR);
        scanf(" %c", &save);
        while(!ok){/* em caso de usuarios que gostem de espancar teclas*/
            switch(save){
                case 's': ok=1; Salvar_replay();break;
                case 'n': ok=1;break;
                default: getchar();
                        break;
            }
        }
    }
    Pontos=0;
    printf(AMARELOCLARO"\tDigite algo pra continuar para continuar: \n"RESETCOLOR);
    scanf(" %s", trash);   
}
void Pausa_tempo(){
    int i;

    for(i = 0; i < 80; i++){
        if(kbhit()){
            switch(getch()){
                case 'p':
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

void Jogar(){/*funcao mestra de execucao do jogo*/
    int LOSE=0, i, ok=0, Posicao_excluir, replayidx=0;
    char Opcao;
    system(CLEAR);

    replayrecidx=0;
    if(!Jogo_rankeado){/*ajuste de pRESETCOLORs para partidas nao ranqueadas e questionamento sobre o replay*/
        config_adjust();
        printf(NEGRITO"\tJogar: \n\n"RESETCOLOR);
        printf(VERDECLARO"\t\t1 - Partida normal\n"RESETCOLOR);
        printf(VERMELHOCLARO"\t\t2 - Replay de uma partida\n"RESETCOLOR);

        while(!ok){/*em caso de usuario que goste de espancar teclas*/
            scanf(" %c", &Opcao);
            switch(Opcao){
                case '1':ok=1; replay=0;break;
                case '2':ok=1; replay=1;break;
                default:getchar();
            }
        }
        if(replay){/* carregar o arquivo selecionado pelo jogador para executar o replay*/
            printf(VERDECLARO"Digite o nome do arquivo em que esta gravado seu replay, com(.txt) no final:\n");
            printf("OBS: Caso o arquivo solicitado nao exista, nao sera possivel rodar um replay\n");
            printf("OBS2: Podem ocorrer erros caso o arquivo com o nome digitado tenha conteudo nao referente ao jogo\n"RESETCOLOR);
            scanf(" %s", Nome_arquivo);
            FILE* fd= fopen(Nome_arquivo,"r");
            if(fd== NULL){/* se nao existir, exibe mensagem de erro*/
                printf(NEGRITO"ERRO:O arquivo solicitado nao existe\n"RESETCOLOR);
                replay=0;
                LOSE=1;
                Arquivo_erro=1;
                usleep(20*Velocidade);
            }
            if(fd!= NULL){
                fclose(fd);
            }
        }
    }
    if(Jogo_rankeado){/*VERMELHOCLAROefinindo os pRESETCOLORs*/
        Altura= 15;
        Largura=9;
        Numero_pecas=7;
        Pontuacao=5;
        Velocidade=36000;
        printf(CYANCLARO"Digite seu nick(sem caracteres especiais): "RESETCOLOR);
        scanf("%s", nickname);
    }

    Gerar_mapa(0);/*gera o cenario inicial*/
    while(!LOSE){
        Gerar_mapa(1);/*Le o que está acontecendo na tela e exibe*/
        if(!replay){
            Gerar_pecas();/*gera as pecas normalmente*/
        }
        if(replay){/*gera as pecas do replay*/
            FILE* fd = fopen(Nome_arquivo,"r");
            for(i=0;i< replayidx;i++){
                fscanf(fd," %c %c %d", &Letra1, &Letra2, &Posicao_excluir);/*vai para a peca seguida da anterior no arquivo de texto*/
            }
                if(fscanf(fd," %c %c %d", &Letra1, &Letra2, &Posicao_excluir)!=EOF){
                    
                    Mat[0][Posicao_excluir]=Letra1;
                    posLetra1Lag=0;
                    posLetra1Col=Posicao_excluir;/*posicao inicial Letra1*/
            
                    Mat[0][Posicao_excluir+1]=Letra2;
                    posLetra2Lag=0;
                    posLetra2Col=Posicao_excluir+1;/*posicao inicial Letra2*/
                    Orientacao_peca=0;/*sempre vai spawnar na horizontal*/
                }
                else{
                    LOSE=1;
                    fclose(fd);
                }
            replayidx++;
        }
        Movimento_peca();/*bota as pecas para se movimentarem*/
        Letras_adjascentes=0;
        Encontra(posLetra1Lag, posLetra1Col);
        if((Letra1!= ' ')&&(Letra1!='*')){/*se ainda nao foi destruida*/
            if(Letras_adjascentes>=Pontuacao-1){/*conta as pecas adjacentes*/
                Destroi_pecas();
                Pontos+=(Letras_adjascentes+1);
            }
            Letras_adjascentes=0;
            if (Mat[posLetra2Lag][posLetra2Col]==' '){
                Letra2= ' ';
            }
        }
        if((Letra2!=' ')&&(Letra2!='*')){/*se ainda nao foi destruida*/
            Encontra(posLetra2Lag, posLetra2Col);
            if(Letras_adjascentes>=Pontuacao-1){
                Destroi_pecas();
                Pontos+=2*(Letras_adjascentes+1);
            }
        }
        Verifica_destricao();
        Pausa_tempo();
        for(i=1;(Mat[0][i]==' ')&&(i<Largura-1);i++){
        }
        if(i<Largura-2){
            LOSE=1;
        }
        system(CLEAR);
    }
    if(!Arquivo_erro){/*se houve erro no arquivo que foi carregado, o jogo pula o Jogar over*/
        if(!replay){/*Jogar over normal*/
            Perdeu();
        }
        else{
            Replay_perdeu();/*Jogar over do replay*/
        }
    }
    Arquivo_erro=0;
    system(CLEAR);
}
void Intrucoes(){/*funcao de exibição das instruções*/
    int sair;
    system(CLEAR);
 
    printf(CYAN"\tComo o jogo funciona:\n"RESETCOLOR);

    printf(AZULCLARO"\t\tAo começar o jogo, pecas de cores aleatorias comecarao a cair\n");
    printf("\t\tO objetivo e fazer com que entrem em contato quatro peças da mesma cor\n");
    printf("\t\tAo fazer isso, voce marcara Pontos\n");
    printf("\t\tAo juntar ao menos quatro pecas da mesma cor, elas explodirao\n");
    printf("\t\tSe uma peca que voce destruiu formar um conjunto de quatro, este novo tambem explodira e dara mais Pontos\n");
    printf("\t\tSe as peças se acumularem ate o topo do Mat, voce perder.\n"RESETCOLOR);
    printf(AMARELO"\tComo Jogar:\n");
    printf("\t\tW - Virar a peca\n");
    printf("\t\tE - Trocar as pecas de posicao\n");
    printf("\t\tA - Mover a peca para a esquerda\n");
    printf("\t\tS - Fazer a peca cair o maximo possivel\n");
    printf("\t\tD - Mover a peca para a direita\n");
    printf("\t\tP - Pausar o jogo\n\n"RESETCOLOR);
    printf(NEGRITO"\tDigite '[1]' para SAIR: "RESETCOLOR);
    scanf("%d", &sair);
}
void Configuracoes(){/*funcao de ajuste de configurações*/
    char Opcao; 
    int sair=0;
    
    FILE* fl=fopen("config.txt","r");
    fscanf(fl,"%d", &Largura);
    fscanf(fl,"%d", &Altura);
    fscanf(fl,"%d", &Velocidade);
    fscanf(fl,"%d", &Numero_pecas);
    fscanf(fl,"%d", &Pontuacao);
    fscanf(fl,"%d", &Jogo_rankeado);
    fclose(fl);

    while(!sair){
        system(CLEAR);

        printf(NEGRITO"\tO que deseja alterar?\n\n"RESETCOLOR);
        printf(VERMELHOCLARO"\t\t1 - Matriz\n"RESETCOLOR);
        printf(VERDECLARO"\t\t2 - Pecas\n"RESETCOLOR);
        printf(AZULCLARO"\t\t3 - Ativar o modo rankeado: "RESETCOLOR);
        if (Jogo_rankeado==0){
            printf(VERMELHOCLARO" Desativado\n"RESETCOLOR);
        }
        else{
            printf(VERDECLARO" Ativado\n"RESETCOLOR);
        }
        printf(NEGRITO"\t\t4 - Sair\n\n"RESETCOLOR);
        printf(VERMELHOCLARO"Selecione a opcao desejada: "RESETCOLOR);
        scanf(" %c", &Opcao);
        if(Opcao=='1'){
            system(CLEAR);
            printf(VERMELHOCLARO"\tSelecione a Altura desejada para a Matriz(def.:13)(min.:5)(max.:30):"RESETCOLOR);
            scanf("%d", &Altura);
            printf("\n");
            if ((Altura<5)||(Altura>30)){
                while((Altura<5)||(Altura>30)){
                    system(CLEAR);
                    printf(VERMELHOCLARO"\tDimensao solicitada invalida\n"RESETCOLOR);
                    printf("\t\tSelecione a Altura desejada para a Matriz(def.:13)(min.:5)(max.:30):");
                    scanf("%d", &Altura);
                    printf("\n");
                }
            }
            printf(VERMELHOCLARO"\tSelecione a Largura desejada para a Matriz(def.:8)(min.:5)(max.:30):"RESETCOLOR);
            scanf("%d", &Largura);
            printf("\n");
            if ((Largura<8)||(Largura>30)){
                while((Largura<8)||(Largura>30)){
                    system(CLEAR);
                    printf(NEGRITO"Dimensao solicitada invalida\n"RESETCOLOR);
                    printf("Selecione a Largura desejada para a Matriz(def.:13)(min.:5)(max.:30):");
                    scanf("%d", &Largura);
                    printf("\n");
                }
            }
        }
        if(Opcao=='2'){
            system(CLEAR);
            printf(VERDECLARO"Selecione a quantidade de peças necessarias para marcar um ponto(def.:4)(min.:3)(max.:10): "RESETCOLOR);
            scanf("%d", &Pontuacao);
            printf("\n");
            if ((Pontuacao<3)||(Pontuacao>10)){
                while((Pontuacao<3)||(Pontuacao>10)){
                    system(CLEAR);
                    printf(VERMELHO"\tNumero solicitado invalido\n"RESETCOLOR);
                    printf("Selecione a quantidade de peças necessarias para marcar um ponto(def.:4)(min.:3)(max.:10):");
                    scanf("%d", &Pontuacao);
                    printf("\n");
                }
            }
            printf(VERDECLARO"Selecione a quantidade de tipos diferentes de pecas que podem aparecer no jogo\n"RESETCOLOR);
            printf("(def.:5)(min.:2)(max.:10):");
            scanf("%d", &Numero_pecas);
            printf("\n");
            if ((Numero_pecas<2)||(Numero_pecas>10)){
                while((Numero_pecas<2)||(Numero_pecas>10)){
                    system(CLEAR);
                    printf(VERMELHO"\tNumero solicitado invalido\n"RESETCOLOR);
                    printf("Selecione a quantidade de tipos diferentes de pecas que podem aparecer no jogo");
                    printf("(def.:5)(min.:2)(max.:10)");
                    scanf("%d", &Numero_pecas);
                    printf("\n");
                }
            }
            printf("Selecione o tempo de atualizacao de tela em microssegundos\n(def.:720000)(min.:500000)(max.:1000000):");
            scanf("%d", &Velocidade);
            printf("\n");
            if ((Velocidade<500000)||(Velocidade>1000000)){
                while((Velocidade<500000)||(Velocidade>1000000)){
                    system(CLEAR);
                    printf(VERMELHO"\tNumero solicitado invalido\n"RESETCOLOR);
                    printf(NEGRITO"Selecione o tempo de atualizacao de tela em microssegundos\n(def.:720000)(min.:500000)(max.:1000000):"RESETCOLOR);
                    scanf("%d", &Velocidade);
                    printf("\n");
                }
            }
            Velocidade= Velocidade/20;
        }
        if(Opcao=='3'){
            Jogo_rankeado=!(Jogo_rankeado);
        }
        if(Opcao=='4'){
            break;
        }
        else{
            getchar();
        }
    }
    fl=fopen("config.txt","w");
    fprintf(fl,"%d\n", Largura);
    fprintf(fl,"%d\n", Altura);
    fprintf(fl,"%d\n", Velocidade);
    fprintf(fl,"%d\n", Numero_pecas);
    fprintf(fl,"%d\n", Pontuacao);
    fprintf(fl,"%d\n", Jogo_rankeado);
    fclose(fl);

    system(CLEAR);
}
void Ranking(){/*funcao de exibicao do Ranking*/
    system(CLEAR);
    int i=0;
    printf(AZULCLARO"\tRanking:\n\n"RESETCOLOR);
    for(i=0;i<10; i++){
        printf("\t\t%d-%s %d\n", i+1, rank[i].rankname, rank[i]. rankpont);
    }
    printf(NEGRITO"\tDigite [1] para SAIR: "RESETCOLOR);
    scanf("%d", &i);
    system(CLEAR);
}
int main(){
    int opcao_menu, Sair=0;
    srand(time(0));
    system(CLEAR);
    Inicializar();
 
    while(!Sair){
        opcao_menu=Menu();
 
        switch(opcao_menu){
        case 1: Jogar();
                break;
        case 2: Intrucoes();
                system(CLEAR);
                break;
        case 3: Configuracoes();
                break;
        case 4: Ranking();
                break;
        case 5: system(CLEAR);
                Sair=1;
                break;
        default:getchar();
    break;
        }

        system(CLEAR);
    }
    return 0;
}