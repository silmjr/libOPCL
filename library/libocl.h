#ifndef LIBOCL_H
#define LIBOCL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

int locl_AMD, locl_NVIDIA, locl_INTEL, locl_POCL, locl_ALL, locl_DEVICE_CPU, locl_DEVICE_ACCELERATOR, locl_DEVICE_GPU;
cl_uint locl_NUM_PLATFORMS;
cl_uint locl_NUM_DEVICES;
cl_platform_id *locl_PLATFORMS;
cl_device_id *locl_DEVICES;

cl_context locl_CONTEXT;
cl_command_queue locl_CMDQUEUE;


typedef struct{
	int numDevice;
	char *Name;	
	char *Vendor;	
	cl_device_type Type;	
	char *Version;	
	cl_uint *AdressSpace;	
	cl_bool *LitleEndian;	
	cl_ulong *MaxBufferSize;	
	cl_ulong *MaxMemAlocSize;	
	size_t *MaxParamSize;	
	cl_ulong *MaxGlobalMemSize;	
	cl_ulong *MaxGlobalMemCacheSize;	
	cl_ulong *MaxLocalMemSize;	
	cl_device_local_mem_type *MemType;	
	size_t *MaxWorkItem;	
	size_t *MaxWorkGroupSize;
	cl_uint *MaxWorkItemDimensions;
	size_t *MaxWorkItemSizes;

}devices;

typedef struct{
	int numPlat;
	char *Name;	
	char *Vendor;	
	char *Version;
	devices *MyDevices;	
}plataforms;

//Discover and list Platforms and Devices
/*Função de inicialização de plataformas, necessária para todas aplicações, aqui será feita a varredura de todas
as plataformas disponíveis e vão ser criados index's para deixa o acesso mais simples.*/
int lolc_Initialize_Platform();
/*Essa função inicializa e cria index's de devices de uma plataforma específica. Recebe o número ou index da plataforma desejada*/
int lolc_Initialize_Device(int locl_PLATFORM_NUM);

/*Essa função cria estruturas com informações de todas as plataformas disponíveis no computador, listando as suas
características. Recebe o número ou index da plataforma desejada. */
int locl_Explore(int locl_PLATAFORM_NUMBER);
/*Essa função cria a fila de comando e o contexto usando um device especificado*/
int locl_CreateCmdQueue(int locl_DEVICE_NUM);
/*Usada somente internamente pela locl_explore*/
int locl_ListDevice(devices *X, cl_device_id device);
/*cria os buffers e enqueueWritre dos dados que serão enviados aos devices, recebe o tamanho e o tipo dos buffers*/
cl_mem locl_CreateBuffer(size_t locl_DATASIZE, cl_mem_flags locl_FLAGS, cl_bool locl_FLAG1, void *a);

// Table of all errors 
void  locl_Errors(int i);

//Aux Functions
char *DiscStr(char *name);
int isEqual(char *name, char *name2);

#endif
