#include <stdio.h>
#define MAX 100001

int tamanho(char str[]){
    int i;
    for (i=0;str[i] != '\0';i++){}
    return i;
}

int main(){
	char string[MAX];
	int tamstring,i;
	scanf("%s",string);

	tamstring = tamanho(string);

	for (i = tamstring-1; i >= 0 ; i--){
		printf("%c", string[i]);
	}
	return 0;
}