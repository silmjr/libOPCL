all: mmulti
mmulti: main.o 
	gcc -o mmulti main.o -lopcl -lOpenCL -fopenmp -O3 -std=c99

main.o: main.c
	gcc -c main.c -I -lopcl -lOpenCL -fopenmp -O3 -std=c99
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl