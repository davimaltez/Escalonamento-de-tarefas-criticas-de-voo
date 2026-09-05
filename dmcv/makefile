scheduler: main.o leitor.o simulador.o
	gcc main.o leitor.o simulador.o -o scheduler -lm

main.o: main.c leitor.h simulador.h
	gcc -c main.c

leitor.o: leitor.c leitor.h
	gcc -c leitor.c

simulador.o: simulador.c simulador.h
	gcc -c simulador.c

run: scheduler
	./scheduler

clean:
	rm -f *.o scheduler

.PHONY: run clean