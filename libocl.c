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

int p_AMD = -1 , p_Nvidia = -1 , p_Intel = -1 , p_Pocl = -1 ;
void DiscPlatforNumber(){
	int i;	
	size_t buffer_size;	
	//Discover Plataforms
	cl_int status;
	cl_uint num_platforms;
	char *aux_name;
	
	status = clGetPlatformIDs(0, NULL, &num_platforms);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL platforms available.\n");
		exit(EXIT_FAILURE);
	}

	cl_platform_id platforms[num_platforms];
	plataforms dispPlat[num_platforms];// Vector of avalaible platforms 
	status = clGetPlatformIDs(num_platforms, platforms, NULL);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the list of OpenCL platforms.\n");
		exit(EXIT_FAILURE);
	}
	
	for(i=0; i<num_platforms; i++){
		//Discover Vendor
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
		dispPlat[i].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, buffer_size, dispPlat[i].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		aux_name = DiscStr(dispPlat[i].Vendor); 
		
		if(isEqual(aux_name,"Intel")){
			p_Intel = i;
			printf("------------- %d Intel -------------	\n", p_Intel);
		}

		if(isEqual(aux_name,"The")){
			p_Pocl = i;
			printf("------------- %d POCL -------------\n", p_Pocl);
		}

		if(isEqual(aux_name,"AMD")){
			p_AMD = i;
			printf("------------- %d AMD-------------\n", p_AMD);
		}

		if(isEqual(aux_name,"NVIDIA")){
			p_Nvidia = i;
			printf("------------- %d NVIDIA -------------\n ", p_Nvidia);
		}
	
	}

}

void listPlatform(){

	int i, j;	
	//size_t buffer_size;	
	devices aux;
	//Discover plataforms
	cl_int status;
	cl_uint num_platforms;
	char *aux_name;
	size_t buffer_size;	

	status = clGetPlatformIDs(0, NULL, &num_platforms);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL platforms available.\n");
		exit(EXIT_FAILURE);
	}

	cl_platform_id platforms[num_platforms];
	plataforms dispPlat[num_platforms];// Vector of avalaible platforms 
	status = clGetPlatformIDs(num_platforms, platforms, NULL);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the list of OpenCL platforms.\n");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < num_platforms; i++){
		
		dispPlat[i].numPlat = i;

		printf("Platform Number: %d\n", dispPlat[i].numPlat);	
		//Discover Name	
		cl_int status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		dispPlat[i].Name = malloc(buffer_size);
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, buffer_size, dispPlat[i].Name, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		printf("Name: %s\n", dispPlat[i].Name);
		
		//Discover Vendor
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		dispPlat[i].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, buffer_size, dispPlat[i].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		printf("Vendor: %s\n", dispPlat[i].Vendor);
		
		//Discover Version
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		dispPlat[i].Version = malloc(buffer_size);
		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, buffer_size, dispPlat[i].Version, NULL);

		printf("Version: %s\n", dispPlat[i].Version);
		
		// END PLATFORMS

		//INIT devices
		cl_uint num_devices;
		status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL devices available on this platform.\n");
			exit(EXIT_FAILURE);
		}
	
		dispPlat[i].MyDevices = malloc(sizeof(devices)*num_devices);
		for (j = 0; j < num_devices; j++)
		{
			cl_device_id devices[num_devices];
			status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL devices.\n");
				exit(EXIT_FAILURE);
			}
			dispPlat[i].MyDevices[j].numDevice = j;
			printf("Device Number: %d\n", dispPlat[i].MyDevices[j].numDevice);	
			listDevice(&aux, devices[j]);
			dispPlat[i].MyDevices[j] = aux;
		}
	}
	
}

