#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int N=1200;

int main()
{
double **matriz2, **resultado, **matriz,*restemp;
double aux,t1,t2;
int i,j,z,l, tamBloque,ini,fin,iam,np,nbloques;
np=1;
tamBloque=N/np;
// 
		//Crear la matriz
		matriz=(double**)malloc(N*sizeof(double *));
		for(i=0;i<N;i++)
		{
			matriz[i]=(double *)malloc(N*sizeof(double));
		}
		//crear la matriz 2
			matriz2=(double**)malloc(N*sizeof(double *));
		for(i=0;i<N;i++)
		{
			matriz2[i]=(double *)malloc(N*sizeof(double));
		}
		//crear matriz resultado
		resultado=(double**)malloc(N*sizeof(double *));
		for(i=0;i<N;i++)
		{
			resultado[i]=(double *)malloc(N*sizeof(double));
		}

  //inicializo las matrices
          for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
		
printf("1 todos los hilos calculan varios elementos sin automatizador\n");

t1=omp_get_wtime();
#pragma omp parallel num_threads(np) private(iam,np,i,j,z,ini,fin) default(shared)
{

	iam = omp_get_thread_num();
	np = omp_get_num_threads();
	tamBloque = N/np;  
	ini = tamBloque * iam;
	fin = tamBloque * (iam + 1);
	for(i=ini;i<fin;i++)
	{
		for(j=0;j<N;j++) 
		{
		for(z=0;z<N;z++)
		{
		 resultado[i][j]+= matriz[i][z]*matriz2[z][j]; 
		}
		}  
			
	}
	
} 
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

		for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }

printf("2 Todos los hilos calculan un elemento sin automatizadores\n");
t1=omp_get_wtime();
		#pragma omp parallel num_threads(np) default(shared) private(i,j,z,iam,np,ini,fin,tamBloque)
	{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		tamBloque = N/np;  
		#pragma omp single
		restemp=(double *) malloc(np*sizeof(double));
		ini = tamBloque * iam;
		fin = tamBloque * (iam + 1);
		for( i=0;i<N;i++){	
			for(j=0;j<N;j++)
			{
			restemp[iam]=0;
				for(z=ini;z<fin;z++)
				{
					restemp[iam]+=matriz[i][z]*matriz2[z][j];
				}
				#pragma omp atomic
			resultado[i][j]+=restemp[iam];
			}
		}
	}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
printf("3con automatizador todos calculan varios elementos\n");
t1=omp_get_wtime();
#pragma omp parallel for num_threads(np) default(shared) private(i,j,z)

	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++) 
		{
		for(z=0;z<N;z++)
		{
		 resultado[i][j]+= matriz[i][z]*matriz2[z][j]; 
		}
		}  
			
	}

t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

		for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
	
printf("4con automatizador todos calculan un elemento\n");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,i,z)
{	
	
	
	for( i=0;i<N;i++)
	{	
	for(j=0;j<N;j++)
			{
			#pragma omp for schedule(static,tamBloque) reduction(+:aux)
				for(z=0;z<N;z++)
				{
					aux+=matriz[i][z]*matriz2[z][j];
				}
					#pragma omp single
					{
						resultado[i][j]=aux;
						aux=0;
					}
				
			}
	}
}

t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);
/*
for(i=0;i<N;i++){
for(j=0;j<N;j++){
printf("%f ",resultado[i][j]);}
printf("\n");
}*/

	for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
		
//Un proceso calcula grupos de filas adyacentes igual a 10  
printf("Un proceso calcula grupos de filas adyacentes igual a 10  sin \n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,i,z,l,tamBloque,nbloques,iam,np)
{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		nbloques=(N/10)/np;
		tamBloque = N/np;
		for(i=0;i<nbloques;i++){
		for(j=0;j<10;j++){
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[(iam*tamBloque)+(i*10)+j][z]+=matriz[(iam*tamBloque)+(i*10)+j][l]*matriz2[l][z];}}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

	for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }

//Un proceso calcula grupos de filas adyacentes igual a 10 
printf("Un proceso calcula grupos de filas adyacentes igual a 10 con\n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,z,l)
{
		
		#pragma omp for schedule(static,10)
		
		for(j=0;j<N;j++){
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[(iam*tamBloque)+j][z]+=matriz[(iam*tamBloque)+j][l]*matriz2[l][z];
		}
		}
		}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

	for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
		
//Un proceso calcula grupos de filas adyacentes  
printf("Un proceso calcula grupos de filas adyacentes sin \n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,i,z,l,tamBloque,iam,np)
{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		tamBloque = N/np;
		for(j=0;j<tamBloque;j++){
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[(iam*tamBloque)+j][z]+=matriz[(iam*tamBloque)+j][l]*matriz2[l][z];}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

	for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }

//Un proceso calcula grupos de filas adyacentes igual a 10 
printf("Un proceso calcula grupos de filas adyacentes con\n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,z,l)
{
		#pragma omp for schedule(static,tamBloque)
		
		for(j=0;j<N;j++){
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[j][z]+=matriz[j][l]*matriz2[l][z];}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }

//Un proceso calcula grupos de columnas adyacentes
printf("Un proceso calcula grupos de columnas adyacentes con\n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,z,l)
{
		
		
		for(j=0;j<N;j++){
		#pragma omp for schedule(static,tamBloque)
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[j][z]+=matriz[j][l]*matriz2[l][z];}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

	for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }

//Un proceso calcula grupos de columnas adyacentes igual a 10 con
printf("Un proceso calcula grupos de columnas adyacentes igual a 10 con\n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,z,l)
{
		
		
		
		for(j=0;j<N;j++){
		#pragma omp for schedule(static,10)
		for(z=0;z<N;z++){
		for(l=0;l<N;l++){		
		resultado[(iam*tamBloque)+j][z]+=matriz[(iam*tamBloque)+j][l]*matriz2[l][z];
		}
		}
		}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
		//alert no va
//Un proceso calcula grupos de columnas adyacentes igual a 10   
printf("Un proceso calcula grupos de columnas adyacentes igual a 10  sin \n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,i,z,l,tamBloque,nbloques,iam,np)
{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		nbloques=(N/10)/np;
		tamBloque = N/np;
		for(i=0;i<nbloques;i++){
		for(z=0;z<10;z++){
		for(j=0;j<N;j++){
		for(l=0;l<N;l++){		
		resultado[j][(iam*tamBloque)+(i*10)+z]+=matriz[j][l]*matriz2[l][(iam*tamBloque)+(i*10)+z];}}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);

for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		matriz2[i][j]=rand();
        	}
        }
		  for(i=0;i<N;i++){
        	for(j=0;j<N;j++){
        		resultado[i][j]=0;
        	}
        }
		
//Un proceso calcula grupos de columnas adyacentes  
printf("Un proceso calcula grupos de columnas adyacentes sin \n ");
t1=omp_get_wtime();
#pragma omp parallel num_threads(np) default(shared) private(j,i,z,l,tamBloque,iam,np)
{
		iam = omp_get_thread_num();  
		np = omp_get_num_threads();
		tamBloque = N/np;
		for(j=0;j<N;j++){
		for(z=0;z<tamBloque;z++){
		for(l=0;l<N;l++){		
		resultado[j][(iam*tamBloque)+z]+=matriz[j][l]*matriz2[l][(iam*tamBloque)+z];}}}
}
t2=omp_get_wtime();
printf("tiempo =%f\n",t2-t1);
}