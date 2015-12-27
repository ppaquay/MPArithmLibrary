# Makefile pour la compilation des tests de la librairie C
#
# Chemin des répertoires utiles pour la compilation
VPATH = ../librairiesC32-64
# CFLAGS pour la compilation
CFLAGS = -m32 -O2 -Wall -ansi -I$(VPATH) -DARCH32
CFLAGS64 = -O2 -Wall -ansi -I$(VPATH) -DARCH64
CFLAGSDB64 = -O2 -Wall -ansi -I$(VPATH) -DARCH64 -DSECURE -DDEBUGSEC
CFLAGSDB = -m32 -O2 -Wall -ansi -I$(VPATH) -DARCH32 -DSECURE -DDEBUGSEC
OBJETSPROG = libmparithm32.o
OBJETSPROG64 = libmparithm64.o
OBJETSDEBOG = libmparithm32.do
OBJETSDEBOG64 = libmparithm64.do
# Création et lancement de tous les tests
tests.32: testarithmc.c testbinc.c testchinc.c testnumthc.c testprimec.c testrandc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o testarithmc.32 testarithmc.c $(OBJETSPROG)
	gcc $(CFLAGS) -o testbinc.32 testbinc.c $(OBJETSPROG)
	gcc $(CFLAGS) -o testchinc.32 testchinc.c $(OBJETSPROG)
	gcc $(CFLAGS) -o testnumthc.32 testnumthc.c $(OBJETSPROG)
	gcc $(CFLAGS) -o testprimec.32 testprimec.c $(OBJETSPROG)
	gcc $(CFLAGS) -o testrandc.32 testrandc.c $(OBJETSPROG)
	./testarithmc.32
	./testbinc.32
	./testchinc.32
	./testnumthc.32
	./testprimec.32
	./testrandc.32
# Création et lancement de l'exécutable testarithmc
testarithmc.32: testarithmc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testarithmc.c $(OBJETSPROG)
	./testarithmc.32
# Création et lancement de l'exécutable testbinc
testbinc.32: testbinc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testbinc.c $(OBJETSPROG)
	./testbinc.32
# Création et lancement de l'exécutable testchinc
testchinc.32: testchinc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testchinc.c $(OBJETSPROG)
	./testchinc.32
# Création et lancement de l'exécutable testnumthc
testnumthc.32: testnumthc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testnumthc.c $(OBJETSPROG)
	./testnumthc.32
# Création et lancement de l'exécutable testprimec
testprimec.32: testprimec.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testprimec.c $(OBJETSPROG)
	./testprimec.32
# Création et lancement de l'exécutable testrandc
testrandc.32: testrandc.c libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS) -o $@ testrandc.c $(OBJETSPROG)
	./testrandc.32
# Création du fichier objet libarithm.o
libmparithm32.o: libmparithm.c libmparithm.h
	gcc $(CFLAGS) -o $@ -c $<
clean.32:
	rm *.o
	rm *.32
# Création et lancement de tous les tests an mode debug
tests.32.db: testarithmc.c testbinc.c testchinc.c testnumthc.c testprimec.c testrandc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testarithmc.32.db testarithmc.c $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testbinc.32.db testbinc.c $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testchinc.32.db testchinc.c $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testnumthc.32.db testnumthc.c $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testprimec.32.db testprimec.c $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o testrandc.32.db testrandc.c $(OBJETSDEBOG)
	./testarithmc.32.db
	./testbinc.32.db
	./testchinc.32.db
	./testnumthc.32.db
	./testprimec.32.db
	./testrandc.32.db
# Création et lancement de l'exécutable testarithmc
testarithmc.32.db: testarithmc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testarithmc.c $(OBJETSDEBOG)
	./testarithmc.32.db
# Création et lancement de l'exécutable testbinc
testbinc.32.db: testbinc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testbinc.c $(OBJETSDEBOG)
	./testbinc.32.db
# Création et lancement de l'exécutable testchinc
testchinc.32.db: testchinc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testchinc.c $(OBJETSDEBOG)
	./testchinc.32.db
# Création et lancement de l'exécutable testnumthc
testnumthc.32.db: testnumthc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testnumthc.c $(OBJETSDEBOG)
	./testnumthc.32.db
