#include <stdio.h>
#include <omp.h>
#include <math.h>

#define N 1000000

int is_prime (int x){
    if (x == 1 ) return 1;
    if (x % 2 == 0) return 0;
    else {

        int flag =1;
        // Parallel loop to check divisibility
        #pragma omp parallel for shared(x)
        for (int i=3; i < (int)ceil(sqrt(x)); i+=2){
            if (x % i == 0){
                #pragma omp critical
                    flag = 0;
            }
        }

        return flag;
    }
}

void gap (){
    int lower=0;
    int upper=0;
    int gap=0;

    double start_time = omp_get_wtime();

    // Loop for finding first prime number
    #pragma omp parallel for shared(lower, upper, gap) 
    for (int i = 2; i<N; i ++){

        // check for prime 
        if (is_prime(i)){

            // Loop for second consecutive prime number
            for (int j = i+1; j<N; j++){

                // check for prime
                if (is_prime(j)){
                    // check for gap
                    if ((j-i) > gap){
                        #pragma omp critical
                        {
                        lower = i;
                        upper = j;
                        gap = j-i;
                        }
                    }
                    break;
                }
            }
        }
    }

    double end_time = omp_get_wtime();
    
    printf("Max Gap : %d\nPrimes : %d %d\nN : %d\n", gap, lower, upper, N);
    printf("Execution Time: %.5f seconds\n", end_time - start_time);
}

int main(){
    gap();
    return 0;
}
