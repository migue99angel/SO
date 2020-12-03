## Sesión 6

Durante esta sesión vamos a estudiar el **Control de archivos y archivos proyectados a memoria**

**Función fcntl**

La llamada al sisteme **fcntl** (file control) es una función multipropósito que, de forma general, permite consultar o ajustar las banderas de control de acceso de un descriptor, es decir, de un archivo abierto. Además, permite realizar la duplicación de descriptores de archivo y bloqueo de un archivo para acceso exclusivo.

    #include <unistd.h>
    #include <fcntl.h>

    int fcntl(int fd, int cmd);
    int fcntl(int fd, int cmd, long arg);
    int fcntl(int fd, int cmd, struct flock *lock)


El argumento **cmd** admite un rango muy diferente de operaciones a realizar sobre el descriptor de archivo que se especifica en fd. El tercer argumento que es opcional va a depender de la orden indicada. Algunas de las ordenes permitidas son las siguientes: 

- **F_GETFL**: Lee las banderas de un descriptor de fichero.
- **F_SETFL**: Ajusta o limpia las banderas de acceso que se especifican como tercer argumento. En Linux, esta orden sólo puede cambiar las banderas O_APPEND, O_NONBLOCK,O_ASYNC  y O_DIRECT.
- **F_GETFD**: Devuelve la bandera **close-on-exec** del archivo indicado. Si devuelve un 0 la bandera está desactivada, en caso contrario devuelve un valor distinto de 0. La bandera **close-on-exec** de un archivo recién abierto está desactivada por defecto.

Si la bandera **close-on-exec** está activa en un descriptor, al ejecutar la llamada **exec()** el proceso hijo no heredará este descriptor. 

- **F_SETFD**: Establece la bandera close-on-exec al valor especificado por el bit  FD_CLOEXEC  de arg.

- **F_DUPFD**: Busca el descriptor de fichero disponible de número, mayor o igual que arg, y lo  convierte en una copia de fd.  Esto es diferente en dup2(2) que usa exactamente el descriptor especificado. Los descriptores antiguo y nuevo pueden  usarse  indistintamente.  Ambos  comparten bloqueos  (''locks''),  indicadores  de  posición de ficheros y opciones o banderas(''flags''); por ejemplo, si la posición del fichero se modifica  usando  lseek  en uno de los descriptores, la posición del otro resulta modificada simultáneamente. Sin embargo, los dos descriptores no comparten la bandera close-on-exec («cerrar al ejecutar»). La bandera close-on-exec de la copia está desactivada, lo que significa que no se cerrará al ejecutar. En caso de éxito, se devuelve el nuevo descriptor.

- **F_SETLK**: Establece un cerrojo sobre un archivo. No bloquea si no tiene éxito inmediatamente.
- **F_SETLKW**: Establece un cerrojo y bloquea al proceso llamador hasta qwe se adquiere el cerrojo
- **F_GETLK**: Consulta si existe un bloqueo sobre una región del archivo.

Más información aquí: http://manpages.ubuntu.com/manpages/bionic/es/man2/fcntl.2.html


**Banderas de estado de un archivo abierto**

Uno de los usos de la función nos permite recuperar o modificar el modo de acceso y las banderas de estado (las especificadas en **open**) de un archivo abierto. Para esto usamos la bandera **F_GETFL**.

Podemos utilizar la orden **F_SETFL** para modificar alguns de las banderas de estado del archivo abierto. Estas banderas son **O_APPEND**, **O_NONBLOCK**, **O_NOATIME**,**O_ASYNC** y **O_DIRECT**. Se ignorará cualquier intento de modificar alguna otra bandera.

El uso de la función fcntl para modificar banderas de estado es útil en distintas situaciones, por ejemplo cuando el archivo no ha sido abierto por el programa llamador o cuando el descriptor de archivo se ha obtenido a través de una llamada la sistema que no es open (pipe o socket por ejemplo).

Normalmente lo que haremos será obtener las banderas con F_GETFL, operaremos con ellas a nuestro antojo y las asignaremos de nuevo con F_SETFL.


### Actividad 1


![Ejercicio1](CodigoEjercicio1_S6.jpeg)
![Ejercicio1](EjecucionEjercicio1_S6.jpeg)

### Actividad 2



![Ejercicio2](CodigoEjercicio2_S6.jpeg)
![Ejercicio2](EjecucionEjercicio2_S6.jpeg)


**La función fcntl y el bloqueo de archivos**

Es evidente que el acceso de varios procesos a un archivo para leer/escribir puede producir condiciones de carrera. Para evitarlas debemos sincronizar las acciones de éstos. Si bien podríamos pensar en utilizar semáforos, el uso de cerrojos de archivos es más corriente debido a que el kernel asocia automáticamente los cerrojos con archivos. Tenermos dos APIs para manejar cerrojos de archivos: 

- **flock()** que utiliza un cerrono para bloquear el archivo completo.
- **fcntl()** que utiliza cerrojos para bloquear regiones de un archivo.