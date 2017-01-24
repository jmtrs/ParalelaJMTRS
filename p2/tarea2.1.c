#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>


int main(){
	
   srand (time(NULL));
	
   int i,j,k,l,ini,fin;
   double **matriz;
   double **matriz_2;
   double **matriz_resul; /*Matriz para parte 1*/
   //double **matriz_resul2        = NULL; /*Matriz para parte 2*/
   //double **matriz_resul3        = NULL; /*Matriz para parte 1*/
   //double **matriz_resul4        = NULL; /*Matriz para parte 2*/
   double aux,tini,tfin;
   
   int nproces = 4;
   int MATRIZ_DIM = 4;
   int myrank;
   
   int tbloque=MATRIZ_DIM/nproces;
   
   /*Creacion matriz 1*/
	matriz=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++)
	{
		matriz[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}

   /* Introducimos los elementos en la matriz 1 */
   for (i=0; i<MATRIZ_DIM; i++){
	  for (j = 0; j<MATRIZ_DIM; j++){
		matriz[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	  }
   }

   /* Imprimimos elementos de la matriz */
   /*for (i = 0; i<MATRIZ_DIM; i++){ 
	  printf ("\nElementos matriz 1 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %lf ", *(*(matriz+i)+j));
	  }
	  printf ("\n\n");
   }
  
   /*Creacion matriz 2*/
	matriz_2=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++)
	{
		matriz_2[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}
 
   /* Introducimos los elementos en la matriz 2 */
   for (i=0; i<MATRIZ_DIM; i++){
	  for (j = 0; j<MATRIZ_DIM; j++){
		matriz_2[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
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
	matriz_resul=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++)
	{
		matriz_resul[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}
   
	/*Creacion matriz resultante 2*/
/*   matriz_resul2 = malloc(sizeof(double*)*MATRIZ_DIM);
   for (i=0; i<MATRIZ_DIM; i++){
	  matriz_resul2[i] = malloc(sizeof(double)*MATRIZ_DIM); 
   }
	/*Creacion matriz resultante3*/
/*   matriz_resul3 = malloc(sizeof(double*)*MATRIZ_DIM);
   for (i=0; i<MATRIZ_DIM; i++){
	  matriz_resul3[i] = malloc(sizeof(double)*MATRIZ_DIM); 
   }
   
	/*Creacion matriz resultante 4*/
 /*  matriz_resul4 = malloc(sizeof(double*)*MATRIZ_DIM);
   for (i=0; i<MATRIZ_DIM; i++){
	  matriz_resul4[i] = malloc(sizeof(double)*MATRIZ_DIM); 
   }*/
   
   printf("1º Todos los hilos calculan varios elementos sin automatizador\n");
   
   tini=omp_get_wtime();
   #pragma omp parallel num_threads(nproces) private(myrank,nproces,i,j,k,ini,fin) default(shared)
   { 
		myrank = omp_get_thread_num();
		nproces = omp_get_num_threads();
		tbloque = MATRIZ_DIM/nproces;
		ini = tbloque * myrank;
		fin = tbloque * (myrank + 1);
		for(i=ini;i<fin;i++){
			for(j=0;j<MATRIZ_DIM;j++) {
				for(k=0;k<MATRIZ_DIM;k++){
					matriz_resul[i][j] = matriz_resul[i][j] + (matriz[i][k]*matriz_2[k][j]); 
				}
			}  
		}
	   
   }
   tfin=omp_get_wtime();
   printf("tiempo =%f\n",tfin-tini);

	for (i = 0; i<MATRIZ_DIM; i++){
	  printf ("\nElementos matriz 2 fila %d :\n", i);
	  
	  for (j = 0; j<MATRIZ_DIM; j++){
		 printf (" %f ", matriz_resul[i][j]);
	  }
	  printf ("\n\n");
   }

   /* PARTE 1 */
   
   /*for (i=0; i<200; i++){
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
   
 /*  for(l=0; l < 10 ; l ++){
	   
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