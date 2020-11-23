//Compilar con el siguiente comando: gcc esclavo.c -o esclavo -lm
//El -lm es para que enlace el math.h

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
    int bytesEscritos;
    char* numeroPrimo;

    //Comprobamos que se han introducido correctamente los argumentos requeridos
    if(argc < 2)
    {
		printf("\nSintaxis de ejecucion: ejercicio [extremoInferior] [extremoSuperior]\n\n");
		exit(EXIT_FAILURE);
	}
    else //Realizo la conversión de char* a int
    {
        extremoInferior = atoi(argv[1]);
        extremoSuperior = atoi(argv[2]);
    }


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
            if(write(STDOUT_FILENO,numeroPrimo,sizeof(int)) != sizeof(int))
            {
                perror("\nError en write");
                exit(EXIT_FAILURE);
		    }
        }

    }

    return EXIT_SUCCESS;

}
