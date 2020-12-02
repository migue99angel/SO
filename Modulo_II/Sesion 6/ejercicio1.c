/*
Implementa un programa que admita t argumentos. El primer argumento será una
orden de Linux; el segundo, uno de los siguientes caracteres “<” o “>”, y el tercero el nombre de
un archivo (que puede existir o no). El programa ejecutará la orden que se especifica como
argumento primero e implementará la redirección especificada por el segundo argumento hacia
el archivo indicado en el tercer argumento. Por ejemplo, si deseamos redireccionar la entrada
estándar de sort desde un archivo temporal, ejecutaríamos:
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char* programa;
    char* nombreArchivo;
    char* operador;
    int fd;

    if( argc < 4 )
    {
        printf("Llamada incorrecta al programa. Uso correcto ./ejercicio1 sort < archivo \n");
        exit(EXIT_FAILURE);
    }

    operador = argv[2];
    nombreArchivo = argv[3];
    programa = argv[1];

    if(strcmp(operador,"<") == 0)
    {
        //En este caso se tiene que redireccionar el descriptor de archivo de lectura
        if( (fd=open(nombreArchivo,O_RDONLY))<0)
        {	
            printf("\nError %d en open",errno);										
            perror("\nError en open de archivo de lectura");											
            exit(EXIT_FAILURE);													
        }

        //Cerramos el descriptor de la entrada estándar 
        close(0);
        if(fcntl(fd,F_DUPFD,0) == -1)
        {
            perror("Fallo en fcntl");
            exit(EXIT_FAILURE);	
        }

    }
    else if (strcmp(operador,">") == 0)
    {
        //En este caso se tiene que redireccionar el descriptor de archivo de escritura
        if( (fd=open(nombreArchivo,O_WRONLY|O_CREAT))<0)
        {	
            printf("\nError %d en open",errno);										
            perror("\nError en open de archivo de escritura");											
            exit(EXIT_FAILURE);													
        }

        //Cerramos el descriptor de la salida estándar 
        close(1);
        if(fcntl(fd,F_DUPFD,1) == -1)
        {
            perror("Fallo en fcntl");
            exit(EXIT_FAILURE);	
        }

    }
    else
    {
        printf("El operador introducido no es válido, este programa solo acepta < y > \n");
        printf("El operador introducido es: %s",operador);
        exit(EXIT_FAILURE);
    }


    if(execlp(programa, programa, NULL) < 0)
    {
        perror("\nError en el execv");
        printf("\nError %d en execl",errno);	
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);

}
