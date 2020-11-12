/*Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se
identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID.
El proceso padre simplemente tendrá que esperar la finalización de todos sus hijos y cada vez que detecte 
la finalización de uno de sus hijos escribirá en la salida estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID> 
Sólo me quedan <NUM_HIJOS> hijos vivos*/

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
    int n_hijos = 5;
    int estado;
    pid_t pid_aux = 0;
    for(int i = 0; i < n_hijos; i++)
    {
        if(childpid != 0)
        {
            if((childpid = fork()) == -1)
            {
                printf("\nError %d en fork",errno);																			
                exit(EXIT_FAILURE);
            }
        }
    }

    if(childpid == 0)
    {
        printf("Soy el hijo %d\n",getpid());
        exit(EXIT_SUCCESS);
    }


    while(childpid != 0 && n_hijos > 0)
    {
        pid_aux = wait(&estado);
        if(pid_aux == -1)
        {
            perror("En wait()");										
        }
        printf("\nAcaba de finalizar mi hijo con PID %d",pid_aux);
        n_hijos--;
        printf("\nSolo me quedan %d hijos vivos",n_hijos);
        
    }

    

    return EXIT_SUCCESS;
}
