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

short int lopcl_INIT = 0, lopcl_INIT_DEVICE = 0;

/* Inicializa as variáveis que serão usadas como index e plataformas */

int lopcl_Initialize_Platforms(){
	//"Zerando" index's
	listPlatforms[0] = listPlatforms[1] = listPlatforms[2] = listPlatforms[3] = listPlatforms[4] = listPlatforms[5] = listPlatforms[6] = -1;
	listDevices[0] = listDevices[1] = listDevices[2] = -1;

	lopcl_INIT = 1;

	int i, j;
	size_t buffer_size;
	char *aux_name;

	//Numero de plataformas disponíveis
	cl_int status = clGetPlatformIDs(0, NULL, &lopcl_NUM_PLATFORMS);
	listPlatforms[lopcl_ALL] = lopcl_NUM_PLATFORMS;
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_PLATFORMS available.\n");
		printf("Status %d\n",status);
		exit(EXIT_FAILURE);
	}

	lopcl_PLATFORMS = malloc(sizeof(cl_platform_id)*lopcl_NUM_PLATFORMS);

	lopcl_DispPlats = malloc(sizeof(plataforms)*lopcl_NUM_PLATFORMS);

	status = clGetPlatformIDs(lopcl_NUM_PLATFORMS, lopcl_PLATFORMS, NULL);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the list of OpenCL lopcl_PLATFORMS.\n");
		printf("Status %d\n",status);

		exit(EXIT_FAILURE);
	}

	/*Varredura das plataformas disponíveis*/
	for(i=0; i<lopcl_NUM_PLATFORMS; i++){
		//Discover Vendor
		status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS){
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		lopcl_DispPlats[i].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, lopcl_DispPlats[i].Vendor, NULL);
		if (status != CL_SUCCESS){
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		aux_name = DiscStr(lopcl_DispPlats[i].Vendor);
		if(isEqual(aux_name,"Intel(R)"))
			listPlatforms[lopcl_INTEL] = i;

		if(isEqual(aux_name,"Intel"))
			listPlatforms[lopcl_INTEL_GPU] = i;

		if(isEqual(aux_name,"The"))
			listPlatforms[lopcl_POCL] = i;

		if(isEqual(aux_name,"AMD"))
			listPlatforms[lopcl_AMD] = i;

		if(isEqual(aux_name,"NVIDIA"))
			listPlatforms[lopcl_NVIDIA] = i;

		if(isEqual(aux_name,"Brown")){
			listPlatforms[lopcl_COPRTHR] = i;
		}

	}
	return 0;
}

int lopcl_Initialize_Device(int lopcl_PLATFORM_NUM){
	int j;
	size_t buffer_size;
	cl_int status;
	cl_device_type aux_type;
	lopcl_INIT_DEVICE = 1;

	if(listPlatforms[lopcl_PLATFORM_NUM] > listPlatforms[lopcl_ALL] || listPlatforms[lopcl_PLATFORM_NUM] == -1 || lopcl_PLATFORM_NUM < 0){
		return 2;
	}
	else{
		//INIT DEVICES
		status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[lopcl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		lopcl_DEVICES = malloc(sizeof(cl_device_id)*lopcl_NUM_DEVICES);

		status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[lopcl_PLATFORM_NUM]], CL_DEVICE_TYPE_ALL, lopcl_NUM_DEVICES, lopcl_DEVICES, NULL);
		if (status != CL_SUCCESS)
		{
			printf(" Cannot get the list of OpenCL lopcl_DEVICES.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		for (j = 0; j < lopcl_NUM_DEVICES; j++){
			aux_type = NULL;
			status = clGetDeviceInfo(lopcl_DEVICES[j], CL_DEVICE_TYPE, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS){
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}

			aux_type = *(cl_device_type*) malloc(buffer_size);
			status = clGetDeviceInfo(lopcl_DEVICES[j], CL_DEVICE_TYPE, buffer_size, &aux_type, NULL);
			if (status != CL_SUCCESS) {
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}

			if(aux_type & CL_DEVICE_TYPE_CPU){
				listDevices[lopcl_DEVICE_CPU] = j;
			}

			if (aux_type & CL_DEVICE_TYPE_GPU){
				listDevices[lopcl_DEVICE_GPU] = j;
			}

			if(aux_type & CL_DEVICE_TYPE_ACCELERATOR){
				listDevices[lopcl_DEVICE_ACCELERATOR] = j;
			}
		}
	}
	return 0;
}

