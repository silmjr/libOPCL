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

int locl_Initialize_Platforms(){
	//"Zerando" index's
	listPlatforms[0] = -1;
	listPlatforms[1] = -1;
	listPlatforms[2] = -1;
	listPlatforms[3] = -1;
	listPlatforms[4] = -1;
	listPlatforms[5] = -1;
	listPlatforms[6] = -1;

	locl_DEVICE_CPU = locl_DEVICE_ACCELERATOR = locl_DEVICE_GPU = -1;
	
	locl_INIT = 1;
	
	int i, j;	
	size_t buffer_size;	
	char *aux_name;
	
	//Numero de plataformas disponíveis
	cl_int status = clGetPlatformIDs(0, NULL, &locl_NUM_PLATFORMS);
	listPlatforms[locl_ALL] = locl_NUM_PLATFORMS;
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL locl_PLATFORMS available.\n");
		exit(EXIT_FAILURE);
	}	
	
	
	locl_PLATFORMS = malloc(sizeof(cl_platform_id)*locl_NUM_PLATFORMS);
	
	locl_DispPlats = malloc(sizeof(plataforms)*locl_NUM_PLATFORMS);
	
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
	
		locl_DispPlats[i].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, locl_DispPlats[i].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		aux_name = DiscStr(locl_DispPlats[i].Vendor); 
		
		if(isEqual(aux_name,"Intel(R)"))
			listPlatforms[locl_INTEL] = i;
			
		if(isEqual(aux_name,"Intel"))
			listPlatforms[locl_INTEL_GPU] = i;

		if(isEqual(aux_name,"The"))
			listPlatforms[locl_POCL] = i;
			
		if(isEqual(aux_name,"AMD"))
			listPlatforms[locl_AMD] = i;

		if(isEqual(aux_name,"NVIDIA"))
			listPlatforms[locl_NVIDIA] = i;

		if(isEqual(aux_name,"Brown"))
			listPlatforms[locl_COPRTHR] = i;


		
	}
	return 0;		
}

