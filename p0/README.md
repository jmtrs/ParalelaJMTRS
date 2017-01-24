
# Practica 0 de COMPUTACIÓN PARALELA

## Para compilar:

```sh
    $ make
    $ ./tarea1
    $ ./tarea2
``` 

## Tarea1

Para la realización de esta tarea se hace uso de dos vectores con acceso mediante puntero
y una matriz de dimensión `9000 x 9000` con acceso mediante doble puntero.

Primero reservamos memoria para los dos vectores, y después para matriz primero todas las
filas y luego para cada fila el número de columnas que tiene.

Asignamos valores a la matriz mediante un srand siendo valores comprendidos de `0` a `1`

Calculo iterativo: Para el valor de m introducido por el usuario se realizan m cálculos, en cada cálculo se calcula primero la matriz por el vector inicialmente unitario, el vector resultante se va actualizando al nuevo valor asignado tras calcular los alphas, para calcular los alphas y la norma 1 recorremos todo el vector haciendo:

```sh
v_aux = (alpha * v_aux) + ((1- alpha) * v) 
```

Por lo que volvemos a actualizar el vector v_aux, hacemos una resta de:

```sh
v_aux - v
```
Elevamos y vamos sumando todas las componentes del vector al cuadrado para sacar la norma 1.

## Tarea2 

Para la multiplicación de matriz por matriz se hace uso de:

- `matriz` (inicialmente con valores rand de entre 0 y 1)
- `matriz_2` (inicialmente con valores rand de entre 0 y 1)
- `matriz_resul` (resultante para el cálculo 1)
- `matriz_resul2` (resultante para el cálculo 2)

Para el cálculo en 5 pasos realizamos 5 bucles diferentes los cuales calculan 200 filas cada uno, el primero calcula las 200 primeras el segundo las 200 siguientes así sucesivamente hasta 1000.
Se realizan 3 bucles anidados para calcular cada una de las componentes de la matriz y asignarla en la posición correspondiente de la `matriz_resul`, se va sumando a cada posición de i-j la multiplicación de `matriz ik * matriz_2 kj`.

Para el cálculo en bloques de 100 filas, tenemos un bucle de 10 iteraciones y dentro de este bucle siendo s inicialmente 0 hasta s+100 por lo que siempre se calculas 100 filas más de s en cada iteración del bucle l, posteriormente se van asignando valores a la matriz y sumamos 100 a s para la nueva iteración.

## Tiempos 

- `Tarea 1` El resulta de está parte es el siguiente: ![N|Solid](http://i.imgur.com/PYZd0Xi.png) y el tiempo de ejecución es:

    ![N|Solid](http://i.imgur.com/N8Ao59A.png)
    
- `Tarea 2` El tiempo de está parte es el siguiente:
- 
    ![N|Solid](http://i.imgur.com/qOiQj5o.png)