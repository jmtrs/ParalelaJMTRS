#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

    

int main(int argc,char *argv[]){
	
	double *vector		= NULL; 
	double *vector_aux	= NULL;
	double **ptr    	= NULL;
	
	int myrank, ini, fin;
	int NUM_HILOS = 5;
	int MATRIZ_DIM = 2000;
	int nproces = NUM_HILOS;
	
	int tbloque = 0; 
	
	
	double tini,tfin;
	double  m, alpha,n,s ,aux = 0;
	m = 3, alpha = 0.99;  
	
	int i,j,k;
	
	if((MATRIZ_DIM%nproces) != 0){
    	tbloque=MATRIZ_DIM/nproces+1;
   	}else{
   		tbloque=MATRIZ_DIM/nproces;
   	} 
	
	
	
	
	//________________________________________CON AUTOMATIZADORES______________________________________
	
	//*************** Todos los procesos cooperan para calcular cada uno de los elementos****************
	
	
	
	printf("\n____________CON AUTOMATIZADORES__________________\n\n");
	
	
	
	ptr = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	  
	if (!ptr){
		printf ("\nFallo de Memoria !2\n\n");
		exit (EXIT_FAILURE);
	}
	  
	for (i=0; i<MATRIZ_DIM; i++){
		*(ptr+i) = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
	  
		if (!*(ptr+i)){
			printf ("\nFallo de Memoria !3\n");
			exit (EXIT_FAILURE);
		}
	} 
		
	vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
	vector_aux = (double*)malloc(MATRIZ_DIM*sizeof(double));
			 
	if (!vector || !vector_aux){
		printf ("\nFallo de Memoria !1\n\n");
		exit (EXIT_FAILURE);
	}
		
	for (i=0; i<MATRIZ_DIM; i++){
		*(vector+i) = 1;
		*(vector_aux+i) = 0;
	}
	

	srand (time(NULL));
	for (i=0; i<MATRIZ_DIM; i++){
		for (j = 0; j<MATRIZ_DIM; j++){
			*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		}
	}

 
   	printf("\nTodos los procesos cooperan para calcular cada uno de los elementos\n\n");
   	tini=omp_get_wtime();
	for(k=0;k<m;k++){
		#pragma omp parallel num_threads(nproces) default(shared) private(myrank, nproces,i,j)
		{
		    nproces = omp_get_num_threads();
	        myrank = omp_get_thread_num();
	        
	        
		    #pragma omp for reduction(+:aux)
	         for (i=0; i<MATRIZ_DIM; i++){
	         	aux = 0; 
	   			for (j = 0; j<MATRIZ_DIM; j++){
	   				
	   				aux = aux + ( *(*(ptr+i)+j) * *(vector+j));
	   				
	   				
	   			}
	   			#pragma omp critical 
	    		{
	   				 *(vector_aux+i) =  aux;
	    		} 
	   			
			 }
			 
	

			#pragma omp for
	   		for (i=0; i<MATRIZ_DIM; i++){
	   	      /*Calculo de xk+1 de los alphas*/
	   	      
	   	 		*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+i));
	   	 		
	   	 		n = n + fabs((*(vector_aux+i)) - (*(vector+i)));
	   	 		
	   	 		*(vector+i) = *(vector_aux+i);
	   	 		*(vector_aux+i) = 0;
	 
		   	 }
		   	 
	
		}
			printf("Iteraccion:%d, calculo: %lf. \n\n",k,n); 
			n=0;
	}
	tfin=omp_get_wtime();
	printf("Ejecucion: %lf ms\n",tfin-tini);

		free(vector); 
		free(vector_aux); 
				
		for(i=0;i<MATRIZ_DIM;i++){ 
					
			free(*(ptr+i)); 
		}free(ptr);
		n=0;

	
	
	
	
	
	
	

	//*************** Cada proceso calcula un elemento del vector resultado ****************
	
	
	
	
	
	
	
	

	
	ptr = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	  
	if (!ptr){
		printf ("\nFallo de Memoria !2\n\n");
		exit (EXIT_FAILURE);
	}
	  
	for (i=0; i<MATRIZ_DIM; i++){
		*(ptr+i) = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
	  
		if (!*(ptr+i)){
			printf ("\nFallo de Memoria !3\n");
			exit (EXIT_FAILURE);
		}
	} 

	vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
	vector_aux = (double*)malloc(MATRIZ_DIM*sizeof(double));
			 
	if (!vector || !vector_aux){
		printf ("\nFallo de Memoria !1\n\n");
		exit (EXIT_FAILURE);
	}
		
	for (i=0; i<MATRIZ_DIM; i++){
		*(vector+i) = 1;
		*(vector_aux+i) = 0;
	}
	

	srand (time(NULL));
	for (i=0; i<MATRIZ_DIM; i++){
		for (j = 0; j<MATRIZ_DIM; j++){
			*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		}
	}
	
	
   	
   	printf("\nCada proceso calcula un elemento del vector resultado\n\n");
   	tini=omp_get_wtime();

	for(k=0;k<m;k++){
	#pragma omp parallel num_threads(nproces) default(shared) private(myrank, nproces,i,j)
	{
	    nproces = omp_get_num_threads();
        myrank = omp_get_thread_num();
        

        
		double* resul = (double*)malloc(MATRIZ_DIM*sizeof(double));
        for (i=0; i<MATRIZ_DIM; i++){
        	
         	#pragma omp for
   			for (j = 0; j<MATRIZ_DIM; j++){
   				
   				*(resul+i) = *(resul+i) + ( *(*(ptr+i)+j) * *(vector+j));  
   				
   			}
		}
		
	
		 
		#pragma omp critical
	    {
	        for(i=0; i<MATRIZ_DIM; i++) *(vector_aux+i) += *(resul+i);
	    }
	    
	    
		 
		#pragma omp for
   		for (i=0; i<MATRIZ_DIM; i++){
   	      /*Calculo de xk+1 de los alphas*/
   	      
   	 		*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+i));
   	 		
   	 		s = s + fabs((*(vector_aux+i)) - (*(vector+i)));
   	 		
   	 		*(vector+i) = *(vector_aux+i);
   	 		*(vector_aux+i) = 0;
 
	   	 }
	   	 
	
	}
	printf("Iteraccion:%d, calculo: %lf. \n\n",k,s); 
	s=0;
	}
	tfin=omp_get_wtime();  
	printf("Ejecucion: %lf ms\n",tfin-tini);
	//LIBERAR MEMORIA
	free(vector); 
	free(vector_aux); 
		
	for(i=0;i<MATRIZ_DIM;i++){ 
			
		free(*(ptr+i)); 
	}free(ptr);
	n=0;	
   
   
   
   
   
   
   	
	//________________________________________SIN AUTOMATIZADORES______________________________________
	
	//*************** Todos los procesos cooperan para calcular cada uno de los elementos****************
	
	
	
	printf("\n____________SIN AUTOMATIZADORES__________________\n\n");
	
	
	
	ptr = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	  
	if (!ptr){
		printf ("\nFallo de Memoria !2\n\n");
		exit (EXIT_FAILURE);
	}
	  
	for (i=0; i<MATRIZ_DIM; i++){
		*(ptr+i) = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
	  
		if (!*(ptr+i)){
			printf ("\nFallo de Memoria !3\n");
			exit (EXIT_FAILURE);
		}
	} 
		
	vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
	vector_aux = (double*)malloc(MATRIZ_DIM*sizeof(double));
			 
	if (!vector || !vector_aux){
		printf ("\nFallo de Memoria !1\n\n");
		exit (EXIT_FAILURE);
	}
		
	for (i=0; i<MATRIZ_DIM; i++){
		*(vector+i) = 1;
		*(vector_aux+i) = 0;
	}
	

	srand (time(NULL));
	for (i=0; i<MATRIZ_DIM; i++){
		for (j = 0; j<MATRIZ_DIM; j++){
			*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		}
	}

 
   	printf("\nTodos los procesos cooperan para calcular cada uno de los elementos\n\n");
   	tini=omp_get_wtime();
	for(k=0;k<m;k++){
		#pragma omp parallel num_threads(nproces) default(shared) private(myrank, nproces,i,j,ini,fin)
		{
		    nproces = omp_get_num_threads();
	        myrank = omp_get_thread_num();
	        
			ini = tbloque * myrank;
			fin= tbloque * (myrank + 1);
	        //printf("Inicio, Fin: %d - %d tb: %d del hilo %d\n\n",ini,fin,tbloque,myrank); 

	        
		   for(i=ini;i<fin;i++){
	   			for (j = 0; j<MATRIZ_DIM; j++){
	   				
	   				 *(vector_aux+i) =  *(vector_aux+i) + ( *(*(ptr+i)+j) * *(vector+j));
	   			}
			}
			 
		}
		for(i=0;i<MATRIZ_DIM;i++){ 
	   	     /*Calculo de xk+1 de los alphas*/
	   	      
	   	 		*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+i));
	   	 		
	   	 		n = n + fabs((*(vector_aux+i)) - (*(vector+i)));
	   	 		
	   	 		*(vector+i) = *(vector_aux+i);
	   	 		*(vector_aux+i) = 0;
		 }
			printf("Iteraccion:%d, calculo: %lf. \n\n",k,n); 
			n=0;
	}
	tfin=omp_get_wtime();
	printf("Ejecucion: %lf ms\n",tfin-tini);

		free(vector); 
		free(vector_aux); 
				
		for(i=0;i<MATRIZ_DIM;i++){ 
					
			free(*(ptr+i)); 
		}free(ptr);
