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

short int locl_INIT = 0, locl_INIT_DEVICE = 0;

/* Inicializa as variáveis que serão usadas como index e plataformas */
int lolc_Initialize_Platform(){
	//"Zerando" index's
	locl_AMD = locl_NVIDIA = locl_INTEL = locl_POCL = locl_ALL = locl_DEVICE_CPU = locl_DEVICE_ACCELERATOR = locl_DEVICE_GPU = -1;
	locl_INIT = 1;
	
	int i, j;	
	size_t buffer_size;	
	char *aux_name;
	
	//Numero de plataformas disponíveis
	cl_int status = clGetPlatformIDs(0, NULL, &locl_NUM_PLATFORMS);
	locl_ALL = locl_NUM_PLATFORMS;
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL locl_PLATFORMS available.\n");
		exit(EXIT_FAILURE);
	}	
	
	
	locl_PLATFORMS = malloc(sizeof(cl_platform_id)*locl_NUM_PLATFORMS);
	plataforms dispPlat[locl_NUM_PLATFORMS];
	status = clGetPlatformIDs(locl_NUM_PLATFORMS, locl_PLATFORMS, NULL);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the list of OpenCL locl_PLATFORMS.\n");
		exit(EXIT_FAILURE);
	}
	
	/*Varredura das plataformas disponíveis*/
	for(i=0; i<locl_NUM_PLATFORMS; i++){
		//Discover Vendor
		status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
		dispPlat[i].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, dispPlat[i].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		aux_name = DiscStr(dispPlat[i].Vendor); 
		
		if(isEqual(aux_name,"Intel"))
			locl_INTEL = i;

		if(isEqual(aux_name,"The"))
			locl_POCL = i;
		
		if(isEqual(aux_name,"AMD"))
			locl_AMD = i;
		
		
		if(isEqual(aux_name,"NVIDIA"))
			locl_NVIDIA = i;
	}		
}
int lolc_Initialize_Device(int locl_PLATFORM_NUM){
	int j;
	size_t buffer_size;	
	cl_int status;
	cl_device_type aux_type;
	locl_INIT_DEVICE = 1;

	if(locl_PLATFORM_NUM >= locl_NUM_PLATFORMS)
		return 3;
	else{	
		
		//INIT DEVICES
		status = clGetDeviceIDs(locl_PLATFORMS[locl_PLATFORM_NUM], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}
		
		for (j = 0; j < locl_NUM_DEVICES; j++){
			aux_type = NULL;
			locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
			status = clGetDeviceIDs(locl_PLATFORMS[locl_PLATFORM_NUM], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}

			status = clGetDeviceInfo(locl_DEVICES[j], CL_DEVICE_TYPE, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			aux_type = malloc(buffer_size);
			status = clGetDeviceInfo(locl_DEVICES[j], CL_DEVICE_TYPE, buffer_size, &aux_type, NULL);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			

			if (aux_type & CL_DEVICE_TYPE_GPU){
				locl_DEVICE_GPU = j;
			}

			if(aux_type & CL_DEVICE_TYPE_CPU){
				locl_DEVICE_CPU = j;
			}

			if(aux_type & CL_DEVICE_TYPE_ACCELERATOR){
				locl_DEVICE_ACCELERATOR = j;			
			}
		}
	}
	return 0;
}
int locl_Explore(int locl_PLATAFORM_NUMBER){
	
	if(locl_INIT != 1)
		return 1;
	
	int i, j;	
	devices aux; /*Estrutura auxiliar de Devices*/
	char *aux_name;
	size_t buffer_size;	

	plataforms *dispPlat = malloc(sizeof(plataforms)*locl_NUM_PLATFORMS);// Vector of avalaible locl_PLATFORMS 
	
	if(locl_PLATAFORM_NUMBER == -1){
		return 2;
	}else if(locl_PLATAFORM_NUMBER == locl_NUM_PLATFORMS){ //If the user pass the number of platforms, list all available 
	
		for (i = 0; i < locl_NUM_PLATFORMS; i++){

			dispPlat[i].numPlat = i;	
			printf("Platform Number: %d\n", dispPlat[i].numPlat);	

			//Discover Name	
			cl_int status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			dispPlat[i].Name = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_NAME, buffer_size, dispPlat[i].Name, NULL);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			printf("Name: %s\n", dispPlat[i].Name);
		
			//Discover Vendor
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			dispPlat[i].Vendor = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, dispPlat[i].Vendor, NULL);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			printf("Vendor: %s\n", dispPlat[i].Vendor);
		
			//Discover Version
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			dispPlat[i].Version = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VERSION, buffer_size, dispPlat[i].Version, NULL);

			printf("Version: %s\n", dispPlat[i].Version);
		
			
			// END PLATFORMS
			//INIT locl_DEVICES
			status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
				exit(EXIT_FAILURE);
			}

			dispPlat[i].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
			for (j = 0; j < locl_NUM_DEVICES; j++)
			{
				locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
				status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
				if (status != CL_SUCCESS)
				{
					printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
					exit(EXIT_FAILURE);
				}
				dispPlat[i].MyDevices[j].numDevice = j;
				printf("Device Number: %d\n", dispPlat[i].MyDevices[j].numDevice);	

				locl_ListDevice(&aux, locl_DEVICES[j]);
				dispPlat[i].MyDevices[j] = aux;
			}
		}
	}else{

		/* Imprimir informações especificas */		
		dispPlat[locl_PLATAFORM_NUMBER].numPlat = locl_PLATAFORM_NUMBER;

		printf("Platform Number: %d\n", dispPlat[locl_PLATAFORM_NUMBER].numPlat);	
		//Discover Name	
		cl_int status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		dispPlat[locl_PLATAFORM_NUMBER].Name = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_NAME, buffer_size, dispPlat[locl_PLATAFORM_NUMBER].Name, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		printf("Name: %s\n", dispPlat[locl_PLATAFORM_NUMBER].Name);
	
		//Discover Vendor
		status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		dispPlat[locl_PLATAFORM_NUMBER].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_VENDOR, buffer_size, dispPlat[locl_PLATAFORM_NUMBER].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		printf("Vendor: %s\n", dispPlat[locl_PLATAFORM_NUMBER].Vendor);
	
		//Discover Version
		status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		dispPlat[locl_PLATAFORM_NUMBER].Version = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_PLATFORM_VERSION, buffer_size, dispPlat[locl_PLATAFORM_NUMBER].Version, NULL);

		printf("Version: %s\n", dispPlat[locl_PLATAFORM_NUMBER].Version);
	
		// END PLATFORMS

		//INIT locl_DEVICES
		status = clGetDeviceIDs(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}

		dispPlat[locl_PLATAFORM_NUMBER].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
		for (j = 0; j < locl_NUM_DEVICES; j++)
		{
			locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
			status = clGetDeviceIDs(locl_PLATFORMS[locl_PLATAFORM_NUMBER], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}
			dispPlat[locl_PLATAFORM_NUMBER].MyDevices[j].numDevice = j;
			printf("Device Number: %d\n", dispPlat[locl_PLATAFORM_NUMBER].MyDevices[j].numDevice);	

			locl_ListDevice(&aux, locl_DEVICES[j]);
			dispPlat[locl_PLATAFORM_NUMBER].MyDevices[j] = aux;
		}

	}
	return 0;
}

