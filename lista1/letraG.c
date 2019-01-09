#include <stdio.h>

int main(){
	int d,sla;
	int ano,mes,dia;
	scanf("%d",&d);
	ano = d/365;
	d = d%365;
	mes = d/30;
	d = d-mes*30;
	printf("%d %d %d\n", ano,mes,d);
	return 0;
}