# Création et lancement de l'exécutable testprimec
testprimec.32.db: testprimec.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testprimec.c $(OBJETSDEBOG)
	./testprimec.32.db
# Création et lancement de l'exécutable testrandc
testrandc.32.db: testrandc.c libmparithm.h $(OBJETSDEBOG)
	gcc $(CFLAGSDB) -o $@ testrandc.c $(OBJETSDEBOG)
	./testrandc.32.db
# Création du fichier objet libarithm.o
libmparithm32.do: libmparithm.c libmparithm.h
	gcc $(CFLAGSDB) -o $@ -c $<
clean.32.db:
	rm *.do
	rm *.db
# Création et lancement de tous les tests (64bits)
tests.64: testarithmc.c testbinc.c testchinc.c testnumthc.c testprimec.c testrandc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testarithmc.64 testarithmc.c $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testbinc.64 testbinc.c $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testchinc.64 testchinc.c $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testnumthc.64 testnumthc.c $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testprimec.64 testprimec.c $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testrandc.64 testrandc.c $(OBJETSPROG64)
	./testarithmc.64
	./testbinc.64
	./testchinc.64
	./testnumthc.64
	./testprimec.64
	./testrandc.64
# Création et lancement de l'exécutable testarithmc
testarithmc.64: testarithmc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testarithmc.c $(OBJETSPROG64)
	./testarithmc.64
# Création et lancement de l'exécutable testbinc
testbinc.64: testbinc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testbinc.c $(OBJETSPROG64)
	./testbinc.64
# Création et lancement de l'exécutable testchinc
testchinc.64: testchinc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testchinc.c $(OBJETSPROG64)
	./testchinc.64
# Création et lancement de l'exécutable testnumthc
testnumthc.64: testnumthc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testnumthc.c $(OBJETSPROG64)
	./testnumthc.64
# Création et lancement de l'exécutable testprimec
testprimec.64: testprimec.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testprimec.c $(OBJETSPROG64)
	./testprimec.64
# Création et lancement de l'exécutable testrandc
testrandc.64: testrandc.c libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o $@ testrandc.c $(OBJETSPROG64)
	./testrandc.64
# Création du fichier objet libarithm64.o
libmparithm64.o: libmparithm.c libmparithm.h
	gcc $(CFLAGS64) -o $@ -c $<
clean.64:
	rm *.o
	rm *.64
# Création et lancement de tous les tests an mode debug (64bits)
tests.64.db: testarithmc.c testbinc.c testchinc.c testnumthc.c testprimec.c testrandc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testarithmc.64.db testarithmc.c $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testbinc.64.db testbinc.c $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testchinc.64.db testchinc.c $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testnumthc.64.db testnumthc.c $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testprimec.64.db testprimec.c $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o testrandc.64.db testrandc.c $(OBJETSDEBOG64)
	./testarithmc.64.db
	./testbinc.64.db
	./testchinc.64.db
	./testnumthc.64.db
	./testprimec.64.db
	./testrandc.64.db
# Création et lancement de l'exécutable testarithmc
testarithmc.64.db: testarithmc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testarithmc.c $(OBJETSDEBOG64)
	./testarithmc.64.db
# Création et lancement de l'exécutable testbinc
testbinc.64.db: testbinc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testbinc.c $(OBJETSDEBOG64)
	./testbinc.64.db
# Création et lancement de l'exécutable testchinc
testchinc.64.db: testchinc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testchinc.c $(OBJETSDEBOG64)
	./testchinc.64.db
# Création et lancement de l'exécutable testnumthc
testnumthc.64.db: testnumthc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testnumthc.c $(OBJETSDEBOG64)
	./testnumthc.64.db
# Création et lancement de l'exécutable testprimec
testprimec.64.db: testprimec.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testprimec.c $(OBJETSDEBOG64)
	./testprimec.64.db
# Création et lancement de l'exécutable testrandc
testrandc.64.db: testrandc.c libmparithm.h $(OBJETSDEBOG64)
	gcc $(CFLAGSDB64) -o $@ testrandc.c $(OBJETSDEBOG64)
	./testrandc.64.db
# Création du fichier objet libarithm.o
libmparithm64.do: libmparithm.c libmparithm.h
	gcc $(CFLAGSDB64) -o $@ -c $<
clean.64.db:
	rm *.do
	rm *.db