int lopcl_PrintInfo(int lopcl_PLATAFORM_NUMBER){
    if(lopcl_INIT != 1)
		return 1;

	int i, j;
	devices aux; /*Estrutura auxiliar de Devices*/
	char *aux_name;
	size_t buffer_size;

	if(listPlatforms[lopcl_PLATAFORM_NUMBER] == -1){
		return 2;
	}else if(listPlatforms[lopcl_PLATAFORM_NUMBER] == lopcl_NUM_PLATFORMS){ //If the user pass the number of platforms, list all available
		for (i = 0; i < lopcl_NUM_PLATFORMS; i++){
			printf("Platform Number: %d\n", lopcl_DispPlats[i].numPlat);
			printf("Name: %s\n", lopcl_DispPlats[i].Name);
			printf("Vendor: %s\n", lopcl_DispPlats[i].Vendor);
			printf("Version: %s\n", lopcl_DispPlats[i].Version);
			printf("Extensions:\n%s\n", lopcl_DispPlats[i].Extensions);

			// END PLATFORMS
			//INIT lopcl_DEVICES
			cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}

			for (j = 0; j < lopcl_NUM_DEVICES; j++)
			{
				printf("Device Number: %d\n", lopcl_DispPlats[i].MyDevices[j].numDevice);
				lopcl_ListDevice(&lopcl_DispPlats[i].MyDevices[j], lopcl_DEVICES[j], 1);
			}
		}
	}else{

		/* Imprimir informações especificas */

		printf("Platform Number: %d\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].numPlat);
		printf("Name: %s\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Name);
		printf("Vendor: %s\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Vendor);
		printf("Version: %s\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Version);
		printf("Extesions: %s\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Extensions);

		// END PLATFORMS

		//INIT lopcl_DEVICES
		cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		for (j = 0; j < lopcl_NUM_DEVICES; j++)
		{
			printf("Device Number: %d\n", lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].MyDevices[j].numDevice);

			lopcl_ListDevice(&lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].MyDevices[j], lopcl_DEVICES[j], 1);
		}

	}
	return 0;
}

int lopcl_Explore(int lopcl_PLATAFORM_NUMBER){

	if(lopcl_INIT != 1)
		return 1;

	int i, j;
	char *aux_name;
	size_t buffer_size;

	if(listPlatforms[lopcl_PLATAFORM_NUMBER] == -1){
		return 2;
	}else if(listPlatforms[lopcl_PLATAFORM_NUMBER] == lopcl_NUM_PLATFORMS){ //If the user pass the number of platforms, list all available
		for (i = 0; i < lopcl_NUM_PLATFORMS; i++){
			lopcl_DispPlats[i].numPlat = i;

			//Discover Name
			cl_int status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}
			lopcl_DispPlats[i].Name = malloc(buffer_size);
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_NAME, buffer_size, lopcl_DispPlats[i].Name, NULL);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
			 	exit(EXIT_FAILURE);
		 	}

			//Discover Vendor
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
			 	exit(EXIT_FAILURE);
			}
			lopcl_DispPlats[i].Vendor = malloc(buffer_size);
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VENDOR, buffer_size, lopcl_DispPlats[i].Vendor, NULL);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
			 	exit(EXIT_FAILURE);
			}
			//Discover Version
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}
			lopcl_DispPlats[i].Version = malloc(buffer_size);
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_VERSION, buffer_size, lopcl_DispPlats[i].Version, NULL);

			//Discover Extensions
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &buffer_size);
			if (status != CL_SUCCESS)
			{
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}
			lopcl_DispPlats[i].Extensions = malloc(buffer_size);
			status = clGetPlatformInfo(lopcl_PLATFORMS[i], CL_PLATFORM_EXTENSIONS, buffer_size, lopcl_DispPlats[i].Extensions, NULL);

			// END PLATFORMS
			//INIT lopcl_DEVICES
			status = clGetDeviceIDs(lopcl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
			if (status != CL_SUCCESS)
			{
				printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}

			lopcl_DEVICES = malloc(sizeof(cl_device_id)*lopcl_NUM_DEVICES);
			status = clGetDeviceIDs(lopcl_PLATFORMS[i], CL_DEVICE_TYPE_ALL, lopcl_NUM_DEVICES, lopcl_DEVICES, NULL);
			if (status != CL_SUCCESS)
			{
				printf(" Cannot get the list of OpenCL lopcl_DEVICES.\n");
				printf("Status %d\n",status);
				exit(EXIT_FAILURE);
			}

			lopcl_DispPlats[i].MyDevices = malloc(sizeof(devices)*lopcl_NUM_DEVICES);
			for (j = 0; j < lopcl_NUM_DEVICES; j++)
			{
				lopcl_DispPlats[i].MyDevices[j].numDevice = j;
				lopcl_ListDevice(&lopcl_DispPlats[i].MyDevices[j], lopcl_DEVICES[j], 0);
			}
		}
	}else{

		/* Imprimir informações especificas */
		lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].numPlat = listPlatforms[lopcl_PLATAFORM_NUMBER];

		//Discover Name
		cl_int status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}
		lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Name = malloc(buffer_size);
		status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_NAME, buffer_size, lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Name, NULL);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		//Discover Vendor
		status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Vendor = malloc(buffer_size);
		status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_VENDOR, buffer_size, lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Vendor, NULL);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		//Discover Version
		status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
		 	exit(EXIT_FAILURE);
		}
		lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Version = malloc(buffer_size);
		status = clGetPlatformInfo(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_PLATFORM_VERSION, buffer_size, lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].Version, NULL);


		// END PLATFORMS

		//INIT lopcl_DEVICES
		status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
		if (status != CL_SUCCESS)
		{
			printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		lopcl_DEVICES = malloc(sizeof(cl_device_id)*lopcl_NUM_DEVICES);
		status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[lopcl_PLATAFORM_NUMBER]], CL_DEVICE_TYPE_ALL, lopcl_NUM_DEVICES, lopcl_DEVICES, NULL);
		if (status != CL_SUCCESS)
		{
			printf(" Cannot get the list of OpenCL lopcl_DEVICES.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].MyDevices = malloc(sizeof(devices)*lopcl_NUM_DEVICES);
		for (j = 0; j < lopcl_NUM_DEVICES; j++)
		{
			lopcl_ListDevice(&lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].MyDevices[j], lopcl_DEVICES[j], 0);
			lopcl_DispPlats[listPlatforms[lopcl_PLATAFORM_NUMBER]].MyDevices[j].numDevice = j;
		}

	}
	return 0;
}

