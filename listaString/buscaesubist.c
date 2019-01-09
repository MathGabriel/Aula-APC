#include <stdio.h>
#define MAX 101

int tamanho(char str[]){
    int i;
    for (i=0;str[i] != '\0';i++){}
    return i;
}

int comparar(char strA[], char strB[], int i, int b,char strC[])
{
    int j;
    for(j=0;j<b;i++,j++)
    {
        if(strA[i] != strB[j])
        {
            printf("%c",strA[i-j]);
            return 0;
        }
    }
    printf("%s",strC);
    return b-1;

}

int main()
{

    char stra[MAX],strb[MAX],strc[MAX],strd[MAX];
    int i=0,tamA,tamB,tamC;

    scanf("%s",stra);
    scanf("%s",strb);
    scanf("%s",strc);

    tamA=tamanho(stra);
    tamB=tamanho(strb);
    tamC=tamanho(strc);


   for(i=0;i<tamA;i++)
   {
       if(stra[i] == strb[0])
       {
           i+=comparar(stra, strb,i,tamB,strc);
       }
       else
        printf("%c",stra[i]);
   }


    return 0;
}