#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool is_point_inside (double x, double y){
	return x*x + y*y <= 1 ? true : false; 
}


double* generate_random_points (){
	double* points = (double*) malloc (sizeof(double) * 2);
	points[0] = (double) rand() / (double)RAND_MAX;
	points[1] = (double) rand() / (double)RAND_MAX;
	return points;
}


void show_time (double start, double end){
	printf("Time Taken : %f seconds\n",end-start);
}


double calc_pie_reduction (int num_iter){
	int num_points_in_circle = 0;
	int num_points_total = 0;

	double start = omp_get_wtime();
	#pragma omp parallel for reduction (+:num_points_in_circle, num_points_total)
	for (int i=0; i<num_iter; i++)
	{
		double * point = generate_random_points();
		double x = point[0];
		double y = point[1];

		if (is_point_inside(x,y)) num_points_in_circle +=1;

		num_points_total +=1 ;
	}

	double end = omp_get_wtime();

	show_time(start, end);

	return  4 * (double)num_points_in_circle / (double)num_points_total;
}


double calc_pie_critical (int num_iter){
	int num_points_in_circle = 0;
	int num_points_total = 0;

	double start = omp_get_wtime();

	#pragma omp parallel for shared(num_points_in_circle, num_points_total)
	for (int i=0; i<num_iter; i++){

		double * point = generate_random_points();
		double x = point[0];
		double y = point[1];

		if (is_point_inside(x,y));

		#pragma omp critical
		{
			if (is_point_inside(x,y)) num_points_in_circle ++;
			num_points_total ++ ;
		}

	}

	double end = omp_get_wtime();
	
	show_time(start,end);

	return  4 * (double)num_points_in_circle / (double)num_points_total;
}


double calc_pie_atomic (int num_iter){
	int num_points_in_circle = 0;
	int num_points_total = 0;

	double start = omp_get_wtime();

	#pragma omp parallel for shared(num_points_in_circle, num_points_total)
	for (int i=0; i<num_iter; i++){

		double * point = generate_random_points();
		double x = point[0];
		double y = point[1];


		if (is_point_inside(x,y)){ 
			#pragma omp atomic update 
				num_points_in_circle ++;
		}

		#pragma omp atomic update
			num_points_total ++ ;

	}

	double end = omp_get_wtime();
	
	show_time(start,end);

	return  4 * (double)num_points_in_circle / (double)num_points_total;
}



// ----------------------------------------------------------------------------------------------------------
void start_calc (int num_iter){
	double Pie;
	printf("Reduction : \n");
	Pie = calc_pie_reduction (num_iter) ;
	printf("%f\n",Pie);
printf("Critical : \n");
	Pie = calc_pie_critical (num_iter);
	printf("%f\n",Pie);
printf("Atomic : \n");
	Pie = calc_pie_atomic (num_iter);
	printf("%f\n",Pie);
}

int main (){
	srand(time (0));

	int num_iter = 0;

	while (1){
		printf("Num of iterations : ");
		scanf("%d",&num_iter);
		start_calc (num_iter);
	}

	return 0;
}