int locl_Initialize_Device(int locl_PLATFORM_NUM){
	int j;
	size_t buffer_size;	
	cl_int status;
	cl_device_type aux_type;
	locl_INIT_DEVICE = 1;

	if(listPlatforms[locl_PLATFORM_NUM] > locl_NUM_PLATFORMS || listPlatforms[locl_PLATFORM_NUM] == -1 )
		return 3;
	else{	
		
		//INIT DEVICES
		status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}
		
		for (j = 0; j < locl_NUM_DEVICES; j++){
			//aux_type = NULL;
			locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
			status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}

			status = clGetDeviceInfo(locl_DEVICES[j], CL_DEVICE_TYPE, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			aux_type = *(cl_device_type*) malloc(buffer_size);
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

int locl_PrintInfo(int locl_PLATAFORM_NUMBER){
    if(locl_INIT != 1)
		return 1;
	
	int i, j;	
	devices aux; /*Estrutura auxiliar de Devices*/
	char *aux_name;
	size_t buffer_size;	

	if(listPlatforms[locl_PLATAFORM_NUMBER] == -1){
		return 2;
	}else if(listPlatforms[locl_PLATAFORM_NUMBER] == locl_NUM_PLATFORMS){ //If the user pass the number of platforms, list all available 
		for (i = 0; i < locl_NUM_PLATFORMS; i++){
			printf("Platform Number: %d\n", locl_DispPlats[i].numPlat);	
			printf("Name: %s\n", locl_DispPlats[i].Name);
			printf("Vendor: %s\n", locl_DispPlats[i].Vendor);
			printf("Version: %s\n", locl_DispPlats[i].Version);
			
			// END PLATFORMS
			//INIT locl_DEVICES
			cl_int status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
				exit(EXIT_FAILURE);
			}

			for (j = 0; j < locl_NUM_DEVICES; j++)
			{
				printf("Device Number: %d\n", locl_DispPlats[i].MyDevices[j].numDevice);	
				locl_ListDevice(&locl_DispPlats[i], locl_DEVICES[j], 1);
			}	
		}
	}else{

		/* Imprimir informações especificas */		
		
		printf("Platform Number: %d\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].numPlat);		
		printf("Name: %s\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Name);
		printf("Vendor: %s\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Vendor);
		printf("Version: %s\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Version);
	
		// END PLATFORMS

		//INIT locl_DEVICES
		cl_int status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}

		for (j = 0; j < locl_NUM_DEVICES; j++)
		{
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}
			printf("Device Number: %d\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].MyDevices[j].numDevice);	

			locl_ListDevice(&locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]], locl_DEVICES[j], 1);
			
		}

	}
	return 0;
}

int locl_Explore(int locl_PLATAFORM_NUMBER){

	if(locl_INIT != 1)
		return 1;
	
	int i, j;	
	char *aux_name;
	size_t buffer_size;	

	if(listPlatforms[locl_PLATAFORM_NUMBER] == -1){
		return 2;
	}else if(listPlatforms[locl_PLATAFORM_NUMBER] == locl_NUM_PLATFORMS){ //If the user pass the number of platforms, list all available 
		for (i = 0; i < locl_NUM_PLATFORMS; i++){
			locl_DispPlats[i].numPlat = i;	
			
			//Discover Name	
			cl_int status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			locl_DispPlats[i].Name = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_NAME, buffer_size, locl_DispPlats[i].Name, NULL);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			
			//Discover Vendor
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			locl_DispPlats[i].Vendor = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, locl_DispPlats[i].Vendor, NULL);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);

			//Discover Version
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			locl_DispPlats[i].Version = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_VERSION, buffer_size, locl_DispPlats[i].Version, NULL);

			// END PLATFORMS
			//INIT locl_DEVICES
			status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
				exit(EXIT_FAILURE);
			}
			locl_DispPlats[i].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
			for (j = 0; j < locl_NUM_DEVICES; j++)
			{
				devices aux; /*Estrutura auxiliar de Devices*/

				locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
				status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
				if (status != CL_SUCCESS)
				{
					printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
					exit(EXIT_FAILURE);
				}
				
				locl_ListDevice(&locl_DispPlats[i], locl_DEVICES[j], 0);
				locl_DispPlats[i].MyDevices[j].numDevice = j;
				
				//clfreeDevice(&aux);

			}
		}
	}else{

		/* Imprimir informações especificas */		
		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].numPlat = listPlatforms[locl_PLATAFORM_NUMBER];

		//Discover Name	
		cl_int status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Name = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_NAME, buffer_size, locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Name, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	
		//Discover Vendor
		status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_VENDOR, buffer_size, locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

	
		//Discover Version
		status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Version = malloc(buffer_size);
		status = clGetPlatformInfo(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_PLATFORM_VERSION, buffer_size, locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Version, NULL);

	
		// END PLATFORMS

		//INIT locl_DEVICES
		status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}

		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
		for (j = 0; j < locl_NUM_DEVICES; j++)
		{
			devices aux; /*Estrutura auxiliar de Devices*/

			locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
			status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}

			locl_ListDevice(&locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]], locl_DEVICES[j], 0);
			locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].MyDevices[j].numDevice = j;
		}

	}
	return 0;
}

