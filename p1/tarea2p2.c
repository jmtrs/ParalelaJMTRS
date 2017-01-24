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
	
	int nbloques, bloquesproc;  
	int nproces, myrank;
	
    MPI_Init(&argc,&argv);  
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    nbloques = MATRIZ_DIM/TAM_BLOQUES; //5 en 10 2
	
	vector = (int*)malloc(nproces*sizeof(int));

	
	for(i=0;i<nproces;i++){
		vector[i] = 0;
	}
	j=0;
	for(i=0;i<nbloques;i++){
		vector[j]++;
		j++;
		if(j>=nproces){j=0;}//en el caso de ser np mayor = que j, se pone a 0 y sigue incrementando
		
	}
	for(i=0;i<nproces;i++){
		//printf("%d ",vector[i]);
	}
	
	if(myrank == 0){
		
		bloquesproc = vector[myrank]*TAM_BLOQUES; //Si np=1 5 si np=2 3 2	si np=3 2 2 1  
		
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
	    
	    for (i=0; i<MATRIZ_DIM; i++){
		    for (j = 0; j<MATRIZ_DIM; j++){
				matriz_2[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
			}
	    }
	    
		
		
	}else{
		bloquesproc = vector[myrank]*TAM_BLOQUES; 
		
		 /*Creación y reserva matriz 1*/
		matriz = (double **)malloc(sizeof(double*)*bloquesproc);
	
	    if (!matriz){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<bloquesproc; i++){
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
		matriz_resul = (double **)malloc(sizeof(double*)*bloquesproc);
	
	    if (!matriz_resul){
			printf ("\nFallo de Memoria !\n\n");
			exit (EXIT_FAILURE);
	    }
	
	    for (i=0; i<bloquesproc; i++){
			matriz_resul[i] = (double*)malloc(sizeof(double)*MATRIZ_DIM); 
		
			if (!matriz_resul[i]){
				printf ("\nFallo de Memoria !\n");
				exit (EXIT_FAILURE);
			}
	    }
	    
		
	}
	
	if(myrank == 0){
		
		//ENVIO MATRIZ 1 por partes
		int pr=0,n=0;
		for(i=0;i<nbloques;i++){
			
			if(pr == 0){
				n=n+TAM_BLOQUES;
					
			}else{
				for(j=n;j<n+TAM_BLOQUES;j++){
					MPI_Send(matriz[j],MATRIZ_DIM,MPI_DOUBLE,pr,5,MPI_COMM_WORLD);
					
				}
				n=n+TAM_BLOQUES;
			}
			pr++;	
		if(pr>=nproces){pr=0;}	
		}
		
		//envio de matriz_2
		for(i=1;i<nproces;i++){
			for(j=0;j<MATRIZ_DIM;j++){
				MPI_Send(matriz_2[j],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
			
	
		
	}else{
		
		for(i=0;i<bloquesproc;i++){
			MPI_Recv(matriz[i],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
			
		//recepción de matriz_2
		for(i=0;i<MATRIZ_DIM;i++){
			MPI_Recv(matriz_2[i],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
	   
		
	}
	
	/* PARTE 2 */
	
	if(myrank == 0){
		
		
		int pr=0,n=0;
		for(i=0;i<nbloques;i++){
			
			if(pr == 0){
				
				for(l=n;l<n+TAM_BLOQUES;l++){
					for(j=0;j<MATRIZ_DIM;j++){
						for(k=0;k<MATRIZ_DIM;k++){
							matriz_resul[l][j] = matriz_resul[l][j] + matriz[l][k]*matriz_2[k][j];
						}
					}
				}
				
				n=n+TAM_BLOQUES;
			}else{
				n=n+TAM_BLOQUES;
			}
			pr++;	
		if(pr>=nproces){pr=0;}	
		}
		
		
		
	}else{
		
		for(l=0;l<bloquesproc;l++){
			for(j=0;j<MATRIZ_DIM;j++){ 
				for(k=0;k<MATRIZ_DIM;k++){
					matriz_resul[l][j] = matriz_resul[l][j] + matriz[l][k]*matriz_2[k][j];
				}
			}
		}
		
	}
        
	
	if(myrank>0){	
	
		for(j=0;j<bloquesproc;j++){
			MPI_Send(matriz_resul[j],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
		}
				
		
	}else{
		
		   
		int pr=0,n=0;
		for(i=0;i<nbloques;i++){
			
			if(pr == 0){
				n=n+TAM_BLOQUES;
					
			}else{
				for(j=n;j<n+TAM_BLOQUES;j++){
					MPI_Recv(matriz_resul[j],MATRIZ_DIM,MPI_DOUBLE,pr,5,MPI_COMM_WORLD,&status);
					
				}
				n=n+TAM_BLOQUES;
			}
			pr++;	
		if(pr>=nproces){pr=0;}	
		}
		
		printf("\nRESULTADO PARTE 2\n");
		/* Imprimimos elementos de la matriz resultado */
		   for (i = 0; i<MATRIZ_DIM; i++){
			  printf ("\nElementos matriz pro: %d Resultante fila %d :\n",myrank, i);
			  
			  for (j = 0; j<MATRIZ_DIM; j++){
				 printf (" %lf ", matriz_resul[i][j]); 
			  }
			  printf ("\n\n");
		   }
		
	}
	
	
	
	
	/*Barrera para que lleguen todos los procesos para posteriormente liberar la memoria*/
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (myrank>0){
		
		for(i=0;i<bloquesproc;i++){
			free(*(matriz+i)); 
		}free(matriz);
		
		for(i=0;i<MATRIZ_DIM;i++){ 
			
			free(*(matriz_2+i)); 
			
		}free(matriz_2);
		
		for(i=0;i<bloquesproc;i++){ 
			
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
