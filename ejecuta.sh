#!/bin/bash

helpFunction()
{
   echo ""
   echo "Uso: $0 -c NumCores -n NombreCodigo"
   echo -e "\t-c Numero de cors a ejecutar"
   echo -e "\t-n Nombre del codigo in extension"
   exit 1 
}

while getopts "c:n:" opt
do
   case "$opt" in
      c ) parameterC="$OPTARG" ;;
      n ) parameterN="$OPTARG" ;;
      ? ) helpFunction ;; 
   esac
done


if [ -z "$parameterC" ] || [ -z "$parameterN" ]
then
   echo "Faltan Parametros por agregar";
   helpFunction
fi

mpicc -o $parameterN $parameterN.c
#mpirun -np $parameterC ./$parameterN
./$parameterN

python grafica.py
