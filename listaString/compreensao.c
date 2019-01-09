#include <stdio.h>
#define MAX 100001

int tamanho(char str[]){
    int i;
    for (i=0;str[i] != '\0';i++){}
    return i;
}

int main(){
	char string[MAX];
	int tamstring,i,aux;
	scanf("%s",string);

	tamstring=tamanho(string);
	
	for (i = 0; i < tamstring; i++){
		if (string[i]==string[i+1]){
			aux++;
		}
		else{

			if (aux+1 != 1){
				printf("%d%c", aux+1,string[i]);
				aux = 0;
			}
			else
				printf("%c",string[i]);
		}
	}

	return 0;
}