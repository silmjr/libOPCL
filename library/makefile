all: 
	gcc -c libocl.c -lOpenCL -fopenmp -lm -w -O3 -std=c99 -fpic
	gcc -o libocl.so libocl.o -shared -fpic
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl