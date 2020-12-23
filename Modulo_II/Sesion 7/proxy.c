#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include<dirent.h>
#define longnombre 50
#define tam 1024


int main(int argc, char **argv)
{
    int fd_lock, fd_fifo_s;
    int pid;
    char buff[tam];
    FILE *temporal;
    struct flock cerrojo;
    char nombrefifoproxy[longnombre];
    sprintf(nombrefifoproxy,"FIFO.%d",getpid());

    //Creamos el archivo temporal que servirá para almacenar la información que el cliente quiere imprimir
    temporal = tmpfile();

    //Abrimos el archivo de bloqueo para que los procesos proxy puedan acceder a la impresora en exclusión mútua
    fd_lock = open("lockfile", 0666);
    if( fd_lock < 0)
    {										
        perror("\nProxy: Error en open de archivo de bloqueo");											
        exit(EXIT_FAILURE);		
    }

    int bytesLeidos = 1024;
    int bytesEscritos = 0;
    while(bytesLeidos > 0)
    {
        bytesLeidos =  read(STDIN_FILENO,buff,tam);
        if(bytesLeidos > 0)
        {   
            bytesEscritos = fwrite(buff,sizeof(char),bytesLeidos,temporal);

            if( bytesEscritos < 0 )
            {
                perror ("Proxy: Error al escribir en el archivo temporal");
                exit(EXIT_FAILURE);
            }

        }

    } 

    if((fseek(temporal,0,SEEK_SET)) < 0)
    {
        printf("Proxy: Error al cambiar el puntero del archivo temporal %d",errno);
        perror("Proxy: Error al cambiar el puntero del archivo temporal");
        exit(EXIT_FAILURE);
    }


    //Establecemos un cerrojo que bloqueará al archivo entero
    cerrojo.l_type = F_WRLCK;
    cerrojo.l_whence = SEEK_SET;
    cerrojo.l_start = 0;
    cerrojo.l_len = 0;

    int error;
    do
    {   

        error = fcntl(fd_lock,F_SETLK,&cerrojo);    //Intentamos bloquear el lockfile
        if( error  == 0)                            //Accedemos a la impresora en exclusión mútua
        {
            bytesLeidos = -1;
            do
            {
                
                bytesLeidos = fread(buff,sizeof(char),tam,temporal);


                if(bytesLeidos < 0 )
                {
                    perror ("Proxy: Error al leer del archivo temporal para realizar el volvado en la pantalla");
                    exit(EXIT_FAILURE);
                }

                bytesEscritos = write(STDOUT_FILENO,buff,bytesLeidos);
                if( bytesEscritos < 0)
                {
                    perror ("Proxy: Error al volcar en la pantalla el trabajo del cliente");
                    exit(EXIT_FAILURE);
                }

            } while (bytesLeidos > 0);

            write(STDOUT_FILENO,"\n\n",sizeof(char)*3);   //Ponemos un salto de línea después de cada trabajo
            
            error = fcntl(fd_lock,F_SETLKW,&cerrojo);   //Desbloqueamos el lockfile
            if(error != 0)
            {
                printf("Proxy: Error al liberar el lockfile %d",errno);
                perror ("Proxy: Error al liberar el lockfile");
                exit(EXIT_FAILURE);
            }
        }

    }while(error != 0);


    if((unlink(nombrefifoproxy)) < 0)
    {
        perror ("Proxy: Error al eliminar el FIFO ");
		exit(EXIT_FAILURE);
    }


}