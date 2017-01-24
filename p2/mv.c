#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int N=12000;

int main()
{
int iam,np;
double *vector, *resultado, **matriz;
double aux;
double t1,t2;
int i,j, tamBloque,ini,fin;
np=4;
tamBloque=N/np;
//reserva de memoria en el hilo secuencial
vector=(double *)malloc(N*sizeof(double));
resultado=(double *)malloc(N*sizeof(double));
//Crear la matriz
matriz=(double**)malloc(N*sizeof(double *));
//crear filas de matriz
for(i=0;i<N;i++){matriz[i]=(double *)malloc(N*sizeof(double));}

//inicializo los vectores
        for(i=0;i<N;i++){
        	vector[i]=rand();
        	resultado[i]=0;
        }	
        //inicializo la matriz
        for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		
printf("1 todos los hilos calculan varios elementos sin automatizador\n");

t1=omp_get_wtime();
#pragma omp parallel num_threads(np) private(iam,np,i,j,ini,fin) default(shared)
{

	iam = omp_get_thread_num();
	np = omp_get_num_threads();
	tamBloque = N/np;  
	ini = tamBloque * iam;
	fin = tamBloque * (iam + 1);
	//printf("Ini, Fin y tambloque: %d - %d % d del hilo %d\n\n",ini,fin,tamBloque,iam);
	for(i=ini;i<fin;i++)
	{
		for(j=0;j<N;j++)   
		{
		 resultado[i]+= matriz[i][j]*vector[j]; 
		}
	}
	
} 
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);
//prueba para ver el resultado
//for(i=0;i<N;i++){printf("%f\n",resultado[i] );}
for(i=0;i<N;i++){
        	vector[i]=rand();
        	resultado[i]=0;
        }	
        //inicializo la matriz
        for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }

printf("2 Todos los hilos calculan un elemento sin automatizadores\n");
t1=omp_get_wtime();
for(i=0;i<N;i++)
{
	aux=0;
		#pragma omp parallel  num_threads(np) default(shared) firstprivate(i) private(j,iam,np,ini,fin) reduction(+:aux)
	{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		//printf("\ni=%d, soy=%d", i, iam);
		tamBloque = N/np;  
		ini = tamBloque * iam;
		fin = tamBloque * (iam + 1);
		//printf("Ini, Fin y tambloque: %d - %d % d del hilo %d\n\n",ini,fin,tamBloque,iam);
		for(j=ini;j<fin;j++)
		{
		aux+=matriz[i][j]*vector[j];
		resultado[i]=aux;
		}
	}
}

t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);
//prueba para ver el resultado
//for(i=0;i<N;i++){printf("%f\n",resultado[i] );}

for(i=0;i<N;i++){
        	vector[i]=rand();
        	resultado[i]=0;
        }	
        //inicializo la matriz
        for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }

printf("3con automatizador todos calculan varios elementos\n");
t1=omp_get_wtime();
#pragma omp parallel for num_threads(np) default(shared) private(i,j)

		for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)   
		{
		 resultado[i]+= matriz[i][j]*vector[j]; 
		 //printf("\ni=%d \n", i);
		}
	}

t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);
//prueba para ver el resultado
//for(i=0;i<N;i++){printf("%f \n",resultado[i] );}

for(i=0;i<N;i++){
        	vector[i]=rand();
        	resultado[i]=0;
        }	
        //inicializo la matriz
        for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
printf("4con automatizador todos calculan un elemento\n");
t1=omp_get_wtime();
for(i=0;i<N;i++)
{
	aux=0;
		#pragma omp parallel for num_threads(np) default(shared) firstprivate(i) private(j) reduction(+:aux)	
		for(j=0;j<N;j++)
		{
		 aux+=matriz[i][j]*vector[j];
		 resultado[i]=aux;
		}
}

t2=omp_get_wtime();
printf("tiempo =%f/n",t2-t1);
//prueba para ver el resultado
//for(i=0;i<N;i++){printf("%f \n",resultado[i] );}


}