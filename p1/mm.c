#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#define N 8

/*PROTOTIPOS*/
void imprimirMatriz(double **matriz);
int main(int argc,char *argv[]){
	/*DECLARACIÓN DE VARIABLES MPI*/
    int nproces, myrank;
	MPI_Status status;
	/*DECLARACIÓN DE VARIABLES DEL PROGRAMA*/
	double **A,**B,**R,numAleatorio,t1,t2;
	int tamBloque,i,i2,tamfila,tamcol, j;
	
	/*INICIALIZACIÓN*/
	/*semilla num aleatorio*/
	srand48(time(NULL));
	
	/*INICIO MPI*/
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	
	/*TAMAÑO DEL BLOQUE*/
	tamBloque=N/nproces;
	
	/*DESARROLLO DEL PROGRAMA*/
	
	/*RESERVA DE MEMORIA*/
	if(myrank==0){
		//reserva de memoria para A
		A=(double **) malloc(N*sizeof(double*));
		for(tamcol=0;tamcol<N;tamcol++){
			A[tamcol]=(double *) malloc(N*sizeof(double));
		}
		//reserva de memoria para B
		B=(double **) malloc(N*sizeof(double*));
		for(tamcol=0;tamcol<N;tamcol++){
			B[tamcol]=(double *) malloc(N*sizeof(double));
		}
		//reserva de memoria para matriz resultado
		R=(double **) malloc(N*sizeof(double*));
		for(tamcol=0;tamcol<N;tamcol++){
			R[tamcol]=(double*) malloc(N*sizeof(double));
		}
		
		//llenando datos en A y B
		/*for(tamfila=0;tamfila<N;tamfila++){
			for(tamcol=0;tamcol<N;tamcol++){
				numAleatorio=drand48();
				A[tamfila][tamcol]=numAleatorio;
				numAleatorio=drand48();
				B[tamfila][tamcol]=numAleatorio;
			}
		}*/
		for (i=0;i<N;i++){
            for (j=0;j<N;j++)
                A[i][j] = j+1;
                B[i][j] = j+1;
        }
        A[1][0]=4;A[1][1]=5;A[1][2]=6;
        A[2][0]=7;A[2][1]=8;
        B[1][0]=40;B[1][1]=50;B[1][2]=60;
        B[2][0]=70;B[2][1]=80;
        imprimirMatriz(A);
        imprimirMatriz(B);
        
	}
	else{
		//reserva de memoria para A
		A=(double**) malloc(tamBloque*sizeof(double*));
		for(tamcol=0;tamcol<tamBloque;tamcol++){
			A[tamcol]=(double*) malloc(N*sizeof(double));
		}
		//reserva de memoria para B
		B=(double**) malloc(N*sizeof(double*));
		for(tamcol=0;tamcol<N;tamcol++){
			B[tamcol]=(double*) malloc(N*sizeof(double));
		}
		//reserva de memoria para R
		R=(double**) malloc(tamBloque*sizeof(double*));
		for(tamcol=0;tamcol<tamBloque;tamcol++){
			R[tamcol]=(double*) malloc(N*sizeof(double));
		}
	}
	
	/*ENVIO Y RECEPCIÓN DE DATOS*/
	
	
	
	//envio
	if(myrank==0){
		//envio de A
		for(i=1;i<nproces;i++){//para repartir entre los procesos menos la raiz
			for(i2=0;i2<tamBloque;i2++){//para enviar tamBloque bloques
				MPI_Send(A[i*tamBloque+i2],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
		//envio de B
		for(i=1;i<nproces;i++){//reparte entre nproces
			for(i2=0;i2<N;i2++){//envia tamBloque
				MPI_Send(B[i2],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
	}
	//recepción
	else{
		//recepción de A
		for(i=0;i<tamBloque;i++){
			MPI_Recv(A[i],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
		//recepción de B
		for(i=0;i<N;i++){
			MPI_Recv(B[i],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
		//printf("\nProceso %d ha recibido A y B\n",myrank);
	}
	
	/*REALIZANDO LA MULTIPLIACIÓN*/
	for(i=0;i<tamBloque;i++){
		for(tamfila=0;tamfila<N;tamfila++){
			for(tamcol=0;tamcol<N;tamcol++){
				R[i][tamfila]+=A[i][tamcol]*B[tamcol][tamfila];
			}
		}
	}
	
	/*CALCULO DEL TIEMPO PARA CADA PROCESADOR-SÓLO COMPUTO*/
	t2=MPI_Wtime();
	printf("\nEl proceso %d, se tarda %lf segundos\n",myrank,t2-t1);
	

	if(myrank==0){
		for(i=1;i<nproces;i++){
			for(i2=0;i2<tamBloque;i2++){
				MPI_Recv(R[i*tamBloque+i2],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
			}
		}
		printf("\nMatrizResultado\n");
		imprimirMatriz(R);
		
	}
	else{
		for(i=0;i<tamBloque;i++){
			MPI_Send(R[i],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
		}
	}
	
	
	
	/*LIBERACIÓN DE MEMORIA*/
	if(myrank==0){
		//lib para A
		for(tamcol=0;tamcol<N;tamcol++){
			free(A[tamcol]);
		}
		free(A);
		//lib para B
		for(tamcol=0;tamcol<N;tamcol++){
			free(B[tamcol]);
		}
		free(B);
		//lib para R
		for(tamcol=0;tamcol<N;tamcol++){
			free(R[tamcol]);
		}
		free(R);
	}
	else{
		//lib para A
		for(tamcol=0;tamcol<tamBloque;tamcol++){
			free(A[tamcol]);
		}
		free(A);
		//lib para B
		for(tamcol=0;tamcol<N;tamcol++){
			free(B[tamcol]);
		}
		free(B);
		//lib para R
		for(tamcol=0;tamcol<tamBloque;tamcol++){
			free(R[tamcol]);
		}
		free(R);
	}
	
	/****FIN MPI*/
	MPI_Finalize();
}
void imprimirMatriz(double **matriz){
	/*declaración de variable*/
	int tamfila,tamcol;
	/*impresión de matriz*/
	for(tamfila=0;tamfila<N;tamfila++){
		printf("\n");
		for(tamcol=0;tamcol<N;tamcol++){
			printf("  %lf",matriz[tamfila][tamcol]);
		}
	}
	printf("\n");
}