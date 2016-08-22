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

//Index's de plataformas disponíveis
#define locl_INTEL 0
#define locl_INTEL_GPU 1
#define locl_AMD 2
#define locl_NVIDIA 3 
#define locl_POCL 4
#define locl_COPRTHR 5
#define locl_ALL 6
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

#endif


