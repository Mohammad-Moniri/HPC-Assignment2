#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

/*
 * Use the models to calculate the times 
 *
 */


int main(int argc, char *argv[])
{
    // define integers
    int i,size,nc,padsize;
    int mt;
    
    // define variables
    double *x;
    double serial_sum_x,omp_sum_x,serial_el,omp_el,omp_crit_sum_x,omp_crit_el;
    double omp_loc_sum_x,omp_loc_el,omp_loc_padded_sum_x,omp_loc_padded_el;
    double sum_val,t1,t2;

    
    
    // Define parameters for the timer
    int j,repeat;
    double diff_mean,totaltime,mean,std_dev,*t_e;
	    
    // Allocate space
    size=pow(10,7);
    x  = (double*)malloc(size*sizeof(double));
    
    //omp terms
    nc=32; // number of threads
    omp_set_num_threads(nc);
    mt=omp_get_max_threads();  
     
    // Initialize the vector
    for (size_t i = 0; i < size; i++) {
      x[i] = rand() / (double)(RAND_MAX);
    }   
   
   // Start the outer loop
   totaltime=0;
   std_dev=0;
   repeat=5;
   t_e = (double*)malloc(repeat*sizeof(double));
   printf("====================================================================\n");
   printf("Results from multiple runs:\n");

   for (j=0;j<repeat;j++)
   {

   //Calculate the parallel sum 
    sum_val = 0.0;

    t1=omp_get_wtime();
    #pragma omp parallel for
      for (i = 0; i < size; i++) {
       sum_val += x[i];
    }
    t2=omp_get_wtime();
    omp_sum_x=sum_val;
    omp_el=t2-t1;
    t_e[j]=omp_el;
    totaltime+=omp_el;
   
    printf("iteration %d: omp sum= %f, time= %f \n",j+1, omp_sum_x,omp_el);
    
    }
    
    // Calculate the mean
  	mean=totaltime/(float)repeat;
        // Calculate the std deviation
  	for (j=0;j<repeat;j++) {
  	diff_mean += pow(t_e[j]-mean,2);
  	}
	std_dev = sqrt(diff_mean/repeat);

	printf("====================================================================\n");
  	printf("Experemintal performance stats:\n");
  	printf("Number of threads: %d \n", mt);
  	printf("Mean: %11.8f s \n", mean);
  	printf("Std deviation: %11.8f s \n", std_dev);
  	printf("====================================================================\n");
    
    return 0;
}
