
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
	//Descobrir e inicializar as plataformas e Devices
	//Segundo argumento -1 é usado quando se deseja descobrir todas plataformas disponíveis na máquina usada
   	lopcl_Init(lopcl_NVIDIA, lopcl_DEVICE_GPU);
   	//Imprime todas as informações disponíveis sobre a máquina usada
	lopcl_PrintInfo(lopcl_NVIDIA);
	//Libera os recursos usados durante a execução do programa
	lopcl_Finalize();

}
