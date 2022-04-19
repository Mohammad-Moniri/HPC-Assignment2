#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

//compile with cc -Wall -fopenmp -o omp_local_sum 4.omp_local_sum.c -lm

/*
 * Use the models to calculate the times 
 *
 */


int main(int argc, char *argv[])
{
    // define integers
    int i,size,nc,padsize;
    
    // define variables
    double *x;
    double serial_sum_x,omp_sum_x,serial_el,omp_el,omp_crit_sum_x,omp_crit_el;
    double omp_loc_sum_x,omp_loc_el,omp_loc_padded_sum_x,omp_loc_padded_el;
    double sum_val,t1,t2;

    
    
    // Define parameters for the timer
    int j,repeat;
    double totaltime,mean,std_dev,*t_e;
	    
    // Allocate space
    size=pow(10,7);
    x  = (double*)malloc(size*sizeof(double));
    
    
  
     
    // Initialize the vector
    for (size_t i = 0; i < size; i++) {
      x[i] = rand() / (double)(RAND_MAX);
    }   
   
    //Set the max number of threads
	int maxthreads=32;
	
	for( nc=1; nc <= maxthreads;nc++)
	{
	omp_set_num_threads(nc);
	
	double *local_sum;
	local_sum  = (double*)malloc(nc*sizeof(double));
	
	for (i = 0; i < nc; i++) {
        local_sum[i] = 0;
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

   //Calculate the parallel sum with local 
    sum_val = 0.0;

    t1=omp_get_wtime();
    #pragma omp parallel shared(local_sum)
    {
      int id=omp_get_thread_num(); //IMPORTANT! declare id inside the paralell region, 
                                   //so each thread has a copy, otherwise it is also shared.
      local_sum[id]=0;
      #pragma omp for
        for (i = 0; i < size; i++) {
          local_sum[id] += x[i];
        }
     }
      // sum the local values in serial
    for (i = 0; i < nc; i++) {
      sum_val += local_sum[i];
    }
    t2=omp_get_wtime();
    omp_loc_sum_x=sum_val;
    omp_loc_el=t2-t1;
    
    //acumulate time
    t_e[j]=omp_loc_el;
    totaltime+=omp_loc_el;
   
    printf("iteration %d: local sum= %f, time= %f \n",j+1, omp_loc_sum_x,omp_loc_el);
    
    }
    
    // Calculate the mean
  	mean=totaltime/(float)repeat;
        // Calculate the std deviation
  	for (j=0;j<repeat;j++) {
  	std_dev += sqrt(pow(t_e[j]-mean,2)/repeat);
  	}
	  
	printf("====================================================================\n");
  	printf("Experemintal performance stats:\n");
  	printf("Number of threads: %d \n", nc);
  	printf("Mean: %11.8f s \n", mean);
  	printf("Std deviation: %11.8f s \n", std_dev);
  	printf("====================================================================\n");
    }
    return 0;
}
