 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include "libopcl.h"

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

int main(int argc, char *argv[]){	
	
   	lopcl_Init(lopcl_ALL, -1);
	lopcl_PrintInfo(lopcl_ALL);
	lopcl_Finalize();

}           
                    