int lopcl_ListDevice(devices *X, cl_device_id device, int tipo){
	if(lopcl_INIT != 1)
		return 1;

	cl_int status;
	size_t buffer_size;
	//Name
	int aux;
	if(tipo == 0){
		status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}

		X->Name = malloc(buffer_size);

		status = clGetDeviceInfo(device, CL_DEVICE_NAME, buffer_size, X->Name, NULL);
		if (status != CL_SUCCESS)
		{
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
		}
		//End Name

		//Vendor
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Vendor = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR, buffer_size, X->Vendor, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Vendor

		//Type
		status = clGetDeviceInfo(device, CL_DEVICE_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Type = *(cl_device_type*) malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_TYPE, buffer_size, &X->Type, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Type

		//VendorID
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->VendorId = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID, buffer_size, &X->VendorId, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End VendorID

		//Profile
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Profile = malloc(buffer_size);

		status = clGetDeviceInfo(device, CL_DEVICE_PROFILE, buffer_size, X->Profile, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Name

		//Device Available
		status = clGetDeviceInfo(device, CL_DEVICE_AVAILABLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Available = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_AVAILABLE, buffer_size, &X->Available, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Device Available

		//Version
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Version = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_VERSION, buffer_size, X->Version, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Version

		//DriverVersion
		status = clGetDeviceInfo(device, CL_DRIVER_VERSION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->DriverVersion = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DRIVER_VERSION, buffer_size, X->DriverVersion, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Version

		//CompilerAvailable
		status = clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->CompilerAvailable = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_COMPILER_AVAILABLE, buffer_size, &X->CompilerAvailable, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End CompilerAvailable

		//AdressSpace
		status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->AdressSpace = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, buffer_size, &X->AdressSpace, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End AdressSpace

		//Endian
		status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->LitleEndian = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ENDIAN_LITTLE, buffer_size, X->LitleEndian, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Endian

		//ErrorCorrection
		status = clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->ErrorCorrection = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, buffer_size, X->ErrorCorrection, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ErrorCorrection

		//AdressAligment
		status = clGetDeviceInfo(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->AdressAligment = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, buffer_size, &X->AdressAligment, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End AdressAligment

		//SmalltestAligment
		status = clGetDeviceInfo(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->SmallestAlignment = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, buffer_size, &X->SmallestAlignment, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End SmalltestAligment

		//ResolutionTimer
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->ResolutionTimer = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION, buffer_size, &X->ResolutionTimer, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ResolutionTimer


		//MaxClock
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxClock = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, buffer_size, &X->MaxClock, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxClock

		//MaxComputeUnits
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxComputeUnits = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, buffer_size, &X->MaxComputeUnits, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxComputeUnits

		//MaxConstantArgs
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxConstantArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, buffer_size, &X->MaxConstantArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxConstantArgs

		//Buffer
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxBufferSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, buffer_size, &X->MaxBufferSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//EndBuffer

		//MaxMemAlocSize
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxMemAlocSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_ARGS, buffer_size, &X->MaxMemAlocSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxMemAlocSize

		//MaxParamSize;
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxParamSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_PARAMETER_SIZE, buffer_size, &X->MaxParamSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxParamSize;

		//ExecutionCapabilities;
		status = clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->ExecutionCapabilities = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_EXECUTION_CAPABILITIES, buffer_size, X->ExecutionCapabilities, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End ExecutionCapabilities

		//MaxGlobalMemSize;
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxGlobalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, buffer_size, &X->MaxGlobalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxGlobalMemSize;

		//MaxGlobalMemCacheSize
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxGlobalMemCacheSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, buffer_size, &X->MaxGlobalMemCacheSize, NULL);
		//End MaxGlobalMemCacheSize

		//GlobalMemLineCacheSize
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->GlobalMemLineCacheSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, buffer_size, &X->GlobalMemLineCacheSize, NULL);
		//End CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE


		//MaxLocalMemSize
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxLocalMemSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, buffer_size, &X->MaxLocalMemSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxLocalMemSize

		//MemType
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MemType = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, buffer_size, X->MemType, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MemType

		//GlobalMemCahcheType
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->GlobalMemCahcheType = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, buffer_size, X->GlobalMemCahcheType, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End GlobalMemCahcheType

		//MaxWorkItemDimensions
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxWorkItemDimensions = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, buffer_size, &X->MaxWorkItemDimensions, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWorkItemDimensions

		//printf("%d\n", X->MaxWorkItem);
		//End MaxWorkItem

		//MaxWorkGroupSize
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxWorkGroupSize = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, buffer_size, &X->MaxWorkGroupSize, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWorkGroupSize

		//MaxWorkItemSizes
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxWorkItemSizes = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, buffer_size, &X->MaxWorkItemSizes, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//EndMaxWorkItemSizes

		//Max2dHeight
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Max2dHeight = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, buffer_size, &X->Max2dHeight, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max2dHeight

		//Max2dWidth
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Max2dWidth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, buffer_size, &X->Max2dWidth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max2dWidth

		//Max3dDepth
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Max3dDepth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, buffer_size, &X->Max3dDepth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dDepth

		//Max3dHeight
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Max3dHeight = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, buffer_size, &X->Max3dHeight, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dHeight

		//Max3dWidth
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Max3dWidth = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, buffer_size, &X->Max3dWidth, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End Max3dWidth

		//MaxReadImageArgs
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxReadImageArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, buffer_size, &X->MaxReadImageArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxReadImageArgs

		//MaxWriteImageArgs
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxWriteImageArgs = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, buffer_size, &X->MaxWriteImageArgs, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxWriteImageArgs

		//MaxSamplers
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_SAMPLERS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->MaxSamplers = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_MAX_SAMPLERS, buffer_size, &X->MaxSamplers, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//End MaxSamplers

		//PreferredVectorWidthChar
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthChar = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, buffer_size, &X->PreferredVectorWidthChar, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthShort
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthShort = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, buffer_size, &X->PreferredVectorWidthShort, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthShort

		//PreferredVectorWidthInt
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthInt = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, buffer_size, &X->PreferredVectorWidthInt, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthInt

		//PreferredVectorWidthLong
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthLong = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, buffer_size, &X->PreferredVectorWidthLong, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthFloat
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthFloat = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, buffer_size, &X->PreferredVectorWidthFloat, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//PreferredVectorWidthDouble
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->PreferredVectorWidthDouble = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, buffer_size, &X->PreferredVectorWidthDouble, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//PreferredVectorWidthChar

		//Extensions
		status = clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, NULL, &buffer_size);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);

		X->Extensions = malloc(buffer_size);
		status = clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, buffer_size, X->Extensions, NULL);
		if (status != CL_SUCCESS) exit(EXIT_FAILURE);
		//Extensions


	}
	else if(tipo == 1){
		if(lopcl_INIT != 1)
			return 1;
		//Name
		printf("Name: %s\n", X->Name);

		//Vendor
		printf("Vendor: %s\n", X->Vendor);

		//Type
		if(X->Type & CL_DEVICE_TYPE_GPU)
			printf("Type: GPU\n");

		if(X->Type & CL_DEVICE_TYPE_CPU)
			printf("Type: CPU\n");

		if(X->Type & CL_DEVICE_TYPE_ACCELERATOR)
			printf("Type: ACCELERATOR\n");

		//Vendor Id
		printf("Vendor Id: %u\n", X->VendorId);

		//Profile
		printf("Profile: %s\n", X->Profile);

		//Available
		if(X->Available)
			printf("Available: Yes\n");
		else
			printf("Available:  No\n");

		//Version
		printf("Version: %s\n", X->Version);

		//DriverVersion
		printf("Driver Version: %s\n", X->DriverVersion);

		//CompilerAvailable
		if(X->CompilerAvailable)
			printf("Compiler Available: Yes\n");
		else
			printf("Compiler Available:  No\n");

		//AdressSpace
		printf("Address space size: %u\n", X->AdressSpace);

		//Endian
		if(X->LitleEndian)
			printf("Litlle Endian: Yes\n");
		else
			printf("Litlle Endian:  No\n");

		//ErrorCorrection
		if(X->ErrorCorrection)
			printf("Error Correction: Yes\n");
		else
			printf("Error Correction:  No\n");

		//AdressAligment
		printf("Adress Aligment: %u\n", X->AdressAligment);

		//SmallestAlignment
		printf("Smallest Alignment: %u\n", X->SmallestAlignment);

		//ResolutionTimer;
		printf("Resolution Timer: %zu\n", X->ResolutionTimer);

		//MaxClock
		printf("Max Clock: %u\n", X->MaxClock);

		//MaxComputeUnits
		printf("Max Compute Units: %u\n", X->MaxComputeUnits);

		//MaxConstantArgs

		printf("Max Constant Args: %u\n", X->MaxConstantArgs);

		//Buffer Size
		if (X->MaxBufferSize > 1023 && X->MaxBufferSize < 1048576)
			printf("Max Buffer Size: %llu KB\n", (int)X->MaxBufferSize/1024);
		else if (X->MaxBufferSize > 1048576 && X->MaxBufferSize < 1073741824)
			printf("Max Buffer Size: %llu MB\n", (int)X->MaxBufferSize/1048576);
		else if (X->MaxBufferSize > 1073741824)
			printf("Max Buffer Size: %llu GB %llu MB\n", (long int)X->MaxBufferSize/1073741824, (long int)((long int)X->MaxBufferSize%1073741824)/1048576);


		//MaxMemAlocSize
		if (X->MaxMemAlocSize > 1023 && X->MaxMemAlocSize < 1048576)
			printf("Max Aloc Size: %llu KB\n", (int)X->MaxMemAlocSize/1024);
		else if (X->MaxMemAlocSize> 1048576 && X->MaxMemAlocSize < 1073741824)
			printf("Max Aloc Size: %llu MB\n", (int)X->MaxMemAlocSize/1048576);
		else if (X->MaxMemAlocSize > 1073741824)
			printf("Max Aloc Size: %llu GB %llu MB\n", (long int)X->MaxMemAlocSize/1073741824, (long int)((long int)X->MaxMemAlocSize%1073741824)/1048576);

		//MaxParamSize;
		printf("Max Parameter size: %zu\n", X->MaxParamSize);

		switch (*((cl_device_exec_capabilities*)X->ExecutionCapabilities))
		{
	  		case CL_EXEC_KERNEL:
				printf("OpenCL kernels\n");
			break;

	  		case CL_EXEC_NATIVE_KERNEL:
				printf("Native Kernels\n");
			break;
		}

		//MaxGlobalMemSize;
		if (X->MaxGlobalMemSize > 1023 && X->MaxGlobalMemSize < 1048576)
			printf("Max Global Size: %llu KB\n", (int)X->MaxGlobalMemSize/1024);
		else if (X->MaxGlobalMemSize > 1048576 && X->MaxGlobalMemSize < 1073741824)
			printf("Max Global Size: %llu MB\n", (int)X->MaxGlobalMemSize/1048576);
		else if (X->MaxGlobalMemSize > 1073741824)
			printf("Max Global Size: %llu GB %llu MB\n", (long int)X->MaxGlobalMemSize/1073741824, (long int)((long int)X->MaxGlobalMemSize%1073741824)/1048576);

		//MaxGlobalMemCacheSize
		if (X->MaxGlobalMemCacheSize> 1023 && X->MaxGlobalMemCacheSize < 1048576)
			printf("Max Cache Mem Size: %llu KB\n", (int)X->MaxGlobalMemCacheSize/1024);
		else if (X->MaxGlobalMemCacheSize> 1048576 && X->MaxGlobalMemCacheSize< 1073741824)
			printf("Max Cache Mem Size: %llu MB\n", (int)X->MaxGlobalMemCacheSize/1048576);
		else if (X->MaxGlobalMemCacheSize > 1073741824)
			printf("Max Cache Mem Size: %llu GB %llu MB\n", (long int)X->MaxGlobalMemCacheSize/1073741824, (long int)((long int)X->MaxGlobalMemCacheSize%1073741824)/1048576);

		//GlobalMemLineCacheSize;
		printf("Global Mem Line Cache Size: %zu\n", X->GlobalMemLineCacheSize);

		//MaxLocalMemSize
		if (X->MaxLocalMemSize> 1023 && X->MaxLocalMemSize < 1048576)
			printf("Max Local Mem Size: %llu KB\n", (int)X->MaxLocalMemSize/1024);
		else if (X->MaxLocalMemSize> 1048576 && X->MaxLocalMemSize< 1073741824)
			printf("Max Local Mem Size: %llu MB\n", (int)X->MaxLocalMemSize/1048576);
		else if (X->MaxLocalMemSize > 1073741824)
			printf("Max Local Mem Size: %llu GB %llu MB\n", (long int)X->MaxLocalMemSize/1073741824, (long int)((long int)X->MaxLocalMemSize%1073741824)/1048576);

		switch (*((cl_device_local_mem_type*)X->MemType))
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

		switch (*((cl_device_mem_cache_type*)X->GlobalMemCahcheType))
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
		printf("Max Work Group Size: %zu\n", X->MaxWorkGroupSize);

		//MaxWorkItemDimensions
		printf("Max Work Item Dimensions: %zu\n", X->MaxWorkItemDimensions);

		//MaxWorkItemSizes
		printf("Max Work Items Sizes: %u\n", X->MaxWorkItemSizes);

		//Max2dHeight
		printf("Max 2D Height: %zu\n", X->Max2dHeight);

		//Max2dWidth
		printf("Max 2D Width: %zu\n", X->Max2dWidth);

		//Max3dDepth
		printf("Max 3D Depth: %zu\n", X->Max3dDepth);

		//Max3dHeight
		printf("Max 3D Height: %zu\n", X->Max3dHeight);

		//Max3dWidth
		printf("Max 3D Width: %zu\n", X->Max3dWidth);

		//MaxReadImageArgs
		printf("Max Read Image Args: %u\n", X->MaxReadImageArgs);

		//MaxWriteImageArgs
		printf("Max Write Image Args: %u\n", X->MaxWriteImageArgs);

		//MaxSamplers
		printf("Max Samplers: %u\n", X->MaxSamplers);

		//PreferredVectorWidthChar
		printf("Preferred Vector Width Char: %u\n", X->PreferredVectorWidthChar);

		//PreferredVectorWidthShort
		printf("Preferred Vector Width Short: %u\n", X->PreferredVectorWidthShort);

		//PreferredVectorWidthInt
		printf("Preferred Vector Width Int: %u\n", X->PreferredVectorWidthInt);

		//PreferredVectorWidthLong
		printf("Preferred Vector Width Long: %u\n", X->PreferredVectorWidthLong);

		//PreferredVectorWidthFloat
		printf("Preferred Vector Width Float: %u\n", X->	PreferredVectorWidthFloat);

		//PreferredVectorWidthDouble
		printf("Preferred Vector Width Double: %u\n", X->	PreferredVectorWidthDouble);

		printf("Extensions:\n%s", X->Extensions);

		printf("\n");
	}
		return 0;
}

