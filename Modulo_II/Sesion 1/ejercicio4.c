/*Define una macro en lenguaje C que tenga la misma funcionalidad que la macro
S_ISREG(mode) usando para ello los flags definidos en <sys/stat.h> para el campo st_mode de
la struct stat, y comprueba que funciona en un programa simple. Consulta en un libro de C o
en internet cómo se especifica una macro con argumento en C.
#define S_ISREG2(mode) ...*/


#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

#define S_ISREG2(modo) ((modo & S_IFMT) == S_IFREG)


int main(int argc, char *argv[])
{
	struct stat atributos;
	char tipoArchivo[30];

	//Comprobamos que como mínimo se ha pasado un archivo como parámetro
	if(argc<2)
    {
		printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
		exit(EXIT_FAILURE);
	}

    if(lstat(argv[1],&atributos) < 0)
    {
        printf("\nError al intentar acceder a los atributos de %s",argv[1]);
        perror("\nError en lstat");
        printf("\nError %d en open",errno);	
    }

    if(S_ISREG2(atributos.st_mode) == 1)
        printf("El archivo pasado como parametro se trata de un fichero regular");
    else
        printf("El archivo pasado como parametro NO es un fichero regular");

    return EXIT_SUCCESS;
}

