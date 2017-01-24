#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void Calcula(double **m1, double **m2, double **m3, int inicio, int final,int N)
{
int i,j,k;

	
	for(i=inicio;i<final;i++)
	{
		for(j=0;j<N;j++)
		{
			for (k=inicio;k<final;k++)
			{
				m3[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}

int main(int argc,char *argv[])
{
	MPI_Init(&argc,&argv);
	int inicio,fin;
	int N = 800;
	int i,j,k;
	int np,err;
	int myrank;
	int tamBloque=50;
	int nblock=N/50;
	int ntanda=nblock/np;
	
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD,&np); 
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	
	//parte 3
	double **matriz1;
    double **matriz2;
    double **matrizResultado;
	if(myrank==0){
		matriz1 = (double**)malloc(N*sizeof(double));
		for(i=0;i<N;i++)
		{
			matriz1[i]=(double*)malloc(N*sizeof(double));
		}
	}
	else
	{
		matriz1 = (double**)malloc(tamBloque*sizeof(double));
		for(i=0;i<tamBloque;i++)
		{
			matriz1[i]=(double*)malloc(N*sizeof(double));
		}
		
	}
	matriz2 = (double**)malloc(N*sizeof(double));
	
	for(i=0;i<N;i++)
	{
		matriz2[i]=(double*)malloc(N*sizeof(double));
	}
	if(myrank==0){
		matrizResultado = (double**)malloc(N*sizeof(double));
		for(i=0;i<N;i++)
		{
			matrizResultado[i]=(double*)malloc(N*sizeof(double));
		}
	}
	else{
		matrizResultado = (double**)malloc(tamBloque*sizeof(double));
		for(i=0;i<tamBloque;i++)
		{
			matrizResultado[i]=(double*)malloc(N*sizeof(double));
		}
	}
	
	
	//inicializar cosas
	if(myrank==0)
	{
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				matriz1[i][j] = rand()%10;
				matriz2[i][j] = rand()%10;
				matrizResultado[i][j] = 0;
			}
		}
	}

		//envia matriz2
		
		for(i=0;i<N;i++)
		{
			MPI_Bcast(matriz2[i],N,MPI_DOUBLE,0,MPI_COMM_WORLD);
		}
		
		//calcula 
		i=0;
		if(myrank==0){
			while(i<=nblock)
			{
				for(j=1;j<np;j++){
					for(k=0;k<tamBloque;k++)
					{
						err=MPI_Send(matriz1[tamBloque*i+50*j+k],N,MPI_DOUBLE,j,5,0);
						i++;
					}
				}
				i=i-j+1;
				Calcula(matriz1,matriz2,matrizResultado,(i*50),(i*50+50),N);
				for(j=1;j<np;j++)
				{
					for(k=0;k<tamBloque;k++)
					{
						err=MPI_Recv(matriz1[50*i*j],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
					}
				}
				i=i+np;
			}
		}else{
			while(i<nblock){
				for(j=0;j<tamBloque;j++)
				{
					err=MPI_Recv(matriz1[j],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
				}
				Calcula(matriz1,matriz2,matrizResultado,0,tamBloque,N);
				for(k=0;k<tamBloque;k++)
					{
						err=MPI_Send(matriz1[k],N,MPI_DOUBLE,j,5,0);
					}
				i=i+np;
			}
		}
		if(myrank==0)
		{
			i=i-np+1;
			Calcula(matriz1,matriz2,matrizResultado,(i*50),(i*50+50),N);
			for(i=1;i<=j;i++)
			{
				for(k=0;k<50;k++)
				{
					err=MPI_Recv(matriz1[50*(i-np+1+i)+k],N,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
				}
			}
		}
		
		//libera memoria
		for (i=0;i<N;i++)
		{
			free(matriz2[i]);
		}
		if(myrank==0){
			for(i=0;i<N;i++){
				free(matriz1[i]);
				free(matrizResultado[i]);
			}
		}else{
			for(i=0;i<tamBloque;i++){
				free(matriz1[i]);
				free(matrizResultado[i]);
			}
		}
	
	free(matriz1);
    free(matriz2);
    free(matrizResultado);	
	
	MPI_Finalize();	
}

