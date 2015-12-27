#***********************************************************************
#*                                                                     *
#*    Script de test pour la librairie de fonctions d'arithm�tique     *
#*           arithm.c pour les grands entiers (4096 bits)              *
#*                                                                     *
#*                                                                     *
#*                             Pierre Paquay                           *
#*                                                                     *
#***********************************************************************

gcc -c -O2 -Wall -ansi arithm.c

# Test des fonctions arithm�tiques en C
gcc -O2 -Wall -ansi -o testarithmc testarithmc.c arithm.o
echo Compilation de testarithmc Final OK
# testarithmc

# Test des fonctions binaires en C
gcc -O2 -Wall -ansi -o testbinc testbinc.c arithm.o
echo Compilation de testbinc Final OK
# testbinc

# Test des fonctions de th�orie des nombres en C
gcc -O2 -Wall -ansi -o testnumthc testnumthc.c arithm.o
echo Compilation de testnumthc Final OK
# testnumthc

# Test des fonctions de nombres al�atoires en C
gcc -O2 -Wall -ansi -o testrandc testrandc.c arithm.o
echo Compilation de testrandc Final OK
# testrandc

# Test des fonctions de nombres premiers en C
gcc -O2 -Wall -ansi -o testprimec testprimec.c arithm.o
echo Compilation de testprimec Final OK
# testprimec

echo Test des fonctions arithm�tiques:
./testarithmc

echo Test des fonctions binaires:
./testbinc

echo Test des fonctions de th�orie des nombres:
./testnumthc

echo Test des fonction de nombres al�atoires:
./testrandc

echo Test des fonction de nombres premiers:
./testprimec