void lopcl_Errors(int i){
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
		case 5:
			printf("Operação não permitida\n");
			exit(1);
	}
}

int lopcl_CreateCmdQueue(int lopcl_PLATAFORM_NUMBER ,int lopcl_DEVICE_NUMBER){
	if(lopcl_INIT_DEVICE != 1)
		return 4;
	cl_int status;
	lopcl_CONTEXT = clCreateContext(NULL, lopcl_NUM_DEVICES, lopcl_DEVICES, NULL, NULL, &status);

	if(listDevices[lopcl_DEVICE_NUMBER] > lopcl_NUM_DEVICES || listDevices[lopcl_DEVICE_NUMBER] < 0 || lopcl_DEVICE_NUMBER < 0){
		return 3;
		exit(1);
	}
	if (status != CL_SUCCESS) {
		printf ("Unable create a context\n");
		printf("Status %d\n",status);
		exit(1);
	}

	lopcl_CMDQUEUE = clCreateCommandQueue(lopcl_CONTEXT, lopcl_DEVICES[listDevices[lopcl_DEVICE_NUMBER]], 0, &status);

	if (status != CL_SUCCESS) {
	printf ("Unable create a command queue\n");
	printf("Status %d\n",status);
	exit(1);
	}
	return 0;
}

cl_mem lopcl_CreateBuffer(size_t lopcl_DATASIZE, cl_mem_flags lopcl_FLAGS, cl_bool lopcl_FLAG1, void *a ){
	cl_int status;
	cl_mem bufferA;
	bufferA = clCreateBuffer(lopcl_CONTEXT, lopcl_FLAGS, lopcl_DATASIZE, NULL, &status);
	if (status != CL_SUCCESS) {
		printf ("Unable to create buffer\n");
		printf("Status %d\n",status);
		exit(1);
	}

	status = clEnqueueWriteBuffer(lopcl_CMDQUEUE, bufferA, lopcl_FLAG1, 0, lopcl_DATASIZE, a, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf ("Unable to copy to buffer\n");
				printf("Status %d\n",status);
        exit(1);
    }

	return bufferA;
}

