all: libocl
libocl: libocl.o main.o 
        # O compilador faz a ligação entre os dois objetos
	gcc -o libocl libocl.o main.o -lOpenCL
	
#-----> Distancia com o botão TAB ### e não com espaços
libocl.o: libocl.c
	gcc -o libocl.o -c libocl.c -w -lOpenCL
main.o: main.c libocl.h
	gcc -o main.o -c main.c -w -lOpenCL
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libocl
