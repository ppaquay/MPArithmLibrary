# Makefile pour la compilation des tests de la librairie C++
#
# Chemin des répertoires utiles pour la compilation
VPATH = ../librairiesC32-64:../librairiesCPP32-64
# CFLAGS pour la compilation
CFLAGS32 = -m32 -O2 -Wall -ansi -I../librairiesC32-64 -DARCH32
CXXFLAGS32 = -m32 -O2 -Wall -ansi -I../librairiesC32-64 -I../librairiesCPP32-64 -DARCH32
CFLAGSDB32 = -m32 -O2 -Wall -ansi -I../librairiesC32-64 -DSECURE -DDEBUGSEC -DARCH32
CXXFLAGSDB32 = -m32 -O2 -Wall -ansi -I../librairiesC32-64 -I../librairiesCPP32-64 -DSECURE -DDEBUGSEC -DLEXCEPT -DARCH32
CFLAGS64 = -O2 -Wall -ansi -I../librairiesC32-64 -DARCH64
CXXFLAGS64 = -O2 -Wall -ansi -I../librairiesC32-64 -I../librairiesCPP32-64 -DARCH64
CFLAGSDB64 = -O2 -Wall -ansi -I../librairiesC32-64 -DSECURE -DDEBUGSEC -DARCH64
CXXFLAGSDB64 = -O2 -Wall -ansi -I../librairiesC32-64 -I../librairiesCPP32-64 -DSECURE -DDEBUGSEC -DLEXCEPT -DARCH64
OBJETSPROG = libmparithm32.o libmparithm++32.o
OBJETSDEBOG = libmparithm32.do libmparithm++32.do
OBJETSPROG64 = libmparithm64.o libmparithm++64.o
OBJETSDEBOG64 = libmparithm64.do libmparithm++64.do
# Création et lancement de tous les tests
tests.32: testarithmcpp.cpp testconstcpp.cpp testnumthcpp.cpp libmparithm++.hpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o testarithmcpp.32 testarithmcpp.cpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o testconstcpp.32 testconstcpp.cpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o testnumthcpp.32 testnumthcpp.cpp $(OBJETSPROG)
	./testarithmcpp.32
	./testconstcpp.32
	./testnumthcpp.32
# Création et lancement de l'exécutable testarithmcpp
testarithmcpp.32: testarithmcpp.cpp libmparithm++.hpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o $@ testarithmcpp.cpp $(OBJETSPROG)
	./testarithmcpp.32
# Création et lancement de l'exécutable testconstcpp
testconstcpp.32: testconstcpp.cpp libmparithm++.hpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o $@ testconstcpp.cpp $(OBJETSPROG)
	./testconstcpp.32
# Création et lancement de l'exécutable testnumthcpp
testnumthcpp.32: testnumthcpp.cpp libmparithm++.hpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o $@ testnumthcpp.cpp $(OBJETSPROG)
	./testnumthcpp.32
# Création du fichier objet libarithm.o
libmparithm32.o: libmparithm.c libmparithm.h
	g++ $(CFLAGS32) -o $@ -c $<
# Création du fichier objet libarithmcpp.o
libmparithm++32.o: libmparithm++.cpp libmparithm++.hpp libmparithm32.o
	g++ $(CXXFLAGS32) -o $@ -c $<
clean.32:
	rm *.o
	rm *.32
# Création et lancement de tous les tests
tests.32.db: testarithmcpp.cpp testconstcpp.cpp testnumthcpp.cpp libmparithm++.hpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o testarithmcpp.32.db testarithmcpp.cpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o testconstcpp.32.db testconstcpp.cpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o testnumthcpp.32.db testnumthcpp.cpp $(OBJETSDEBOG)
	./testarithmcpp.32.db
	./testconstcpp.32.db
	./testnumthcpp.32.db
# Création et lancement de l'exécutable testarithmcpp
testarithmcpp.32.db: testarithmcpp.cpp libmparithm++.hpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o $@ testarithmcpp.cpp $(OBJETSDEBOG)
	./testarithmcpp.32.db
# Création et lancement de l'exécutable testconstcpp
testconstcpp.32.db: testconstcpp.cpp libmparithm++.hpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o $@ testconstcpp.cpp $(OBJETSDEBOG)
	./testconstcpp.32.db
# Création et lancement de l'exécutable testnumthcpp
testnumthcpp.32.db: testnumthcpp.cpp libmparithm++.hpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o $@ testnumthcpp.cpp $(OBJETSDEBOG)
	./testnumthcpp.32.db
# Création du fichier objet libarithm.do
libmparithm32.do: libmparithm.c libmparithm.h
	g++ $(CFLAGSDB32) -o $@ -c $<
# Création du fichier objet libarithmcpp.do
libmparithm++32.do: libmparithm++.cpp libmparithm++.hpp libmparithm32.do
	g++ $(CXXFLAGSDB32) -o $@ -c $<
clean.32.db:
	rm *.do
	rm *.32.db
# Création et lancement de tous les tests
tests.64: testarithmcpp.cpp testconstcpp.cpp testnumthcpp.cpp libmparithm++.hpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o testarithmcpp.64 testarithmcpp.cpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o testconstcpp.64 testconstcpp.cpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o testnumthcpp.64 testnumthcpp.cpp $(OBJETSPROG64)
	./testarithmcpp.64
	./testconstcpp.64
	./testnumthcpp.64
# Création et lancement de l'exécutable testarithmcpp
testarithmcpp.64: testarithmcpp.cpp libmparithm++.hpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o $@ testarithmcpp.cpp $(OBJETSPROG64)
	./testarithmcpp.64
# Création et lancement de l'exécutable testconstcpp
testconstcpp.64: testconstcpp.cpp libmparithm++.hpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o $@ testconstcpp.cpp $(OBJETSPROG64)
	./testconstcpp.64
# Création et lancement de l'exécutable testnumthcpp
testnumthcpp.64: testnumthcpp.cpp libmparithm++.hpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o $@ testnumthcpp.cpp $(OBJETSPROG64)
	./testnumthcpp.64
# Création du fichier objet libarithm.o
libmparithm64.o: libmparithm.c libmparithm.h
	g++ $(CFLAGS64) -o $@ -c $<
# Création du fichier objet libarithmcpp.o
libmparithm++64.o: libmparithm++.cpp libmparithm++.hpp libmparithm64.o
	g++ $(CXXFLAGS64) -o $@ -c $<
clean.64:
	rm *.o
	rm *.64
tests.64.db: testarithmcpp.cpp testconstcpp.cpp testnumthcpp.cpp libmparithm++.hpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o testarithmcpp.64.db testarithmcpp.cpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o testconstcpp.64.db testconstcpp.cpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o testnumthcpp.64.db testnumthcpp.cpp $(OBJETSDEBOG64)
	./testarithmcpp.64.db
	./testconstcpp.64.db
	./testnumthcpp.64.db
# Création et lancement de l'exécutable testarithmcpp
testarithmcpp.64.db: testarithmcpp.cpp libmparithm++.hpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o $@ testarithmcpp.cpp $(OBJETSPROG64)
	./testarithmcpp.64.db
# Création et lancement de l'exécutable testconstcpp
testconstcpp.64.db: testconstcpp.cpp libmparithm++.hpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o $@ testconstcpp.cpp $(OBJETSDEBOG64)
	./testconstcpp.64.db
# Création et lancement de l'exécutable testnumthcpp
testnumthcpp.64.db: testnumthcpp.cpp libmparithm++.hpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o $@ testnumthcpp.cpp $(OBJETSDEBOG64)
	./testnumthcpp.64.db
# Création du fichier objet libarithm.o
libmparithm64.do: libmparithm.c libmparithm.h
	g++ $(CFLAGSDB64) -o $@ -c $<
# Création du fichier objet libarithmcpp.o
libmparithm++64.do: libmparithm++.cpp libmparithm++.hpp libmparithm64.o
	g++ $(CXXFLAGSDB64) -o $@ -c $<
clean.64.db:
	rm *.do
	rm *.64.db


