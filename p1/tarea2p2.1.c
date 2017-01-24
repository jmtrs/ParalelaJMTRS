#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define MATRIZ_DIM  800 
#define TAM_BLOQUES 50

int main(int argc,char *argv[]){
	
	srand (time(NULL));
    
    int i,j,k,l;
	double **matriz;
	double **matriz_2;
	double **matriz_resul;/*Matriz para parte 1*/
	
	int *vector;
	
	int nbloques;  
	int nproces, myrank;
	
    MPI_Init(&argc,&argv);  
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    nbloques = MATRIZ_DIM/TAM_BLOQUES;
	
	if(myrank == 0){
		
		
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
		
		
	}else{
		
		 /*Creación y reserva matriz 1*/
		matriz = (double **)malloc(sizeof(double*)*TAM_BLOQUES);
	
	    if (!matriz){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<TAM_BLOQUES; i++){
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
		matriz_resul = (double **)malloc(sizeof(double*)*TAM_BLOQUES);
	
	    if (!matriz_resul){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<TAM_BLOQUES; i++){
			matriz_resul[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_resul[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
		
	}
	
	for(i=0;i<MATRIZ_DIM;i++){
		MPI_Bcast(matriz_2[i],MATRIZ_DIM,MPI_DOUBLE,0,MPI_COMM_WORLD);
	}
	
	if(myrank == 0){
		
		while(i<=nbloques){
			for(j=1;j<nproces;j++){
				for(k=0;k<TAM_BLOQUES;k++){
					MPI_Send(matriz[TAM_BLOQUES*i+50*j+k],MATRIZ_DIM,MPI_DOUBLE,j,5,MPI_COMM_WORLD);
					i++;
				}
			}
			i=i-j+1;
			
			int c=i*50,f=i*100;
			for(i=c;i<f;i++){
				for(j=0;j<MATRIZ_DIM;j++){
					for (k=c;k<f;k++){
						matriz_resul[i][j] = matriz_resul[i][j] + matriz[i][k] * matriz_2[k][j];
					}
				}
			}
			
			for(j=1;j<nproces;j++){
				for(k=0;k<TAM_BLOQUES;k++){
					MPI_Recv(matriz[i*j*50],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
				}
			}
			i=i+nproces;
		}
		
	}else{
		while(i<nbloques){
			for(j=0;j<TAM_BLOQUES;j++){
				MPI_Recv(matriz[j],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
			}
			printf ("\n Proc: %d Elementos matriz Resultante fila %d :\n",myrank, i);
			  
					  for (l = 0; l<TAM_BLOQUES; l++){
						 printf (" %lf ", matriz[j][l]); 
					  }
					  printf ("\n\n");	
			for(i=0;i<TAM_BLOQUES;i++){
				for(j=0;j<MATRIZ_DIM;j++){ 
					for (k=0;k<TAM_BLOQUES;k++){
							matriz_resul[i][j] = matriz_resul[i][j] + matriz[i][k] * matriz_2[k][j];
					}
					printf ("\n Proc: %d Elementos matriz Resultante fila %d :\n",myrank, i);
			  
					  for (l = 0; l<TAM_BLOQUES; l++){
						 printf (" %lf ", matriz_resul[i][j]); 
					  }
					  printf ("\n\n");
					
				}
			}
			
	
				
			for(k=0;k<TAM_BLOQUES;k++){
				MPI_Send(matriz[k],MATRIZ_DIM,MPI_DOUBLE,j,5,MPI_COMM_WORLD);
			}
			i=i+nproces;
		}
	}
	
/*	if(myrank==0){ 
		
			i=i-nproces+1;
			int c=i*50,f=i*100;
			for(i=c;i<f;i++){
				for(j=0;j<MATRIZ_DIM;j++){
					for (k=c;k<f;k++){
						matriz_resul[i][j] = matriz_resul[i][j] + matriz[i][k] * matriz_2[k][j];
					}
				}
			}
			for(i=1;i<=j;i++)
			{
				for(k=0;k<50;k++)
				{
					MPI_Recv(matriz[50*(i-nproces+1+i)+k],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);

				}
			}
			
			for (i = 0; i<MATRIZ_DIM; i++){
			  printf ("\nElementos matriz resultante final parte 2 fila %d :\n", i);
			  
			  for (j = 0; j<MATRIZ_DIM; j++){
				 printf (" %lf ", matriz_resul[i][j]);
			  }
			  printf ("\n\n");
		   }
   
	}
		
	
	
	/*Barrera para que lleguen todos los procesos para posteriormente liberar la memoria*/
	
	/*MPI_Barrier(MPI_COMM_WORLD);
	
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
		
	}*/
	
	MPI_Finalize();
}
