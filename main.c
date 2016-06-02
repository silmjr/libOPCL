
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include "libocl.h"

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif




int main(int argc, char *argv[]){	
	int i, j;	
	//size_t buffer_size;	
	//devices1 aux;
	//Discover plataforms1
	//cl_int status;
	//cl_uint num_platforms;
	//char *aux_name;

	DiscPlatforNumber();
	listPlatform();
}
