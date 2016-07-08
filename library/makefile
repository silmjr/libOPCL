all: libocl
libocl: libocl.o 
        # O compilador faz a ligação entre os dois objetos
	gcc -o mmulti libocl.o -lOpenCL -fopenmp -O3
	
#-----> Distancia com o botão TAB ### e não com espaços
libocl.o: libocl.c
	gcc -o libocl.o -c libocl.c -w -lOpenCL -fopenmp -O3 
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl


