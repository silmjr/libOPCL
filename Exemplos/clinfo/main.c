
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
	
	lolc_Initialize();
	locl_Explore(locl_ALL);

}
