/*
Construir un programa que se asegure que solo hay una instancia de él en
ejecución en un momento dado. El programa, una vez que ha establecido el mecanismo para
asegurar que solo una instancia se ejecuta, entrará en un bucle infinito que nos permitirá
comprobar que no podemos lanzar más ejecuciones del mismo. En la construcción del mismo,
deberemos asegurarnos de que el archivo a bloquear no contiene inicialmente nada escrito en
una ejecución anterior que pudo quedar por una caída del sistema.
*/


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
    char buff[40];
    //Con O_TRUNC nos aseguramos de que no haya nada escrito 
    if((fd = open("lockfile", O_RDWR, O_TRUNC)) == -1)
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
        sprintf(buff,"El pid del la unica instancia es %d\n",getpid());
        if(write(fd,buff,40*sizeof(char)) != 40*sizeof(char))
		{
			perror("\nError en write");
			exit(EXIT_FAILURE);
		}
        //Iniciamos un bucle infinito
        while(1)
        {

        }
    }
    else
    {
        printf("Ya hay una instancia de este programa ejecutándose\n");
    }

}