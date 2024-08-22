# Parallel-Distributed-Computing
### Bubble Sort
```c
#include <omp.h>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include <string.h>

#define NT 16

void print_array (int * arr, int arr_len) {
  for (int i=0; i<arr_len; i++) printf("%d ", arr[i]);
  printf("\n");
}

void swap (int * a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

// to merge to sorted arrays in parallel 
void merge (int * arr1, int* arr2, int size1, int size2){
  int l=0;
  int r=0;

  while (l<size1 && r<size2){
    if (arr1[l] > arr2[r]){
      int temp = arr2[r];
      arr2[r] = arr1[size1-1];

      int i = size1-1;
      while(i>l){
        arr1[i] = arr1[i-1];
        i--;
      }
      arr1[l] = temp;
      size1 ++;
      r++;
    }
    l++;
  }
}


// sequential bubble sort 
void bubble_sort(int *arr, int len){
  int sorted = 0;
  while(!sorted){
    sorted = 1;
    for (int i=0; i<len-1; i++){
      if (arr[i] > arr[i+1]){
        swap(arr+i, arr+i+1);
        sorted = 0;
      } 
    }
  }
}



void work_thread(int thread_num, int * arr, int arr_len, int chunk_size){
  if (thread_num > arr_len) return;
  // defining start and end for bubble sorting for each thre
  int start_idx = omp_get_thread_num() * chunk_size;
  int end_idx = start_idx + chunk_size -1 ;

  if (omp_get_thread_num() == NT-1) end_idx = arr_len-1;

  // length of subarray for each thread
  int len = end_idx - start_idx + 1;

  // sorting each subarray
  bubble_sort(arr + start_idx, len);
}




void merge_thread(int thread_num, int num_threads_req, int * arr, int arr_len, int chunk_size){
  if (thread_num > num_threads_req - 1) return;

  int first_start = thread_num * chunk_size * 2;
  int second_start = thread_num * chunk_size * 2 + chunk_size;

  /*
   * if the last thread => check for the remaining elements
   * remaining elements can be fitted in by changing the chunk size for particular chunk
  */

  // if the last thread
  if (thread_num == num_threads_req - 1){
    // if the chunk doesnt count in the last elements
    if (thread_num * chunk_size*2 + chunk_size*2 != arr_len){
      merge(arr + first_start , arr + second_start, chunk_size, arr_len - second_start);
      return;
    }
  }

  merge(arr + first_start, arr + second_start, chunk_size, chunk_size);
}



void parallel_bubble_sort (int * arr, int arr_len){
  // defining chunk
  int chunk_size = 0;
  chunk_size = arr_len/NT;
  if (!chunk_size) chunk_size = 2;

//to divide array into chunks and sort each chunk in parallel
  #pragma omp parallel shared (arr,arr_len,chunk_size)
  {
    work_thread(omp_get_thread_num(), arr, arr_len, chunk_size);
  }
  

  /*
   * This is platform specific merging code, my CPU supports 16 threads.
   * The algorithm iteratively merge two chunks together resulting in count of sorted array changing from 16->8->4->2->1
  */


  // to merge the arrays in parallel
  int num_threads_req = NT/2;

  while (num_threads_req && chunk_size < arr_len){
    #pragma omp parallel shared(arr, arr_len, num_threads_req, chunk_size)
    {
      merge_thread(omp_get_thread_num(), num_threads_req, arr, arr_len, chunk_size);
    }
    num_threads_req/=2;
    chunk_size *=2;
  }

}



int main (){
  // initialising array
  int arr_len = 100000;
  int * arr = (int*)malloc(sizeof(int) * arr_len);
  // filling up array in decreasing order
  for (int i=0; i<arr_len; i++) arr[arr_len-i-1] =i+1;

  int * arr_ = (int*) malloc (sizeof(int) * arr_len);
  memcpy(arr_, arr, sizeof(int) * arr_len);

  // serial sort
  time_t start_time = time(NULL);
  bubble_sort(arr, arr_len);
  time_t end_time = time(NULL);
  printf("Serial Bubble sort took : %.5f sec\n", (double)  difftime(end_time, start_time));

  // parallel sort
  start_time = time(NULL);
  parallel_bubble_sort(arr_, arr_len);
  end_time = time(NULL);
  printf("Parallel Bubble sort took : %.5f sec\n", (double)  difftime(end_time, start_time));

}
```
```
Serial Bubble sort took : 16.00000 sec
Parallel Bubble sort took : 4.00000 sec
```

---
```c
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
```
```
Serial
Pie = 3.1415926687
Time = 3.000 sec
Parallel
Pie = 3.1415926546
Time = 0.000 sec
```
