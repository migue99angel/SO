/*Implementa lo mismo que el ejercicio 3, pero usando la llamada al sistema nftw()*/

#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include<ftw.h>

int cantidad;
int tamanio;

int busquedaRecursiva(const char* path, const struct stat* stat, int flags)
{
    if( S_ISREG(stat->st_mode) && (stat->st_mode & S_IXGRP)  && (stat->st_mode  & S_IXOTH) )
    {
        printf("%s  %ld \n",path,stat->st_ino);

        cantidad++;
        tamanio += stat->st_blksize;
    }

    return 0;
    
}

int main(int argc, char *argv[])
{
    char* pathname;
    cantidad=0;
    tamanio=0;

    if(argc == 2)
    {
        pathname = argv[1];
	}
    else
    {
        pathname = ".";
    }

    nftw(pathname,busquedaRecursiva,10,1);

    printf("\nExisten %d archivos con permisos de ejecución para grupo y otros \n",cantidad);
    printf("\nEl tamaño total ocupado por dichos archivos es de %d  \n",tamanio);

    return EXIT_SUCCESS;
}
