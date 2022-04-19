  // the code calculates a DFT of a random complex number input and 
    // then an IDFT. The IDFT result should be the input vector 
    // to compile with gcc
    // gcc -Wall -O2 -fopenmp -o DFTW_omp 2.DFTW_omp.c -lm 
    // written by stef

    // exercise


	#include "stdio.h" // printf
	#include "stdlib.h" // malloc and rand for instance. Rand not thread safe!
	#include "time.h"   // time(0) to get random seed
	#include "math.h"  // sine and cosine
	#include "omp.h"   // openmp library like timing

	
	// two pi
	#define PI2 6.28318530718
	// this for the rounding error, increasing N rounding error increases
        // 0.01 precision good for N > 8000
	#define R_ERROR 0.01
	
	// main routine to calculate DFT
	int DFT(int idft, double* xr, double* xi, double* Xr_o, double* Xi_o, int N);
	// set the input array with random number
	int fillInput(double* xr, double* xi, int N);
	// set to zero the input vector
	int setOutputZero(double* Xr_o, double* Xi_o, int N);
	// check if x = IDFT(DFT(x))
	int checkResults(double* xr, double* xi, double* xr_check, double* xi_check, double* Xr_o, double* Xi_r, int N);
	// print the results of the DFT
	int printResults(double* xr, double* xi, int N);


	int main(int argc, char* argv[]){
	
	
	// Define parameters for the timer
  	int j,repeat;
  	double totaltime,mean,std_dev,*t_e;
	
	//Set omp variables
//	 int nc=32;
//	 omp_set_num_threads(nc);
         int mt=omp_get_max_threads();
                  
         
	
	
	  // Start the outer loop
          totaltime=0;
          std_dev=0;
          repeat=5;
          t_e = (double*)malloc(repeat*sizeof(double));
          printf("====================================================================\n");
          printf("Results from multiple runs:\n");
	  for (j=0;j<repeat;j++)
  	  {
	
	
	  // size of input array
	  int N = 10000; // 8,000 is a good number for testing
	  printf("DFTW calculation with N = %d \n",N);
	  
	  // Allocate array for input vector
	  double* xr = (double*) malloc (N *sizeof(double));
	  double* xi = (double*) malloc (N *sizeof(double));
	  fillInput(xr,xi,N);

	  // for checking purposes
	  double* xr_check = (double*) malloc (N *sizeof(double));
	  double* xi_check = (double*) malloc (N *sizeof(double));
	  setOutputZero(xr_check,xi_check,N);
	  
	  // Allocate array for output vector
	  double* Xr_o = (double*) malloc (N *sizeof(double));
	  double* Xi_o = (double*) malloc (N *sizeof(double));
	  setOutputZero(Xr_o,Xi_o,N);
      
          
          
      
          // start timer
	  double start_time = omp_get_wtime();
	  

	  // DFT
	  int idft = 1;
	  DFT(idft,xr,xi,Xr_o,Xi_o,N);
	  // IDFT
	  idft = -1;
	  DFT(idft,Xr_o,Xi_o,xr_check,xi_check,N);
	  

	  // stop timer
	  double run_time = omp_get_wtime() - start_time;
	  
	  totaltime+=run_time;
	  t_e[j] = run_time;
	
	  
	
  	
	printf("Iteration %d: DFTW computation in %f seconds\n",j, run_time);
	    
	  // check the results: easy to make correctness errors with openMP
	  checkResults(xr,xi,xr_check,xi_check,Xr_o, Xi_o, N);
	  // print the results of the DFT
      #ifdef DEBUG
      	printResults(Xr_o,Xi_o,N);
	  #endif
	  
	  // take out the garbage
	  free(xr); free(xi);
	  free(Xi_o); free(Xr_o);
	  free(xr_check); free(xi_check);
	  
	  }
	  
	// Calculate the mean
  	mean=totaltime/(float)repeat;
        // Calculate the std deviation
  	for (j=0;j<repeat;j++) {
  	std_dev += sqrt(pow(t_e[j]-mean,2)/repeat);
  	}
	  
	printf("====================================================================\n");
  	printf("Experemintal performance stats:\n");
  	printf("Number of threads: %d \n", mt);
  	printf("Mean: %11.8f s \n", mean);
  	printf("Std deviation: %11.8f s \n", std_dev);
  	printf("====================================================================\n");
	  
	  

	  return 0;
	}

	// DFT/IDFT routine
	// idft: 1 direct DFT, -1 inverse IDFT (Inverse DFT)
	int DFT(int idft, double* xr, double* xi, double* Xr_o, double* Xi_o, int N){
	  #pragma omp parallel
	  { 
	  #pragma omp for
	  for (int k=0 ; k<N ; k++)
	    {
	        
	        for (int n=0 ; n<N ; n++)  {
	        	// Real part of X[k]
	            Xr_o[k] += xr[n] * cos(n * k * PI2 / N) + idft*xi[n]*sin(n * k * PI2 / N);
	            // Imaginary part of X[k]
	            Xi_o[k] += -idft*xr[n] * sin(n * k * PI2 / N) + xi[n] * cos(n * k * PI2 / N);
	            
	        } 
	    }
	    
	    // normalize if you are doing IDFT
	    if (idft==-1){
	        #pragma omp for
	    	for (int n=0 ; n<N ; n++){
	    	Xr_o[n] /=N;
	    	Xi_o[n] /=N;
	    }
	    }
	    }
	  return 0; 
	}
	
	// set the initial signal 
        // be careful with this 
        // rand() is NOT thread safe in case
	int fillInput(double* xr, double* xi, int N){
	    srand(time(0));
	    for(int n=0; n < 100000;n++) // get some random number first 
	    	rand();
	    for(int n=0; n < N;n++){
	       // Generate random discrete-time signal x in range (-1,+1)
	       //xr[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
	       //xi[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
	       // constant real signal
	       xr[n] = 1.0;
	       xi[n] = 0.0;
	    }
		return 0; 
	}

	// set to zero the output vector
	int setOutputZero(double* Xr_o, double* Xi_o, int N){
	for(int n=0; n < N;n++){
	       Xr_o[n] = 0.0;
	       Xi_o[n] = 0.0; 
	    }
		return 0;
	}

	// check if x = IDFT(DFT(x))
	int checkResults(double* xr, double* xi, double* xr_check, double* xi_check, double* Xr_o, double* Xi_r, int N){
		// x[0] and x[1] have typical rounding error problem
		// interesting there might be a theorem on this
		for(int n=0; n < N;n++){
			if (fabs(xr[n] - xr_check[n]) > R_ERROR)
			    printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n",n,xr[n], n,xr_check[n]);
			if (fabs(xi[n] - xi_check[n]) > R_ERROR)
			    printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n",n,xi[n], n,xi_check[n]);

		}
		printf("Xre[0] = %f \n",Xr_o[0]);
		return 0;
	}

	// print the results of the DFT
	int printResults(double* xr, double* xi, int N){
		for(int n=0; n < N;n++)
			    printf("Xre[%d] = %f, Xim[%d] = %f \n", n, xr[n], n, xi[n]);
		return 0;
	}
