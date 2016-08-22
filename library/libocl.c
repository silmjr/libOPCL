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
	listPlatforms[0] = listPlatforms[1] = listPlatforms[2] = listPlatforms[3] = listPlatforms[4] = listPlatforms[5] = listPlatforms[6] = -1;
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

		if(isEqual(aux_name,"Brown")){
			listPlatforms[locl_COPRTHR] = i;
		}
		
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
		return 2;
	else{	
		
		//INIT DEVICES
		status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
		}
		
		locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);		

		status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
		if (status != CL_SUCCESS)
		{
			printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
			exit(EXIT_FAILURE);
		}
		
		for (j = 0; j < locl_NUM_DEVICES; j++){
			//aux_type = NULL;
		
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
			printf("Extensions:\n%s\n", locl_DispPlats[i].Extensions);
			
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
		printf("Extesions: %s\n", locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].Extensions);
	
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

			//Discover Extensions
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS) exit(EXIT_FAILURE);
			locl_DispPlats[i].Extensions = malloc(buffer_size);
			status = clGetPlatformInfo(locl_PLATFORMS[i], CL_PLATFORM_EXTENSIONS, buffer_size, locl_DispPlats[i].Extensions, NULL);
			
			// END PLATFORMS
			//INIT locl_DEVICES
			status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &locl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL locl_DEVICES available on this platform.\n");
				exit(EXIT_FAILURE);
			}

			locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
			status = clGetDeviceIDs(locl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
				exit(EXIT_FAILURE);
			}

			locl_DispPlats[i].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
			for (j = 0; j < locl_NUM_DEVICES; j++)
			{
				locl_DispPlats[i].MyDevices[j].numDevice = j;
				locl_ListDevice(&locl_DispPlats[i], locl_DEVICES[j], 0);
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

		locl_DEVICES = malloc(sizeof(cl_device_id)*locl_NUM_DEVICES);
		status = clGetDeviceIDs(locl_PLATFORMS[listPlatforms[locl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, locl_NUM_DEVICES, locl_DEVICES, NULL);
		if (status != CL_SUCCESS)
		{
			printf(" Cannot get the list of OpenCL locl_DEVICES.\n");
			exit(EXIT_FAILURE);
		}

		locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].MyDevices = malloc(sizeof(devices)*locl_NUM_DEVICES);
		for (j = 0; j < locl_NUM_DEVICES; j++)
		{
			locl_ListDevice(&locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]], locl_DEVICES[j], 0);
			locl_DispPlats[listPlatforms[locl_PLATAFORM_NUMBER]].MyDevices[j].numDevice = j;
		}

	}
	return 0;
}

