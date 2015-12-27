# Makefile pour la compilation des tests de la librairie Rijndael
#
# Chemin des répertoires utiles pour la compilation
VPATH = ../../MPArithmC_C++/librairiesC32-64:../librairiesRijndael
# CFLAGS pour la compilation
CFLAGS32 = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../librairiesRijndael -DARCH32
CFLAGS32DB = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../librairiesRijndael -DARCH32 -DSECURE
CFLAGS64 = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../librairiesRijndael -DARCH64
CFLAGS64DB = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../librairiesRijndael -DARCH64 -DSECURE
OBJETSPROG = libmparithm32.o rijndael32.o
OBJETSPROGDB = libmparithm32.do rijndael32.do
OBJETSPROG64 = libmparithm64.o rijndael64.o
OBJETSPROG64DB = libmparithm64.do rijndael64.do
# Création et lancement du test
testrijndael.32: testrijndael.c rijndael.h libmparithm.h $(OBJETSPROG)
	gcc $(CFLAGS32) -o testrijndael.32 testrijndael.c $(OBJETSPROG)
	./testrijndael.32
# Création et lancement du test API
testrijndael-api.32: testrijndael.c rijndael.h libmparithm.h rijndael-api.h $(OBJETSPROG) rijndael-api32.o
	gcc $(CFLAGS32) -o testrijndael-api.32 testrijndael-api.c $(OBJETSPROG) rijndael-api32.o
	./testrijndael-api.32
# Création du fichier objet libarithm.o
libmparithm32.o: libmparithm.c libmparithm.h
	gcc $(CFLAGS32) -o $@ -c $<
# Création du fichier objet rijndael.o
rijndael32.o: rijndael.c rijndael.h
	gcc $(CFLAGS32) -o $@ -c $<
# Création du fichier objet rijndael-api.o
rijndael-api32.o: rijndael-api.c rijndael-api.h rijndael.h
	gcc $(CFLAGS32) -o $@ -c $<
clean.32:
	rm *.o
	rm *.32
# Création et lancement du test de débogage
testrijndael.32.db: testrijndael.c rijndael.h libmparithm.h $(OBJETSPROGDB)
	gcc $(CFLAGS32DB) -o testrijndael.32.db testrijndael.c $(OBJETSPROGDB)
	./testrijndael.32.db
# Création et lancement du test API
testrijndael-api.32.db: testrijndael.c rijndael.h libmparithm.h rijndael-api.h $(OBJETSPROGDB) rijndael-api32.do
	gcc $(CFLAGS32DB) -o testrijndael-api.32.db testrijndael-api.c $(OBJETSPROGDB) rijndael-api32.do
	./testrijndael-api.32.db
# Création du fichier objet libarithm.do
libmparithm32.do: libmparithm.c libmparithm.h
	gcc $(CFLAGS32DB) -o $@ -c $<
# Création du fichier objet rijndael.o
rijndael32.do: rijndael.c rijndael.h
	gcc $(CFLAGS32DB) -o $@ -c $<
# Création du fichier objet rijndael-api.o
rijndael-api32.do: rijndael-api.c rijndael-api.h rijndael.h
	gcc $(CFLAGS32DB) -o $@ -c $<
clean.32.db:
	rm *.do
	rm *.32.db
# Création et lancement du test
testrijndael.64: testrijndael.c rijndael.h libmparithm.h $(OBJETSPROG64)
	gcc $(CFLAGS64) -o testrijndael.64 testrijndael.c $(OBJETSPROG64)
	./testrijndael.64
# Création et lancement du test API
testrijndael-api.64: testrijndael.c rijndael.h libmparithm.h rijndael-api.h $(OBJETSPROG64) rijndael-api64.o
	gcc $(CFLAGS64) -o testrijndael-api.64 testrijndael-api.c $(OBJETSPROG64) rijndael-api64.o
	./testrijndael-api.64
# Création du fichier objet libarithm.o
libmparithm64.o: libmparithm.c libmparithm.h
	gcc $(CFLAGS64) -o $@ -c $<
# Création du fichier objet rijndael.o
rijndael64.o: rijndael.c rijndael.h
	gcc $(CFLAGS64) -o $@ -c $<
# Création du fichier objet rijndael-api.o
rijndael-api64.o: rijndael-api.c rijndael-api.h rijndael.h
	gcc $(CFLAGS64) -o $@ -c $<
clean.64:
	rm *.o
	rm *.64
# Création et lancement du test de débogage
testrijndael.64.db: testrijndael.c rijndael.h libmparithm.h $(OBJETSPROG64DB)
	gcc $(CFLAGS64DB) -o testrijndael.64.db testrijndael.c $(OBJETSPROG64DB)
	./testrijndael.64.db
# Création et lancement du test API
testrijndael-api.64.db: testrijndael.c rijndael.h libmparithm.h rijndael-api.h $(OBJETSPROG64DB) rijndael-api64.do
	gcc $(CFLAGS64DB) -o testrijndael-api.64.db testrijndael-api.c $(OBJETSPROG64DB) rijndael-api64.do
	./testrijndael-api.64.db
# Création du fichier objet libarithm.do
libmparithm64.do: libmparithm.c libmparithm.h
	gcc $(CFLAGS64DB) -o $@ -c $<
# Création du fichier objet rijndael.o
rijndael64.do: rijndael.c rijndael.h
	gcc $(CFLAGS64DB) -o $@ -c $<
# Création du fichier objet rijndael-api.o
rijndael-api64.do: rijndael-api.c rijndael-api.h rijndael.h
	gcc $(CFLAGS64DB) -o $@ -c $<
clean.64.db:
	rm *.do
	rm *.64.db