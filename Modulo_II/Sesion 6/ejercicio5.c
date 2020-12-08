/*
Escribir un programa, similar a la orden cp, que utilice para su implementación la
llamada al sistema mmap() y una función de C que nos permite copiar memoria, como por
ejemplo memcpy(). Para conocer el tamaño del archivo origen podemos utilizar stat() y para
establecer el tamaño del archivo destino se puede usar ftruncate().
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
    int fd_origen;
    int fd_destino;
    struct stat atributos_origen;
    struct stat atributos_destino;

    char* mapeado_origen;
    char* mapeado_destino;

    if( argc < 3 )
    {
        printf("Llamada incorrecta al programa. Uso correcto ./copy origen destino \n");
        exit(EXIT_FAILURE);
    }

    fd_origen = open (argv[1], O_RDONLY);
    fd_destino = open (argv[2], O_RDWR);

    if (fd_origen < 0) {
            printf("Error al abrir archivo origen\n");
            return EXIT_FAILURE;
    }

    if (fd_destino < 0) {
            printf("Error al abrir archivo destino\n");
            return EXIT_FAILURE;
    }
    
    if(stat(argv[1],&atributos_origen) < 0)
    {
        printf("\nError al intentar acceder a los atributos de %s",argv[1]);
        perror("\nError en primer stat");
        exit(EXIT_FAILURE);
    }


    if(stat(argv[2],&atributos_destino) < 0)
    {
        printf("\nError al intentar acceder a los atributos de %s",argv[2]);
        perror("\nError en el segundo stat");
        exit(EXIT_FAILURE);
    }

    //Trunco el tamaño del archivo destino al del origen
    if((ftruncate(fd_destino, atributos_origen.st_size)) < 0)
    {
        printf("Fallo al truncar el destino al tamaño del origen\n");
        perror("\nError en ftruncate");
        return EXIT_FAILURE;
    }

    mapeado_origen = (char *) mmap (0, atributos_origen.st_size, PROT_READ, MAP_SHARED, fd_origen, 0);
    if (mapeado_origen == MAP_FAILED)
    {
       printf("Fallo el mapeo del origen\n");
       perror("\nError en mmap origen");
       return EXIT_FAILURE;
    }

    mapeado_destino = (char *) mmap (0, atributos_destino.st_size, PROT_WRITE, MAP_SHARED, fd_destino, 0);
    if (mapeado_destino == MAP_FAILED)
    {
       printf("Fallo el mapeo del destino\n");
       perror("\nError en mmap destino");
       return EXIT_FAILURE;
    }

    close(fd_origen);
    close(fd_destino);

    if((memcpy(mapeado_destino, mapeado_origen,atributos_origen.st_size)) < 0)
    {
        printf("Fallo al copiar\n");
        perror("\nError en memcpy");
        return EXIT_FAILURE;
    }

    exit(EXIT_SUCCESS);
}