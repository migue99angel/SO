/*
tarea1.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
Probar tras la ejecuci�n del programa: $>cat archivo y $> od -c archivo
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
			*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

char buf1[]="abcdefghij";
char buf2[]="ABCDEFGHIJ";

int main(int argc, char *argv[])
{
	int fd;

	if( (fd=open("archivo",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {	//Con la llamada al sistema open abrimos el archivo y obtenemos el descriptor de archivo
	printf("\nError %d en open",errno);										//La forma de usar open es : 
		perror("\nError en open");											//int open(const char *pathname, int flags);
		exit(EXIT_FAILURE);													//int open(const char *pathname, int flags, mode_t mode);
	}


// El flag O_CREAT es para que si no existe el fichero que se cree.
// El flag O_TRUNC si el fichero ya existe y es un fichero regular  y  el  modo  de  apertura  permite escribir (p.e., es O_RDWR o O_WRONLY) será truncado a longitud 0.
// El flag O_WRONLY es para abrir el fichero solo en modo escritura (aunque tengamos permisos de lectura tambien.)
// Los flags de modo sirven para indicar que permisos tendra el archivo en caso de que no exista y por tanto se cree ya que se usa O_CREAT
// El flag S_IRUSR indica que el user tiene permiso de lectura
// El flag S_IWUSR indica que el user tiene permiso de escritura

// Con la llamada al sistema write (write(int fd, const void *buf, size_t num))
// Escribimos en el archivo identificado por el descriptor de fichero fd, desde el buffer 
// que comienza en buf hasta la cantidad de num bytes 
// Si no hay ningun error devuelve el número de bytes escritos
	if(write(fd,buf1,10) != 10) {
		perror("\nError en primer write");
		exit(EXIT_FAILURE);
	}
// La llamada al sistema lseek (lseek(int fildes, off_t offset, int whence)) sirve para reposicionar
// el puntero del descriptor de fichero (fildes) con el argumento offset de acuerdo a las directivas whence
// La directiva SEEK_SET indica que el puntero se coloca a offset bytes 
	if(lseek(fd,40,SEEK_SET) < 0) {
		perror("\nError en lseek");
		exit(EXIT_FAILURE);
	}

	if(write(fd,buf2,10) != 10) {
		perror("\nError en segundo write");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