n=0;

	
	
	
	
	
	
	

	//*************** Cada proceso calcula un elemento del vector resultado ****************
	
	
	
	
	
	
	
	

	
	ptr = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	  
	if (!ptr){
		printf ("\nFallo de Memoria !2\n\n");
		exit (EXIT_FAILURE);
	}
	  
	for (i=0; i<MATRIZ_DIM; i++){
		*(ptr+i) = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
	  
		if (!*(ptr+i)){
			printf ("\nFallo de Memoria !3\n");
			exit (EXIT_FAILURE);
		}
	} 

	vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
	vector_aux = (double*)malloc(MATRIZ_DIM*sizeof(double));
			 
	if (!vector || !vector_aux){
		printf ("\nFallo de Memoria !1\n\n");
		exit (EXIT_FAILURE);
	}
		
	for (i=0; i<MATRIZ_DIM; i++){
		*(vector+i) = 1;
		*(vector_aux+i) = 0;
	}
	

	srand (time(NULL));
	for (i=0; i<MATRIZ_DIM; i++){
		for (j = 0; j<MATRIZ_DIM; j++){
			*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		}
	}
	
	
   	
   	printf("\nCada proceso calcula un elemento del vector resultado\n\n");
   	tini=omp_get_wtime();

	for(k=0;k<m;k++){
	
    	aux = 0;
	
		for (i=0; i<MATRIZ_DIM; i++){
        	
	        #pragma omp parallel num_threads(nproces) default(shared) firstprivate(i) private(myrank, nproces,j) reduction(+:aux)
			{ 
			    nproces = omp_get_num_threads();
		        myrank = omp_get_thread_num();
	        	
	        	   
				ini = tbloque * myrank;
				fin= tbloque * (myrank + 1);
		        //printf("Inicio, Fin: %d - %d tb: %d del hilo %d\n\n",ini,fin,tbloque,myrank); 
	
		        
			   for(j=ini;j<fin;j++){
	   				aux = aux + ( *(*(ptr+i)+j) * *(vector+j));  
	   				*(vector_aux+i) = aux;
	   				
	   			}
			}
		}
		 
		 
   		for (i=0; i<MATRIZ_DIM; i++){
   	      /*Calculo de xk+1 de los alphas*/
   	      
   	 		*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+i));
   	 		
   	 		s = s + fabs((*(vector_aux+i)) - (*(vector+i)));
   	 		
   	 		*(vector+i) = *(vector_aux+i);
   	 		*(vector_aux+i) = 0;
 
	   	 }
 
	   	 
	printf("Iteraccion:%d, calculo: %lf. \n\n",k,s); 
	s=0;
	}

	tfin=omp_get_wtime();  
	printf("Ejecucion: %lf ms\n",tfin-tini);
	//LIBERAR MEMORIA
	free(vector); 
	free(vector_aux); 
		
	for(i=0;i<MATRIZ_DIM;i++){ 
			
		free(*(ptr+i)); 
	}free(ptr);
	n=0;	
	




	

	return 0;
}