int locl_ListDevice(devices *X, cl_device_id device){
	if(locl_INIT != 1)
		return 1;
	
	
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
	status = clGetDeviceInfo(device, CL_DEVICE_TYPE, buffer_size, &X->Type, NULL);
	if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	
	if(X->Type & CL_DEVICE_TYPE_GPU)
		printf("Type: GPU\n");

	if(X->Type & CL_DEVICE_TYPE_CPU)
		printf("Type: CPU\n");
	
	if(X->Type & CL_DEVICE_TYPE_ACCELERATOR)
		printf("Type: ACCELERATOR\n");

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
	return 0;

	
}

void locl_Errors(int i){
	switch(i){
		case 0:
		break;
		case 1:
			printf("OpenCL Platforms is not initialized \n");
			exit(1);
		break;
		case 2:
			printf("The platform doesn't exist\n");
			exit(1);
		break;
		
		case 3:
			printf ("The device doens't exist\n");
			exit(1);
		break;
		case 4:
			printf ("OpenCL Devices is not initialized\n");
			exit(1);
		break;


	}
}

int locl_CreateCmdQueue(int locl_DEVICE_NUMBER){
	if(locl_INIT_DEVICE != 1)
		return 4;	
	cl_int status;
	locl_CONTEXT = clCreateContext(NULL, locl_NUM_DEVICES, locl_DEVICES, NULL, NULL, &status);
	
	if(locl_DEVICE_NUMBER > locl_NUM_DEVICES){
		return 3;
		exit(1);
	}
	if (status != CL_SUCCESS) {
		printf ("Unable create a context\n");
		exit(1);
	}
	    
	locl_CMDQUEUE = clCreateCommandQueue(locl_CONTEXT, locl_DEVICES[locl_DEVICE_NUMBER], 0, &status);

	if (status != CL_SUCCESS) {
	printf ("Unable create a command queue\n");
	exit(1);
	}
	return 0;
}

cl_mem locl_CreateBuffer(size_t locl_DATASIZE, cl_mem_flags locl_FLAGS ){
	if(locl_INIT_DEVICE != 1)
		return 4;
	if(locl_INIT != 1)
		return 1;	
			
	cl_int status;	
	cl_mem bufferA;
	bufferA = clCreateBuffer(locl_CONTEXT, locl_FLAGS, locl_DATASIZE, NULL, &status);
	if (status != CL_SUCCESS) {
		printf ("Unable to create buffer for A\n");
		exit(1);
	}
	return bufferA;
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
