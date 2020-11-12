#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<stdio.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    int childpid;
    for(int i = 1; i <= 3; i++)
    {
        if((childpid = fork()) == -1)
        {
            fprintf(stderr, "Could not create child %d: %d\n",i,strerror(errno));
            exit(-1);
        }

        if(childpid)
            printf("\nMensaje de prueba");
    }

    

    return EXIT_SUCCESS;
}
