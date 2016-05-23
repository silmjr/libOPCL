all: libOCL

libOCL: libOCL.o
	gcc -o libOCL libOCL.o -lm -lOpenCL

libOCL.o: libOCL.c
	gcc -c libOCL.c 

clean:
	rm -rf libOCL.o libOCL