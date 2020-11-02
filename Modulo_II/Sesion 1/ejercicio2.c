//Implementa un programa que acepte como argumento el nombre de un archivo, 
//lo abra y lea en bloques de tamaño 80 Bytes y crea un archivo salida.txt
//En el que aparezca la información separada por bloques.

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char *argv[])
{
    int fd_in, fd_out;
    char buffer[80] = "";
	char bloque[17];
	char cabecera[100];

    //Abrimos el archivo del que vamos a leer
	if( (fd_in=open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR))<0)
	{	
	    printf("\nError %d en open",errno);										
		perror("\nError en open de archivo de lectura");											
		exit(EXIT_FAILURE);													
	}

    //Si no existe salida.txt lo creamos
    if( (fd_out=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0)
	{	
	    printf("\nError %d en open",errno);										
		perror("\nError en open de archivo de escritura");											
		exit(EXIT_FAILURE);													
	}

	//Dejamos un espacio para la cabecera
	if(lseek(fd_out,39,SEEK_SET) < 0) 
	{
		perror("\nError en lseek");
		exit(EXIT_FAILURE);
	}

	int i = 1;
	while(read(fd_in,buffer,80*sizeof(char))>0 )
	{
		sprintf(bloque,"\nBloque %d\n",i); //Añade '\0' al final de la cadena
		if(write(fd_out,bloque,10) != 10)
		{
			perror("\nError en primer write");
			exit(EXIT_FAILURE);
		}

		if(write(fd_out,buffer,80) != 80)
		{
			perror("\nError en tercer write");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	if(lseek(fd_out,0,SEEK_SET) < 0) 
	{
		perror("\nError en lseek");
		exit(EXIT_FAILURE);
	}

	sprintf(cabecera,"La cantidad de bloques escritos es %d\n",i-1);
	if(write(fd_out,cabecera,39) < 0)
	{
		perror("\nError en write de la cabecera");
		exit(EXIT_FAILURE);
	}

	close(fd_in);
	close(fd_out);

    return EXIT_SUCCESS;
}


