all: mmulti
mmulti: main.o 
	gcc -o mmulti main.o   ../../library/libocl.o -lOpenCL -fopenmp -O3

main.o: main.c
	gcc -c main.c -I ../../library/ -w -lOpenCL -fopenmp -O3 
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl
