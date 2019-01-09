#include <stdio.h>
//protótipo da função fatorial
double fatorial(int n){
    if( n <= 1 )
        return 1;
    else{
        return n*fatorial(n-1);
    }
}
int main(){
    int numero;
    double f;

    printf("Digite o numero que deseja calcular o fatorial: ");
    scanf("%d",&numero);

    //chamada da função fatorial
    f = fatorial(numero);

    printf("Fatorial de %d = %.0lf",numero,f);
    return 0;
}
