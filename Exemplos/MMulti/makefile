all: mmulti
mmulti: main.o 
	gcc -o mmulti main.o -lolc -lOpenCL -fopenmp -O3 -std=c99

main.o: main.c
	gcc -c main.c -I -locl -lOpenCL -fopenmp -O3 -std=c99
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl