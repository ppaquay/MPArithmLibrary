# Makefile pour la compilation des tests de la librairie RSA
#
# Chemin des répertoires utiles pour la compilation
VPATH = ../../MPArithmC_C++/librairiesC32-64:../../MPArithmC_C++/librairiesCPP32-64:../librairiesHash:../librairiesRSA
# CFLAGS pour la compilation
CFLAGS32 = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -DARCH32
CXXFLAGS32 = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../../MPArithmC_C++/librairiesCPP32-64 -I../librairiesHash -I../librairiesRSA -DARCH32
CFLAGSDB32 = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -DSECURE -DDEBUGSEC -DARCH32
CXXFLAGSDB32 = -m32 -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../../MPArithmC_C++/librairiesCPP32-64 -I../librairiesHash -I../librairiesRSA -DSECURE -DDEBUGSEC -DLEXCEPT -DARCH32
CFLAGS64 = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -DARCH64
CXXFLAGS64 = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../../MPArithmC_C++/librairiesCPP32-64 -I../librairiesHash -I../librairiesRSA -DARCH64
CFLAGSDB64 = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -DSECURE -DDEBUGSEC -DARCH64
CXXFLAGSDB64 = -O2 -Wall -ansi -I../../MPArithmC_C++/librairiesC32-64 -I../../MPArithmC_C++/librairiesCPP32-64 -I../librairiesHash -I../librairiesRSA -DSECURE -DDEBUGSEC -DLEXCEPT -DARCH64
OBJETSPROG = libmparithm32.o ripemd32.o libmparithm++32.o librsa++32.o
OBJETSDEBOG = libmparithm32.do ripemd32.do libmparithm++32.do librsa++32.do
OBJETSPROG64 = libmparithm64.o ripemd64.o libmparithm++64.o librsa++64.o
OBJETSDEBOG64 = libmparithm64.do ripemd64.do libmparithm++64.do librsa++64.do
# Création et lancement du test
testrsa.32: testrsa.cpp librsa++.hpp $(OBJETSPROG)
	g++ $(CXXFLAGS32) -o testrsa.32 testrsa.cpp $(OBJETSPROG)
	./testrsa.32
# Création du fichier objet libarithm.o
libmparithm32.o: libmparithm.c libmparithm.h
	g++ $(CFLAGS32) -o $@ -c $<
# Création du fichier objet ripemd.o
ripemd32.o: ripemd.c ripemd.h libmparithm.h
	g++ $(CFLAGS32) -DLITTLE_ENDIAN -o $@ -c $<
# Création du fichier objet libarithmcpp.o
libmparithm++32.o: libmparithm++.cpp libmparithm++.hpp libmparithm32.o
	g++ $(CXXFLAGS32) -o $@ -c $<
librsa++32.o:librsa++.cpp librsa++.hpp libmparithm++32.o ripemd32.o
	g++ $(CXXFLAGS32) -o $@ -c $<
clean.32:
	rm *.o
	rm *.32
	rm rsa.*
# Création et lancement du test
testrsa.32.db: testrsa.cpp librsa++.hpp $(OBJETSDEBOG)
	g++ $(CXXFLAGSDB32) -o testrsa.32.db testrsa.cpp $(OBJETSDEBOG)
	./testrsa.32.db
# Création du fichier objet libarithm.do
libmparithm32.do: libmparithm.c libmparithm.h
	g++ $(CFLAGSDB32) -o $@ -c $<
# Création du fichier objet ripemd.do
ripemd32.do: ripemd.c ripemd.h libmparithm.h
	g++ $(CFLAGSDB32) -DLITTLE_ENDIAN -o $@ -c $<
# Création du fichier objet libarithmcpp.do
libmparithm++32.do: libmparithm++.cpp libmparithm++.hpp libmparithm32.do
	g++ $(CXXFLAGSDB32) -o $@ -c $<
librsa++32.do:librsa++.cpp librsa++.hpp libmparithm++32.do ripemd32.do
	g++ $(CXXFLAGSDB32) -o $@ -c $<
clean.32.db:
	rm *.do
	rm *.32.db
	rm rsa.*
# Création et lancement du test
testrsa.64: testrsa.cpp librsa++.hpp $(OBJETSPROG64)
	g++ $(CXXFLAGS64) -o testrsa.64 testrsa.cpp $(OBJETSPROG64)
	./testrsa.64
# Création du fichier objet libarithm.o
libmparithm64.o: libmparithm.c libmparithm.h
	g++ $(CFLAGS64) -o $@ -c $<
# Création du fichier objet ripemd.o
ripemd64.o: ripemd.c ripemd.h libmparithm.h
	g++ $(CFLAGS64) -DLITTLE_ENDIAN -o $@ -c $<
# Création du fichier objet libarithmcpp.o
libmparithm++64.o: libmparithm++.cpp libmparithm++.hpp libmparithm64.o
	g++ $(CXXFLAGS64) -o $@ -c $<
librsa++64.o:librsa++.cpp librsa++.hpp libmparithm++64.o ripemd64.o
	g++ $(CXXFLAGS64) -o $@ -c $<
clean.64:
	rm *.o
	rm *.64
	rm rsa.*
# Création et lancement du test
testrsa.64.db: testrsa.cpp librsa++.hpp $(OBJETSDEBOG64)
	g++ $(CXXFLAGSDB64) -o testrsa.64.db testrsa.cpp $(OBJETSDEBOG64)
	./testrsa.64.db
# Création du fichier objet libarithm.do
libmparithm64.do: libmparithm.c libmparithm.h
	g++ $(CFLAGSDB64) -o $@ -c $<
# Création du fichier objet ripemd.do
ripemd64.do: ripemd.c ripemd.h libmparithm.h
	g++ $(CFLAGSDB64) -DLITTLE_ENDIAN -o $@ -c $<
# Création du fichier objet libarithmcpp.do
libmparithm++64.do: libmparithm++.cpp libmparithm++.hpp libmparithm64.do
	g++ $(CXXFLAGSDB64) -o $@ -c $<
librsa++64.do:librsa++.cpp librsa++.hpp libmparithm++64.do ripemd64.do
	g++ $(CXXFLAGSDB64) -o $@ -c $<
clean.64.db:
	rm *.do
	rm *.64.db
	rm rsa.*
