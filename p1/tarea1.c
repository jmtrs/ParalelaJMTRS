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
	
	int i,j,k   	= INVALIDO;
	
	m = 30, alpha = 0.99;
	
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
	
	  
	/*Inicialmente hemos declarado una matriz y dos vectores, Ahora es cuando decidimos que necesita cada proceso.
	
	En el caso del proceso 0. Necesita la Matriz completa, y los dos vectores tambien completos, incialmente "vector"
	es el vector unitario, y "vector_aux" inicialmente es 0, introducimos valores entre 0 y 1 dentro de la matriz
	por último para cada proceso se le envia su bloque correspondiente de matriz. En el caso de no ser divisible y
	ser el ultimo proceso, envia solo los elementos restantes, en caso contrario, se envian los que correspondan a cada proceso.
	
	En el caso de ser el resto de procesos se Reserva memoria para las matrices, pero solo el tamaño del tbloque
	para "vector" se reserva del tamaño completo de matriz y para el "vector_aux" solo del tbloque, tras esto los
	procesos mayores que 0 reciben sus bloques de matriz.*/
	
	if(myrank == 0){
			 
		//MATRIZ
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
		
		//VECTORES
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
	
	
		//INTRODUCCION MATRIZ
		srand (time(NULL));
		for (i=0; i<MATRIZ_DIM; i++){
			for (j = 0; j<MATRIZ_DIM; j++){
				*(*(ptr+i)+j) = (double)rand() / (double)((unsigned)RAND_MAX + 1);
			}
		}
	  
	
		 //ENVIO MATRIZ
		for(i=1;i<nproces;i++){
		
			if((MATRIZ_DIM%nproces)!=0 && (i==nproces-1) ){
				for(j=0;j<tbloqueprc;j++){
					MPI_Send(&(*(*(ptr+(i*tbloque+j))+(0))),MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
				}
					
			}
			else{
				for(j=0;j<tbloque;j++){
					MPI_Send(&(*(*(ptr+(i*tbloque+j))+(0))),MATRIZ_DIM,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
				}
			}
		}
		 
		 
		 
	} else {
		
		//VECTORES
		vector = (double*)malloc(MATRIZ_DIM*sizeof(double));
		vector_aux = (double*)malloc(tbloque*sizeof(double));
			 
		if (!vector || !vector_aux){
			printf ("\nFallo de Memoria !4\n\n");
			exit (EXIT_FAILURE);
		}
		
		//MATRIZ
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
		
		//RECEPCION MATRIZ
		for(i=0;i<tbloque;i++){
			MPI_Recv(&(*(*(ptr+i)+(0))),MATRIZ_DIM,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		} 
		
		
	}
   
	/*								CALCULO ITERATIVO
	Primero debemos enviar el vector "vector" a todos los procesos y en cada iteración, tambien ponemos la norma a 0 solo
	en la primera iteración, todos los procesos deben recibirlo.
	
	Debemos hacer la multiplicacion, cada proceso multiplica su trozo de matriz por el vector que le ha llegado de p0
	y lo guarda en vaux, despues calculamos el valor de alpha. El proceso 0 va a recibir todos los vectores auxiliares
	para despues calcular la norma, tambien debe recibir del ultimmo proceso en caso de no ser divisible, por ultimo se
	actualiza "vector" al valor de vector_aux para la siguiente iteracion
	*/
	for(k=0; k<m; k++){
			
		if(myrank == 0){
			if(k==0){n=0;}
			
			for(i=1;i<nproces;i++){
				MPI_Send(&(*(vector+(0))),MATRIZ_DIM,MPI_DOUBLE,i,6,MPI_COMM_WORLD);
			} 
			
		}else{
			//INICIALIZAR VECTOR
			for(i=0;i<tbloque;i++){
				*(vector_aux+(i))= 0;
			}
			
			MPI_Recv(&(*(vector+(0))),MATRIZ_DIM,MPI_DOUBLE,0,6,MPI_COMM_WORLD,&status);
			
		}
		 
		
		//MULTIPLICACION
		for(i=0;i<tbloque;i++){
			for(j=0;j<MATRIZ_DIM;j++){
				*(vector_aux+i) = *(vector_aux+i) + *(*(ptr+i)+j) * *(vector+j); 
			}
		}
	
		 
	 	//CALCULO ALPHA
		for(i=0;i<tbloque;i++){
	    	*(vector_aux+i) = (alpha * *(vector_aux+i)) + ((1- alpha) * *(vector+(i+tbloquetot*myrank)));
		}

		if(myrank==0){
			
			for(i=1;i<nproces;i++){
				if(((MATRIZ_DIM%nproces)!=0) && (i==nproces-1) ){
					
					  MPI_Recv(&(*(vector_aux+(i*tbloque))),tbloqueprc,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
				}else{MPI_Recv(&(*(vector_aux+(i*tbloque))),tbloque,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);}	
				
			}
			
			if((((k+1)%10)==0) || (m==k+1)){
				for(i=0;i<MATRIZ_DIM;i++){
			        n = n + fabs(*(vector_aux+(i)) - *(vector+(i)));
			    }
				printf("Iteraccion:%d, calculo: %lf. \n\n",k,n); 
			}
			
			
		    for(i=0; i<MATRIZ_DIM; i++){
		        *(vector+(i)) = *(vector_aux+(i));
		    }
				
		}else{MPI_Send(&(*(vector_aux+(0))),tbloque,MPI_DOUBLE,0,5,MPI_COMM_WORLD);}
		
	}
	
	/*Barrera para que lleguen todos los procesos para posteriormente liberar la memoria*/
	
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