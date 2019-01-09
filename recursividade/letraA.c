#include <stdio.h>
#define MOD (10^9)+7;

int Multiplica(int base,int expoente){
	if (expoente == 0){
		return 1;
	}
	if(expoente == 1){
		return base;
	}
	else{
		long long ok;
		ok=Multiplica(base,expoente/2);
		if (expoente%2==0){
			return ok*ok;
		}
		else{
			return ok*ok*base; 
		}
	}
}

int main(){
	int x,e,res;
	scanf("%d %d", &x,&e);
	res=Multiplica(x,e);
	printf("%d\n", res);
	return 0;
}