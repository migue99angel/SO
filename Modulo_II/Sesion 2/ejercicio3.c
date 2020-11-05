/*Ejercicio 3. Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a
partir de uno dado como argumento y devuelva la cuenta de todos aquellos archivos regulares
que tengan permiso de ejecución para el grupo y para otros. Además del nombre de los archivos
encontrados, deberá devolver sus números de inodo y la suma total de espacio ocupado por
dichos archivos. El formato de la nueva orden será:

$> ./buscar <pathname>

donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a
analizar la estructura del árbol de subdirectorios. En caso de que no se le de argumento, tomará
como punto de partida el directorio actual. Ejemplo de la salida después de ejecutar el
programa:
Los i-nodos son:
./a.out 55
./bin/ej 123
./bin/ej2 87
...
Existen 24 archivos regulares con permiso x para grupo y otros
El tamaño total ocupado por dichos archivos es 2345674 bytes*/


#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>

void busquedaRecursiva(char* pathname, int *cantidad, int *tamanio)
{
    DIR *directorio;
    struct dirent *entrada;
    struct stat atributos;
    char ruta[512];
	
    directorio = opendir(pathname);
    if(directorio==NULL)
    {
        printf("\nError %d en opendir, se ha pasado como parámetro %s",errno,pathname);										
		perror("\nError en opendir del directorio pasado como primer parámetro");											
        exit(EXIT_FAILURE);
    }

    while( (entrada = readdir(directorio)) != NULL)
    {
        
        if( entrada->d_type == DT_DIR && strcmp(".",entrada->d_name) != 0 && strcmp("..",entrada->d_name) != 0 )
        {
            sprintf(ruta,"%s/%s",pathname,entrada->d_name);
            busquedaRecursiva(ruta,cantidad,tamanio);
        }
        else 
        {
            sprintf(ruta,"%s/%s",pathname,entrada->d_name);
            if(stat(ruta,&atributos) < 0)
            {
                printf("\nError al intentar acceder a los atributos de %s",ruta);
                perror("\nError en primer stat");
                exit(EXIT_FAILURE);
            }

            if(entrada->d_type == DT_REG && (atributos.st_mode & S_IXGRP) && (atributos.st_mode  & S_IXOTH))
            {
                printf("%s  %ld \n",ruta,atributos.st_ino);

                (*cantidad)++;
                (*tamanio) += atributos.st_blksize;
            }
        }
    }

    closedir(directorio);
}


int main(int argc, char *argv[])
{
    char* pathname;
    int cantidad=0, tamanio=0;

    if(argc != 2)
    {
        pathname = argv[1];
	}
    else
    {
        pathname = ".";
    }

    busquedaRecursiva(pathname, &cantidad, &tamanio);

    printf("\nExisten %d archivos con permisos de ejecución para grupo y otros \n",cantidad);
    printf("\nEl tamaño total ocupado por dichos archivos es de %d  \n",tamanio);

    return EXIT_SUCCESS;
}
