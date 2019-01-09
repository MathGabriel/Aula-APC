#include <stdio.h>

int trocar(long long int si[],int n) {
    int aux,res=0,i,j;

    for(i = 0; i < n; i++){
        for(j = 0; j < n-1-i; j++){
            if(si[j] > si[j+1]){
                aux = si[j];
                si[j] = si[j+1];
                si[j+1] = aux;
                res++;
            }
        }
    }
    return res;
}

int main(){
    long long int n,i,resultado;
    long long int si[100000];
   
    scanf("%lld",&n);

    for(i=0;i<n;i++){
        scanf("%lld",&si[i]);
    }

    resultado = trocar(si,n);

    printf("%lld\n",resultado);

    return 0;
}