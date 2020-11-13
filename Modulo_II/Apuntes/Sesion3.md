## Sesión 3

Esta sesión consiste en estudiar y practicar, las llamadas al sistema para el control de procesos.

Se empieza explicando que es un identificador de procesos, además se mencionan las siguientes llamadas al sistema para trabajar con dichos identificadores.

    #include <unistd.h>
    #include <sys/types.h>

    pid_t getpid(void); // devuelve el PID del proceso que la invoca.
    pid_t getppid(void); // devuelve el PID del proceso padre del proceso que la invoca.
    uid_t getuid(void); // devuelve el identificador de usuario real del proceso que la invoca.
    uid_t geteuid(void); // devuelve el identificador de usuario efectivo del proceso que la invoca.
    gid_t getgid(void); // devuelve el identificador de grupo real del proceso que la invoca.
    gid_t getegid(void); // devuelve el identificador de grupo efectivo del proceso que la invoca.

**Llamada al sistema fork**

    #include <sys/types.h>
    #include <unistd.h>

    pid_t fork(void);

fork  crea  un proceso hijo que difiere de su proceso padre sólo en su PID y PPID, y en el hecho de que el uso de recursos esté asignado a 0.  Los candados de fichero (file locks) y las señales pendientes no se heredan.

Valor devuelto:  En caso de éxito, se devuelve el PID del proceso hijo en el hilo de ejecución de su padre, y se devuelve un 0  en el hilo de ejecución del hijo. En caso de fallo, se devolverá un -1 en el contexto del padre, no se creará ningún proceso hijo, y se pondrá en errno un  valor apropiado.

Más información en : http://manpages.ubuntu.com/manpages/bionic/es/man2/fork.2.html

### Actividad 1

Es destacable comentar el uso de la función **atoi** para pasar de char *, tipo de dato de los argumentos del main a int.

![Ejercicio2](CodigoEjercicio1_S3.jpeg)

![Ejercicio2](EjecucionEjercicio1_S3.jpeg)

### Actividad 2

![Ejercicio2](CodigoEjercicio2_S3.jpeg)

![Ejercicio2](EjecucionEjercicio2_S3.jpeg)

La primera ejecución corresponde al código con el apartado (1) comentada, y la segunda sin comentar. 

Lo que ocurre en este ejercicio es que, a diferencia de las llamadas al sistema del núcleo (write) en las que no se realiza buffering (por lo que se escribe directamente en la salida),  mientras que en las llamada al sistema de la biblioteca de entrada y salida de C, si se realiza buffering, de manera que cuando se crea el proceso hijo con la orden fork, este hereda el buffer del padre, que contiene el mensaje "Mensaje previo a la ejecución del fork", de manera que cuando el hijo ejecuta el último printf añade a su buffer el nuevo mensaje y escribe ambos mensajes juntos. Para evitar esto se utiliza la llamada al sistema setvbuf, de manera que le decimos al padre que no utilice buffering y por tanto en la segunda ejecución el hijo no hereda el buffer del padre.

**Llamadas al sistema wait y waitpid**


Algunas llamadas al sistema importantes son:

    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t wait(int *status);
    pid_t waitpid(pid_t pid, int *status, int options);

La  función  wait  suspende  la  ejecución del proceso actual haste que un proceso hijo ha terminado, o hasta que se produce una señal cuya acción es terminar el  proceso  actual  o llamar  a  la  función  manejadora  de la señal. Si un hijo ha salido cuando se produce la llamada (lo que se entiende por proceso "zombie"), la función vuelve inmediatamente. Todos los recursos del sistema reservados por el hijo son liberados.

La  función  waitpid  suspende  la  ejecución  del  proceso  en  curso  hasta  que un hijo especificado por el argumento pid ha terminado, o hasta que  se  produce  una  señal  cuya acción es finalizar el proceso actual o llamar a la función manejadora de la señal.

Más información: http://manpages.ubuntu.com/manpages/bionic/es/man2/wait.2.html

    void _Exit(int status);

La  función  _exit termina "inmediatamente" la ejecución del proceso invocador.  Todos los descriptores de ficheros abiertos  que  pertenezcan  al  proceso  se  cierran todos  los procesos  hijos  son  heredados  por el proceso 1, init, y al proceso padre se le envía la señal SIGCHLD.

El valor status se le devuelve al proceso padre como estado de terminación del proceso,  y se puede leer mediante una de las funciones de la familia de wait.



### Actividad 4

![Ejercicio2](CodigoEjercicio4_S3.jpeg)

![Ejercicio2](EjecucionEjercicio4_S3.jpeg)


### Actividad 5

![Ejercicio2](CodigoEjercicio5_S3.jpeg)

![Ejercicio2](EjecucionEjercicio5_S3.jpeg)


**Familia de lllamadas al sistema exec**

    #include <unistd.h>

    extern char **environ;

    int execl(const char *path, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execle(const char *path, const char *arg, ..., char * const envp[]);
    int execv(const char *path, char *const argv[]);
    int execvp(const char *file, char *const argv[]);
    int execvpe(const char *file, char *const argv[], char *const envp[]);

Cuando un proceso ejecuta una llamada exec, el espacio de direcciones de usuario del proceso se reemplaza completamente por un nuevo espacio de direcciones; el del programa que se la pasa como argumento, y este proceso empieza a ejecutarse en el contexto del proceso hijo empezando en la función main. El PID no cambia ya que no se crea ningún proceso nuevo.

Más información en : http://manpages.ubuntu.com/manpages/precise/man3/exec.3.html


### Actividad 6

![Ejercicio2](EjecucionTarea5_S3.jpeg)

El programa tarea5.c lo que hace es crear mediante la llamada al sistema fork un proceso hijo, este proceso hijo ejecuta una llamada al sistema execl que ejecuta el programa ldd con el argumento tarea5, mientras que el padre ejecuta una llamada al sitema wait para esperar la finzalización del proceso hijo, finalmente el padre muestra por salida estándar el código de finalización del proceso hijo.


### Actividad 7
En este ejercicio ha sido necesario reservar memoria para el array en el que almaceno los parámetro con el obteivo de no perder la referencia de ellos.

Más información aquí: https://es.stackoverflow.com/questions/406569/problema-al-obtener-chars-pasados-como-par%c3%a1metros-al-main-y-con-la-llamada-al-si/406572#406572

![Ejercicio2](CodigoEjercicio7_S3.jpeg)

![Ejercicio2](EjecucionEjercicio7_S3.jpeg)

**Llamada al sistema clone**

    #define _GNU_SOURCE
    #include <sched.h>
    int clone(int (*func) (void *), void *child_stack, int flags, void *func_arg,
    ...
    /* pid_t *ptid, struct user_desc *tls, pid_t *ctid */ );
    Retorna: si éxito, el PID del hijo; -1, si error

Cuando llamamos a clone se crea un nuevo proceso hijo que comienza ejecutando la función dada por func (primer parámetro) a la que se le pasa el argumento dado por func_arg(otro parámetro de clone). El hijo finaliza cuando la función indicada retorna o cuando haga un exit o _exit. También debemos pasar como argumento un puntero a la pila que debe utilizar el hijo, en el argumento child_stack y que previamente debemos haber reservado. Un apartado interesante de clone son los argumentos de clonación.

Más información aquí: http://manpages.ubuntu.com/manpages/bionic/es/man2/clone.2.html