void listDevice(devices *X, cl_device_id device){
	size_t buffer_size;	
	//Name
	int aux;
	cl_int status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->Name = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_NAME, buffer_size, X->Name, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
	printf("Name: %s\n", X->Name);
	//End Name
	
	//Vendor
	status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->Vendor = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, buffer_size, X->Vendor, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	printf("Vendor: %s\n", X->Vendor);
	//End Vendor

	//Type
	status = clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
	X->Type = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_TYPE, buffer_size, X->Type, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
	//printf("%s\n", X->Type);
	//End Type

	//Version
	status = clGetDeviceInfo(device, CL_DEVICE_VERSION, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->Version = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_VERSION, buffer_size, X->Version, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	printf("Version: %s\n", X->Version);
	//End Version
	
	//AdressSpace
	status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->AdressSpace = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, buffer_size, &X->AdressSpace, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	printf("Address space size: %u\n", X->AdressSpace);
	//End AdressSpace
	
	//Endian
	status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->LitleEndian = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, buffer_size, X->LitleEndian, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	if(X->LitleEndian)
		printf("Litlle Endian: Yes\n");
	else	
		printf("Litlle Endian:  No\n");
	//printf("%\n", X->LitleEndian);
	//End Endian
	
	//Buffer
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxBufferSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, buffer_size, &X->MaxBufferSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	//Sizes
	if (X->MaxBufferSize > 1023 && X->MaxBufferSize < 1048576)
		printf("Max Buffer Size: %llu KB\n", (int)X->MaxBufferSize/1024);
	else if (X->MaxBufferSize > 1048576 && X->MaxBufferSize < 1073741824)
		printf("Max Buffer Size: %llu MB\n", (int)X->MaxBufferSize/1048576);
	else if (X->MaxBufferSize > 1073741824)
		printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MaxBufferSize/1073741824, (long int)((long int)X->MaxBufferSize%1073741824)/1048576);
	//End Size 
	
	//End Buffer
	
	//MaxMemAlocSize
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE	, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxMemAlocSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE	, buffer_size, &X->MaxMemAlocSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	if (X->MaxMemAlocSize > 1023 && X->MaxMemAlocSize < 1048576)
		printf("Max Buffer Size: %llu KB\n", (int)X->MaxMemAlocSize/1024);
	else if (X->MaxMemAlocSize> 1048576 && X->MaxMemAlocSize < 1073741824)
		printf("Max Buffer Size: %llu MB\n", (int)X->MaxMemAlocSize/1048576);
	else if (X->MaxMemAlocSize > 1073741824)
		printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MaxMemAlocSize/1073741824, (long int)((long int)X->MaxMemAlocSize%1073741824)/1048576);

	
	//End MaxMemAlocSize

	//MaxParamSize;	
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxParamSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, buffer_size, &X->MaxParamSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	printf("Max Parameter size: %zu\n", X->MaxParamSize);
	//End MaxParamSize;

	//MaxGlobalMemSize;	
	status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxGlobalMemSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, buffer_size, &X->MaxGlobalMemSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	//printf("%\n", X->MaxGlobalMemSize);
	if (X->MaxGlobalMemSize > 1023 && X->MaxGlobalMemSize < 1048576)
		printf("Max Global Size: %llu KB\n", (int)X->MaxGlobalMemSize/1024);
	else if (X->MaxGlobalMemSize > 1048576 && X->MaxGlobalMemSize < 1073741824)
		printf("Max Global Size: %llu MB\n", (int)X->MaxGlobalMemSize/1048576);
	else if (X->MaxGlobalMemSize > 1073741824)
		printf("Max Global Size: %llu GB %llu MB\n", (long int)X->MaxGlobalMemSize/1073741824, (long int)((long int)X->MaxGlobalMemSize%1073741824)/1048576);

	//End MaxGlobalMemSize;

	//MaxGlobalMemCacheSize	
	status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxGlobalMemCacheSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, buffer_size, &X->MaxGlobalMemCacheSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	if (X->MaxGlobalMemCacheSize> 1023 && X->MaxGlobalMemCacheSize < 1048576)
		printf("Max Cache Mem Size: %llu KB\n", (int)X->MaxGlobalMemCacheSize/1024);
	else if (X->MaxGlobalMemCacheSize> 1048576 && X->MaxGlobalMemCacheSize< 1073741824)
		printf("Max Cache Mem Size: %llu MB\n", (int)X->MaxGlobalMemCacheSize/1048576);
	else if (X->MaxGlobalMemCacheSize > 1073741824)
		printf("Max Cache Mem Size: %llu GB %llu MB\n", (long int)X->MaxGlobalMemCacheSize/1073741824, (long int)((long int)X->MaxGlobalMemCacheSize%1073741824)/1048576);

	//End MaxGlobalMemCacheSize

	//MaxLocalMemSize	
	status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxLocalMemSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, buffer_size, &X->MaxLocalMemSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	if (X->MaxLocalMemSize> 1023 && X->MaxLocalMemSize < 1048576)
		printf("Max Local Mem Size: %llu KB\n", (int)X->MaxLocalMemSize/1024);
	else if (X->MaxLocalMemSize> 1048576 && X->MaxLocalMemSize< 1073741824)
		printf("Max Local Mem Size: %llu MB\n", (int)X->MaxLocalMemSize/1048576);
	else if (X->MaxLocalMemSize > 1073741824)
		printf("Max Lcaol Mem Size: %llu GB %llu MB\n", (long int)X->MaxLocalMemSize/1073741824, (long int)((long int)X->MaxLocalMemSize%1073741824)/1048576);

	//printf("%\n", X->MaxLocalMemSize);
	//End MaxLocalMemSize
	
	//MemType	
	status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MemType = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, buffer_size, X->MemType, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	//printf("%\n", X->MemType);
	//End MemType
	
	//MaxWorkItemDimensions	
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxWorkItemDimensions = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, buffer_size, &X->MaxWorkItemDimensions, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	printf("Max Work Item Dimensions: %zu\n", X->MaxWorkItemDimensions);		
	//printf("%\n", X->MaxWorkItemDimensions);	
	//End MaxWorkItemDimensions		

	//MaxWorkItem	
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxWorkItem = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MaxWorkItem, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	printf("Max Work Item: %zu\n", X->MaxWorkItem);	
	//printf("%d\n", X->MaxWorkItem);
	//End MaxWorkItem
		
	//MaxWorkGroupSize	
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxWorkGroupSize = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, buffer_size, &X->MaxWorkGroupSize, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	printf("Max Work Group Size: %zu\n", X->MaxWorkGroupSize);		
	//printf("%\n", X->MaxWorkGroupSize);	
	//End MaxWorkGroupSize
	
	//MaxWorkItemSizes	
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	X->MaxWorkItemSizes = malloc(buffer_size);
	status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MaxWorkItemSizes, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	printf("Max Work Items Sizes: %zu\n", X->MaxWorkItemSizes);		
	//printf("%\n", X->MaxWorkItemSizes);	
	//End MaxWorkItemSizes
	printf("\n");
	
}

char *DiscStr(char *name){
	int i = 0;
	char aux[10];
	while(isalpha(name[i])){
		aux[i] = name [i];
		i++;
	}
	strcpy(name, aux)
;	return name;
}

int isEqual(char *name, char *name2){
	int i = 0;
	while(isalpha(name[i])){
		if(name[i] != name2[i])
			return 0;
		i++;
	}
		return 1;

}