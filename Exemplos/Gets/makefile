all: gets
gets: main.o 
	gcc -o gets main.o   ../../library/libocl.o -lOpenCL -fopenmp -O3 -std=c99

main.o: main.c
	gcc -c main.c -I ../../library/ -lOpenCL -fopenmp -O3 -std=c99
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl