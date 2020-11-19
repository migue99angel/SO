#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int extremoInferior, extremoSuperior;
    int esPrimo = 0;
    int fd[2];
    int bytesEscritos;
    char* numeroPrimo;

    //Comprobamos que se han introducido correctamente los argumentos requeridos
    if(argc < 4)
    {
		printf("\nSintaxis de ejecucion: ejercicio [extremoInferior] [extremoSuperior] [descriptorFicheroEntrada] [descriptorFicheroSalida]\n\n");
		exit(EXIT_FAILURE);
	}
    else //Realizo la conversión de char* a int
    {
        extremoInferior = atoi(argv[1]);
        extremoSuperior = atoi(argv[2]);
        fd[0] = atoi(argv[3]);
        fd[1] = atoi(argv[4]);
    }
    //Cierro el descriptor de Lectura, ya que no vamos a leer del cauce
    close(fd[0]);
    //Duplico el descriptro de escritura en cauce en el descriptor correspondiente a la salida estándar, cerrado previamente por la misma llamada al sistema dup2
    dup2(fd[1],STDOUT_FILENO);

    //Parte correspondiente para obtener los números primos en el intervalo
    for(int i = extremoInferior; i <= extremoSuperior; i++)
    {
        esPrimo = 0;
        for(int j = 2; j <= sqrt(i) && esPrimo == 0; j++)
        {
            if(i%j == 0)
                esPrimo = 1;
        }

        if(esPrimo == 0)
        {
            //Reservo memoria para el número que vamos a escribir, y lo paso de int a char* 
            numeroPrimo = (char *)(malloc (sizeof(int)));
            sprintf (numeroPrimo, "%d",i);
            if(write(fd[1],numeroPrimo,sizeof(int)) != sizeof(int))
            {
                perror("\nError en write");
                exit(EXIT_FAILURE);
		    }
        }

    }

    return EXIT_SUCCESS;

}
