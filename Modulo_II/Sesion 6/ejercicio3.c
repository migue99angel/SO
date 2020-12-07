/*Construir un programa que verifique que, efectivamente, el kernel comprueba que
puede darse una situación de interbloqueo en el bloqueo de archivos.*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char * argv[]) 
{   
    int fd;
    struct flock cerrojo;

    if((fd = open("lockfile", O_RDWR)) == -1)
    {
        printf("\nError %d en open",errno);										
        perror("\nError en open de archivo de escritura");											
        exit(EXIT_FAILURE);		
    }

    //Establecemos un cerrojo que bloqueará al archivo entero
    cerrojo.l_type = F_WRLCK;
    cerrojo.l_whence = SEEK_SET;
    cerrojo.l_start = 0;
    cerrojo.l_len = 0;

    int error = fcntl(fd,F_SETLK,&cerrojo);
    if( error != EDEADLK && error != -1)
    {
        //Iniciamos un bucle infinito
        while(1)
        {

        }
    }
    else
    {
        printf("El kernel ha comprobado que puede darse una situación de interbloqueo\n");
    }

    
}
