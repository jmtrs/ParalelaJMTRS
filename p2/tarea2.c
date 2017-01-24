#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIZ_DIM 1000

int main()
{
	srand (time(NULL));
	double **matriz;
    double **matriz_2;
    double **matriz_resul;
 
    double *vectaux;
    
    double tini,tfin,aux;
    int i,j,k,l, tbloque,ini,fin,myrank,nproces,nbloques;
    nproces=4; 
    
    tbloque=MATRIZ_DIM/nproces;

	//Crea la matriz
	matriz=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++){
		matriz[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}
	//crea la matriz 2
	matriz_2=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++){
		matriz_2[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}
	//crea matriz matriz_resul
	matriz_resul=(double**)malloc(MATRIZ_DIM*sizeof(double *));
	for(i=0;i<MATRIZ_DIM;i++){
		matriz_resul[i]=(double *)malloc(MATRIZ_DIM*sizeof(double));
	}

    for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
  			matriz[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
       	}
    }
	for(i=0;i<MATRIZ_DIM;i++){
      	for(j=0;j<MATRIZ_DIM;j++){
		  	matriz_2[i][j] = (double)rand() / (double)((unsigned)RAND_MAX + 1);
        }
    }
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
        	matriz_resul[i][j]=0;
        }
    }
		
	printf("1º Todos los procesos calculan varios elementos SIN automatizador OK \n\n");
	
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) private(myrank,nproces,i,j,k,ini,fin) default(shared)
	{
	
		myrank = omp_get_thread_num();
		nproces = omp_get_num_threads();
		tbloque = MATRIZ_DIM/nproces;  
		ini = tbloque * myrank;
		fin = tbloque * (myrank + 1);
		for(i=ini;i<fin;i++){
			for(j=0;j<MATRIZ_DIM;j++) {
				for(k=0;k<MATRIZ_DIM;k++){
					matriz_resul[i][j]+= matriz[i][k]*matriz_2[k][j]; 
				}
			}  
		}
		
	} 
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
	
	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p1 fila %d :\n", i);
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}	*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }


	printf("2º Todos los procesos calculan un elemento SIN automatizador OK\n\n");
	tini=omp_get_wtime();
			#pragma omp parallel num_threads(nproces) default(shared) private(i,j,k,myrank,nproces,ini,fin,tbloque)
		{
			myrank = omp_get_thread_num();  
			nproces = omp_get_num_threads();
			tbloque = MATRIZ_DIM/nproces;  
			#pragma omp single
			vectaux=(double *) malloc(nproces*sizeof(double));
			ini = tbloque * myrank;
			fin = tbloque * (myrank + 1);
			for( i=0;i<MATRIZ_DIM;i++){	
				for(j=0;j<MATRIZ_DIM;j++){
					vectaux[myrank]=0;
					for(k=ini;k<fin;k++){
						vectaux[myrank]+=matriz[i][k]*matriz_2[k][j];
					}
					#pragma omp atomic
					matriz_resul[i][j]+=vectaux[myrank];
				}
			}
		}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);

	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p2 fila %d :\n", i);
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}	*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }
    

    
    printf("3º Todos calculan varios elementos CON automatizador OK\n\n");
	tini=omp_get_wtime();
	#pragma omp parallel for num_threads(nproces) default(shared) private(i,j,k)
	for(i=0;i<MATRIZ_DIM;i++){
		for(j=0;j<MATRIZ_DIM;j++) {
			for(k=0;k<MATRIZ_DIM;k++){
			 matriz_resul[i][j]+= matriz[i][k]*matriz_2[k][j]; 
			}
		}  
			
	}

	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);

	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p3 fila %d :\n", i);
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}	*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }
    
    
    printf("4º Todos calculan un elemento CON automatizador OK\n\n");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,i,k)
	{	
		
		for( i=0;i<MATRIZ_DIM;i++){	
			for(j=0;j<MATRIZ_DIM;j++){
				#pragma omp for schedule(static,tbloque) reduction(+:aux)
				for(k=0;k<MATRIZ_DIM;k++){
					aux+=matriz[i][k]*matriz_2[k][j];
				}
				#pragma omp single
				{
					matriz_resul[i][j]=aux;
					aux=0;
				}
			}
		}
	}
    tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);

	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p4 fila %d :\n", i);
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}	*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }


	printf("5º Un proceso calcula grupos de filas adyacentes igual a 20 SIN automatizador OK con valores de MATRIZ_DIM grandes\n\n");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,i,k,l,tbloque,nbloques,myrank,nproces)
	{
			myrank = omp_get_thread_num();  
			nproces = omp_get_num_threads();
			nbloques=(MATRIZ_DIM/20)/nproces; 
			tbloque = MATRIZ_DIM/nproces;
			
			for(i=0;i<nbloques;i++){
				for(j=0;j<20;j++){ 
					for(k=0;k<MATRIZ_DIM;k++){
						for(l=0;l<MATRIZ_DIM;l++){ 		
							matriz_resul[(myrank*tbloque)+(i*20)+j][k]+=matriz[(myrank*tbloque)+(i*20)+j][l]*matriz_2[l][k]; 
						} 
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);

	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p5 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }

 
	printf("6º Un proceso calcula grupos de filas adyacentes igual a 20 CON automatizador OK\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,k,l)
	{
			
			#pragma omp for schedule(static,20) 
			for(j=0;j<MATRIZ_DIM;j++){
				for(k=0;k<MATRIZ_DIM;k++){
					for(l=0;l<MATRIZ_DIM;l++){	
						matriz_resul[j][k]+= matriz[j][l]*matriz_2[l][k]; 
						
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);

/*		for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p6 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }
 
 
 
 
	printf("7º Un proceso calcula grupos de filas adyacentes SIN automatizador OK\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,i,k,l,tbloque,myrank,nproces)
	{
			myrank = omp_get_thread_num();  
			nproces = omp_get_num_threads();
			tbloque = MATRIZ_DIM/nproces;
			for(j=0;j<tbloque;j++){
				for(k=0;k<MATRIZ_DIM;k++){
					for(l=0;l<MATRIZ_DIM;l++){		
					matriz_resul[(myrank*tbloque)+j][k]+=matriz[(myrank*tbloque)+j][l]*matriz_2[l][k];
						
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p7 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }
 
 
 
	 printf("8º Un proceso calcula grupos de filas adyacentes CON automatizador OK\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,k,l)
	{
			#pragma omp for schedule(static,tbloque)
			
			for(j=0;j<MATRIZ_DIM;j++){
				for(k=0;k<MATRIZ_DIM;k++){
					for(l=0;l<MATRIZ_DIM;l++){		
					matriz_resul[j][k]+=matriz[j][l]*matriz_2[l][k];
						
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
	/*for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p8 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }
 
 
	printf("9º Un proceso calcula grupos de columnas adyacentes CON automatizador OK\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,k,l)
	{
			
			for(j=0;j<MATRIZ_DIM;j++){
				#pragma omp for schedule(static,tbloque)
				for(k=0;k<MATRIZ_DIM;k++){
					for(l=0;l<MATRIZ_DIM;l++){		
					matriz_resul[j][k]+=matriz[j][l]*matriz_2[l][k];
						
					}
				}
			}
	}
	
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
	/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p9 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }


	printf("10º Un proceso calcula grupos de columnas adyacentes igual a 20 CON automatizador OK\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,k,l)
	{
			for(j=0;j<MATRIZ_DIM;j++){
				#pragma omp for schedule(static,20)
				for(k=0;k<MATRIZ_DIM;k++){
					for(l=0;l<MATRIZ_DIM;l++){		
						matriz_resul[j][k]+= matriz[j][l]*matriz_2[l][k]; 
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
/*		for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p10 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	}*/
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }


	printf("11º Un proceso calcula grupos de columnas adyacentes igual a 20  SIN automatizador OK con valores de MATRIZ_DIM grandes\n\n ");
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,i,k,l,tbloque,nbloques,myrank,nproces)
	{
			myrank = omp_get_thread_num();  
			nproces = omp_get_num_threads();
			nbloques=(MATRIZ_DIM/20)/nproces;
			tbloque = MATRIZ_DIM/nproces;
			for(i=0;i<nbloques;i++){
				for(k=0;k<20;k++){
					for(j=0;j<MATRIZ_DIM;j++){
						for(l=0;l<MATRIZ_DIM;l++){		
						matriz_resul[j][(myrank*tbloque)+(i*20)+k]+=matriz[j][l]*matriz_2[l][(myrank*tbloque)+(i*20)+k];
							
						}
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
	/*for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p11 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	} */
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }



	printf("12º Un proceso calcula grupos de columnas adyacentes SIN automatizador OK\n\n ");
	
	tini=omp_get_wtime();
	#pragma omp parallel num_threads(nproces) default(shared) private(j,i,k,l,tbloque,myrank,nproces)
	{
			myrank = omp_get_thread_num();  
			nproces = omp_get_num_threads();
			tbloque = MATRIZ_DIM/nproces;
			for(j=0;j<MATRIZ_DIM;j++){
				for(k=0;k<tbloque;k++){
					for(l=0;l<MATRIZ_DIM;l++){		
					matriz_resul[j][(myrank*tbloque)+k]+=matriz[j][l]*matriz_2[l][(myrank*tbloque)+k];
						
					}
				}
			}
	}
	tfin=omp_get_wtime();
	printf("Duracion: %lf\n\n",tfin-tini);
/*	for (i = 0; i<MATRIZ_DIM; i++){
		 printf ("\nElementos matriz resultado p12 fila %d :\n", i); 
		 for (j = 0; j<MATRIZ_DIM; j++){
			 printf (" %f ", matriz_resul[i][j]);
		 }
		 printf ("\n\n");
	} */
	
	for(i=0;i<MATRIZ_DIM;i++){
       	for(j=0;j<MATRIZ_DIM;j++){
       		matriz_resul[i][j]=0;
       	}
    }

 
   return 0;
}