int lopcl_CreateProgram(const char** source_str, char *kernel){
	cl_int status;

	lopcl_PROGRAM = clCreateProgramWithSource(lopcl_CONTEXT, 1, source_str,  NULL, &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to create a program from source\n");
				printf("Status %d\n",status);
        exit(1);
    }

    // Build (compile) the program for the lopcl_DEVICES with
    // clBuildProgram()
    status = clBuildProgram(lopcl_PROGRAM, lopcl_NUM_DEVICES, lopcl_DEVICES, NULL, NULL, NULL);
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
        clGetProgramBuildInfo(lopcl_PROGRAM, lopcl_DEVICES[0], CL_PROGRAM_BUILD_LOG, sizeof(logBuffer), logBuffer, NULL);
        printf("CL Compilation failed:\n%s", logBuffer);
        exit(1);
    }

    // Use clCreateKernel() to create a kernel from the
    lopcl_KERNEL = clCreateKernel(lopcl_PROGRAM, kernel, &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to set a kernel from program\n");
				printf("Status %d\n",status);
        exit(1);
    }

    return 0;
}

int lopcl_SetKernelArg(cl_int i, size_t tipo,const void *buffer){
	cl_int status;
	status = clSetKernelArg(lopcl_KERNEL, i, tipo, buffer);
    if (status != CL_SUCCESS) {
        printf ("Unable to set kernel argument\n");
				printf("Status %d\n",status);
        exit(1);
    }
    return 0;
}

