#include <stdio.h>
#include <stdlib.h>
#define MAX 21

typedef struct {
	long long int ID;
	char name[MAX];
	int tel1,tel2;
}Pessoa;

void lerstruct(){

}

int main(){
	int n,q,i,aux;
	Pessoa vetor[n];
	scanf("%d %d", &n,&q);
	
	for (i = 0; i < n; i++){
		scanf("%lld",&vetor[i].ID);
		scanf("%[^\n]s",vetor[i].name);
		getchar();
		scanf("%d-%d",&vetor[i].tel1,&vetor[i].tel2);
	}
	for (i = 0; i < q ; i++){
		scanf("%d",&aux);
	}
	return 0;
}