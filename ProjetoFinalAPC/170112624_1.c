/*
Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programa��o de Computadores - Ver�o/2018
Aluno: Pedro Catelli Quaranta Trindade Silva
Matricula: 17/0112624
Turma: B
Vers�o do compilador: <versao utilizada>
Descricao:  Implementar um jogo similar ao jogo "Tetris", que vamos chamar de "APCTetris",
ou seja, o Tetris da Disciplina de APC.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void tela_inicial(){/*fun��o para imprimir a tela inicial, mostrada apenas uma vez, logo quando o programa � iniciado*/
    int i; /*a fun��o imprime apenas a borda do jogo e uma mensagem de boas vindas ao usuario*/
    printf("\n    BEM-VINDO AO TETRIS\n\n");
    printf("  +--------------------+\n");
    for (i=0; i<=5; i++){
        printf("  |                    |\n");
    }
    printf("=>+                    +<=\n");
    for (i=0; i<=7; i++) {
        printf("  |                    |\n"); /*20 espa�os*/
    }
    printf("  +--------------------+\n");
    printf("Tecle enter para iniciar o jogo ");
    getchar();
}

int leSeq(char *MAT, char JOGO[15][20]){/*fun��o para copiar a sequencia de char lida do arquivo de texto para a matriz JOGO*/
    int i, tam = strlen(MAT);/*strlen retorna o tamanho da string lida na linha na MAT*/
    for(i=0; i<tam; i++){/*anda linha por linha da matriz MAT*/
        JOGO[i][9] = MAT[i];/*copia string lida para a 9� coluna da matriz*/
    }
    return 1;
}

void limpa_telaSuperior(char JOGO[15][20]){
    int i, j;
    for(i=0; i<6; i++){/*limpa todas as colunas ate a 7� linha*/
        for(j=0; j<20; j++){
            JOGO[i][j] = ' ';/*preenche com espa�os vazios*/
        }
    }
}

void clear(){/*fun��o para limpar a tela do terminal tanto em windows, quanto em linux*/
    system("clear || cls");
    printf("\n");
}

void rotacionar(char JOGO[15][20], char *MAT, int *vertical, int *coluna){/*fun��o para girar a string no sentido anti-horario*/
    int i, tam = strlen(MAT);/*o valor de vertical pode mudar na main()*/
    if(*vertical){/*se vertical for = 1, ou seja, se a pe�a estiver na vertical*/
        if((*coluna)+tam <= 20){/*se o tamanho da string nao estourar a ultima coluna da matriz JOGO[15][20]*/
            limpa_telaSuperior(JOGO);
            for(i=0; i<tam; i++){/*primeira linha de JOGO recebe da MAT cada caracter a partir de coluna ate i*/
                JOGO[0][(*coluna)+i] = MAT[i];
            }
            if(*vertical == 1){/*se a vertical valer 1, passara a valer 0*/
                *vertical = 0;/*isso ajudara a identificar se a string esta na vertical ou se esta na horizontal nessa e em outras fun��es*/
            }else{              /*por isso � passada por referencia em parametros (ponteiro)*/
                *vertical = 1;/*caso contrario, volta a valer 1*/
            }
        }
    }else{/*caso a string (imagem do jogo) esteja na horizontal*/
        limpa_telaSuperior(JOGO);
        for(i=0; i<tam; i++){/*cada caracter da string � copiado para a coluna do primeiro caracter*/
            JOGO[i][*coluna] = MAT[i];/*anda linha por linha, e a coluna recebe a string*/
        }
        if(*vertical == 1){/*se a vertical valer 1, passara a valer 0*/
            *vertical = 0;/*isso ajudara a identificar se a string esta na vertical ou se esta na horizontal nessa e em outras fun��es*/
        }else{
            *vertical = 1;/*caso contrario, volta a valer 1*/
        }
    }
}

