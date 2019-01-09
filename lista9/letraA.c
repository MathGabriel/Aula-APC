#include <stdio.h>

int main(){
	long int n,i,res=0;
	long int u[100000],v[100000];

	scanf("%ld",&n);

	for (i = 0; i < n; i++){
		scanf("%ld",&u[i]);
	}
	for (i=0; i<n; i++){
		scanf("%ld",&v[i]);
	}

	for (i = 0; i < n; i++){
		res = res + (u[i]*v[i]);
	}

	if (res==0){
		printf("ortogonais\n");	
	}
	else{
		printf("%ld\n", res);
	}

	return 0;
}