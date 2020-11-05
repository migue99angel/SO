/*
 Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte
como entrada:
• Un argumento que representa el 'pathname' de un directorio.
• Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar
para cambiar los permisos en la llamada al sistema chmod). Para convertir este argumento
tipo cadena a un tipo numérico puedes utilizar la función strtol. Consulta el manual en
línea para conocer sus argumentos.
El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los
permisos de todos los archivos que se encuentren en el directorio indicado en el primer
argumento.
El programa debe proporcionar en la salida estándar una línea para cada archivo del directorio
que esté formada por:
<nombre_de_archivo> : <permisos_antiguos> <permisos_nuevos>
Si no se pueden cambiar los permisos de un determinado archivo se debe especificar la siguiente
información en la línea de salida:
<nombre_de_archivo> : <errno> <permisos_antiguos>
*/


#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>



int main(int argc, char *argv[])
{

    DIR *directorio;
    struct dirent *entrada;
    struct stat atributos;
    char* pathname;
    int permisos;

	//Comprobamos que se han introducido los parámertros correctos
	if(argc != 3)
    {
		printf("\nSintaxis de ejecucion: ejercicio [<nombre_archivo>]+\n\n");
		exit(EXIT_FAILURE);
	}
    else
    {
        pathname = argv[1];
        permisos = strtol(argv[2],NULL,8);
    }

    directorio = opendir(pathname);
    if(directorio==NULL)
    {
        printf("\nError %d en opendir",errno);										
		perror("\nError en opendir del directorio pasado como primer parámetro");											
        exit(EXIT_FAILURE);
    }
    else
    {

        while( (entrada = readdir(directorio)) != NULL)
        {
            char ruta[512];
            //Concateno el pathname con el nombre del a archivo
            sprintf(ruta,"%s/%s",pathname,entrada->d_name);

            if(stat(ruta,&atributos) < 0)
            {
                printf("\nError al intentar acceder a los atributos de %s",ruta);
                perror("\nError en primer stat");
                exit(EXIT_FAILURE);
            }

            //Solo modifico los permisos de los archivos regulares, de . y .. no 
            if(S_ISREG(atributos.st_mode))
            {
                mode_t permisos_antiguos = atributos.st_mode;
                int error_chmod = chmod(ruta, permisos);

                if(error_chmod == 0)
                {
                    if(stat(ruta,&atributos) < 0)
                    {
                        printf("\nError al intentar acceder a los atributos de %s",ruta);
                        perror("\nError en segundo stat");
                        exit(EXIT_FAILURE);
                    }

                    printf("%s: %o  %o \n",ruta,permisos_antiguos, atributos.st_mode );
                }
                else
                {
                    printf("%s: %d  %o \n",ruta,errno ,permisos_antiguos);
                }
            }
            
        }
    }

    return EXIT_SUCCESS;
}