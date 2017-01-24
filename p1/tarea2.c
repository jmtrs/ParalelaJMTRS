#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define MATRIZ_DIM  800  //800 


int main(int argc,char *argv[]){
	
	srand (time(NULL));
    
    int i,j,k;
	double **matriz;
	double **matriz_2;
	double **matriz_resul;/*Matriz para parte 1*/
	
	
	int tbloque; 
	int nproces, myrank;
	
    MPI_Init(&argc,&argv);  
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	
	tbloque=MATRIZ_DIM/nproces;
	
	if(myrank==0){
	/*Creación y reserva matriz 1*/
		matriz = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	
	    if (!matriz){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<MATRIZ_DIM; i++){
			matriz[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
	     /*Creación y reserva matriz 2 */
		matriz_2 = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	
	    if (!matriz_2){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<MATRIZ_DIM; i++){
				matriz_2[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_2[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
	     /*Creación y reserva matriz resultante */
		matriz_resul = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	
	    if (!matriz_resul){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<MATRIZ_DIM; i++){
			matriz_resul[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_resul[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
		
		
		/* Introducimos los elementos en la matriz 1 */
	    for (i=0; i<MATRIZ_DIM; i++){
		    for (j = 0; j<MATRIZ_DIM; j++){
				matriz[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
			}
	    }
	   
	      /* Imprimimos elementos de la matriz */
		  /* for (i = 0; i<MATRIZ_DIM; i++){
			  printf ("\nElementos matriz 1 fila %d :\n", i);
			  
			  for (j = 0; j<MATRIZ_DIM; j++){
				 printf (" %lf ", matriz[i][j]);
			  }
			  printf ("\n\n");
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
				 printf (" %lf ", matriz_2[i][j]); 
			  }
			  printf ("\n\n");
		   }*/
        
	}
	else{
		
					   /*Creación y reserva matriz 1*/
		matriz = (double **)malloc(sizeof(double*)*tbloque);
	
	    if (!matriz){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<tbloque; i++){
			matriz[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
	     /*Creación y reserva matriz 2 */
		matriz_2 = (double **)malloc(sizeof(double*)*MATRIZ_DIM);
	
	    if (!matriz_2){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<MATRIZ_DIM; i++){
				matriz_2[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_2[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
	     /*Creación y reserva matriz resultante */
		matriz_resul = (double **)malloc(sizeof(double*)*tbloque);
	
	    if (!matriz_resul){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<tbloque; i++){
			matriz_resul[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_resul[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
	}
	
	
	
	if(myrank==0){
		//envio de matriz
		for(i=1;i<nproces;i++){//para repartir entre los procesos menos la raiz
			for(j=0;j<tbloque;j++){//para enviar tbloque bloques
				MPI_Send(matriz[i*tbloque+j],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
		//envio de matriz_2
		for(i=1;i<nproces;i++){//reparte entre nproces
			for(j=0;j<MATRIZ_DIM;j++){//envia tbloque
				MPI_Send(matriz_2[j],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
		
	}else{
		//recepción de matriz
		for(i=0;i<tbloque;i++){
			MPI_Recv(matriz[i],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
		//recepción de matriz_2
		for(i=0;i<MATRIZ_DIM;i++){
			MPI_Recv(matriz_2[i],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
	
	}
	
	/* PARTE 1 */
	for(i=0;i<tbloque;i++){
		for(j=0;j<MATRIZ_DIM;j++){
			for(k=0;k<MATRIZ_DIM;k++){
				matriz_resul[i][j] = matriz_resul[i][j] + matriz[i][k]*matriz_2[k][j];
			}
		}
	}
	
	

	if(myrank>0){
		for(i=0;i<tbloque;i++){
			MPI_Send(matriz_resul[i],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
		}
	
	}
	else{
		for(i=1;i<nproces;i++){
			for(j=0;j<tbloque;j++){
				MPI_Recv(matriz_resul[i*tbloque+j],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
			}
		}
		printf("\nRESULTADO PARTE 1\n");
		/* Imprimimos elementos de la matriz resultado */
		   for (i = 0; i<MATRIZ_DIM; i++){
			  printf ("\nElementos matriz Resultante fila %d :\n", i);
			  
			  for (j = 0; j<MATRIZ_DIM; j++){
				 printf (" %lf ", matriz_resul[i][j]); 
			  }
			  printf ("\n\n");
		   }
        
		
	}
	
	/*Barrera para que lleguen todos los procesos para posteriormente liberar la memoria*/
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (myrank>0){
		
		for(i=0;i<tbloque;i++){
			free(*(matriz+i)); 
		}free(matriz);
		
		for(i=0;i<MATRIZ_DIM;i++){ 
			
			free(*(matriz_2+i)); 
			
		}free(matriz_2);
		
		for(i=0;i<tbloque;i++){ 
			
			free(*(matriz_resul+i)); 
			
		}free(matriz_resul);
		
		
		
	}else{
		
		for(i=0;i<MATRIZ_DIM;i++){
			free(*(matriz+i)); 
		}free(matriz);
		
		for(i=0;i<MATRIZ_DIM;i++){ 
			
			free(*(matriz_2+i)); 
			
		}free(matriz_2);
		
		for(i=0;i<MATRIZ_DIM;i++){ 
			
			free(*(matriz_resul+i)); 
			
		}free(matriz_resul);
		
	}
	
	MPI_Finalize();
}
