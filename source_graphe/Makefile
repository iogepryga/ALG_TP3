all: test_graphe

test_graphe: test_graphe.o graphe.o io_graphe.o file.o
	gcc -g -o test_graphe test_graphe.o file.o graphe.o io_graphe.o

graphe.o: graphe.c graphe.h
	gcc -Wall -g -c graphe.c

io_graphe.o: io_graphe.c graphe.h
	gcc -Wall -g -c io_graphe.c

file.o: file.c file.h
	gcc -Wall -g -c file.c

test_graphe.o: test_graphe.c graphe.h
	gcc -Wall -g -c test_graphe.c

todot.o: todot.c graphe.h
	gcc -Wall -g -c todot.c

todot: todot.o io_graphe.o graphe.o file.o
	gcc -Wall -g -o todot todot.o io_graphe.o graphe.o file.o

%.png: todot
	./todot data/$* > data/$*.dot
	dot -Tpng data/$*.dot -o data/$@
	xdg-open data/$@
	rm data/$*.dot

clean:
	rm -f *.o test_graphe todot *~ data/*~ data/*.dot
