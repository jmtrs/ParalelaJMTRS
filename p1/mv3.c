#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
//Con N=8 ok hasta 5 procesos
int N=2;//800;

/*Prototipos de las funciones*/
void imprimirVector(double *vector);
void imprimirMatriz(double **matriz);

int main(int argc,char *argv[]){
	
    int nproces,myrank, i,j;
	double **matriz = NULL;
	double *vector = NULL;// Vector que vamos a multiplicar
	double *vectorRes = NULL; // Vector donde almacenamos el resultado
	int tamBloque=0, tamBloqueReducido=0, tamBloqueGlobal=0;
	double t1,t2;//Para calcular el tiempo de ejecución
	int m=1, k=0;
	float alpha=0.99, norma=0.0;
	
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Status status;
	
	//ASIGNO EL TAMAÑO DEL BLOQUE
	//Establezco el valor de tamBloque a enviar a cada proceso (el nº de filas de M que envio)
    if((N%nproces)==0)
    	tamBloqueGlobal=tamBloque=N/nproces;
    else{
        tamBloqueGlobal=tamBloque=N/nproces+1;//Si no es divisible, envio una fila más a cada proceso, y al último el restante
        tamBloqueReducido = N - (tamBloque*(nproces-1));//Necesita saberlo P0 y el ultimo
        if(myrank==nproces-1)//Si soy el ultimo mi tamaño de bloque será reducido con el resto
        	tamBloque=tamBloqueReducido;
    } 
	
    if(myrank == 0){
    	printf("TamBloqueReducido: %d\n", tamBloqueReducido);
		//RESERVO MEMORIA PARA LA MATRIZ 
		matriz= (double **)malloc(N*sizeof(double)); 
		for (i=0;i<N;i++) {
			matriz[i] = (double*)malloc(N*sizeof(double)); 
		}
		//RESERVO MEMORIA PARA LOS VECTORES
		vector= (double*)malloc(N*sizeof(double)); 	
		vectorRes= (double*)malloc(N*sizeof(double)); 	
		//INICIALIZAR MATRIZ
		int s=1;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				//matriz[i][j]=(float)(rand() % 101)/100;//Asigno a la matriz valores aleatorios entre 0 y 1
				matriz[i][j]=s;
				s++;
			}
		}
		//INICIALIZAR VECTORES
		for(i=0;i<N;i++){
			vector[i]=1;
			vectorRes[i]=0;
		}
		//Imprimo matriz
		printf("M%dP%d/%d, MATRIZ:\n",k,myrank,nproces);
		imprimirMatriz(matriz);
		printf("M%dP%d/%d, V: ",k,myrank,nproces);
		imprimirVector(vector);
		printf("\n");
		
		//ENVIAR MATRIZ A CADA PROCESO REPARTIDA EN BLOQUES
		//Solo es necesario enviarla una vez
		for(i=1;i<nproces;i++){
			//Si no es divisible y soy el ultimo proceso, envio solo los elem restantes
			if( ((N%nproces)!=0) && (i==nproces-1) ){
				for(j=0;j<tamBloqueReducido;j++)//envio menos filas al ultimo proceso
					MPI_Send(&matriz[i*tamBloque+j][0],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD);//Las columnas son las mismas
			}
			else{//SI ES DIVISIBLE --> ENVIO NORMAL
				for(j=0;j<tamBloque;j++)
					MPI_Send(&matriz[i*tamBloque+j][0],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
			}
		}
		//CALCULAR EL TIEMPO EN ENVIAR Y RECIBIR LOS DATOS
		//t1=MPI_Wtime();
	
	}else{
		//RESERVO MEMORIA PARA LOS VECTORES (Vres solo necesita de tamBloque)
		vector= (double*)malloc(N*sizeof(double)); 	
		vectorRes= (double*)malloc(tamBloque*sizeof(double));
		//RESERVO MEMORIA PARA LA MATRIZ (Solo recibo tamBloque filas)
		matriz= (double **)malloc(tamBloque*sizeof(double)); 
		for (i=0;i<tamBloque;i++) {
			matriz[i] = (double*)malloc(N*sizeof(double)); 
		}
		//RECIBIO MI BLOQUE DE LA MATRIZ
		//Si no es divisible y soy el ultimo proceso, recibo solo los elem restantes
		//SI ES DIVISIBLE --> RECEPCION NORMAL
		for(i=0;i<tamBloque;i++){
			MPI_Recv(&matriz[i][0],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
		}
		printf("M%dP%d/%d, MATRIZ:\n",k,myrank,nproces);
		for(i=0;i<tamBloque;i++){
			for(j=0;j<N;j++){
			printf("%.2f\t",matriz[i][j]);
			}		
			printf("\n");
		}printf("\n");
	}
	
for(k=0;k<m;k++){//m iteracciones	

	//P0 ENVIA VECTOR A TODOS LOS PROCESOS
	MPI_Bcast(&vector[0],N,MPI_DOUBLE,0,MPI_COMM_WORLD);

	if(myrank == 0){
		//INICIALIZAR vectorRes
		for(i=0;i<N;i++){
			vectorRes[i]=0;
		}
		//ENVIAR VECTOR
		/*for(i=1;i<nproces;i++)
			MPI_Send(&vector[0],N,MPI_DOUBLE,i,5,MPI_COMM_WORLD);*/
		
		norma=0;
	}
    else{
		//INICIALIZAR vectorRes
		for(i=0;i<tamBloque;i++){
			vectorRes[i]=0;
		}

		//RECIBIR VECTOR
		//MPI_Recv(&vector[0],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);//Se sobreescribe V
		
	}

	//REALIZAR LA MULTIPLICACION
	//Todos los procesos realiza la multiplicación de su bloque de la matriz * vector y lo guardan en su bloque de vRes
	for(i=0;i<tamBloque;i++){
		for(j=0;j<N;j++){
			vectorRes[i]+=matriz[i][j]*vector[j];
		}
	}
	
	// imprimo resultado multiplicación
	printf("M%dP%d/%d, Op1Vres: ",k,myrank,nproces);
	for(i=0;i<tamBloque;i++)
		printf("%.2f\t",vectorRes[i]);
	printf("\n");
	
	//REALIZO LA OPERACION 2
	for(i=0;i<tamBloque;i++){//filas
	//En caso de q no sea divisible, tamBloque será reducido, para el ultimo proceso
        vectorRes[i] = (alpha * vectorRes[i]) + ( (1-alpha) * vector[i+tamBloqueGlobal*myrank]);//aqui necesito q sea simpre tamBloque global
    }
	printf("M%dP%d/%d, Op2Vres: ",k,myrank,nproces);
	for(i=0;i<tamBloque;i++)
		printf("%.2f\t",vectorRes[i]);
	printf("\n");

	if(myrank>0){
		//ENVIAR VERTORRES
			MPI_Send(&vectorRes[0],tamBloque,MPI_DOUBLE,0,5,MPI_COMM_WORLD);
			//Imprimo mi vectorRes
			printf("M%dP%d/%d, Vres final: ",k,myrank,nproces);
				for(i=0;i<tamBloque;i++)
			printf("%.2f\t",vectorRes[i]);
			printf("\n");
	}
	else{
		//RECIBIR VECTORRES
		for(i=1;i<nproces;i++){
			if( ((N%nproces)!=0) && (i==nproces-1) ){
					MPI_Recv(&vectorRes[i*tamBloque],tamBloqueReducido,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
			}
			else{//SI ES DIVISIBLE --> RECEPCION NORMAL
				MPI_Recv(&vectorRes[i*tamBloque],tamBloque,MPI_DOUBLE,i,5,MPI_COMM_WORLD,&status);
			}	
		}
		printf("M%dP%d/%d, VRES: ",k,myrank,nproces);
		imprimirVector(vectorRes);
		printf("\n");
		
		if((((k+1)%10)==0) ||  (k==m-1)){
			//P0 calcula la norma cada 10 iteraciones y en la última iteración
			for(i=0;i<N;i++){//filas
		        //Calculando la norma1 
		        norma += fabs(vectorRes[i]-vector[i]);
		    }
			printf("P%d/%d, Iteracion %d, NORMA: %f.\n",myrank,nproces,k, norma);
		}
		
		//CALCULAR EL TIEMPO EN ENVIAR Y RECIBIR LOS DATOS
		//t2=MPI_Wtime();
		//printf("EL PROGRAMA TARDA %f SEGUNDOS\n",t2-t1);
		
	    //Actualizo V al valor de Vres para la siguiente iteracion
	    for(i=0; i<N; i++){
	        vector[i] = vectorRes[i];
	    }
    	printf("M%dP%d/%d, Vnuevo: ",k,myrank,nproces);
		imprimirVector(vector);
		printf("\n");
	}
}


	//LIBERAR MEMORIA
	if (myrank==0){
		free(vector);
		free(vectorRes);
		
		for(i=0;i<N;i++){
			free(matriz[i]);
		}
		free(matriz);
	}
	else{
		free(vector);
		free(vectorRes);
		for(i=0;i<tamBloque;i++){
			free(matriz[i]);
		}
		free(matriz);
	}
	
    MPI_Finalize();

}
//FUNCIONES DE IMPRESION
void imprimirMatriz(double **matriz){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("%.2f\t",matriz[i][j]);
		}		
		printf("\n");
	}printf("\n");
}
void imprimirVector(double *vector){
	int i;
	for(i=0;i<N;i++){
		printf("%.2f\t",vector[i]);
	}
	printf("\n");
}