int locl_ListDevice(plataforms *X, cl_device_id device, int tipo){
	if(locl_INIT != 1)
		return 1;
	
	
	size_t buffer_size;	
	//Name
	int aux;
	if(tipo == 0){	
		cl_int status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Name = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_NAME, buffer_size, X->MyDevices->Name, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		
		//End Name
		
		//Vendor
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Vendor = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, buffer_size, X->MyDevices->Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);


		//End Vendor

		//Type
		status = clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		
		X->MyDevices->Type = *(cl_device_type*) malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_TYPE, buffer_size, &X->MyDevices->Type, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		
		
		//printf("%s\n", X->MyDevices->Type);
		//End Type

		//Version
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Version = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, buffer_size, X->MyDevices->Version, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		
		//End Version
		
		//AdressSpace
		status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->AdressSpace = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, buffer_size, &X->MyDevices->AdressSpace, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		
		//End AdressSpace
		
		//Endian
		status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->LitleEndian = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, buffer_size, X->MyDevices->LitleEndian, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		
		//printf("%\n", X->MyDevices->LitleEndian);
		//End Endian
		
		//Buffer
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxBufferSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, buffer_size, &X->MyDevices->MaxBufferSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//Sizes
		
		//End Size 
		
		//End Buffer
		
		//MaxMemAlocSize
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE	, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxMemAlocSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE	, buffer_size, &X->MyDevices->MaxMemAlocSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

				
		//End MaxMemAlocSize

		//MaxParamSize;	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxParamSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, buffer_size, &X->MyDevices->MaxParamSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		
		//End MaxParamSize;

		//MaxGlobalMemSize;	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxGlobalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, buffer_size, &X->MyDevices->MaxGlobalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//printf("%\n", X->MyDevices->MaxGlobalMemSize);
		

		//End MaxGlobalMemSize;

		//MaxGlobalMemCacheSize	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxGlobalMemCacheSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, buffer_size, &X->MyDevices->MaxGlobalMemCacheSize, NULL);
		
		//End MaxGlobalMemCacheSize

		//MaxLocalMemSize	
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxLocalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, buffer_size, &X->MyDevices->MaxLocalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		

		//printf("%\n", X->MyDevices->MaxLocalMemSize);
		//End MaxLocalMemSize
		
		//MemType	
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MemType = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, buffer_size, X->MyDevices->MemType, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//printf("%\n", X->MyDevices->MemType);
		//End MemType
		
		//MaxWorkItemDimensions	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkItemDimensions = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, buffer_size, &X->MyDevices->MaxWorkItemDimensions, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//printf("%\n", X->MyDevices->MaxWorkItemDimensions);	
		//End MaxWorkItemDimensions		

		//MaxWorkItem	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkItem = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MyDevices->MaxWorkItem, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		//printf("%d\n", X->MyDevices->MaxWorkItem);
		//End MaxWorkItem
			
		//MaxWorkGroupSize	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkGroupSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, buffer_size, &X->MyDevices->MaxWorkGroupSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		//printf("%\n", X->MyDevices->MaxWorkGroupSize);	
		//End MaxWorkGroupSize
		
		//MaxWorkItemSizes	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkItemSizes = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MyDevices->MaxWorkItemSizes, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
	}
	else if(tipo == 1){
		if(locl_INIT != 1)
		return 1;
		
		size_t buffer_size;	
		//Name
				
		printf("Name: %s\n", X->MyDevices->Name);
		//End Name
		
		//Vendor
		printf("Vendor: %s\n", X->MyDevices->Vendor);
		//End Vendor

		//Type
				
		if(X->MyDevices->Type & CL_DEVICE_TYPE_GPU)
			printf("Type: GPU\n");

		if(X->MyDevices->Type & CL_DEVICE_TYPE_CPU)
			printf("Type: CPU\n");
		
		if(X->MyDevices->Type & CL_DEVICE_TYPE_ACCELERATOR)
			printf("Type: ACCELERATOR\n");

		//printf("%s\n", X->MyDevices->Type);
		//End Type

		//Version
		printf("Version: %s\n", X->MyDevices->Version);
		//End Version
		
		//AdressSpace
		printf("Address space size: %u\n", X->MyDevices->AdressSpace);
		//End AdressSpace
		
		//Endian
		
		if(X->MyDevices->LitleEndian)
			printf("Litlle Endian: Yes\n");
		else	
			printf("Litlle Endian:  No\n");
		//printf("%\n", X->MyDevices->LitleEndian);
		//End Endian
		
		//Buffer
		
		//Sizes
		if (X->MyDevices->MaxBufferSize > 1023 && X->MyDevices->MaxBufferSize < 1048576)
			printf("Max Buffer Size: %llu KB\n", (int)X->MyDevices->MaxBufferSize/1024);
		else if (X->MyDevices->MaxBufferSize > 1048576 && X->MyDevices->MaxBufferSize < 1073741824)
			printf("Max Buffer Size: %llu MB\n", (int)X->MyDevices->MaxBufferSize/1048576);
		else if (X->MyDevices->MaxBufferSize > 1073741824)
			printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxBufferSize/1073741824, (long int)((long int)X->MyDevices->MaxBufferSize%1073741824)/1048576);
		//End Size 
		
		//End Buffer
		
		//MaxMemAlocSize
		
		if (X->MyDevices->MaxMemAlocSize > 1023 && X->MyDevices->MaxMemAlocSize < 1048576)
			printf("Max Buffer Size: %llu KB\n", (int)X->MyDevices->MaxMemAlocSize/1024);
		else if (X->MyDevices->MaxMemAlocSize> 1048576 && X->MyDevices->MaxMemAlocSize < 1073741824)
			printf("Max Buffer Size: %llu MB\n", (int)X->MyDevices->MaxMemAlocSize/1048576);
		else if (X->MyDevices->MaxMemAlocSize > 1073741824)
			printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxMemAlocSize/1073741824, (long int)((long int)X->MyDevices->MaxMemAlocSize%1073741824)/1048576);

		
		//End MaxMemAlocSize

		//MaxParamSize;	
		
		printf("Max Parameter size: %zu\n", X->MyDevices->MaxParamSize);
		//End MaxParamSize;

		//MaxGlobalMemSize;	
		
		//printf("%\n", *X->MyDevices->MaxGlobalMemSize);
		if (X->MyDevices->MaxGlobalMemSize > 1023 && X->MyDevices->MaxGlobalMemSize < 1048576)
			printf("Max Global Size: %llu KB\n", (int)X->MyDevices->MaxGlobalMemSize/1024);
		else if (X->MyDevices->MaxGlobalMemSize > 1048576 && X->MyDevices->MaxGlobalMemSize < 1073741824)
			printf("Max Global Size: %llu MB\n", (int)X->MyDevices->MaxGlobalMemSize/1048576);
		else if (X->MyDevices->MaxGlobalMemSize > 1073741824)
			printf("Max Global Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxGlobalMemSize/1073741824, (long int)((long int)X->MyDevices->MaxGlobalMemSize%1073741824)/1048576);

		//End MaxGlobalMemSize;

		//MaxGlobalMemCacheSize	
		
		if (X->MyDevices->MaxGlobalMemCacheSize> 1023 && X->MyDevices->MaxGlobalMemCacheSize < 1048576)
			printf("Max Cache Mem Size: %llu KB\n", (int)X->MyDevices->MaxGlobalMemCacheSize/1024);
		else if (X->MyDevices->MaxGlobalMemCacheSize> 1048576 && X->MyDevices->MaxGlobalMemCacheSize< 1073741824)
			printf("Max Cache Mem Size: %llu MB\n", (int)X->MyDevices->MaxGlobalMemCacheSize/1048576);
		else if (X->MyDevices->MaxGlobalMemCacheSize > 1073741824)
			printf("Max Cache Mem Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxGlobalMemCacheSize/1073741824, (long int)((long int)X->MyDevices->MaxGlobalMemCacheSize%1073741824)/1048576);

		//End MaxGlobalMemCacheSize

		//MaxLocalMemSize	
		
		if (X->MyDevices->MaxLocalMemSize> 1023 && X->MyDevices->MaxLocalMemSize < 1048576)
			printf("Max Local Mem Size: %llu KB\n", (int)X->MyDevices->MaxLocalMemSize/1024);
		else if (X->MyDevices->MaxLocalMemSize> 1048576 && X->MyDevices->MaxLocalMemSize< 1073741824)
			printf("Max Local Mem Size: %llu MB\n", (int)X->MyDevices->MaxLocalMemSize/1048576);
		else if (X->MyDevices->MaxLocalMemSize > 1073741824)
			printf("Max Lcaol Mem Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxLocalMemSize/1073741824, (long int)((long int)X->MyDevices->MaxLocalMemSize%1073741824)/1048576);

		//printf("%\n", X->MyDevices->MaxLocalMemSize);
		//End MaxLocalMemSize
		
		//MemType	
		//printf("%\n", X->MyDevices->MemType);
		//End MemType
		
		//MaxWorkItemDimensions	
		printf("Max Work Item Dimensions: %zu\n", X->MyDevices->MaxWorkItemDimensions);		
		//printf("%\n", X->MyDevices->MaxWorkItemDimensions);	
		//End MaxWorkItemDimensions		

		//MaxWorkItem	
		
		printf("Max Work Item: %zu\n", X->MyDevices->MaxWorkItem);	
		//printf("%d\n", X->MyDevices->MaxWorkItem);
		//End MaxWorkItem
				
		printf("Max Work Group Size: %zu\n", X->MyDevices->MaxWorkGroupSize);		
		//printf("%\n", X->MyDevices->MaxWorkGroupSize);	
		//End MaxWorkGroupSize
		
		//MaxWorkItemSizes	
		
		printf("Max Work Items Sizes: %zu\n", X->MyDevices->MaxWorkItemSizes);		
		//printf("%\n", X->MyDevices->MaxWorkItemSizes);	
		//End MaxWorkItemSizes
		printf("\n");

		}
		return 0;
}

void locl_Errors(int i){
	switch(i){
		//Mudar ordem de erros 3 e 4
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
	    
	locl_CMDQUEUE = clCreateCommandQueueWithProperties(locl_CONTEXT, locl_DEVICES[locl_DEVICE_NUMBER], 0, &status);

	if (status != CL_SUCCESS) {
	printf ("Unable create a command queue\n");
	exit(1);
	}
	return 0;
}

cl_mem locl_CreateBuffer(size_t locl_DATASIZE, cl_mem_flags locl_FLAGS, cl_bool locl_FLAG1, void *a ){
	cl_int status;	
	cl_mem bufferA;
	bufferA = clCreateBuffer(locl_CONTEXT, locl_FLAGS, locl_DATASIZE, NULL, &status);
	if (status != CL_SUCCESS) {
		printf ("Unable to create buffer for A\n");
		exit(1);
	}

	status = clEnqueueWriteBuffer(locl_CMDQUEUE, bufferA, locl_FLAG1, 0, locl_DATASIZE, a, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf ("Unable to copy A to buffer\n");
        exit(1);
    }

	return bufferA;
}

int locl_CreateProgram(const char** source_str){
	cl_int status;	

	locl_program = clCreateProgramWithSource(locl_CONTEXT, 1, source_str,  NULL, &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to create a program from source\n");
        exit(1);
    }
    
    // Build (compile) the program for the locl_DEVICES with
    // clBuildProgram()
    status = clBuildProgram(locl_program, locl_NUM_DEVICES, locl_DEVICES, NULL, NULL, NULL); 
    if (status != CL_SUCCESS) {
        printf ("Unable to build a program, %d\n", status);
        printf("CL_INVALID_PROGRAM %d\n", CL_INVALID_PROGRAM);
        printf("CL_INVALID_VALUE %d\n", CL_INVALID_VALUE);
        printf("CL_INVALID_VALUE %d\n", CL_INVALID_VALUE);
        printf("CL_INVALID_DEVICE %d\n", CL_INVALID_DEVICE);
        printf("CL_INVALID_BINARY %d\n", CL_INVALID_BINARY);
        printf("CL_INVALID_BUILD_OPTIONS %d\n", CL_INVALID_BUILD_OPTIONS);
        printf("CL_INVALID_OPERATION %d\n", CL_INVALID_OPERATION);
        printf("CL_COMPILER_NOT_AVAILABLE %d\n", CL_COMPILER_NOT_AVAILABLE);
        printf("CL_BUILD_PROGRAM_FAILURE %d\n", CL_BUILD_PROGRAM_FAILURE);
        printf("CL_INVALID_OPERATION %d\n", CL_INVALID_OPERATION);
        printf("CL_INVALID_OPERATION %d\n", CL_INVALID_OPERATION);
        printf("CL_OUT_OF_RESOURCES %d\n", CL_OUT_OF_RESOURCES);
        printf("CL_OUT_OF_HOST_MEMORY %d\n", CL_OUT_OF_HOST_MEMORY);
        char logBuffer[10240];
        clGetProgramBuildInfo(locl_program, locl_DEVICES[0], CL_PROGRAM_BUILD_LOG, sizeof(logBuffer), logBuffer, NULL);
        printf("CL Compilation failed:\n%s", logBuffer);
        exit(1);
    }
    return 0;
}


char *DiscStr(char *name){
	int i = 0;
	char aux[10];
	while(!isspace(name[i])){
	
		aux[i] = name [i];
		i++;
	}
	strcpy(name, aux)
;	return name;
}

int isEqual(char *name, char *name2){
	int i = 0;
	while(i < strlen(name2)){
		if(name[i] != name2[i] || strlen(name2) != strlen(name))
			return 0;
		i++;
	}
		return 1;

// Mudar de alfabeto para caracteres
}

int locl_Init(int locl_PLATFORM_NUM){
  	int error;
  	error = locl_Initialize_Platforms();
        locl_Errors(error);
    if(locl_PLATFORM_NUM < locl_ALL)
    	error = locl_Initialize_Device(locl_PLATFORM_NUM);  
    locl_Errors(error);
    	error = locl_Explore( locl_PLATFORM_NUM);

    return 0;
}


int locl_Finalize(){
	free(locl_DispPlats);
}

int freeDevice(devices *x){
	free(x->Name);	
	free(x->Vendor);	
	free(x->Version);	
	free(x->AdressSpace);	
	free(x->LitleEndian);	
	free(x->MaxBufferSize);	
	free(x->MaxMemAlocSize);	
	free(x->MaxParamSize);	
	free(x->MaxGlobalMemSize);	
	free(x->MaxGlobalMemCacheSize);	
	free(x->MaxLocalMemSize);	
	free(x->MemType);	
	free(x->MaxWorkItem);	
	free(x->MaxWorkGroupSize);
	free(x->MaxWorkItemDimensions);
	free(x->MaxWorkItemSizes);
}