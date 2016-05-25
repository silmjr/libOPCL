all: libOCL

libOCL: libOCL.o
	gcc -o libOCL libOCL.o -lm -lOpenCL -w 

libOCL.o: libOCL.c
	gcc -c libOCL.c -w 

clean:
	rm -rf libOCL.o libOCL