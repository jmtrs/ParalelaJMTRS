#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MATRIZ_DIM  9000
#define INVALIDO    -1

int main(int argc,char *argv[]){
	
   srand (time(NULL));
	
   int i,j,k    = INVALIDO;
   double **ptr        = NULL;
   double m, alpha, n; 
   double* vector;
   double* vector_aux;
   
   printf("Introduce un valor para m:\n");
   scanf("%lf",&m);
   printf("Introduce un valor para alpha:\n");
   scanf("%lf",&alpha);
   //printf("m = %lf alpha = %lf\n", m, alpha);
   
   
   vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
   vector_aux = (double*)malloc(MATRIZ_DIM*sizeof(double));
   
   if (!vector || !vector_aux){
		 printf ("\nFallo de Memoria !\n\n");
	     exit (EXIT_FAILURE);
   }
   
   for (i=0; i<MATRIZ_DIM; i++){
   	*(vector+i) = 1;
  // 	printf(" %lf ", *(vector+i));
   }
   

   ptr = malloc(sizeof(double*)*MATRIZ_DIM);

   if (!ptr){
		printf ("\nFallo de Memoria !\n\n");
	    exit (EXIT_FAILURE);
   }

   for (i=0; i<MATRIZ_DIM; i++){
	  *(ptr+i) = malloc(sizeof(double)*MATRIZ_DIM); 

	  if (!*(ptr+i)){
		 printf ("\nFallo de Memoria !\n");
		 exit (EXIT_FAILURE);
	  }
   }

   /* Introducimos los elementos en la matriz */
   for (i=0; i<MATRIZ_DIM; i++){
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
	  	*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	  }
   }

   /* Imprimimos elementos de la matriz */
 /*  for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(ptr+i)+j));
	  }
	  printf ("\n\n");
   }*/
   
   
   
   /*Calculo iterativo, cambiar 1 por m*/
   for(k=0; k<m; k++){
   	 /*Calculo de xk+1 de la matriz por vector x*/
   	 for (i=0; i<MATRIZ_DIM; i++){
   			for (j = 0; j<MATRIZ_DIM; j++){
   				
   				*(vector_aux+i) = *(vector_aux+i) + ( *(*(ptr+i)+j) * *(vector+j)); 
   				
   			}
   	 	
   	 }
   	 
   /*	 for (i=0; i<MATRIZ_DIM; i++){
		printf(" %lf ", *(vector_aux+i));
   	 	
   	 } */
   	 
   	 for (i=0; i<MATRIZ_DIM; i++){
   	      /*Calculo de xk+1 de los alphas*/
   	 		*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+i));
   	 		
   	 		n = n + fabs((*(vector_aux+i)) - (*(vector+i)));
   	 		
   	 		*(vector+i) = *(vector_aux+i);
   	 		*(vector_aux+i) = 0;
   	 	
   	 }
   	 
   	 printf("Iteraccion:%d, calculo: %lf. \n\n",k,n); 
   	 n=0;
   	 
   	 
   	
   }
   

   return 0;
}