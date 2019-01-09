#include <stdio.h>

int Fibonacci(int numero){
	if (numero ==0 || numero ==1){
		return 1;
	}
	else{
		return(Fibonacci(numero-2)+Fibonacci(numero-1));
	}
}

int main(){
	int n,i;
	scanf("%d", &n);
	for (i = 0; i < n; i++){
		printf("%d", Fibonacci(i));
	}
	return 0;
}