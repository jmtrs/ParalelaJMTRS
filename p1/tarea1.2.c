#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define MATRIZ_DIM  7000    
#define INVALIDO    0

int main(int argc,char *argv[]){
	
	double *vector		= NULL; 
	double *vector_aux	= NULL;
	double **ptr    	= NULL;
	
	int nproces, myrank;
	
	int tbloque = 0, tbloquetot = 0, tbloqueprc = 0; 
	
	double  m, alpha,n;
	
	m = 30, alpha = 0.99;
	
	MPI_Init(&argc,&argv);  
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	 
	int i,j,k   	= INVALIDO;


		 
	//ASIGNO EL TAMAÑO DEL BLOQUE
	//Establezco el valor de tamBloque a enviar a cada proceso (el nº de filas de M que envio)
	if((MATRIZ_DIM%nproces)==0){
		tbloquetot=tbloque=MATRIZ_DIM/nproces;
	}else{
		tbloquetot=tbloque=MATRIZ_DIM/nproces+1;//Si no es divisible, envio una fila más a cada proceso, y al último el restante
		tbloqueprc = MATRIZ_DIM - (tbloque*(nproces-1));//Necesita saberlo P0 y el ultimo
		if(myrank==nproces-1){//Si soy el ultimo mi tamaño de bloque será reducido con el resto
			tbloque=tbloqueprc;
		}
	} 	
	  
//necesitamos dos vectores para cada hijo uno que calcula se multiplica y otro que manda a p0
	
   
	if(myrank == 0){
			 
		
		//Reservamos memoria para la matriz
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
			 //printf(" %lf", *(vector+i));
		}
	
	
		
		srand (time(NULL));
		//Introducimos los elementos en la matriz
		for (i=0; i<MATRIZ_DIM; i++){
			for (j = 0; j<MATRIZ_DIM; j++){
				*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
			}
		}
	  
	
		 //Envia la matriz a cada proceso dividida en bloques
		for(i=1;i<nproces;i++){
			//Si no es divisible y soy el ultimo proceso, envio solo los elem restantes
			if( ((MATRIZ_DIM%nproces)!=0) && (i==nproces-1) ){
				for(j=0;j<tbloqueprc;j++)//envio menos filas al ultimo proceso
					MPI_Send(&(*(*(ptr+(i*tbloque+j))+(0))),MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);//Las columnas son las mismas
					
					
			}
			else{//SI ES DIVISIBLE --> ENVIO NORMAL
				for(j=0;j<tbloque;j++)
					MPI_Send(&(*(*(ptr+(i*tbloque+j))+(0))),MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
		 
		 
		 
	} else {
		//Reservamos memoria para las matrices, solo el tamaño del tbloque
		
		vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
		vector_aux = (double*)malloc(tbloque*sizeof(double));
			 
		if (!vector || !vector_aux){
			printf ("\nFallo de Memoria !4\n\n");
			exit (EXIT_FAILURE);
		}
		
		
		//Reservamos memoria para las matrices, solo el tamaño del tbloque
		ptr = (double **)malloc(sizeof(double*)*tbloque);
	  
		if (!ptr){
			printf ("\nFallo de Memoria !5\n\n");
			exit (EXIT_FAILURE);
		}
	  
		for (i=0; i<tbloque; i++){
			*(ptr+i) = (double *)malloc(sizeof(double)*MATRIZ_DIM); 
	  
			if (!*(ptr+i)){
				printf ("\nFallo de Memoria !6\n");
				exit (EXIT_FAILURE);
			}
		}
		
		//RECIBIO MI BLOQUE DE LA MATRIZ
		//Si no es divisible y soy el ultimo proceso, recibo solo los elem restantes
		//SI ES DIVISIBLE --> RECEPCION NORMAL
		for(i=0;i<tbloque;i++){
			MPI_Recv(&(*(*(ptr+i)+(0))),MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		} 
		
		
	}
   
	//Calculo iterativo, cambiar 1 por m
	for(k=0; k<m; k++){
			
		if(myrank == 0){
			
			//INICIALIZAR vectorRes
			for(i=0;i<MATRIZ_DIM;i++){
				*(vector_aux+(i))= 0;
			}
			
			//ENVIAR VECTOR
			for(i=1;i<nproces;i++){
				MPI_Send(&(*(vector+(0))),MATRIZ_DIM,MPI_DOUBLE,i,6,MPI_COMM_WORLD);
			} 
			
			n=0;
		}else{
			//INICIALIZAR vectorRes
			for(i=0;i<tbloque;i++){
				*(vector_aux+(i))= 0;
			}
			
			//RECIBIR VECTOR
			MPI_Recv(&(*(vector+(0))),MATRIZ_DIM,MPI_DOUBLE,0,6,MPI_COMM_WORLD,&status);//Se sobreescribe V
			
		}
		 
		
		//Calculo de xk+1 de la matriz por vector x
		//Todos los procesos realiza la multiplicación de su bloque de la matriz * vector y lo guardan en su bloque de vRes
		for(i=0;i<tbloque;i++){
			for(j=0;j<MATRIZ_DIM;j++){
				*(vector_aux+i) = *(vector_aux+i) + *(*(ptr+i)+j) * *(vector+j); 
			}
		}
	
		 
	 	//REALIZO LA OPERACION 2
		for(i=0;i<tbloque;i++){//filas
		//En caso de q no sea divisible, tamBloque será reducido, para el ultimo proceso
	    	*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+(i+tbloquetot*myrank)));
		}

		
		if(myrank>0){
			//ENVIAR VERTORRES
				MPI_Send(&(*(vector_aux+(0))),tbloque,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
		}
		else{
			//RECIBIR VECTORRES
			for(i=1;i<nproces;i++){
				if( ((MATRIZ_DIM%nproces)!=0) && (i==nproces-1) ){
					MPI_Recv(&(*(vector_aux+(i*tbloque))),tbloqueprc,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
				}
				else{//SI ES DIVISIBLE --> RECEPCION NORMAL
					MPI_Recv(&(*(vector_aux+(i*tbloque))),tbloque,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
				}	
			}
			
			if((((k+1)%10)==0) || (k==m-1)){
				//P0 calcula la norma cada 10 iteraciones y en la última iteración
				for(i=0;i<MATRIZ_DIM;i++){//filas
			        //Calculando la norma1 
			        n = n + fabs(*(vector_aux+(i)) - *(vector+(i)));
			    }
				printf("Iteraccion:%d, calculo: %lf. \n\n",k,n); 
			}
			
			
		    //Actualizo V al valor de Vaux para la siguiente iteracion
		    for(i=0; i<MATRIZ_DIM; i++){
		        *(vector+(i)) = *(vector_aux+(i));
		    }
	    
		}
		
	}
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	//LIBERAR MEMORIA
	if (myrank>0){
		free(vector); 
		free(vector_aux); 
		
		for(i=0;i<tbloque;i++){
			
			free(*(ptr+i)); 
		}free(ptr);
		
	}else{

		free(vector); 
		free(vector_aux); 
		
		for(i=0;i<MATRIZ_DIM;i++){ 
			
			free(*(ptr+i)); 
			
		}free(ptr);
	}
	 

	MPI_Finalize();
	return 0;
}