#include <stdio.h>
#include <omp.h>

int main()
{
	int n_thread;
//	n_thread=6; 
//	omp_set_num_threads(n_thread); 
	#pragma omp parallel
	{
		int ID_thread=omp_get_thread_num();
		printf("Hello World from Thread %d \n", ID_thread);

	}	
 return 0;
}
