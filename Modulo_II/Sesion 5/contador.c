#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>


//Contadores globales para el conteo de señales recibidas
int cont_usr1=0;
int cont_usr2=0;
int cont_alrm=0;

static void sig_USR_hdlr(int sigNum)
{
    if(sigNum == SIGUSR1)
    {
        cont_usr1++;
        printf("\nRecibida la senal SIGUSR1, ya van %d \n\n", cont_usr1);
    }
    else if(sigNum == SIGUSR2)
    {
        cont_usr2++;
        printf("\nRecibida la senal SIGUSR2, ya van %d \n\n", cont_usr2); 

    } 
    else if(sigNum == SIGALRM)
    {
        cont_alrm++;
        printf("\nRecibida la senal SIGALRM, ya van %d \n\n", cont_alrm); 
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sig_USR_nact;

    if(setvbuf(stdout,NULL,_IONBF,0)) 
    {    
        perror("\nError en setvbuf");
    }

    printf("Este programa solo cuenta las señales SIGUSR1 SIGUSR2 y SIGALRM, las demás posibles se harían de la misma manera");
    //Inicializar la estructura sig_USR_na para especificar la nueva acci�n para la se�al.

    sig_USR_nact.sa_handler= sig_USR_hdlr;


    //'sigemptyset' inicia el conjunto de se�ales dado al conjunto vacio. 

    sigemptyset (&sig_USR_nact.sa_mask);
    sig_USR_nact.sa_flags = 0;

    //Establecer mi manejador particular de se�al para SIGUSR1
    if( sigaction(SIGUSR1,&sig_USR_nact,NULL) <0) 
    {
        perror("\nError al intentar establecer el manejador de senal para SIGUSR1");
        exit(EXIT_FAILURE);
    }
    //Establecer mi manejador particular de se�al para SIGUSR2
    if( sigaction(SIGUSR2,&sig_USR_nact,NULL) <0) 
    {
        perror("\nError al intentar establecer el manejador de senal para SIGUSR2");
        exit(EXIT_FAILURE);
    }
    //Establecer mi manejador particular de se�al para SIGALRM
    if( sigaction(SIGALRM,&sig_USR_nact,NULL) <0) 
    {
        perror("\nError al intentar establecer el manejador de senal para SIGALRM");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        
    }
}