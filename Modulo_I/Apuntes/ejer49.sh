#Nombre: cron_5.1
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

#minuto hora día-del-mes mes , día-de-la-semana orden
* * * * * /mnt/prueba.sh &> /mnt/fichero.log
* * * * * /mnt/ejer410.sh & >> /mnt/fichero2.log
* * * * * /mnt/ejer411.sh & >> /mnt/fichero3.log



