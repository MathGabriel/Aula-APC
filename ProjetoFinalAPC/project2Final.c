/*
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
||    Universidade de Brasilia                                                                          ||
||    Departamento de Ciencia da Computacao                                                             ||
||    Instituto de Ciencias Exatas                                                                      ||
||    Algoritmos e Programação de Computadores - 1/2018                                                 ||
||    Aluno: Matheus Gabriel da Silva Rodrigues                                                         ||
||    Matricula: 180025031                                                                              ||
||    Curso: Computacao(Licenciatura)                                                                   ||
||    Descricao: Programa que cria uma "arte" da imagem inserida.                                       ||
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 */

#include <stdio.h>
#include <math.h>

// struct da imagem P3
typedef struct{
    char form[10];
    int larg, alt;
    int max;
    int imagem[500][500][3];
}Img;

// funcao usada para comparar strings
int compara (const char *string, const char *string1) {
    const unsigned char *p = (const unsigned char *)string;
    const unsigned char *p1 = (const unsigned char *)string1;

    while (*p != '\0') {
        if(*p1 == '\0'){
            return  1;
        }
        if(*p1 > *p){
            return -1;
        }
        if(*p > *p1){
            return  1;
        }

        p++;
        p1++;
    }
    if (*p1 != '\0'){
        return -1;
    }
    return 0;
}

// funcao que apresenta o menu
void fmenu(int *menu){
    printf("==== SEJA BEM VINDO AO PROJETO 2 ====\n");
    printf("\t\t== menu: ==\n");
    printf("\t1 - Abrir uma imagem em disco.\n");
    printf("2 - Processar a imagem\n");
    printf("3 - Salvar a imagem processada em disco\n");
    printf("0 - Encerrar a execucao do programa\n");
    printf("Digite uma das opcoes: ");
    scanf("%d", &*menu); // pede o valor do menu
}

// funcao que le a imagem inserida pelo usuario
void f1(FILE *fp, Img *chis){
    int cont = 1, i, j;
    
    // le a imagem linha por linha
    while(!feof(fp)){
        if(cont == 1){
            fscanf(fp, "%[^\n]", chis->form);
        }
        else if(cont == 2){
            fscanf(fp, "%d %d\n", &chis->larg, &chis->alt);
        }
        else if(cont == 3){
            fscanf(fp, "%d\n", &chis->max);
        }
        else if(cont>3){
            for(i=0; i<chis->alt; i++){
                for(j=0; j<chis->larg; j++){
                    fscanf(fp, "%d %d %d", &chis->imagem[i][j][0], &chis->imagem[i][j][1], &chis->imagem[i][j][2]);
                }
                
            }
        }
        cont++;
    }
    // imprime o codigo da imagem
    printf("P3\n%d %d\n%d\n", chis->larg, chis->alt, chis->max);
    for(i=0; i<chis->alt; i++){
                for(j=0; j<chis->larg; j++){
                    printf("%d %d %d ", chis->imagem[i][j][0], chis->imagem[i][j][1], chis->imagem[i][j][2]);
                }
                printf("\n");
            }
}

