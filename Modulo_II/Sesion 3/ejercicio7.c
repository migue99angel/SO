/*Ejercicio 7. Escribe un programa que acepte como argumentos el nombre de un programa, sus
argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa deberá ejecutar el
programa pasado como primer argumento en foreground si no se especifica la cadena “bg” y en
background en caso contrario. Si el programa tiene argumentos hay que ejecutarlo con éstos.*/


#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char programa[10] = {}; 
    char *parametros[10];
    char* bg = "-bg";
    char *guion = "-";
    char *ruta = "/bin/";
    char buff[10] = {}; 
    int i;
    int contador = 0;
    int childpid;
    int estado;


    if(argc < 2)
    {
        printf("\nSintaxis de ejecucion: ejercicio7 [programa] [parámetros] [bg]\n\n");
		exit(EXIT_FAILURE);
    }
    else
    {
        sprintf(programa,"%s%s",ruta,argv[1]);
        parametros[contador] = argv[1];
        contador++;
    
        
        if(argc > 2)
        {
            for(i = 2; i < argc ; i++)
            {
                parametros[contador] = (char*) malloc( (strlen(argv[i]) + 2) * sizeof(char));
                sprintf(parametros[contador],"%s%s",guion,argv[i]);
                contador++;
            }

            parametros[contador] = 0;
        }

        if((strcmp(parametros[contador-1],bg)==0))
        {
            if((childpid = fork()) == -1)
            {
                printf("\nError %d en fork",errno);																			
                exit(EXIT_FAILURE);
            }

            if(childpid == 0)
            {
                if((execv(programa,parametros)) < 0 )
                {
                    perror("\nError en el execv");
                    printf("\nError %d en exec",errno);	
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            if((execv(programa,parametros)) < 0 )
            {
                perror("\nError en el execv");
                printf("\nError %d en exec",errno);	
                exit(EXIT_FAILURE);
            }
        }
        
        if(childpid != 0)
            if((waitpid(childpid,&estado,NULL)) == -1)
                perror("Error en llamada a waitpid");										

    }
    return EXIT_SUCCESS;

} 