void direita(char JOGO[15][20], char *MAT, int vertical, int *coluna){/*fun��o para mover a string de MAT para a direita*/
    int i, tam = strlen(MAT);
    if(*coluna < 19){/*se a coluna for menor que a ultima coluna da JOGO[][], entramos no if*/
        if(vertical){/*se vertical for igual a 1, a sequencia esta na vertical*/
            (*coluna)++; /*incrementa coluna e muda seu valor na main*/
            limpa_telaSuperior(JOGO);
            for(i = 0; i<tam; i++){/*ate alcancar o final da string lida em MAT*/
                JOGO[i][*coluna] = MAT[i];/*copia toda a string da primeira linha de MAT para a coluna(agora um numero maior) de JOGO[][]*/
            }
        }else {/*caso contrario(caso esteja na horizontal)*/
			(*coluna)++;/*incrementa coluna, na main()*/
			if(((*coluna)+tam) <= 20){/*se o tamanho da string nao ultrapassar o tamanho do vetor JOGO[][]*/
				limpa_telaSuperior(JOGO);
				for(i = 0; i < tam; i++){
					JOGO[0][(*coluna)+i] = MAT[i];/*copia a string lida em MAT[i](que esta na primeira linha[0]), toda para
					a coluna ja incrementada + i, ja que se deve copiar cada caractere da string*/
				}
			} else {
				*coluna = *coluna - 1;/*caso contrario (caso estoure o valor da matriz[][20]), coluna volta a valer um valor a menos(valor inicial)*/
			}
        }
    }
}

void esquerda(char JOGO[15][20], char *MAT, int vertical, int *coluna){/*fun��o que copia a string para uma coluna � esquerda a partir de uma coluna referencial*/
    int i, tam = strlen(MAT);
    if(*coluna > 0){/*se a coluna tida como inicial pela fun��o for maior que zero, o processo � realizado*/
        limpa_telaSuperior(JOGO);/*limpa a metade superior da matriz JOGO[6][20]*/
        (*coluna)--;/*a coluna tida como referencial passa a ser uma anterior*/
        if(vertical){/*se vertical for = 1*/
            for(i = 0; i<tam; i++){/*caminha por todas as linhas ate o tamanho da string na MAT*/
                JOGO[i][*coluna] = MAT[i];/*copia o caracter da MAT[i] para a matriz JOGO na linha i e coluna, que ja foi decrementada*/
            }/*ou seja, a coluna onde esta a string lida em MAT, volta uma coluna para tras*/
        }
        else{/*caso a string esteja na horizontal*/
            for(i=0; i<tam; i++){
                JOGO[0][(*coluna)+i] = MAT[i];/*cada caracter da matriz MAT � copiado para JOGO na primeira linha e coluna (decrementada) + a pos��o de i*/
            }                                                       /*para que todos os caracteres sejam copiados*/
        }
    }
}

