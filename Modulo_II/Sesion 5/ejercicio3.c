#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

static void sig_USR_hdlr(int sigNum)
{
    if(sigNum == SIGUSR1)
        printf("\nRecibida la senal SIGUSR1\n\n"); 
}

int main(int argc, char *argv[])
{
    struct sigaction sig_USR_nact;

    if(setvbuf(stdout,NULL,_IONBF,0)) 
    {    
        perror("\nError en setvbuf");
    }

    //Inicializar la estructura sig_USR_na para especificar la nueva acci�n para la se�al.

    sig_USR_nact.sa_handler= sig_USR_hdlr;


    //'sigfillset' inicia el conjunto de se�ales a todas las señales

    sigfillset (&sig_USR_nact.sa_mask);
    //sigdelset elimina la señal pasada como argumento de la máscara, le estamos diciendo que bloquee todas las señales (las que se puedan) menos SIGUSR1
    sigdelset(&sig_USR_nact.sa_mask,SIGUSR1); 

    sig_USR_nact.sa_flags = 0;

    //Establecer mi manejador particular de se�al para SIGUSR1
    if( sigaction(SIGUSR1,&sig_USR_nact,NULL) <0) 
    {
        perror("\nError al intentar establecer el manejador de senal para SIGUSR1");
        exit(EXIT_FAILURE);
    }

    printf("Se suspende el proceso hasta que se reciba la señala\n");  

    sigsuspend(&sig_USR_nact.sa_mask);

    printf("Se ha recibido la señal y se va a finalizar la ejecución\n");

}
