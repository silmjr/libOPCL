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

int locl_AMD, locl_NVIDIA, locl_INTEL, locl_POCL, locl_ALL;
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
	cl_device_type *Type;	
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
void lolc_Initialize(int locl_PLATAFORM_NUMBER);
void locl_Explore(int locl_PLATAFORM_NUMBER);
void locl_CreateCmdQueue(int locl_DEVICE_NUMBER);
void locl_ListDevice(devices *X, cl_device_id device);
// Table of all errors 
void  locl_Errors(int i);
//Aux Functions
char *DiscStr(char *name);
int isEqual(char *name, char *name2);

#endif