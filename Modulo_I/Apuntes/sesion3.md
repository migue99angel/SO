**Actividad 3.1** 

    Consulta de las estadísticas del sistema
    En esta pregunta se pide el estudio de las ordenes w y uptime.
    uptime muestra la primera línea del resultado del comando w donde se muestra
    la hora actual, cuanto tiempo lleva el ordenador encendido, cuantos usuarios 
    están logueados en el sistema y las cargas medias para los pasados 1,5 y 15 minutos.



![Solucion](Ejercicio3_1.jpeg)

    En los tres casos aparece en la cabecera del resultado de w por lo que podemos usar tanto w como uptime.
    a) 6 minutos
    b) 1 usuario únicamente, root
    c) 0.00

**Actividad 3.2**

![script](Ejercicio32_script.jpeg)

    b)
    En la línea cuya orden es sh tenemos el proceso que corresponde al script bash lanzado, vemos que se le asigna una prioridad de 25

    En cuanto a la reasignación de prioridades, renice ha funcionado correctamente con la sintaxis de los ejemplos, renice 14 idProceso. No he conseguido hacer funcionar nice.
![Resultado](resultadoTop.jpeg)

c)

![Resultado](resultadoTime.jpeg)


**Actividad 3.3**


b)

![Resultado](Ejercicio3_3.jpeg)

    Los procesos cuyo PPID es ? significa que no tienen asignado ningún terminal en concreto.

**Actividad 3.4**

    a) Se usa mpsat
    b) Se usa mpsat
    c) Se usa top



Un dato a tener en cuenta es que free utiliza menos recursos que top.


**Actividad 3.5**

![Resultado](Ejercicio3_5.jpeg)




**Actividad 3.7**

![Resultado](Ejercicio3_7.jpeg)




**Actividad 3.8**

![Resultado](Ejercicio3_8.jpeg)

**Actividad 3.9**

    a) 
        du /


![Resultado](Ejercicio3_9_a.jpeg)


        du -h /


![Resultado](Ejercicio3_9_a2.jpeg)

    b)
        df -i /
![Resultado](Ejercicio3_9_b.jpeg)

    c)
        du -h /<nombredirectorio>

![Resultado](Ejercicio3_9_c.jpeg)
    
    d)
        du -B 4 /etc


![Resultado](Ejercicio3_9_d.jpeg)


**Actividad 3.10**

    a) ln -s archivo.txt ./softlink
    b) ln archivo.txt hardlink
    c) ln target_hardLink2.txt hardlink2

    Porque los enlaces blandos no cuentan para el contador, solo los duros


![Resultado](Ejercicio3_10.jpeg)


**Actividad 3.11**

    a) ls -laiL

![Resultado](Ejercicio3_11a.jpeg)

    b) ls -laid

![Resultado](Ejercicio3_11b.jpeg)


**Actividad 3.12**


![Resultado](Ejercicio3_12.jpeg)