#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIZ_DIM 40

int main(int argc,char *argv[]){

	srand (time(NULL));
	double **matriz;
 
    double *vectaux;
    
    double aux,maximpar,maxpar;
    int i,j,k,l,ini,fin,tbloque,myrank,nproces,nbloques;
    nproces=4; 
    
    tbloque=MATRIZ_DIM/nproces;

	//Crea la matriz
	matriz=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++){
		matriz[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}

    for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
  			matriz[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
       	}
    }

		




  
	#pragma omp parallel for num_threads(nproces) default(shared) private(i,j,k)
	for(i=1;i<MATRIZ_DIM;i++){
		for(j=0;j<MATRIZ_DIM;j++){
			if (matriz[i][j] > maximpar){
				maximpar = matriz[i][j];
			}
		}
		i++;
	}
	printf("max impar: %lf",maximpar);



	#pragma omp parallel num_threads(nproces) private(myrank,nproces,i,j,k,ini,fin) default(shared)
	{
	
		myrank = omp_get_thread_num();     
		nproces = omp_get_num_threads();  
		tbloque = MATRIZ_DIM/nproces;  
		ini = tbloque * myrank;
		fin = tbloque * (myrank + 1);
		for(i=ini;i<fin;i++){
			for(j=0;j<MATRIZ_DIM;j++){
				for(k=0;k<MATRIZ_DIM;k++){
				 //
				}      
			}  
		}
		
	} 

	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p1 fila %d :\n", i);
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}	*/
	
	

	return 0;
}