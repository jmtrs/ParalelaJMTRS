#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MATRIZ_DIM  1000
#define INVALIDO    -1

int main(){
	
   srand (time(NULL));
	
   int i,j,k,l    = INVALIDO;
   double **matriz        = NULL;
   double **matriz_2        = NULL;
   double **matriz_resul        = NULL; /*Matriz para parte 1*/
   double **matriz_resul2        = NULL; /*Matriz para parte 2*/
   int s=0; //variable para la parte 2 la cual ira incrementando en 100
   
   /*Creacion matriz 1*/
   matriz = malloc(sizeof(double*)*MATRIZ_DIM);

   if (!matriz){
		printf ("\nFallo de Memoria !\n\n");
	    exit (EXIT_FAILURE);
   }

   for (i=0; i<MATRIZ_DIM; i++){
	  *(matriz+i) = malloc(sizeof(double)*MATRIZ_DIM); 

	  if (!*(matriz+i)){
		 printf ("\nFallo de Memoria !\n");
		 exit (EXIT_FAILURE);
	  }
   }

   /* Introducimos los elementos en la matriz 1 */
   for (i=0; i<MATRIZ_DIM; i++){
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
	  	*(*(matriz+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	  }
   }

   /* Imprimimos elementos de la matriz */
  /* for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos matriz 1 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(matriz+i)+j));
	  }
	  printf ("\n\n");
   }
   
   
   
   
   /*Creacion matriz 2*/
   
   matriz_2 = malloc(sizeof(double*)*MATRIZ_DIM);

   if (!matriz_2){
		printf ("\nFallo de Memoria !\n\n");
	    exit (EXIT_FAILURE);
   }

   for (i=0; i<MATRIZ_DIM; i++){
	  *(matriz_2+i) = malloc(sizeof(double)*MATRIZ_DIM); 

	  if (!*(matriz_2+i)){
		 printf ("\nFallo de Memoria !\n");
		 exit (EXIT_FAILURE);
	  }
   }

   /* Introducimos los elementos en la matriz */
   for (i=0; i<MATRIZ_DIM; i++){
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
	  	*(*(matriz_2+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	  }
   }

   /* Imprimimos elementos de la matriz 2 */
  /* for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos matriz 2 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(matriz_2+i)+j));
	  }
	  printf ("\n\n");
   }
   
   
   
   /*Creacion matriz resultante*/
   matriz_resul = malloc(sizeof(double*)*MATRIZ_DIM);

   if (!matriz_resul){
		printf ("\nFallo de Memoria !\n\n");
	    exit (EXIT_FAILURE);
   }

   for (i=0; i<MATRIZ_DIM; i++){
	  *(matriz_resul+i) = malloc(sizeof(double)*MATRIZ_DIM); 

	  if (!*(matriz_resul+i)){
		 printf ("\nFallo de Memoria !\n");
		 exit (EXIT_FAILURE);
	  }
   }
   
    /*Creacion matriz resultante 2*/
   matriz_resul2 = malloc(sizeof(double*)*MATRIZ_DIM);

   if (!matriz_resul2){
		printf ("\nFallo de Memoria !\n\n");
	    exit (EXIT_FAILURE);
   }

   for (i=0; i<MATRIZ_DIM; i++){
	  *(matriz_resul2+i) = malloc(sizeof(double)*MATRIZ_DIM); 

	  if (!*(matriz_resul2+i)){
		 printf ("\nFallo de Memoria !\n");
		 exit (EXIT_FAILURE);
	  }
   }
   

   /* PARTE 1 */
   
   for (i=0; i<200; i++){
       for (j = 0; j<MATRIZ_DIM; j++){
           *(*(matriz_resul+i)+j) = 0;
           
           for(k=0;k<MATRIZ_DIM ;k++){
             *(*(matriz_resul+i)+j)=(*(*(matriz_resul+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
            }
           
       }
       
   }
   
   for (i=200; i<400 ; i++){
       for (j = 0; j<MATRIZ_DIM; j++){
           *(*(matriz_resul+i)+j) = 0;
           
           for(k=0;k<MATRIZ_DIM ;k++){
             *(*(matriz_resul+i)+j)=(*(*(matriz_resul+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
            }
           
       }
       
   }
   
   for (i=400; i<600 ; i++){
       for (j = 0; j<MATRIZ_DIM; j++){
           *(*(matriz_resul+i)+j) = 0;
           
           for(k=0;k<MATRIZ_DIM ;k++){
             *(*(matriz_resul+i)+j)=(*(*(matriz_resul+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
            }
           
       }
       
   }
   
    for (i=600; i<800 ; i++){
       for (j = 0; j<MATRIZ_DIM; j++){
           *(*(matriz_resul+i)+j) = 0;
           
           for(k=0;k<MATRIZ_DIM ;k++){
             *(*(matriz_resul+i)+j)=(*(*(matriz_resul+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
            }
           
       }
       
   }
   
    for (i=800; i<1000 ; i++){
       for (j = 0; j<MATRIZ_DIM; j++){
           *(*(matriz_resul+i)+j) = 0;
           
           for(k=0;k<MATRIZ_DIM ;k++){
             *(*(matriz_resul+i)+j)=(*(*(matriz_resul+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
            }
           
       }
       
   }
   
    /*for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos matriz resultante final parte 1 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(matriz_resul+i)+j));
	  }
	  printf ("\n\n");
    }*/
   
   
   /* PARTE 2 */
   
   for(l=0; l < 10 ; l ++){
       
       for (i=s; i<s+100 ; i++){
           
           for (j = 0; j<MATRIZ_DIM; j++){
               *(*(matriz_resul2+i)+j) = 0;
               
               for(k=0;k<MATRIZ_DIM ;k++){
                   *(*(matriz_resul2+i)+j)=(*(*(matriz_resul2+i)+j)+((*(*(matriz+i)+k))*(*(*(matriz_2+k)+j))));
                   
               }  
           }
       }
       s += 100;
   }
   
   
   
   /*for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos matriz resultante final parte 2 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(matriz_resul2+i)+j));
	  }
	  printf ("\n\n");
   }*/
   
   
  
   
   return 0;
}