void cair(char JOGO[15][20], char *MAT, int vertical, int *coluna){/*fun��o que faz a pe�a cair para a ultima linha que estiver em branco*/
    int i = 14, j;/*i come�a na ultima linha da matriz JOGO[15][20], a 14, ou seja, a 15� linha da matriz*/
    int tam = strlen(MAT), flag;/*a flag ser� usada para interromper um processo na fun��o, caso alguma coisa, a flag � erguida*/
    if(vertical){/*se a string estiver na vertical, se vertical estiver valendo 1*/
        limpa_telaSuperior(JOGO);/*tela superior � limpa*/
        while(JOGO[i][*coluna] != ' '){/*enquanto a coluna de referencia estiver preenchida, i � decrementado(subimos para a linha de cima)*/
            i--;/*sendo assim, todas as linhas da string, que esta na vertical, sao verificadas*/
        }/*quando uma linha preenchida por espa�o em branco for encontrada, saimos do enquanto*/
        for(j=0; j<tam; j++){/*usando outro auxiliar j para a MAT*/
            JOGO[i--][*coluna] = MAT[j];/*copia o char lido em MAT na posi��o j para a a linha i e coluna de referencia da string JOGO, i � decrementado*/
        }
        limpa_telaSuperior(JOGO);
    }else{/*caso a string esteja ne horizontal*/
        flag=0;/*flag vale 0*/
        j = 6;/*j come�a na 7� linha da matriz JOGO[j][20]*/
        while(j<15){/*e ir� at� a ultima linha da matriz JOGO*/
        	for(i=0;i<tam;i++) {/*de i at� o tamanho da string lida em MAT*/
        		if(JOGO[j][*coluna+i]!=' '){/*verifica se todas as linhas de todas as colunas de 6 at� 15 est�o vazias*/
        	   		flag = 1;/*caso esteja preenchida por um caracter diferente de ' ' a flag � levantada*/
        		}
			}
			if(flag != 0){
                break;/*se a flag tiver sido acionada (se ela nao estiver valendo 0), sai do while*/
			}
			j++;/*j � incrementado para ir a proxima linha*/
		}
        for(i=0;i<tam;i++) {
            JOGO[j-1][*coluna+i]=MAT[i];
		}
	}
}
void verifica_linha(char JOGO[][20],int *pontos){/*fun��o para verificar se uma linha esta toda preenchida*/
   int i,j,k, cont;
    for (i=6; i<15; i++) {/*i come�a na 7� linha da matriz JOGO[i][20] e vai at� a ultima*/
        cont = 0;/*contador zerando para cada linha*/
        for (j=0; j<20; j++) {/*outro for para andar coluna por coluna na JOGO, na linha indicada por i*/
            if (JOGO[i][j] == ' ') {/*se JOGO na linha i, posi��o j for vazia, o contador soma + 1*/
                cont++;
            }
        }
        if (cont == 0) {/*apos andar por toda a linha, se cont for = 0, quer dizer que toda a linha esta preenchida por outros caracteres*/
            break;/*se cont == 0, o programa sai do for(i=6; i<15; i++), pois encontrou uma linha toda preenchida*/
        }
    }
    if(cont == 0){/*se toda linha estiver preenchida*/
        for(j=0;j<20;j++) {/*ela � toda substituida pelo caractere 'X'*/
            JOGO[i][j]='X';
        }
		(*pontos) += 100;/*e a variavel que soma pontos, no come�o valendo 0, � somada com 100*/
        limpa_telaSuperior(JOGO);/*JOGO na parte superior � limpa(zerada)*/
        mostra_jogo(JOGO, *pontos);/*novo jogo � mostrado*/
        getchar();/*espera o usuario clicar em alguma tecla*/
        k=i;/*variavel auxiliar k passa a ter o valor de i(linha que foi substituida por 'X')*/
        while(k>6){/*enquanto k for maior que a 7� linha da matriz*/
            for(j=0;j<20;j++){/*a JOGO toda(coluna por coluna) recebe JOGO na linha da posi��o k anterior(uma linha acima)*/
                JOGO[k][j]=JOGO[k-1][j];
            }
            k--;/*k e decrementado para que todas as linhas acima ate a 7� receba uma linha anterior, ou seja, para que a matriz JOGO des�a*/
        }
    }
    mostra_jogo(JOGO, *pontos);/*novo jogo � mostrado na tela*/
}

int fimdeJogo(char JOGO[][20], int pontos){/*funcao para retornar um valor caso o usuario atinja a 7� linha da JOGO*/
    int j, i, k;/*tres variaveis auxiliares*/
    for (j=0; j<20; j++) {/*repeti��o para percorrer a linha toda, coluna por coluna*/
        if (JOGO[6][j] != ' ') {/*se a 7� linha da linha na posi��o j for diferente de espa�o em branco*/
            clear();/*a tela e limpa*/
            for(i=0; i<15; i++){/*e toda a matriz JOGO � preenchida com X*/
                for(k=0; k<20; k++){
                    JOGO[i][k] = 'X';
                }
            }/*e a fun��o retorna 1*/
            return 1;
        }
    }
    return 0;/*caso contrario, ela retornar� 0*/
}

void mostra_jogo(char JOGO[15][20], int pontos){
    clear();/*limpando a tela antes da nova tela ser mostrada*/
    int i, j;

    printf("  +--------------------+ PONTUACAO: %d\n", pontos);
    for(i=0; i<15; i++){
        if(i==6){
            printf("=>+");
        }else{
            printf("  |");
        }
        for(j=0; j<20; j++){
            printf("%c", JOGO[i][j]);/*imprimindo a borda e a matriz JOGO[15][20](que esta preenchida com espacos vazios no comeco do jogo)*/
        }
        if(i==6){
            printf("+<=\n");
        }
        else{
            printf("|\n");
        }
    }
    printf("  +--------------------+\n");
    printf("R-rotar D-direita E-esquerda C-cair\n");/*imprime as op��es para o usuario*/
    printf("ACAO: ");
}

