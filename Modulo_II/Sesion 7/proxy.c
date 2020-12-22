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
    int fd, fd_lock, fd_fifo_s;
    int pid;
    char nombrefifoproxy[longnombre];
    char buff[tam];
    sprintf(nombrefifoproxy,"FIFO.%d",getpid());
    FILE *temporal;
    struct flock cerrojo;

    if(argc != 2)
    {
        printf("Proxy: Error en la llamada al proxy, ./proxy <fd_fifos>\n");
        return EXIT_FAILURE;
    }
    else
    {
        fd_fifo_s = atoi(argv[1]);
    }
    

    //Primero creamos el FIFO que servirá para comunicar el cliente con el proxy
    umask(0);
    int err_fifo = mknod(nombrefifoproxy,S_IFIFO|0777, 0);
    if( err_fifo < 0) 
    {
        perror("\nProxy: Error al crear el FIFO");
        return EXIT_FAILURE;
    }

    //Escribimos en el FIFOs nuestro pid para que un cliente escriba sus trabajos en el FIFO.pid que acabamos de crear 
    pid = getpid();
    if((write(fd_fifo_s,&pid,sizeof(int))) < 0) 
    {
        perror ("Proxy: Error en la escritura en el FIFO conocido de salida");
        exit(EXIT_FAILURE);
    }

    //Creamos el archivo temporal que servirá para almacenar la información que el cliente quiere imprimir
    temporal = tmpfile();

    //Ahora abrimos el FIFO
    if ((fd = open (nombrefifoproxy, O_RDONLY)) < 0) 
	{
        printf("Proxy: Error al abrir el fichero FIFO.pid ");
		perror ("Proxy: Error al abrir el FIFO");
		exit(EXIT_FAILURE);
	}

    //Redireccionamos la entrada del proceso proxy al fichero FIFO
    dup2(fd,STDIN_FILENO);

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
        bytesLeidos =  read(fd,buff,tam);
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