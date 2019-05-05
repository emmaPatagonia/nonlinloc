# nonlinloc
INSTRUCCIONES INSTALACION NONLINLOC:
------------------------------------

- Copiar la carpeta NLL/ a /usr/local/
- Copiar el SeisGram2K70.jar a /usr/share/java/

- Agregar al bashrc:
# NonLinLoc.
export PATH=/usr/local/NLL/bin:$PATH
export CLASSPATH=/usr/share/java/SeisGram2K70.jar

* Si aun no aparecen los comandos en el $PATH, cambiarle los permisos con chmod


-------------------------
INSTRUCCIONES DE USO:

- Crear una carpeta del evento a analizar.
- Dentro de la carpeta del evento, crear: yyyy-mm-dd-hhmm (carpeta de datos) y 
mover ahi la carpeta LOC/ y el archivo execSG2KMOD2.py
- Dentro de LOC/ crear las carpetas time/ (Info tiempos de viaje), gridmodel/ 
(Grilla espacial) y final/ 

- Dentro de LOC/ editar nlloc.in:
TRANS  LAMBERT Clarke-1880   (LAT) (LON)  (esquina inferior izquierda de la grilla)
VGGRID (km hacia el este) (km hacia el norte) (km en prof)
LOCGRID   (km hacia el este) (km hacia el norte) (km en prof)


- Calculo de la grilla con:
$ Vel2Grid nlloc.in
$ Grid2Time nlloc.in (puede demorar mucho)

- Para usar:
python execSG2KMOD2.py
