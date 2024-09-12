#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
			
void are_prime_neighbour (int x, int y, bool * prime){
	// checking for odd no
	assert(x%2 !=0);
	assert(y%2 !=0);
	// check consecutivity
	assert(y == x+2);

	assert(prime[0]);
	assert(prime[1]);

	#pragma omp parallel for shared(prime) 
		for (int i=3; i<x; i+=2){
			if ( prime[0] == false && prime[1] == false) continue;

			if (x%i == 0){
				#pragma omp critical
				prime[0] = false; 
			}

			if (y%i == 0){
				#pragma omp critical
				prime[1] = false;
			}
		}
}



void calc_prime_till (int n){
	// counting occurence of odd prime neighbours 
	int count = 0;

	// parallel loop for finding odd prime numbers
	#pragma omp parallel for reduction(+:count) 
		for (int i=5; i<=n; i+=2){
			int sum=0;
			bool P [2] = {true,true};
			// check neighbours

			// manually check
			are_prime_neighbour(i-2,i,P);

			if (P[0]==true && P[1]==true){
				printf("Sum of [%d %d] = %d\n",i-2,i,2*i-2);
				count+=1;
			}

		}
	
	printf("There are total %d consecutive odd prime no. within range of %d\n",count,n);
}



int main(){
	calc_prime_till(200);
	return 0;
}
