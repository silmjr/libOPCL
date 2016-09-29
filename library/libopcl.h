#ifndef LIBOPCL_H
#define LIBOPCL_H

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

//Index's de plataformas disponíveis
#define locl_INTEL 0
#define locl_INTEL_GPU 1
#define locl_AMD 2
#define locl_NVIDIA 3 
#define locl_POCL 4
#define locl_COPRTHR 5
#define locl_ALL 6
#define GPU CL_DEVICE_TYPE_GPU
#define CPU CL_DEVICE_TYPE_CPU
#define ACCELERATOR CL_DEVICE_TYPE_ACCELERATOR

//Vetor com os números das plataformas (Default é -1)
int listPlatforms[7];
//Tipos de Devices que existem 
int locl_DEVICE_CPU, locl_DEVICE_ACCELERATOR, locl_DEVICE_GPU; 
//Número total de plataformas 
cl_uint locl_NUM_PLATFORMS;
//Número total de devices de uma plataforma específica
cl_uint locl_NUM_DEVICES; //Fazer um vetor similar ao das plataformas
//Vetor de plataformas (Função interna do OPENCL)
cl_platform_id *locl_PLATFORMS;
//Vetor de devices (Função interna do OPENCL)
cl_device_id *locl_DEVICES;
//Contexto
cl_context locl_CONTEXT;
//Command queue
cl_command_queue locl_CMDQUEUE;
//Program
cl_program locl_PROGRAM;
//Kernel
cl_kernel locl_KERNEL;

//Estruturas onde serão armazenadas todas informações de Plataformas e Devices
typedef struct{
	int numDevice;
	char *Name;	
	char *Vendor;	
	cl_device_type Type;	
	cl_uint *VendorId;
	char *Profile;
	cl_bool *Available;
	char *Version;	
	char *DriverVersion;
	cl_bool *CompilerAvailable;
	cl_uint *AdressSpace;	
	cl_bool *LitleEndian;
	cl_bool *ErrorCorrection;
	cl_uint *AdressAligment;
	cl_uint *SmallestAlignment;
	size_t *ResolutionTimer;
	cl_uint *MaxClock;
	cl_uint * MaxComputeUnits;
	cl_uint *MaxConstantArgs;
	cl_ulong *MaxBufferSize;	
	cl_ulong *MaxMemAlocSize;	
	size_t *MaxParamSize;	
	cl_device_exec_capabilities *ExecutionCapabilities;
	cl_ulong *MaxGlobalMemSize;	
	cl_ulong *MaxGlobalMemCacheSize;	
	cl_uint *GlobalMemLineCacheSize;
	cl_ulong *MaxLocalMemSize;	
	cl_device_local_mem_type *MemType;	
	cl_device_mem_cache_type *GlobalMemCahcheType;
	size_t *MaxWorkGroupSize;
	size_t *MaxWorkItem;	
	cl_uint *MaxWorkItemDimensions;
	size_t *MaxWorkItemSizes;
	size_t *Max2dHeight;
	size_t *Max2dWidth;
	size_t *Max3dDepth;
	size_t *Max3dHeight;
	size_t *Max3dWidth;
	cl_uint *MaxReadImageArgs;
	cl_uint *MaxWriteImageArgs;
	cl_uint *MaxSamplers;
	cl_uint *PreferredVectorWidthChar;
	cl_uint *PreferredVectorWidthShort;
	cl_uint *PreferredVectorWidthInt;
	cl_uint *PreferredVectorWidthLong;
	cl_uint *PreferredVectorWidthFloat;
	cl_uint *PreferredVectorWidthDouble;
	cl_device_fp_config *SinglePrecisionFlatCapability;
	cl_device_fp_config *DoublePrecisionFlatCapability;
	char *Extensions;

}devices;

typedef struct{
	int numPlat;
	char *Name;	
	char *Vendor;	
	char *Version;
	char *Extensions;
	devices *MyDevices;	
}plataforms;

//Vetor de Estruturas
plataforms *locl_DispPlats;

/*locl_Init, função essencial para o funcionamento da plataforma, recebe como atributo os index's da(s) plataforma(s) e device(s) desejado(s), 
e é responsável por inicializar as plataformas, os devices, preencher as estruturas com as informações disponíveis na máquina, cria a fila de comando 
e o contexto para a plataforma e o device específicado. Caso o index seja locl_ALL e 0, não criará a fila de comando e o contexto*/
int locl_Init(int locl_PLATFORM_NUM, int locl_DEVICE_NUM);

/*Essa função libera a memória usada durante a execução do código*/
int locl_Finalize();

/*Iniciará todas as plataformas disponíveis no computador, essa função só é usada internamente pela locl_Initialize*/
int locl_Initialize_Platforms();

/*Inicia os devices de uma plataforma especifíca, recebe um index de plataforma como atributo e essa função só é usada internamente pela locl_Initialize*/
int locl_Initialize_Device(int index);

/*Grava todas as informações disponíveis na máquina nas estruturas essa função só é usada internamente pela locl_Initialize*/
int locl_Explore(int locl_PLATAFORM_NUMBER);

/*Cria o contexto e a fila de comando, recebe como atributo os index das plataformas 
e dos devices desejados essa função só é usada internamente pela locl_Initialize*/
int locl_CreateCmdQueue(int locl_PLATAFORM_NUMBER ,int locl_DEVICE_NUMBER);

/*Essa função é usada internamente pela locl_Explore, recebe como referencia a estrura de plataformas, device e tipo, tipo 0 só salva as informações
tipo 1 imprime todas as informações*/
int locl_ListDevice(plataforms *X, cl_device_id device, int tipo);

//Cria os buffers, recebe o tamanho do buffer, se é de leitura ou escrita, e o que se deseja colocar no buffer
cl_mem locl_CreateBuffer(size_t locl_DATASIZE, cl_mem_flags locl_FLAGS, cl_bool locl_FLAG1, void *a);

//Compila o kernel do programa, recebe o arquivo e o nome do kernel
int locl_CreateProgram(const char** source_str, char *kernel);

//Cria os argumentos do kernel, recebe um inteiro, que é a ordem em que o argumento foi criado no código, o tipo, e o que deseja  setar no kernel 
int locl_SetKernelArg(cl_int i, size_t tipo, const void* buffer);

//Lista os erros no programa 
void  locl_Errors(int i);

//Imprime as informações do dispositivo 
int locl_PrintInfo(int locl_PLATAFORM_NUMBER);

//Aux Functions
char *DiscStr(char *name);
int isEqual(char *name, char *name2);
void verifica(int index);

/*------------Funções de Get da plataforma-------------*/
//recuperar numero da plataforma
int locl_getNumPlatform(int index);

//recuperar nome da plaforma 
char *locl_getNamePlatform(int index);

//Recuperar o vendor da plataforma
char *locl_getVendorPlatform(int index);

//recuperar as extensões
char *locl_getExtensiosPlatform(int index);
/*------------Funções de Get do device-------------*/
//Recuperar número do device
int locl_getNumDevice(int index, cl_device_type type);

//Recuperar nome do device
char* locl_getNameDevice(int index, cl_device_type type);

//Recuperar vendor do device
char* locl_getVendorDevice(int index, cl_device_type type);

//Recuperar vendorId do device
cl_uint* locl_getVendorIdDevice(int index, cl_device_type type);

//Recuperar Profile do device
char* locl_getProfileDevice(int index, cl_device_type type);

//Recuperar available do device
cl_bool* locl_getAvailableDevice(int index, cl_device_type type);

//Recuperar Version do device
char* locl_getVersionDevice(int index, cl_device_type type);

//Recuperar Drive Version do device
char* locl_getDriveVersionDevice(int index, cl_device_type type);

//Recuperar compilerAvailable do device
cl_bool* locl_getCompilerAvailableDevice(int index, cl_device_type type);

//Recuperar Adress Space do device
cl_uint* locl_getAdressSpaceDevice(int index, cl_device_type type);

//Recuperar Endian do device
cl_bool* locl_getLitleEndianDevice(int index, cl_device_type type);

//Recuperar ErrorCorrection do device
cl_bool* locl_getErrorCorrectionDevice(int index, cl_device_type type);