int lopcl_clEnqueueNDRangeKernel(cl_uint work_dim, const size_t* global_offset, const size_t* globalWorkSize, const size_t*  localWorkSize, cl_mem buffer, cl_bool blocking_read, size_t datasize, void* ptr){
	cl_int status;
	status = clEnqueueNDRangeKernel(lopcl_CMDQUEUE, lopcl_KERNEL , 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
        printf ("Unable to execute the program\n");
        exit(1);
    }
	status = clEnqueueReadBuffer(lopcl_CMDQUEUE, buffer, blocking_read, 0, datasize, ptr, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf ("Unable to read the C buffer\n");
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
}
/*------------Funções de Get da plataforma-------------*/
//recuperar numero da plataforma
//Verifca a existencia da plataforma e se a biblioteca foi iniciada.
void verifica(int index){
	if(lopcl_INIT != 1){
		lopcl_Errors (1);
		exit(1);
	}
	if (listPlatforms[index] == -1 )
	{
		lopcl_Errors(2);
		exit(1);
	}
}

int lopcl_getNumPlatform(int index){
	verifica(index);
	return lopcl_DispPlats[listPlatforms[index]].numPlat;
}

//recuperar nome da plaforma
char *lopcl_getNamePlatform(int index){
	verifica(index);
	return lopcl_DispPlats[listPlatforms[index]].Name;
}

//Recuperar o vendor da plataforma
char *lopcl_getVendorPlatform(int index){
	verifica(index);
	return lopcl_DispPlats[listPlatforms[index]].Vendor;
}

//recuperar as extensões
char *lopcl_getExtensiosPlatform(int index){
	verifica(index);
	return lopcl_DispPlats[listPlatforms[index]].Extensions;
}
/*------------Funções de Get do device-------------*/

//Recuperar número do device
int lopcl_getNumDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].numDevice;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar nome do device
char* lopcl_getNameDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Name;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar vendor do device
char* lopcl_getVendorDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Vendor;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar vendorId do device
cl_uint* lopcl_getVendorIdDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].VendorId;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Profile do device
char* lopcl_getProfileDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Profile;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar available do device
cl_bool* lopcl_getAvailableDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Available;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Version do device
char* lopcl_getVersionDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Version;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}
//Recuperar Drive Version do device
char* lopcl_getDriveVersionDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].DriverVersion;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar compilerAvailable do device
cl_bool* lopcl_getCompilerAvailableDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].CompilerAvailable;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Adress Space do device
cl_uint* lopcl_getAdressSpaceDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].AdressSpace;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Endian do device
cl_bool* lopcl_getLitleEndianDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].LitleEndian;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar ErrorCorrection do device
cl_bool* lopcl_getErrorCorrectionDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].ErrorCorrection;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar AdressAlingment do device
cl_uint* lopcl_getAdressAlingmentDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].AdressAligment;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar SmallAlingment do device
cl_uint* lopcl_getSmallAlingmentDevice(int index, cl_device_type type){int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].SmallestAlignment;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar ResolutionTimer do device
size_t* lopcl_getResolutionTimerDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].ResolutionTimer;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxClock do device
cl_uint* lopcl_getMaxClockDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxClock;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxComputeUnits do device
cl_uint* lopcl_getMaxComputeUnitsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxComputeUnits;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxConstantArgs do device
cl_uint* lopcl_getMaxConstantArgsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxConstantArgs;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxBufferSize do device
cl_ulong* lopcl_getMaxBufferSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxBufferSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxMemAlocSize do device
cl_ulong* lopcl_getMaxMemAlocSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxMemAlocSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxParamSize do device
size_t* lopcl_getMaxParamSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxParamSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar exec_capabilities do device
cl_device_exec_capabilities* lopcl_getExecutionCapabilitiesDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].ExecutionCapabilities;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxGlobalMemSize do device
cl_ulong* lopcl_getMaxGlobalMemSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxGlobalMemSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxGlobalMemCacheSize do device
cl_ulong* lopcl_getMaxGlobalMemCacheSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxGlobalMemCacheSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar GlobalMemLineChaceSize do device
cl_uint* lopcl_getGlobalMemLineChaceSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].GlobalMemLineCacheSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxLocalMemSize do device
cl_ulong* lopcl_getMaxLocalMemSizeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxLocalMemSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar LocalMemType do device
cl_device_local_mem_type* lopcl_getLocalMemTypeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MemType;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}


