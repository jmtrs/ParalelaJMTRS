#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define MATRIZ_DIM  20

#define INVALIDO    0

int main(int argc,char *argv[]){
    
    int tbloquetot = 0, tbloque = 0, tbloqueprc = 0;
    int nproces, myrank;
    int i,j,k;
    
    double max = 0, maxDef = 0; 
    
	double **matriz = NULL;
    
    MPI_Init(&argc,&argv);  
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	
	/*Para establecer el tamaño del bloque se escoge el valor de tbloque que enviamos a cada proceso
	Primero en el caso de no ser divisible, se envia una fila de más a cada proceso y al último proceso lo que sobre.
	En el caso de ser divisible, enviamos la misma cantidad de matriz a todos los procesos.*/
	
	if((MATRIZ_DIM%nproces) != 0){
		tbloquetot=tbloque;
		tbloque=MATRIZ_DIM/nproces+1;
		tbloqueprc = MATRIZ_DIM - (tbloque*(nproces-1));
		if(myrank==nproces-1){tbloque=tbloqueprc;}
		
	}else{
		tbloquetot=tbloque;
		tbloque=MATRIZ_DIM/nproces;
	} 	
	
	/*Inicialmete se reserva memoria para toda la matriz en el proceso 0 y se envia para cada proceso una parte de la matriz,
	en el caso de ser el ultimo y no ser divisible se envia el tamaño que sobre de matriz, se calcula el maximo de los elementos del proceso 0 
	y se reciben los maximos de los otros porcesos y se busca otra vez el maximo de entre todos los maximos*/
		
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
	    //INTRODUCCION MATRIZ
		for (i=0; i<MATRIZ_DIM; i++){
			for (j = 0; j<MATRIZ_DIM; j++){
				matriz[i][j] = 0;
				matriz[i][j] = ((i+1)*0.1 + (j+1)*0.001);
			}
		}
		


		 //ENVIO MATRIZ
		for(i=1;i<nproces;i++){
		
			if((MATRIZ_DIM%nproces)!=0 && (i==nproces-1) ){
				for(j=0;j<tbloqueprc;j++){
					MPI_Send(&matriz[i*tbloque+j][0],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
				}
					
			}
			else{
				for(j=0;j<tbloque;j++){
					MPI_Send(&matriz[i*tbloque+j][0],MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
				}
			}
		}
		
		double max;
		for (i = 0; i < tbloque; i++) {
			for (j = 0; j < MATRIZ_DIM; j++) {
		  		if(matriz[i][j] > max){
		  			max = matriz[i][j];
		  			
		  		}
		  	}
		}
		printf("El max de %d es %lf \n",myrank,max);
		
		double aux=0;
		maxDef=max;

		for(i=1;i<nproces;i++){
			MPI_Recv(&aux,1,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
			if(aux>maxDef){
				maxDef=aux;
			}
		}
		printf("El maximo total es %lf \n",maxDef);
		
		
	/*Por cada proceso,se crea una matriz de solo el tamaño de bloque necesario para buscar sus maximos, se reciben del proceso 0 
	se busca su maximo, y se envia el maximo al proceso 0 para que el termine el trabajo.*/	 
        
    }else{
      	//MATRIZ
		matriz = (double **)malloc(sizeof(double*)*tbloque);
	  
		if (!matriz){
			printf ("\nFallo de Memoria !1\n\n");
			exit (EXIT_FAILURE);
		}
	  
		for (i=0; i<tbloque; i++){
			matriz[i] = (double *)malloc(sizeof(double)*MATRIZ_DIM); 
	  
			if (!matriz[i]){
				printf ("\nFallo de Memoria !2\n");
				exit (EXIT_FAILURE);
			}
		}
		
		//RECEPCION MATRIZ
		for(i=0;i<tbloque;i++){
			MPI_Recv(&matriz[i][0],MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		} 
		
	
		  double max;
		  for (i = 0; i < tbloque; i++) {
		  	for (j = 0; j < MATRIZ_DIM; j++) {
		  		if(matriz[i][j] > max){
		  			max = matriz[i][j];
		  			
		  		}
		  		
		  	}
		  }
		  printf("El max de %d es %lf \n",myrank,max);
	
        	MPI_Send(&max,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
    }
   
   if(myrank > 0){
		
		
   	
   }else{
   		
   	
   	
   }
   
   
   
	/*Barrera para que lleguen todos los procesos para posteriormente liberar la memoria*/
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	
	//LIBERAR MEMORIA
	if (myrank>0){
		
		for(i=0;i<tbloque;i++){
			free(*(matriz+i)); 
		}free(matriz);
		
	}else{

		
		for(i=0;i<MATRIZ_DIM;i++){
			free(*(matriz+i)); 
		}free(matriz);
	}
    
    
    
    
    
    
    MPI_Finalize();
    
}