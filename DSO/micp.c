#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
 
#define BUFFER 2048
 
int main(int argc, char* argv[]){
	int org,dest,leerbuf,escribirbuf;
	char *buff[BUFFER];
	
	/*Se comprueba que tanto el archivo origen y el archivo destino son recibidos,
	escribiendo --help se muestr la ayuda*/
	if(argc != 3 || argv[1] == "--help"){
		printf("\nUso: ./micp [archivo_origen] [archivo_destino]\n");
		exit(EXIT_FAILURE);
	}
 
	/*Se abre el archivo de origen*/
	org = open(argv[1],O_RDONLY);
 
	if(org == -1){
		printf("\nError abriendo archivo %s errno = %d\n",argv[1],errno);
		exit(EXIT_FAILURE);	
	}
	
	/*Se abre el archivo de destino con los correspondientes flags y modes
	  O_CREAT y O_TRUNC  para remplazar o crear un nuevo archivo
	  S_IXXXX son los premisos del archivo user,groups o others*/
	dest = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
 
	if(dest == -1){
		printf("\nError abriendo archivo %s errno = %d\n",argv[2],errno);
		exit(EXIT_FAILURE);
	}
 
	/*Se transfieren los datos del fichero origen al destino hasta EOF*/
	while((leerbuf = read(org,buff,BUFFER)) > 0){
		if(write(dest,buff,leerbuf) != leerbuf){
			printf("\nError escribiendo informacion %s\n",argv[2]);
		}
	}
	
	if(leerbuf == -1){
		printf("\nError leyendo los datos de: %s\n",argv[1]);
	}
	if(close(org) == -1){
		printf("\nError al cerrar archivo %s\n",argv[1]);
	}
	if(close(dest) == -1){
		printf("\nError al cerrar archivo %s\n",argv[2]);
	}
	exit(EXIT_SUCCESS);
}