//Recuperar CacheMemType do device
cl_device_mem_cache_type* lopcl_getCacheMemTypeDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].GlobalMemCahcheType;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxWorkGroups do device
size_t* lopcl_getMaxWorkGroupsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxWorkGroupSize;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxWorkItem do device
/*Modificar
size_t lopcl_getMaxWorkItemsDevice(int index, cl_device_type type);
*/
//Recuperar MaxWorkItemDimensions do device
cl_uint* lopcl_getMaxWorkItemsDimensionsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxWorkItemDimensions;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxWorkItemSizes do device
size_t* lopcl_getMaxWorkItemSizesDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxWorkItemSizes;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Max2dHeight do device
size_t* lopcl_getMax2dHeightDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Max2dHeight;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Max2dWidth do device
size_t* lopcl_getMax2dWidthDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Max2dWidth;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Max3dDepth do device
size_t* lopcl_getMax3dDepthDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Max3dDepth;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Max3dHeight do device
size_t* lopcl_getMax3dHeightDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Max3dHeight;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar Max3dWidth do device
size_t* lopcl_getMax3dWidthDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Max3dWidth;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxReadImageArgs do device
cl_uint* lopcl_getMaxReadImageArgsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxReadImageArgs;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxWriteImageArgs do device
cl_uint* lopcl_geMaxWriteImageArgsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxWriteImageArgs;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar MaxSamplers do device
cl_uint* lopcl_getMaxSamplersDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].MaxSamplers;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthChar do device
cl_uint* lopcl_getPreferredVectorWidthCharDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthChar;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthShort do device
cl_uint* lopcl_getPreferredVectorWidthShortDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthShort;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthInt do device
cl_uint* lopcl_getPreferredVectorWidthInttDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
			printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthInt;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthLong do device
