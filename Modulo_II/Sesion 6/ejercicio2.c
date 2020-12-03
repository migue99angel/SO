/*
Reescribir el programa que implemente un encauzamiento de dos órdenes pero
utilizando fcntl. Este programa admitirá tres argumentos. El primer argumento y el tercero
serán dos órdenes de Linux. El segundo argumento será el carácter “|”. El programa deberá
ahora hacer la redirección de la salida de la orden indicada por el primer argumento hacia el
cauce, y redireccionar la entrada estándar de la segunda orden desde el cauce. Por ejemplo,
para simular el encauzamiento ls|sort, ejecutaríamos nuestro programa como:
$> ./mi_programa2 ls “|” sort
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{
    char* programa1;
    char* programa2;
    char* operador;
    int fd[2];
    int pid;
    int estado;

    if( argc < 4 )
    {
        printf("Llamada incorrecta al programa. Uso correcto ./ejercicio1 ls | sort \n");
        exit(EXIT_FAILURE);
    }

    operador = argv[2];
    programa2 = argv[3];
    programa1 = argv[1];

    if(strcmp(operador,"|") == 0)
    {
        pipe(fd);   //Creo el cauce sin nombre que comunicará ambos procesos

        if((pid=fork()) < 0)
        {
            perror("Error en el fork\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0)    //Proceso hijo
        {
            close(fd[0]);   //Este proceso no leerá del cauce solo escribirá
            close(STDOUT_FILENO);       //Cerramos el descriptor de la salida estándar 
            if(fcntl(fd[1],F_DUPFD,STDOUT_FILENO) == -1) //Redireccionamos la salida al pipe
            {
                perror("Fallo en fcntl");
                exit(EXIT_FAILURE);	
            }

            if(execlp(programa1, programa1,NULL) < 0)
            {
                perror("\nError en el execv");
                printf("\nError %d en execv",errno);	
                exit(EXIT_FAILURE);
            }
        }
        
        if((pid=fork()) < 0)
        {
            perror("Error en el fork\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0)    //Proceso hijo
        {
            //Ahora lo que queremos es que el programa2 lea del pipe
            //Cerramos el descriptor de la entrada estándar 
            close(fd[1]); //Este hijo no va a escribir en el cauce
            close(STDIN_FILENO);
            if(fcntl(fd[0],F_DUPFD,STDIN_FILENO) == -1)
            {
                perror("Fallo en fcntl");
                exit(EXIT_FAILURE);	
            }

            if(execlp(programa2, programa2, NULL) < 0)
            {
                perror("\nError en el execv");
                printf("\nError %d en execv",errno);	
                exit(EXIT_FAILURE);
            }
        }

    }
    else
    {
        printf("El operador introducido no es válido, este programa solo acepta | \n");
        printf("El operador introducido es: %s",operador);
        exit(EXIT_FAILURE);
    }

    
    exit(EXIT_SUCCESS);

}