//Recuperar AdressAlingment do device
cl_uint* locl_getAdressAlingmentDevice(int index, cl_device_type type);

//Recuperar SmallAlingment do device
cl_uint* locl_getSmallAlingmentDevice(int index, cl_device_type type);

//Recuperar ResolutionTimer do device
size_t* locl_getResolutionTimerDevice(int index, cl_device_type type);

//Recuperar MaxClock do device
cl_uint* locl_getMaxClockDevice(int index, cl_device_type type);

//Recuperar MaxComputeUnits do device
cl_uint* locl_getMaxComputeUnitsDevice(int index, cl_device_type type);

//Recuperar MaxConstantArgs do device
cl_uint* locl_getMaxConstantArgsDevice(int index, cl_device_type type);

//Recuperar MaxBufferSize do device
cl_ulong* locl_getMaxBufferSizeDevice(int index, cl_device_type type);

//Recuperar MaxMemAlocSize do device
cl_ulong* locl_getMaxMemAlocSizeDevice(int index, cl_device_type type);

//Recuperar MaxParamSize do device
size_t* locl_getMaxParamSizeDevice(int index, cl_device_type type);

//Recuperar exec_capabilities do device
cl_device_exec_capabilities* locl_getExecutionCapabilitiesDevice(int index, cl_device_type type);

//Recuperar MaxGlobalMemSize do device
cl_ulong* locl_getMaxGlobalMemSizeDevice(int index, cl_device_type type);

//Recuperar MaxGlobalMemCacheSize do device
cl_ulong* locl_getMaxGlobalMemCacheSizeDevice(int index, cl_device_type type);

//Recuperar GlobalMemLineChaceSize do device
cl_uint* locl_getGlobalMemLineChaceSizeDevice(int index, cl_device_type type);

//Recuperar MaxLocalMemSize do device
cl_ulong* locl_getMaxLocalMemSizeDevice(int index, cl_device_type type);

//Recuperar LocalMemType do device
cl_device_local_mem_type* locl_getLocalMemTypeDevice(int index, cl_device_type type);


//Recuperar CacheMemType do device
cl_device_mem_cache_type* locl_getCacheMemTypeDevice(int index, cl_device_type type);

//Recuperar MaxWorkGroups do device
size_t* locl_getMaxWorkGroupsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItem do device
size_t* locl_getMaxWorkItemsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItemDimensions do device
cl_uint* locl_getMaxWorkItemsDimensionsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItem do device
size_t* locl_getMaxWorkItemsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItemSizes do device
size_t* locl_getMaxWorkItemSizesDevice(int index, cl_device_type type);

//Recuperar Max2dHeight do device
size_t* locl_getMax2dHeightDevice(int index, cl_device_type type);

//Recuperar Max2dWidth do device
size_t* locl_getMax2dWidthDevice(int index, cl_device_type type);

//Recuperar Max3dDepth do device
size_t* locl_getMax3dDepthDevice(int index, cl_device_type type);

//Recuperar Max3dHeight do device
size_t* locl_getMax3dHeightDevice(int index, cl_device_type type);

//Recuperar Max3dWidth do device
size_t* locl_getMax3dWidthDevice(int index, cl_device_type type);

//Recuperar MaxReadImageArgs do device
cl_uint* locl_getMaxReadImageArgsDevice(int index, cl_device_type type);

//Recuperar MaxWriteImageArgs do device
cl_uint* locl_geMaxWriteImageArgsDevice(int index, cl_device_type type);

//Recuperar MaxSamplers do device
cl_uint* locl_getMaxSamplersDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthChar do device
cl_uint* locl_getPreferredVectorWidthCharDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthShort do device
cl_uint* locl_getPreferredVectorWidthShortDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthInt do device
cl_uint* locl_getPreferredVectorWidthInttDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthLong do device
cl_uint* locl_getPreferredVectorWidthLongDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthFloat do device
cl_uint* locl_getPreferredVectorWidthFloatDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_uint* locl_getPreferredVectorWidthDoubleDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* locl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* locl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
char* locl_getExtensionsDevice(int index, cl_device_type type);

#endif


