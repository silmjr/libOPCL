all: libocl
libocl: libocl.o main.o 
        # O compilador faz a ligação entre os dois objetos
	gcc -o mmulti libocl.o main.o -lOpenCL -fopenmp -O3
	
#-----> Distancia com o botão TAB ### e não com espaços
libocl.o: libocl.c
	gcc -o libocl.o -c libocl.c -w -lOpenCL -fopenmp -O3 
main.o: main.c libocl.h
	gcc -o main.o -c main.c -w -lOpenCL -fopenmp -O3 
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl


