all: cptest cptest2

cptest: cptest.o
	gcc -g cptest.o -o cptest

cptest.o: cptest.c
	gcc -c -g cptest.c

cptest2: cptest2.o
	gcc -g cptest2.o -o cptest2

cptest2.o: cptest2.c
	gcc -c -g cptest2.c

clean:
	rm -f *.o cptest
	rm -f cptest2

docs:
	doxygen
	chmod a+r html/*
	cp -p html/* ~/public_html/cs2303assig6


