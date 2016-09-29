 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include <libopcl.h>

#ifdef __APPLE__
	#include <OpenCL/cl.h>
#else
	#include <CL/cl.h>
#endif

int main(int argc, char *argv[]){	
	
   	
   	locl_Init(locl_ALL, -1);
	/*------------Funções de Get da plataforma-------------*/
	//recuperar numero da plataforma 
	printf("Num: %d\n",locl_getNumPlatform(locl_AMD));

	//recuperar nome da plaforma 
	printf("Nome: %s\n",locl_getNamePlatform(locl_AMD));

	//Recuperar o vendor da plataforma
	printf("Vendor: %s\n",locl_getVendorPlatform(locl_AMD));

	//recuperar as extensões
	printf("extensões: %s\n",locl_getExtensiosPlatform(locl_AMD));
	/*------------Funções de Get do device-------------*/
	//Recuperar número do device
	printf("Num Device: %d\n",locl_getNumDevice(locl_AMD, CPU));

	//Recuperar nome do device
	printf("Nome Device: %s\n",locl_getNameDevice(locl_AMD, CPU));

	//Recuperar vendor do device
	printf("vendor: %s\n",locl_getVendorDevice(locl_AMD, CPU));

	//Recuperar vendorId do device
	printf("vendorId: %u\n", locl_getVendorIdDevice(locl_AMD, CPU));

	//Recuperar Profile do device
	printf("Profile: %s\n",locl_getProfileDevice(locl_AMD, CPU));

	//Recuperar Version do device
	printf("Version: %s\n",locl_getVersionDevice(locl_AMD, CPU));

	//Recuperar Drive Version do device
	printf("Drive: %s\n",locl_getDriveVersionDevice(locl_AMD, CPU));

	//Recuperar Adress Space do device
	printf("Adress Space: %u\n", locl_getAdressSpaceDevice(locl_AMD, CPU));

	//Recuperar AdressAlingment do device
	printf("AdressAlingment: %u\n", locl_getAdressAlingmentDevice(locl_AMD, CPU));

	//Recuperar SmallAlingment do device
	printf("SmallAlingment: %u\n", locl_getSmallAlingmentDevice(locl_AMD, CPU));

	//Recuperar ResolutionTimer do device
	printf("ResolutionTimer: %zu\n", locl_getResolutionTimerDevice(locl_AMD, CPU));

	//Recuperar MaxClock do device
	printf("MaxClock: %u\n", locl_getMaxClockDevice(locl_AMD, CPU));

	//Recuperar MaxComputeUnits do device
	printf("MaxComputeUnits: %u\n", locl_getMaxComputeUnitsDevice(locl_AMD, CPU));

	//Recuperar MaxConstantArgs do device
	printf("MaxConstantArgs: %u\n", locl_getMaxConstantArgsDevice(locl_AMD, CPU));

	//Recuperar MaxBufferSize do device
	printf("MaxBufferSize: %llu\n", locl_getMaxBufferSizeDevice(locl_AMD, CPU));

	//Recuperar MaxMemAlocSize do device
	printf("MaxMemAlocSize: %llu\n", locl_getMaxMemAlocSizeDevice(locl_AMD, CPU));

	//Recuperar MaxParamSize do device
	printf("MaxParamSize: %zu\n", locl_getMaxParamSizeDevice(locl_AMD, CPU));

	//Recuperar MaxGlobalMemSize do device
	printf("MaxGlobalMemSize: %llu\n", locl_getMaxGlobalMemSizeDevice(locl_AMD, CPU));

	//Recuperar MaxGlobalMemCacheSize do device
	printf("MaxGlobalMemCacheSize: %llu\n", locl_getMaxGlobalMemCacheSizeDevice(locl_AMD, CPU));

	//Recuperar GlobalMemLineChaceSize do device
	printf("GlobalMemLineChaceSize %u\n", locl_getGlobalMemLineChaceSizeDevice(locl_AMD, CPU));

	//Recuperar MaxLocalMemSize do device
	printf("MaxLocalMemSize: %llu\n", locl_getMaxLocalMemSizeDevice(locl_AMD, CPU));

	//Recuperar MaxWorkGroups do device
	printf("MaxWorkGroups: %zu\n", locl_getMaxWorkGroupsDevice(locl_AMD, CPU));

	//Recuperar MaxWorkItem do device
	printf("MaxWorkItem: %zu\n", locl_getMaxWorkItemSizesDevice(locl_AMD, CPU));

	//Recuperar MaxWorkItemDimensions do device
	printf("MaxWorkItemDimensions: %u\n", locl_getMaxWorkItemsDimensionsDevice(locl_AMD, CPU));

	//Recuperar MaxWorkItemSizes do device
	printf("MaxWorkItemSizes: %zu\n", locl_getMaxWorkItemSizesDevice(locl_AMD, CPU));

	//Recuperar Max2dHeight do device
	printf("Max2dHeight: %zu\n", locl_getMax2dHeightDevice(locl_AMD, CPU));

	//Recuperar Max2dWidth do device
	printf("Max2dWidth: %zu\n", locl_getMax2dWidthDevice(locl_AMD, CPU));

	//Recuperar Max3dDepth do device
	printf("Max3dDepth: %zu\n", locl_getMax3dDepthDevice(locl_AMD, CPU));

	//Recuperar Max3dHeight do device
	printf("Max3dHeight: %zu\n", locl_getMax3dHeightDevice(locl_AMD, CPU));

	//Recuperar Max3dWidth do device
	printf("Max3dWidth: %zu\n", locl_getMax3dWidthDevice(locl_AMD, CPU));

	//Recuperar MaxReadImageArgs do device
	printf("MaxReadImageArgs: %u\n", locl_getMaxReadImageArgsDevice(locl_AMD, CPU));

	//Recuperar MaxWriteImageArgs do device
	printf("MaxWriteImageArgs: %u\n", locl_geMaxWriteImageArgsDevice(locl_AMD, CPU));

	//Recuperar MaxSamplers do device
	printf("MaxSamplers: %u\n", locl_getMaxSamplersDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthChar do device
	printf("PreferredVectorWidthChar: %u\n", locl_getPreferredVectorWidthCharDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthShort do device
	printf(" PreferredVectorWidthShort: %u\n", locl_getPreferredVectorWidthShortDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthInt do device
	printf("PreferredVectorWidthInt: %u\n", locl_getPreferredVectorWidthInttDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthLong do device
	printf("PreferredVectorWidthLong: %u\n", locl_getPreferredVectorWidthLongDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthFloat do device
	printf("PreferredVectorWidthFloat: %u\n", locl_getPreferredVectorWidthFloatDevice(locl_AMD, CPU));

	//Recuperar PreferredVectorWidthDouble do device
	printf("PreferredVectorWidthDouble: %u\n", locl_getPreferredVectorWidthDoubleDevice(locl_AMD, CPU));

	//Recuperar Extensions do device
	printf("Extensions: %s\n",locl_getExtensionsDevice(locl_AMD, CPU));

	locl_Finalize();

}           
                    