cl_uint* lopcl_getPreferredVectorWidthLongDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthLong;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthFloat do device
cl_uint* lopcl_getPreferredVectorWidthFloatDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthFloat;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthDouble do device
cl_uint* lopcl_getPreferredVectorWidthDoubleDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].PreferredVectorWidthDouble;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* lopcl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].SinglePrecisionFlatCapability;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* lopcl_getDoublePrecisionFlatCapabilityDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].DoublePrecisionFlatCapability;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

//Recuperar PreferredVectorWidthDouble do device
char* lopcl_getExtensionsDevice(int index, cl_device_type type){
	int j;
	verifica(index);
	cl_int status = clGetDeviceIDs(lopcl_PLATFORMS[listPlatforms[index]], CL_DEVICE_TYPE_ALL, 0, NULL, &lopcl_NUM_DEVICES);
	if (status != CL_SUCCESS)
	{
		printf("Cannot get the number of OpenCL lopcl_DEVICES available on this platform.\n");
		printf("Status %d\n",status);
			exit(EXIT_FAILURE);
	}
	for (j = 0; j < lopcl_NUM_DEVICES; j++)
	{
		if(lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Type & type)
			return lopcl_DispPlats[listPlatforms[index]].MyDevices[j].Extensions;
		else{
			lopcl_Errors(3);
			exit(1);
		}
	}
}

int lopcl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM){
  	int error;
		//Inicializa todas as plataformas, para que os Macros de plataformas fiquem disponiveis.
  	error = lopcl_Initialize_Platforms();
    lopcl_Errors(error);

		/*Inicializa os dispositivos de uma plataforma especifica, para que os Macros de plataformas fiquem disponíveis.
		*As verificações de validade sobre a plataforma são feitas internenamente.
		*Função será executada somente se o programador escolher uma plataforma especifica.*/
		if(lopcl_PLATFORM_NUM < lopcl_ALL){
			error = lopcl_Initialize_Device(lopcl_PLATFORM_NUM);
			lopcl_Errors(error);
		}
		/*Função irá explorar as caracteristicas da(s) plataforma(s) e salvar essas informações
		* nas estruturas de dados correspondentes, permitindo a recuperação por meio das funções de get
		* ou por meio da função PrintInfo.
		*/
		error = lopcl_Explore(lopcl_PLATFORM_NUM);
    lopcl_Errors(error);

		/*Função responsável por criar contexto e fila de comandos.
		*recebe como parametro plataforma e dispositivos especificos.
		*O dispositivo não pode ter valor -1.
		*/
		if(lopcl_DEVICE_NUM > -1){
			if(listDevices[lopcl_DEVICE_NUM] < 0)
				lopcl_Errors(4);
			error = lopcl_CreateCmdQueue(lopcl_PLATFORM_NUM, lopcl_DEVICE_NUM);
			lopcl_Errors(error);
		}else if(lopcl_PLATFORM_NUM != lopcl_ALL){
			/*
			* A combinação lopcl_ALL, -1 é a usada para imprimir todas as informações referentes ao hardware,
			*	se  a flag lopcl_ALL não estiver sendo usada, o campo de device não poderá ser -1 em nenhuma circunstáncia.
			*/
			lopcl_Errors(5);
		}
    return 0;
}


int lopcl_Finalize(){
	free(lopcl_DispPlats);
}
