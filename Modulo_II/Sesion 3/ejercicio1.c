/*Ejercicio 1. Implementa un programa en C que tenga como argumento un número entero. Este
programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un
número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar*/

#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<stdio.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    int pid;
    //Comprobamos que se han introducido los parámertros correctos
	if(argc < 2)
    {
		printf("\nSintaxis de ejecucion: ejercicio [numero]+\n\n");
		exit(EXIT_FAILURE);
	}

   
    if((pid=fork()) < 0)
    {
        perror("Error en el fork\n");
        exit(EXIT_FAILURE);
    }

    int a = atoi(argv[1]); //Convertimos de char* a int 
    if(pid == 0)    //Proceso hijo
    {
        printf("El proceso hijo realiza la comprobación de paridad\n");
        if(a%2==0)
        {
            printf("El número %d es par \n",a);
        }
        else
        {
            printf("El número %d es impar \n",a);
        }
        
    }
    else            //Proceso padre
    {
        printf("El proceso padre realiza la comprobación de divisibilidad entre 4\n");
        if(a%4==0)
        {
            printf("El número %d es divisible por 4 \n",a);
        }
        else
        {
            printf("El número %d no es divisible por 4 \n",a);
        }
    }

    return EXIT_SUCCESS;
}