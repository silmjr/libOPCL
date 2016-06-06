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

int p_AMD, p_Nvidia, p_Intel, p_Pocl;

cl_uint num_platforms;

typedef struct 
{
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

typedef struct 
{
	int numPlat;
	char *Name;	
	char *Vendor;	
	char *Version;
	devices *MyDevices;	
}plataforms;

//Discover and list Platforms and Devices
void lolc_Initialize(void);
void locl_Explore(int locl_PLATAFORM_NUMBER);
void listDevice(devices *X, cl_device_id device);

//Aux String.
char *DiscStr(char *name);
int isEqual(char *name, char *name2);

#endif