int main(){
    FILE *fp; /*declarando um arquivo*/
    char MAT[70][7], opcao, JOGO[15][20];
    int pontuacao=0, nseq, i, j;
    int vertical, coluna;
    tela_inicial(); /*Chamando a fun��o apenas para mostrar a tela inicial para o usuario*/

    fp = fopen("sequencias.txt","r");/*Abrindo o arquivo de texto*/
    nseq = 0;/*iniciando um contador auxiliar para a matriz JOGO*/
    for(i=0; i<15; i++){ /*Zerando toda a matriz JOGO preenchendo-a com espa�os em branco*/
        for(j=0; j<20; j++){
            JOGO[i][j] = ' ';/*' ' = espa�o em branco*/
        }
    }

    while(fscanf(fp, "%s", MAT[nseq]) > 0){/*copiando os caracteres do arquivo "sequecias.txt" linha por linha para uma matriz MAT*/
        nseq++;/*cada string lida do arquivo sera armazenada na matriz MAT*/
    }
    fclose(fp);/*fechando o arquivo, pois agora trabalharemos apenas com a matriz MAT para onde foram copiadas as strings*/
    nseq = 0;/*zerando a variavel auxiliar novamente*/
    while(nseq < 70 && leSeq(MAT[nseq], JOGO)){/*estrutura de repeti��o para ler toda as strings armazenadas em MAT*/
        vertical = 1;
        coluna = 9;/*declarando uma variavel que se inicia na coluna do meio de cada linha da matriz JOGO*/
        do{
            mostra_jogo(JOGO, pontuacao);
            scanf("%c", &opcao);
            getchar();
            switch(opcao){ /*usando switch case para identificar uma a��o do teclado do usuario*/
                case 'r': /*r para rodacionar a sequencia de char lida da matriz MAT*/
                    rotacionar(JOGO, MAT[nseq], &vertical, &coluna);/*chamando a fu��o rotacionar*/
                    mostra_jogo(JOGO, pontuacao);
                    break;
                case 'd':
                    direita(JOGO, MAT[nseq], vertical, &coluna);/*chamando a fun��o direita*/
                    mostra_jogo(JOGO, pontuacao);
                    break;
                case 'e':
                    esquerda(JOGO, MAT[nseq], vertical, &coluna);/*chamando a fun��o esquerda, a coluna � enviada por parametros pois o valor pode ser alterado*/
                    mostra_jogo(JOGO, pontuacao);/*imprime a tela do jogo apos modifica��es*/
                    break;
                default:/*caso o usuario digite uma opcao diferente de r, d ou e, nada acontece com o jogo*/
                    break;
            }
        }while(opcao != 'c');/*ficara no enquanto ate o usuario digitar 'c'*/
        cair(JOGO, MAT[nseq], vertical, &coluna);/*se a opcao escolhida for 'c' o programa sai do la�o de repeti��o*/
        limpa_telaSuperior(JOGO);
        mostra_jogo(JOGO, pontuacao);/*e executa a fun��o cair(), mostra a tela do jogo atual*/
        verifica_linha(JOGO, &pontuacao);
        mostra_jogo(JOGO, pontuacao);
        limpa_telaSuperior(JOGO);/*e limpa a tela superior, ou seja, cada coluna ate a linha 7 da matriz JOGO*/
        nseq++;/*e incrementa 'nseq' para o programa ler a proxima linha da matriz MAT[nseq], ou seja, a prosima string armazenada*/
        if(fimdeJogo(JOGO, pontuacao) == 1){/*depois que cada pe�a cair, a fun��o fimdeJogo � chamada, se ela retornar 1 � porque o usuario chegou a linha 6*/
            mostra_jogo(JOGO, pontuacao);/*a nova matriz JOGO preenchida com X � mostrada*/
            printf("FIM DE JOGO! VOCE PERDEU COM %d PONTOS\n", pontuacao);/*uma mensagem informando que o usuario perdeu o jogo � mostrada com seus pontos*/
            return 0;/*o programa chega ao fim retornando 0*/
        }
    }
    /*se o programa saiu do while, quer dizer que toda a sequencia de caractere foi lida, e o usuario venceu o jogo*/
    for(i=0; i<15; i++){/*ap�s ler toda a MAT, a matriz JOGO � preenchida com X*/
        for(j=0; j<20; j++){
            JOGO[i][j] = 'X';
        }
    }
    mostra_jogo(JOGO, pontuacao);/*JOGO � mostrada*/
    printf("\n\nPARABENS!!! VOCE VENCEU COM %d PONTOS\n\n", pontuacao);/*junto de uma mensagem informando ao usuario sua vitoria e sua potua��o no jogo*/
    return 0;/*fim do programa, a fun��o principal retorna 0*/
}
