/*Ejercicio 5. Implementa una modificación sobre el anterior programa en la que el proceso
padre espera primero a los hijos creados en orden impar (1o,3o,5o) y después a los hijos pares (2o
y 4o) */


#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<stdio.h>
#include<errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    int childpid=1;
    int n_hijos = 6;
    int estado;
    int pidsHijos[6];
    pid_t pid_aux = 0;
    for(int i = 0; i < n_hijos; i++)
    {
        if(childpid != 0)
        {
            if(setvbuf(stdout,NULL,_IONBF,0))
                perror("\nError en setvbuf");

            if((childpid = fork()) == -1)
            {
                printf("\nError %d en fork",errno);																			
                exit(EXIT_FAILURE);
            }
            else
            {
                //Almaceno en un array todos los pids
                pidsHijos[i] = childpid;
            }
            
        }
    }

    if(childpid == 0)
    {
        printf("\nSoy el hijo %d",getpid());
        exit(EXIT_SUCCESS);
    }
    else
    {
        int contador = n_hijos;
        printf("\nEsperando a los procesos lanzados en iteración impar\n");
        for(int i = 1; i <= n_hijos && contador > 0; i+=2)
        {
            pid_aux = waitpid(pidsHijos[i],&estado,NULL);
            if(pid_aux == -1)
            {
                perror("En primera llamada a wait()");										
            }
            contador--;
            printf("\nAcaba de finalizar mi hijo con PID %d",pid_aux);
            printf("\nSolo me quedan %d hijos vivos",contador);
        }

        printf("\nEsperando a los procesos lanzados en iteración par\n");
        for(int i = 0; i <= n_hijos && contador > 0; i+=2)
        {
            pid_aux = waitpid(pidsHijos[i],&estado,NULL);
            if(pid_aux == -1)
            {
                perror("En segunda llamada a waitpid()");										
            }
            contador--;
            printf("\nAcaba de finalizar mi hijo con PID %d",pid_aux);
            printf("\nSolo me quedan %d hijos vivos",contador);
        }
    }

    return EXIT_SUCCESS;
}


