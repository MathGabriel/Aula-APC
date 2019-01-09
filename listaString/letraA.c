#include <stdio.h>
#define MAX 100000

int tamanho(char str[]){
    int i;
    for (i=0;str[i] != '\0';i++){}
    return i;
}


int main(){
	char string[MAX];
	int tam;

	scanf("%s",string);
	
	tam=tamanho(string);
	
	printf("%d\n",tam);
	return 0;
}