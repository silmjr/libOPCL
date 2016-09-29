all: 
	gcc -c libopcl.c -lOpenCL -fopenmp -lm -w -O3 -std=c99 -fpic
	gcc -o libopcl.so libopcl.o -shared -fpic
clean:
	rm -rf *.o
mrproper: clean
	rm -rf libopcl
install: 
	./install.sh 
uninstall: 
	./uninstall.sh 
