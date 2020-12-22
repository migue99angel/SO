
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#define longnombre 50

int main(int argc, char **argv)
{
    int fd_lock,fd_e, fd_s;
    int bytesLeidos;
    int pidLeido;
    int PID;
    char nombrefifoe[longnombre], nombrefifos[longnombre];
    char arg1[4];

    // Compone los nombres de los FIFOs conocidos a partir del parametro,
	// uno de entrada y otro de salida (desde el punto de vista del servidor).
    if(argc == 2)
    {
        sprintf(nombrefifoe,"%se",argv[1]);
        sprintf(nombrefifos,"%ss",argv[1]);
    }
    else
    {
        printf("Servidor: Error en la llamada al servidor, ./servidor <NombreArchivoFIFO>\n");
        return EXIT_FAILURE;
    }
    
    umask(0);
    //Primero creamos los FIFO con nombre que servirá para comunicar los clientes con el servidor
    int err_fifo = mknod(nombrefifoe,S_IFIFO|0777, 0);
    if( err_fifo < 0) 
    {
        perror("\nServidor: Error al crear el FIFO de entrada");
        return EXIT_FAILURE;
    }

    err_fifo = mknod(nombrefifos,S_IFIFO|0777, 0);
    if( err_fifo < 0) 
    {
        perror("\nServidor: Error al crear el FIFO de salida");
        return EXIT_FAILURE;
    }

    //Ahora abrimos los FIFO
    if ((fd_e = open (nombrefifoe, O_RDWR)) < 0) 
	{
		perror ("Servidor: Error al abrir el FIFO de entrada");
		exit(EXIT_FAILURE);
	}

    if ((fd_s = open (nombrefifos, O_RDWR)) < 0) 
	{
		perror ("Servidor: Error al abrir el FIFO de salida");
		exit(EXIT_FAILURE);
	}
    sprintf (arg1, "%d", fd_s); //Preparo el parámetro para pasarselo al hijo

    //Creamos el archivo que servirá para que los procesos hijos accedan en exclusión mútua a la pantalla
    //Con O_TRUNC nos aseguramos de que no haya nada escrito 
    if((creat("lockfile", 0644)) == -1)
    {
        printf("\nError %d en creat",errno);										
        perror("\nError en creat de archivo de bloqueo");											
        exit(EXIT_FAILURE);		
    }

    printf("Servidor: Los archivos FIFO han sido creados y abiertos correctamente\n");

    /***********************************************************************/
       

        while(bytesLeidos =  read(fd_e,&pidLeido,sizeof(int))  >=0 )
        {
            if(pidLeido > 0)
            {
                printf("El servidor lee el siguiente pid:%d\n",pidLeido);

                if ( (PID = fork())<0)
                {
                    perror("Servidor: Error al crear un proceso hijo");
                    exit(EXIT_FAILURE);
                }

                if(PID == 0)    //Proceso hijo
                {

                    //Lanzo el proxy
                    if((execl("./proxy","proxy",arg1,NULL)) < 0 )
                    {
                        perror("\nServidor: Error en el execl para lanzar el proxy");
                        printf("\nServidor: Error %d en execl",errno);	
                        exit(EXIT_FAILURE);
                    }

                }
                

            }
        }

        if(bytesLeidos < 0)
        {
            perror ("Servidor: Error en la lectura en el FIFO conocido de salida");
            exit(EXIT_FAILURE);
        }

    /*********************************************************************/


    if((unlink(nombrefifoe)) < 0)
    {
        perror ("Servidor: Error al eliminar el FIFO de entrada");
		exit(EXIT_FAILURE);
    }

    if((unlink(nombrefifos)) < 0)
    {
        perror ("Servidor: Error al eliminar el FIFO de salida");
		exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}