#!/bin/bash

chmod +x Makefile

#clear

SEGUIR="s";


while [ $SEGUIR == "s" ]
do
	# Elegir que ejercicio se quiere ejecutar
	clear
	echo "Ingrese el numero de acuerdo al inciso que desea ejecutar:"
	echo
	echo "(1) 1.1.1 - Banco con hilos (requiere ser detenido con CTRL C)"
	echo "(2) 1.1.1 - Banco con procesos (requiere ser detenido con CTRL C)"
	echo "(3) 1.1.2 - MiniShell"
	echo "(4) 1.2.1 - Secuencia 1 (ABABC) con hilos (requiere ser detenido con CTRL C)"
	echo "(5) 1.2.1 - Secuencia 1 (ABABC) con procesos (requiere ser detenido con CTRL C)"
	echo "(6) 1.2.1 - Secuencia 2 (ABABCABCD) con hilos (requiere ser detenido con CTRL C)"
	echo "(7) 1.2.1 - Secuencia 2 (ABABCABCD) con procesos (requiere ser detenido con CTRL C)"  
	echo "(8) 1.2.2 - Aulas implementado con hilos (requiere ser detenido con CTRL C)"
	echo "(9) 1.2.2 - Aulas implementado con procesos (requiere ser detenido con CTRL C)"
	
	read nro_inciso

	#Ejecutar el inciso elegido
	if [ "$nro_inciso" == 1 ]
	then
		echo "-----------------------"
		echo "1.1.1 - Banco con hilos"
		echo "-----------------------"
		echo 
		cd 1.1Banco
		gcc -o Banco Banco.c -pthread && clear && ./Banco #compilar y ejecutar
		cd .. #volver a la carpeta original
	else 
	if [ "$nro_inciso" == 2 ] 
	then
		echo "--------------------------"
		echo "1.1.1 - Banco con procesos"
		echo "--------------------------"
		echo 
		cd 1.1Banco
		gcc -o BancoP BancoProcesos.c -pthread && clear && ./BancoP #compilar y ejecutar
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 3 ]
	then
		echo "---------"
		echo "MiniShell"
		echo "---------"
		echo 
		cd 1.1MiniShell
		gcc -o help help.c
		gcc -o mkdir mkdir.c
		gcc -o rmdir rmdir.c
		gcc -o touch touch.c
		gcc -o ls ls.c
		gcc -o cat cat.c
		gcc -o chmod chmod.c
		gcc -o clear clear.c
		gcc -o exit exit.c
		gcc -o MiniShell MiniShell.c && clear && ./MiniShell #compilar y ejecutar
		cd ..
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 4 ]
	then
		echo "-----------------------------------"
		echo "1.2.1 Secuencia 1 (ABABC) con hilos"
		echo "-----------------------------------"
		echo 
		cd 1.2Secuencia
		cd ABABC
		gcc -o Secuencia1 ABABC.c -pthread && clear && ./Secuencia1 #compilar y ejecutar
		cd ..
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 5 ]
	then
		echo "--------------------------------------"
		echo "1.2.1 Secuencia 1 (ABABC) con procesos"
		echo "--------------------------------------"
		echo 
		cd 1.2Secuencia
		cd ABABC
		gcc -o Secuencia2 ABABCPipes.c -pthread && clear && ./Secuencia2 #compilar y ejecutar
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 6 ]
	then
		echo "---------------------------------------"
		echo "1.2.1 Secuencia 2 (ABABCABCD) con hilos"
		echo "---------------------------------------"
		echo 
		cd 1.2Secuencia
		cd ABABCABCD
		gcc -o Secuencia3 ABABCABCD.c -pthread && clear && ./Secuencia3 #compilar y ejecutar
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 7 ] 
	then
		echo "------------------------------------------"
		echo "1.2.1 Secuencia 2 (ABABCABCD) con procesos"
		echo "------------------------------------------"
		echo 
		cd 1.2Secuencia
		cd ABABCABCD
		gcc -o Secuencia4 ABABCABCDPipes.c && clear && ./Secuencia4 #compilar y ejecutar
		cd ..
		cd .. #volver a la carpeta original
	else
	if [ "$nro_inciso" == 8 ] 
	then
		echo "-------------------------------------"
		echo "1.2.2 - Aulas implementado con hilos "
		echo "-------------------------------------"
		echo 
		cd 1.2Aulas
		gcc -o AulasH aulas.c -pthread && clear && ./AulasH #compilar y ejecutar
		cd ..
	else
	if [ "$nro_inciso" == 9 ] 
	then
		echo "-------------------------------------"
		echo "1.2.2 - Aulas implementado con procesos "
		echo "-------------------------------------"
		echo 
		cd 1.2Aulas
		gcc -o AulasP aulasProcesos.c -pthread && clear && ./AulasP #compilar y ejecutar
		cd ..
		
	fi #finalizar if's
	fi
	fi
	fi
	fi
	fi
	fi
	fi
	fi
	

	echo
	echo "Desea ejecutar otro inciso? (s) / (n) "
	echo
	read SEGUIR

done

$SHELL #para evitar que se cierre la terminal


