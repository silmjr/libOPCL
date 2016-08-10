all: clinfo
clinfo: main.o 
	gcc -o clinfo main.o   ../../library/libocl.o -lOpenCL -fopenmp -O3

main.o: main.c
	gcc -c main.c -I ../../library/ -lOpenCL -fopenmp -O3 
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl
