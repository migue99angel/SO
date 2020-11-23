#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i = 0;
    pid_t PID = -1;
    int extremoInferior, extremoSuperior;
    char* arg1;
    char* arg2;
    int fdPrimeraMitad[2], fdSegundaMitad[2];
    int bytesLeidos;

    //Comprobamos que se han introducido correctamente los argumentos requeridos
    if(argc < 3)
    {
		printf("\nSintaxis de ejecucion: ejercicio [extremoInferior] [extremoSuperior]\n\n");
		exit(EXIT_FAILURE);
	}
    else
    {
        //Realizo la conversión de char* a int
        extremoInferior = atoi(argv[1]);
        extremoSuperior = atoi(argv[2]);
    }

    //Creo primero los dos cauces sin nombre para que los procesos hijos hereden los descriptores de archivo
    pipe(fdPrimeraMitad);
    pipe(fdSegundaMitad);

    //Llamada al sistema fork para crear dos procesos hijos, que los identificaré mediante su valor guardado en la variable i
    // 1 -> Primer hijo que ejecutará el programa esclavo para la mitad de menor valor del intervalo 
    // 2 -> Segundo hijo que ejecutará el programa esclavo para la mitad de mayor valor del intervalo 
    for(i = 0; i < 2 && PID !=0; i++)
    {
        if ( (PID = fork())<0)
        {
		    perror("fork");
		    exit(EXIT_FAILURE);
	    }
    }

    if(PID == 0)    //Procesos hijos
    {
        //Reservo memoria para los argumentos
        arg1 = (char *)(malloc (sizeof(int)));
        arg2 = (char *)(malloc (sizeof(int)));

        if(i == 1)
        {
            extremoSuperior = (extremoInferior+extremoSuperior)/2;

            //En el primer esclavo cierro los descriptores de archivo del segundo esclavo
            close(fdSegundaMitad[0]);
            close(fdSegundaMitad[1]);
            //Cierro el descriptor de Lectura, ya que no vamos a leer del cauce
            close(fdPrimeraMitad[0]);
            //Duplico el descriptor de escritura en cauce en el descriptor correspondiente a la salida estándar, cerrado previamente por la misma llamada al sistema dup2
            dup2(fdPrimeraMitad[1],STDOUT_FILENO);
        }
        else
        {
            if(i == 2)
            {
                extremoInferior = ((extremoInferior+extremoSuperior)/2) + 1;

                //En el segundo esclavo cierro los descriptores de archivo del primer esclavo
                close(fdPrimeraMitad[0]);
                close(fdPrimeraMitad[1]);
                //Cierro el descriptor de Lectura, ya que no vamos a leer del cauce
                close(fdSegundaMitad[0]);
                //Duplico el descriptor de escritura en cauce en el descriptor correspondiente a la salida estándar, cerrado previamente por la misma llamada al sistema dup2
                dup2(fdSegundaMitad[1],STDOUT_FILENO);
            }
        }


        //Con la función sprintf consigo pasar los valores de tipo int a tipo char* que es lo que admite execl
        sprintf (arg1, "%d", extremoInferior);
        sprintf (arg2, "%d", extremoSuperior);

        //Ejecuto el ejecutable resultante al compilar el programa esclavo.c, 
        //es necesario que esté en el mismo directorio que el ejecutable resultante a compilar maestro.c
        if((execl("./esclavo","esclavo",arg1,arg2,NULL)) < 0 )
        {
            perror("\nError en el execv");
            printf("\nError %d en exec",errno);	
            exit(EXIT_FAILURE);
        }
    }
    else //Proceso padre
    {
        //Cierro en el padre los descriptores de archivo de escritura de los pipe ya que desde el padre no vamos a escribir en el pipe
        close(fdPrimeraMitad[1]);
        close(fdSegundaMitad[1]);

        //Reservo memoria para el entero que vamos a leer del pipe
        char* numeroLeido = (char *)(malloc (sizeof(int)));;

        printf("Primos calculados por el primer hijo: \n");
        //Duplicamos el descriptor de lectura del cauce (Primer esclavo) en el descriptor correspondiente a la entrada estándar. Cerrado previamente por dup2
        dup2(fdPrimeraMitad[0],STDIN_FILENO);
        //Leo números del cauce mientras haya
        while(bytesLeidos = (read(fdPrimeraMitad[0],numeroLeido,sizeof(int)))>0 )
        {
            printf("%d\n",atoi(numeroLeido));
        }
        //Cerramos el cauce de lectura del primer esclavo, ya no lo necesitamos
        close(fdPrimeraMitad[0]);
        printf("Primos calculados por el segundo hijo: \n");
        //Duplicamos el descriptor de lectura del cauce (Segundo esclavo) en el descriptor correspondiente a la entrada estándar. Cerrado previamente por dup2
        dup2(fdSegundaMitad[0],STDIN_FILENO);
        //Leo números del cauce mientras haya
        while(read(fdSegundaMitad[0],numeroLeido,sizeof(int))>0 )
        {
            printf("%d\n",atoi(numeroLeido));
        }

        close(fdSegundaMitad[0]);

    }
    

    return EXIT_SUCCESS;

}