int locl_ListDevice(plataforms *X, cl_device_id device, int tipo){
	if(locl_INIT != 1)
		return 1;
	
	cl_int status;
	size_t buffer_size;	
	//Name
	int aux;
	if(tipo == 0){	
		status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &buffer_size);
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
		//End Type

		//VendorID
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->VendorId = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, buffer_size, &X->MyDevices->VendorId, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End VendorID

		//Profile
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Profile = malloc(buffer_size);
		
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILE, buffer_size, X->MyDevices->Profile, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Name

		//Device Available
		status = clGetDeviceInfo(device, CL_DEVICE_AVAILABLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Available = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_AVAILABLE, buffer_size, &X->MyDevices->Available, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Device Available

		//Version
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Version = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, buffer_size, X->MyDevices->Version, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Version

		//DriverVersion
		status = clGetDeviceInfo(device, CL_DRIVER_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->DriverVersion = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DRIVER_VERSION, buffer_size, X->MyDevices->DriverVersion, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Version

		//CompilerAvailable
		status = clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->CompilerAvailable = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE, buffer_size, &X->MyDevices->CompilerAvailable, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End CompilerAvailable
		
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
		//End Endian

		//ErrorCorrection
		status = clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->ErrorCorrection = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, buffer_size, X->MyDevices->ErrorCorrection, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ErrorCorrection
		
		//AdressAligment
		status = clGetDeviceInfo(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->AdressAligment = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, buffer_size, &X->MyDevices->AdressAligment, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End AdressAligment

		//SmalltestAligment
		status = clGetDeviceInfo(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->SmallestAlignment = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, buffer_size, &X->MyDevices->SmallestAlignment, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End SmalltestAligment
		
		//ResolutionTimer
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->ResolutionTimer = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION, buffer_size, &X->MyDevices->ResolutionTimer, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ResolutionTimer
		

		//MaxClock
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxClock = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, buffer_size, &X->MyDevices->MaxClock, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxClock
		
		//MaxComputeUnits
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxComputeUnits = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, buffer_size, &X->MyDevices->MaxComputeUnits, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxComputeUnits

		//MaxConstantArgs
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxConstantArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, buffer_size, &X->MyDevices->MaxConstantArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxConstantArgs

		//Buffer
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxBufferSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, buffer_size, &X->MyDevices->MaxBufferSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//EndBuffer
		
		//MaxMemAlocSize
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxMemAlocSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, buffer_size, &X->MyDevices->MaxMemAlocSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxMemAlocSize

		//MaxParamSize;	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxParamSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, buffer_size, &X->MyDevices->MaxParamSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxParamSize;

		//ExecutionCapabilities;	
		status = clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->ExecutionCapabilities = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES, buffer_size, X->MyDevices->ExecutionCapabilities, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ExecutionCapabilities

		//MaxGlobalMemSize;	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxGlobalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, buffer_size, &X->MyDevices->MaxGlobalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxGlobalMemSize;

		//MaxGlobalMemCacheSize	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxGlobalMemCacheSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, buffer_size, &X->MyDevices->MaxGlobalMemCacheSize, NULL);
		//End MaxGlobalMemCacheSize

		//GlobalMemLineCacheSize	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->GlobalMemLineCacheSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, buffer_size, &X->MyDevices->GlobalMemLineCacheSize, NULL);
		//End CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE


		//MaxLocalMemSize	
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxLocalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, buffer_size, &X->MyDevices->MaxLocalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxLocalMemSize
		
		//MemType	
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MemType = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, buffer_size, X->MyDevices->MemType, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MemType
		
		//GlobalMemCahcheType	
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->GlobalMemCahcheType = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, buffer_size, X->MyDevices->GlobalMemCahcheType, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End GlobalMemCahcheType
		
		//MaxWorkItemDimensions	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkItemDimensions = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, buffer_size, &X->MyDevices->MaxWorkItemDimensions, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWorkItemDimensions		

		//printf("%d\n", X->MyDevices->MaxWorkItem);
		//End MaxWorkItem
			
		//MaxWorkGroupSize	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkGroupSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, buffer_size, &X->MyDevices->MaxWorkGroupSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWorkGroupSize
		
		//MaxWorkItemSizes	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWorkItemSizes = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MyDevices->MaxWorkItemSizes, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//EndMaxWorkItemSizes

		//Max2dHeight	
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Max2dHeight = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, buffer_size, &X->MyDevices->Max2dHeight, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max2dHeight

		//Max2dWidth	
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Max2dWidth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, buffer_size, &X->MyDevices->Max2dWidth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max2dWidth
		
		//Max3dDepth	
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Max3dDepth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, buffer_size, &X->MyDevices->Max3dDepth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dDepth

		//Max3dHeight	
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Max3dHeight = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, buffer_size, &X->MyDevices->Max3dHeight, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dHeight

		//Max3dWidth	
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Max3dWidth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, buffer_size, &X->MyDevices->Max3dWidth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dWidth

		//MaxReadImageArgs	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxReadImageArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, buffer_size, &X->MyDevices->MaxReadImageArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxReadImageArgs

		//MaxWriteImageArgs	
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxWriteImageArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, buffer_size, &X->MyDevices->MaxWriteImageArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWriteImageArgs

		//MaxSamplers
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_SAMPLERS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->MaxSamplers = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_SAMPLERS, buffer_size, &X->MyDevices->MaxSamplers, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxSamplers

		//PreferredVectorWidthChar
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthChar = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, buffer_size, &X->MyDevices->PreferredVectorWidthChar, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthShort
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthShort = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, buffer_size, &X->MyDevices->PreferredVectorWidthShort, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthShort

		//PreferredVectorWidthInt
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthInt = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, buffer_size, &X->MyDevices->PreferredVectorWidthInt, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthInt

		//PreferredVectorWidthLong
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthLong = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, buffer_size, &X->MyDevices->PreferredVectorWidthLong, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthFloat
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthFloat = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, buffer_size, &X->MyDevices->PreferredVectorWidthFloat, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthDouble
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->PreferredVectorWidthDouble = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, buffer_size, &X->MyDevices->PreferredVectorWidthDouble, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//Extensions
		status = clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MyDevices->Extensions = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, buffer_size, X->MyDevices->Extensions, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//Extensions

		
	}
	else if(tipo == 1){
		if(locl_INIT != 1)
			return 1;
		//Name		
		printf("Name: %s\n", X->MyDevices->Name);
		
		//Vendor
		printf("Vendor: %s\n", X->MyDevices->Vendor);

		//Type
		if(X->MyDevices->Type & CL_DEVICE_TYPE_GPU)
			printf("Type: GPU\n");

		if(X->MyDevices->Type & CL_DEVICE_TYPE_CPU)
			printf("Type: CPU\n");
		
		if(X->MyDevices->Type & CL_DEVICE_TYPE_ACCELERATOR)
			printf("Type: ACCELERATOR\n");

		//Vendor Id
		printf("Vendor Id: %u\n", X->MyDevices->VendorId);

		//Profile
		printf("Profile: %s\n", X->MyDevices->Profile);

		//Available
		if(X->MyDevices->Available)
			printf("Available: Yes\n");
		else	
			printf("Available:  No\n");

		//Version
		printf("Version: %s\n", X->MyDevices->Version);

		//DriverVersion
		printf("Driver Version: %s\n", X->MyDevices->DriverVersion);
		
		//CompilerAvailable
		if(X->MyDevices->CompilerAvailable)
			printf("Compiler Available: Yes\n");
		else	
			printf("Compiler Available:  No\n");

		//AdressSpace
		printf("Address space size: %u\n", X->MyDevices->AdressSpace);
		
		//Endian
		if(X->MyDevices->LitleEndian)
			printf("Litlle Endian: Yes\n");
		else	
			printf("Litlle Endian:  No\n");

		//ErrorCorrection
		if(X->MyDevices->ErrorCorrection)
			printf("Error Correction: Yes\n");
		else	
			printf("Error Correction:  No\n");

		//AdressAligment
		printf("Adress Aligment: %u\n", X->MyDevices->AdressAligment);

		//SmallestAlignment
		printf("Smallest Alignment: %u\n", X->MyDevices->SmallestAlignment);

		//ResolutionTimer;	
		printf("Resolution Timer: %zu\n", X->MyDevices->ResolutionTimer);		

		//MaxClock
		printf("Max Clock: %u\n", X->MyDevices->MaxClock);

		//MaxComputeUnits
		printf("Max Compute Units: %u\n", X->MyDevices->MaxComputeUnits);

		//MaxConstantArgs
		printf("Max Constant Args: %u\n", X->MyDevices->MaxConstantArgs);

		//Buffer Size
		if (X->MyDevices->MaxBufferSize > 1023 && X->MyDevices->MaxBufferSize < 1048576)
			printf("Max Buffer Size: %llu KB\n", (int)X->MyDevices->MaxBufferSize/1024);
		else if (X->MyDevices->MaxBufferSize > 1048576 && X->MyDevices->MaxBufferSize < 1073741824)
			printf("Max Buffer Size: %llu MB\n", (int)X->MyDevices->MaxBufferSize/1048576);
		else if (X->MyDevices->MaxBufferSize > 1073741824)
			printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxBufferSize/1073741824, (long int)((long int)X->MyDevices->MaxBufferSize%1073741824)/1048576);
		
		
		//MaxMemAlocSize
		if (X->MyDevices->MaxMemAlocSize > 1023 && X->MyDevices->MaxMemAlocSize < 1048576)
			printf("Max Aloc Size: %llu KB\n", (int)X->MyDevices->MaxMemAlocSize/1024);
		else if (X->MyDevices->MaxMemAlocSize> 1048576 && X->MyDevices->MaxMemAlocSize < 1073741824)
			printf("Max Aloc Size: %llu MB\n", (int)X->MyDevices->MaxMemAlocSize/1048576);
		else if (X->MyDevices->MaxMemAlocSize > 1073741824)
			printf("Max Aloc Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxMemAlocSize/1073741824, (long int)((long int)X->MyDevices->MaxMemAlocSize%1073741824)/1048576);

		//MaxParamSize;	
		printf("Max Parameter size: %zu\n", X->MyDevices->MaxParamSize);

		switch (*((cl_device_exec_capabilities*)X->MyDevices->ExecutionCapabilities))
		{	
	  		case CL_EXEC_KERNEL:
				printf("OpenCL kernels\n");
			break;
			
	  		case CL_EXEC_NATIVE_KERNEL:
				printf("Native Kernels\n");
			break;
		}
		
		//MaxGlobalMemSize;	
		if (X->MyDevices->MaxGlobalMemSize > 1023 && X->MyDevices->MaxGlobalMemSize < 1048576)
			printf("Max Global Size: %llu KB\n", (int)X->MyDevices->MaxGlobalMemSize/1024);
		else if (X->MyDevices->MaxGlobalMemSize > 1048576 && X->MyDevices->MaxGlobalMemSize < 1073741824)
			printf("Max Global Size: %llu MB\n", (int)X->MyDevices->MaxGlobalMemSize/1048576);
		else if (X->MyDevices->MaxGlobalMemSize > 1073741824)
			printf("Max Global Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxGlobalMemSize/1073741824, (long int)((long int)X->MyDevices->MaxGlobalMemSize%1073741824)/1048576);
		
		//MaxGlobalMemCacheSize	
		if (X->MyDevices->MaxGlobalMemCacheSize> 1023 && X->MyDevices->MaxGlobalMemCacheSize < 1048576)
			printf("Max Cache Mem Size: %llu KB\n", (int)X->MyDevices->MaxGlobalMemCacheSize/1024);
		else if (X->MyDevices->MaxGlobalMemCacheSize> 1048576 && X->MyDevices->MaxGlobalMemCacheSize< 1073741824)
			printf("Max Cache Mem Size: %llu MB\n", (int)X->MyDevices->MaxGlobalMemCacheSize/1048576);
		else if (X->MyDevices->MaxGlobalMemCacheSize > 1073741824)
			printf("Max Cache Mem Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxGlobalMemCacheSize/1073741824, (long int)((long int)X->MyDevices->MaxGlobalMemCacheSize%1073741824)/1048576);

		//GlobalMemLineCacheSize;	
		printf("Global Mem Line Cache Size: %zu\n", X->MyDevices->GlobalMemLineCacheSize);

		//MaxLocalMemSize			
		if (X->MyDevices->MaxLocalMemSize> 1023 && X->MyDevices->MaxLocalMemSize < 1048576)
			printf("Max Local Mem Size: %llu KB\n", (int)X->MyDevices->MaxLocalMemSize/1024);
		else if (X->MyDevices->MaxLocalMemSize> 1048576 && X->MyDevices->MaxLocalMemSize< 1073741824)
			printf("Max Local Mem Size: %llu MB\n", (int)X->MyDevices->MaxLocalMemSize/1048576);
		else if (X->MyDevices->MaxLocalMemSize > 1073741824)
			printf("Max Local Mem Size: %llu GB %llu MB\n", (long int)X->MyDevices->MaxLocalMemSize/1073741824, (long int)((long int)X->MyDevices->MaxLocalMemSize%1073741824)/1048576);

		switch (*((cl_device_local_mem_type*)X->MyDevices->MemType))
		{	
	  		case CL_NONE:
				printf("Device Mem Type: None\n");
			break;
			
	  		case CL_READ_ONLY_CACHE:
				printf("Device Mem Type: Local\n");
			break;
	  		case CL_READ_WRITE_CACHE:
				printf("Device Mem Type: Global\n");
			break;
		}

		switch (*((cl_device_mem_cache_type*)X->MyDevices->GlobalMemCahcheType))
		{	
	  		case CL_NONE:
				printf("Device Cache Mem Type: \n");
			break;
			
	  		case CL_READ_ONLY_CACHE:
				printf("Device Cache Mem Type: Read Only\n");
			break;
	  		case CL_READ_WRITE_CACHE:
				printf("Device Cache Mem Type: Read and Write\n");
			break;
		}
		
		
		//MaxWorkGroupSize
		printf("Max Work Group Size: %zu\n", X->MyDevices->MaxWorkGroupSize);			
		
		//MaxWorkItemDimensions	
		printf("Max Work Item Dimensions: %zu\n", X->MyDevices->MaxWorkItemDimensions);		
		
		//MaxWorkItemSizes	
		printf("Max Work Items Sizes: %u\n", X->MyDevices->MaxWorkItemSizes);		

		//Max2dHeight
		printf("Max 2D Height: %zu\n", X->MyDevices->Max2dHeight);			
		
		//Max2dWidth
		printf("Max 2D Width: %zu\n", X->MyDevices->Max2dWidth);			
		
		//Max3dDepth
		printf("Max 3D Depth: %zu\n", X->MyDevices->Max3dDepth);			
		
		//Max3dHeight
		printf("Max 3D Height: %zu\n", X->MyDevices->Max3dHeight);
		
		//Max3dWidth			
		printf("Max 3D Width: %zu\n", X->MyDevices->Max3dWidth);	

		//MaxReadImageArgs
		printf("Max Read Image Args: %u\n", X->MyDevices->MaxReadImageArgs);		

		//MaxWriteImageArgs
		printf("Max Write Image Args: %u\n", X->MyDevices->MaxWriteImageArgs);

		//MaxSamplers
		printf("Max Samplers: %u\n", X->MyDevices->MaxSamplers);

		//PreferredVectorWidthChar
		printf("Preferred Vector Width Char: %u\n", X->MyDevices->PreferredVectorWidthChar);

		//PreferredVectorWidthShort
		printf("Preferred Vector Width Short: %u\n", X->MyDevices->PreferredVectorWidthShort);

		//PreferredVectorWidthInt
		printf("Preferred Vector Width Int: %u\n", X->MyDevices->PreferredVectorWidthInt);

		//PreferredVectorWidthLong
		printf("Preferred Vector Width Long: %u\n", X->MyDevices->PreferredVectorWidthLong);

		//PreferredVectorWidthFloat
		printf("Preferred Vector Width Float: %u\n", X->MyDevices->	PreferredVectorWidthFloat);

		//PreferredVectorWidthDouble
		printf("Preferred Vector Width Double: %u\n", X->MyDevices->	PreferredVectorWidthDouble);	

		printf("Extensions:\n%s", X->MyDevices->Extensions);

		printf("\n");
	}
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
			printf ("OpenCL Devices is not initialized\n");
			exit(1);
		break;
		case 4:
			printf ("The device doens't exist\n");
			exit(1);
		break;
	}
}

int locl_CreateCmdQueue(int locl_PLATAFORM_NUMBER ,int locl_DEVICE_NUMBER){

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

int locl_CreateProgram(const char** source_str, char *kernel){
	cl_int status;	

	locl_PROGRAM = clCreateProgramWithSource(locl_CONTEXT, 1, source_str,  NULL, &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to create a program from source\n");
        exit(1);
    }
    
    // Build (compile) the program for the locl_DEVICES with
    // clBuildProgram()
    status = clBuildProgram(locl_PROGRAM, locl_NUM_DEVICES, locl_DEVICES, NULL, NULL, NULL); 
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
        clGetProgramBuildInfo(locl_PROGRAM, locl_DEVICES[0], CL_PROGRAM_BUILD_LOG, sizeof(logBuffer), logBuffer, NULL);
        printf("CL Compilation failed:\n%s", logBuffer);
        exit(1);
    }

    // Use clCreateKernel() to create a kernel from the 
    locl_KERNEL = clCreateKernel(locl_PROGRAM, kernel, &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to set a kernel from program\n");
        exit(1);
    }

    return 0;
}

int locl_SetKernelArg(cl_int i, size_t tipo,const void *buffer){
	cl_int status;
	status = clSetKernelArg(locl_KERNEL, i, sizeof(tipo), &buffer);
    if (status != CL_SUCCESS) {
        printf ("Unable to set kernel argument\n");
        exit(1);
    }
    return 0; 
}


char *DiscStr(char *name){
	int i = 0;
	char aux[10] = {' ', ' ', ' ', ' ', ' ', ' ',' ',' ',' ',' '};
	while(!isspace(name[i])){
	
		aux[i] = name [i];
		i++;
	}
	strcpy(name, aux)
;	return name;
}

int isEqual(char *name, char *name2){
	int i = 0;
	while(!isspace(name[i])){
		if(name[i] != name2[i]){
			return 0;
		}
		i++;
	}
		return 1;

// Mudar de alfabeto para caracteres
}

int locl_Init(int locl_PLATFORM_NUM, int locl_DEVICE_NUM){
  	int error;
  	error = locl_Initialize_Platforms();
        locl_Errors(error);
    
    if(locl_PLATFORM_NUM < locl_ALL && locl_DEVICE_NUM != 1){
    	error = locl_Initialize_Device(locl_PLATFORM_NUM);  
    	locl_Errors(error);
    }

    error = locl_Explore( locl_PLATFORM_NUM);
    	locl_Errors(error);

    if(locl_PLATFORM_NUM < locl_ALL && locl_DEVICE_NUM != 1){
    	error = locl_CreateCmdQueue(locl_PLATFORM_NUM, locl_DEVICE_NUM);
    	locl_Errors(error);
    }
    return 0;
}


int locl_Finalize(){
	free(locl_DispPlats);
	
}