// funcao que processa a imagem usando o algoritmo K-means
void f2(Img *chis, Img *seg){
    // declaracao de variaveis
    int j, i, s, r, g, b, t, T, K, ind, menor, somaR, somaG, somaB, cont = 0, D = 0;
    int c[100][3];
    int distancia[100];
    int rotulos[300][300];
    
    
    // pede o numero de K centroides e T iteracoes
    printf("Digite o numero de K: \n");
    scanf("%d", &K);
    printf("Digite o numeto de iteracoes: \n");
    scanf("%d", &T);
    
    // declara os centroides
    for(i = 0; i<K; i++){
        c[i][0] = 255/(i+1);
        c[i][1] = 255/(i+1);
        c[i][2] = 255/(i+1);
    }
    // Processa a imagem T vezes
    for(t = 0; t<T; t++){
        // calcula a distancia euclidiana de cada pixel a todos os centroides
        // armazena em um vetor as distancias
        // e cria uma matriz de tamanho alt*larg em que para cada [i][j], representando os pixels
        // eh salvo o indice do centroide mais proximo
        for (i = 0; i < chis->alt ; i++){
            for (j = 0; j < chis->larg; j++){
                for (s = 0; s < K; s++){
                    r = chis->imagem[i][j][0] - c[s][0];
                    g = chis->imagem[i][j][1] - c[s][1];
                    b = chis->imagem[i][j][2] - c[s][2];
                    
                    r = pow(r,2);
                    g = pow(g,2);
                    b = pow(b,2);
                    D = sqrt(r+g+b);
                    distancia[s] = D;
                }
                menor = distancia[0];
                for(s = 1; s < K; s++){
                    if(distancia[s] < menor){
                        menor = distancia[s];
                        ind = s;
                    }
                }
                rotulos[i][j] = ind;
            }
        }
        // Atualiza os valores dos centroides
        for(s = 0; s < K; s++){
            somaR = 0;
            somaG = 0;
            somaB = 0;
            cont = 0;
            for(i = 0; i < chis->alt ; i++){
                for(j = 0; j < chis->larg; j++){
                    if(rotulos[i][j] == s){
                        somaR = somaR + chis->imagem[i][j][0];
                        somaG = somaG + chis->imagem[i][j][1];
                        somaB = somaB + chis->imagem[i][j][2];
                        cont++;
                    }
                }
            }
            if(cont != 0){
                c[s][0] = somaR/cont;
                c[s][1] = somaG/cont;
                c[s][2] = somaB/cont;
            }
        }
    }
    // atualiza os valores da nova imagem
    seg->alt = chis->alt;
    seg->larg = chis->larg;
    seg->max = chis->max;
    
    for (i = 0; i < seg->alt ; i++){
        for (j = 0; j < seg->larg; j++){
            seg->imagem[i][j][0] = c[rotulos[i][j]][0];
            seg->imagem[i][j][1] = c[rotulos[i][j]][1];
            seg->imagem[i][j][2] = c[rotulos[i][j]][2];
        }
    }
}

// funcao que cria a nova imagem
void f3(Img *seg){
    FILE *new;
    int i, j, cont = 1;
    
    new = fopen("new.ppm", "w"); // cria ou sobrescreve um arquivo
    if(new == NULL){
        printf("Erro ao abrir a nova imagem");
        return ;
    }
    
    while(cont<5){ // escreve dentro do arquivo
        if(cont == 1){
            fprintf(new, "P3\n");
        }
        else if(cont == 2){
            fprintf(new, "%d %d\n", seg->larg, seg->alt);
        }
        else if(cont == 3){
            fprintf(new, "%d\n", seg->max);
        }
        else if(cont>3){
            for(i=0; i<(seg->alt); i++){
                for(j=0; j<(seg->larg); j++){
                    fprintf(new, "%d %d %d ", seg->imagem[i][j][0], seg->imagem[i][j][1], seg->imagem[i][j][2]);
                }
                fprintf(new, "\n");
            }
        }
        cont++;
    }
    fclose(new); // fecha o arquivo
}

// funcao principal / inicio do programa
int main(){
    // declaracao de variaveis
    FILE *fp;
    char file_name[201], pp[5];
    int menu = 1;
    Img chis, seg;
    
    // condicional que obedece o menu
    // caso o usuario digite 0 o programa eh encerrado
    while(menu != 0){
        fmenu(&menu);
    
        if(menu == 1){
            getchar();
            printf("Digite o nome do seu arquivo: \n");
            scanf("%[^\n]", file_name);
            getchar();
    
            fp = fopen(file_name, "r" ); // abre o arquivo para a leitura
    
            if(fp == NULL){
                printf("ERRO NA ABERTURA DO SEU ARQUIVO \n");
                return 0;
            }
            fscanf(fp, "%s", pp);
            if(compara(pp, "P3") != 0){
                printf("Nao eh um arquivo ppm, eh %s\n", pp);
                fclose(fp);
                return 0;
            }
            printf("Imagem P3!\n\n");
            
            f1(fp, &chis); // chama a funcao 1
            fclose(fp);
        }
        else if(menu == 2){
            f2(&chis, &seg); // chama a funcao 2
        }
        else if(menu == 3){
            f3(&seg); // chama a funcao 3
        }
    }
    return 0; // fim do programa
}
