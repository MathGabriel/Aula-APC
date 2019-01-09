#include <stdio.h>


int main(){
	int i;
	for(i=0;i<250;i++){
		printf("%d:\033[1m \033[%dm Cor:          \033[0m\n",i,i);

	}
	




	return 0;
}