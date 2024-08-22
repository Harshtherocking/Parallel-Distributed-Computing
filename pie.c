#include <stdio.h>
#include <omp.h>
#include <time.h>

#define PieFunc(x) 4/(1+x*x)
#define dx 0.000000001

// rectangular area calc
double calc_step_area (double x){
  return PieFunc(x) * dx;
}

// serial implementation
double sequential_pie (){
  double pie = 0;
  double x = 0;
  while (x<=1){
    pie += calc_step_area(x);
    x += dx;
  }
  return pie;
}


double parallel_pie (){
  int num_rect = 1/dx;
  double pie = 0;
  #pragma omp parallel for reduction(+:pie)
    for (int i=0; i<=num_rect; i++){
      pie += calc_step_area(i*dx);
    }
  return pie;
}


// driver code
int main (){

  // serial 
  time_t start, end;
  start = time(NULL);
  double pie = sequential_pie();
  end = time(NULL);
  printf("Serial\nPie = %.10f\nTime = %.3f sec\n",pie, (double)difftime(end,start));

  start = time(NULL);
  pie =  parallel_pie();
  end = time(NULL);
  printf("Parallel\nPie = %.10f\nTime = %.3f sec\n",pie, (double)difftime(end,start));
}
