# TareaIA
Tarea de inteligencia artificial
Para ejecutar el codigo se requerira del compilador g++ o algun otro compilador compatible, a continuacion se indican los pasos para ejecutar el programa
#Paso 1: compilar
primero se requerira compilar con g++ como se indica a continuacion:

```
g++ main.cpp
```

#Paso 2: Ejecutar
a continuacion se podra ejecutar el programa para lo cual se requerira un archivo en txt y ademas se debera decidir con cual de los cuatro algoritmos
para encontrar la ruta entre los nodos seleccionados puede ser `--d para búsqueda en profundidad (escogiendo un sucesor al azar)`, `--u para 
búsqueda por costo uniforme`, `--g para busqueda greedy` y `--a para A*`:

```
./a.out < <nombre_del_archivo>.txt --<busqueda escogida>
```
#Ejemplos:

##Ejemplo de ejecucion:
A continuacion se muestra un ejemplo de ejecucion para el programa para un archivo `example2.txt` con `A*`
```
./a.out < example2.txt --a
```

##Ejemplo de archivo:

El siguiente es un ejemplo de archivo que corresponde al ejemplo incluido en el pdf
```
Init: A
Goal: H
A 10
B 7
C 10
D 3
E 1
F 2
G 2
H 0
A, C, 6
C, F, 3
F, G, 1
G, E, 4
C, E, 10
A, B, 11
B, D, 4
B, E, 3
D, H, 3
E, H, 5
```
