/*
tarea3.c
Trabajo con llamadas al sistema del Sistema de Archivos ''POSIX 2.10 compliant''
Este programa fuente est� pensado para que se cree primero un programa con la parte
 de CREACION DE ARCHIVOS y se haga un ls -l para fijarnos en los permisos y entender
 la llamada umask.
En segundo lugar (una vez creados los archivos) hay que crear un segundo programa
 con la parte de CAMBIO DE PERMISOS para comprender el cambio de permisos relativo
 a los permisos que actualmente tiene un archivo frente a un establecimiento de permisos
 absoluto.
*/

#include<sys/types.h>
#include<unistd.h>	
#include<sys/stat.h>
#include<fcntl.h>		
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int fd1,fd2;
	struct stat atributos;

	// //Se abre el archivo1, si no existe lo crea
	if( (fd1=open("archivo1",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0) {
		printf("\nError %d en open(archivo1,...)",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	}

	//Fija la máscara de creación de ficheros a 0 & 0777 = 0
	umask(0);
	//Se abre el archivo2, si no existe lo crea. Se toman los permisos fijados por la llamada la sistema anterior
	if( (fd2=open("archivo2",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0) {
		printf("\nError %d en open(archivo2,...)",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	}

	//He llegado a la conclusión de que en este caso concreto la llamada al sistema umask(0)
	//No hace nada, he probado a ejecutar la primera parte del programa tanto con como sin 
	//Dicha llamada al sistema y el resultado es el mismo, siempre se crean los dos archivos con los mismos
	//permisos, lectura, escritura y ejecución para el grupo.

	//CAMBIO DE PERMISOS
	//Obtenemos los datos del primer archivo en la estructuta de datos atributos 
	if(stat("archivo1",&atributos) < 0) {
		printf("\nError al intentar acceder a los atributos de archivo1");
		perror("\nError en lstat");
		exit(EXIT_FAILURE);
	}
	//Se cambian los permisos de  ambos archivos
	//Esto le va a quitar el permiso de ejecución al grupo y va a poner el bit de propietario a true, aparecerá una S
	//En el guión de prácticas sobre el S_ISGID aparece: 
	//Activar la asignación del GID del propietario al GID efectivo del proceso que ejecute el archivo
	if(chmod("archivo1", (atributos.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
		perror("\nError en chmod para archivo1");
		exit(EXIT_FAILURE);
	}
	//Como vemos en el resultado, al ejecutar esto el archivo 2 gana una serie de permisos,
	// y pierde el de ejecución para el grupo que aquí no es mencionado. Por tanto deduzco que la 
	// llamada al sistema se dedica a poner los permisos que se le pasan y quitar todos los demás.
	if(chmod("archivo2",S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) < 0) {
		perror("\nError en chmod para archivo2");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
