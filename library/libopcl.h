/****************************************************************************
 * Copyright (C) 2016 by SilvanO MOreira Junior                             *
 *                                                                          *
 * A libOPCL é uma biblioteca que visa facilitar o uso do OpenCL, 			*
 * ela foi criada em linguagem C e está disponível no GitHub				*
****************************************************************************/

/**
* @file libopcl.h
*  
* @author Silvano Moreira Junior
* @date 21 novembro 2016
* @brief Esse arquivo contém as definições e declarações das funções que compõe a biblioteca libOPCL
* @see https://github.com/silmjr/libOPCL
*/

/**
* @file libopcl.c
*  
* @author Silvano Moreira Junior
* @date 21 novembro 2016
* @brief Esse arquivo contém o código fonte de todas as funções que compõe a biblioteca libOPCL
* @see https://github.com/silmjr/libOPCL
*/


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

///Identificador atribuído a plataforma INTEL. 
#define lopcl_INTEL 0
///Identificador atribuído a plataforma INTEL GPU.
#define lopcl_INTEL_GPU 1
///Identificador atribuído a plataforma AMD.
#define lopcl_AMD 2
///Identificador atribuído a plataforma NVIDIA .
#define lopcl_NVIDIA 3 
///Identificador atribuído a plataforma POCL. 
#define lopcl_POCL 4
///Identificador atribuído a plataforma COPRTHR.
#define lopcl_COPRTHR 5
///Identificador atribuído para referencia a todas plataformas disponíveis em uma máquina.
#define lopcl_ALL 6

/// Macro para device do tipo GPU.
#define GPU CL_DEVICE_TYPE_GPU
/// Macro para device do tipo CPU.
#define CPU CL_DEVICE_TYPE_CPU
/// Macro para device do tipo ACCELERATOR.
#define ACCELERATOR CL_DEVICE_TYPE_ACCELERATOR

///Vetor responsável pelo armazenamento dos números das plataformas em uma máquina específica (Default é -1).
int listPlatforms[7];

///Identificador do dispositivos CPU. 
int lopcl_DEVICE_CPU,
///Identificador do dispositivos ACCELERATOR.
lopcl_DEVICE_ACCELERATOR, 
///Identificador do dispositivos GPU. 
lopcl_DEVICE_GPU; 

///Armazena o número total de plataformas em uma máquina.
cl_uint lopcl_NUM_PLATFORMS;
///Armazena número total de dispositivos de uma plataforma específica.
cl_uint lopcl_NUM_DEVICES; 
///Armazena um vetor de plataformas (Usado pelo OPENCL).
cl_platform_id *lopcl_PLATFORMS;
/// Armazena um Vetor de dispositivos (Usado pelo OPENCL).
cl_device_id *lopcl_DEVICES;
///Armazena um contexto para um dispositivo a ser escolhido.
cl_context lopcl_CONTEXT;
///Armazena uma fila de comando.
cl_command_queue lopcl_CMDQUEUE;
///Armazena o programa (kernel) compilado.
cl_program lopcl_PROGRAM;
///Armazena um kernel. 
cl_kernel lopcl_KERNEL;

/**
* @struct devices A estrutura é responsável por armazenar todas as informações sobre um dispositivo especificado.
*/
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
/**
* @struct devices A estrutura é responsável por armazenar todas as informações sobre uma plataforma especificada.
*/
typedef struct{
	int numPlat;
	char *Name;	
	char *Vendor;	
	char *Version;
	char *Extensions;
	devices *MyDevices;	
}plataforms;

///Vetor onde será armazenado todos os dados sobre plataformas e dispositivos presentes em uma máquina.
plataforms *lopcl_DispPlats;

/*!A função é responsável por inicializar as plataformas, os dispositivos, 
preencher as estruturas com as informações disponíveis na máquina, criar a fila de comando e o contexto para a plataforma e o dispositivo específicados. 
Caso o index seja lopcl_ALL e 0, não criará a fila de comando e o contexto. 
@param [in] int lopcl_PLATAFORM_NUM - Número inteiro que representa a plataforma escolhida.
<table> 
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
	<tr><th>lopcl_ALL	<th> Seleciona e incializa todas as plataformas disponíveis na máquina.
<\table>

@param [in] int lopcl_DEVICE_NUM - Número inteiro que representa o dispositivo escolhido.
<table> 
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_DEVICE_CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
<\table>
*/
int lopcl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM);

//!lopcl_Finaliza, função libera a memória usada durante a execução do programa.
int lopcl_Finalize(void);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todas plataformas disponiveis,
* atribuindo os valores correspondentes aos índices que correspondem as plataformas. Essa função é usada internamente pela função
* locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
*/
int lopcl_Initialize_Platforms(void);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todos os dispositivos de uma plataforma especificada,
* atribuindo os valores correspondentes aos índices que correspondem a esses dispostivos. Essa função é usada internamente pela função
* locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@param [in] int index - Número inteiro que representa a plataforma escolhida.
<table> 
	<caption id="index"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
	<tr><th>lopcl_ALL	<th> Seleciona e incializa todas as plataformas disponíveis na máquina.
<\table>
*/
int lopcl_Initialize_Device(int index);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todas as informações sobre as plataformas e dispositivos especificados,
* e armazenando esses dados nas estruturas correspondentes aos dispositivos e plataformas, tornando possível recuperar esses dados futuramente.
*	Essa função é usada internamente pela função locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@param [in] int index - Número inteiro que representa a plataforma escolhida.
<table> 
	<caption id="index"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
	<tr><th>lopcl_ALL	<th> Seleciona e incializa todas as plataformas disponíveis na máquina.
<\table>
*/
int lopcl_Explore(int lopcl_PLATAFORM_NUMBER);

/*!A função é responsável por criar o contexto OpenCL e a fila de comando. 
Essa função é usada internamente pela função locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM). 
@param [in] int lopcl_PLATAFORM_NUM - Número inteiro que representa a plataforma escolhida.
<table> 
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
	<tr><th>lopcl_ALL	<th> Seleciona e incializa todas as plataformas disponíveis na máquina.
<\table>

@param [in] int lopcl_DEVICE_NUM - Número inteiro que representa o dispositivo escolhido.
<table> 
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th>lopcl_DEVICE_CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
<\table>
*/
int lopcl_CreateCmdQueue(int lopcl_PLATAFORM_NUMBER ,int lopcl_DEVICE_NUMBER);

/** Função responsável por recuperar as informações dos dispositivos de uma plataforma especifica.
* A função é usada internenamente pela lopcl_Explore(int lopcl_PLATFORMA_NUMBER) e pela função int lopcl_PrintInfo(int lopcl_PLATAFORM_NUMBER);
@param [in] devices *X - Um ponteiro para uma estrutura de dispositivos. 
@param [in] cl_device_id *devices - A lista dos dispositivos OpenCL encontrados em uma plataforma.
@param int tipo
<table> 
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição    
	<tr><th> 0	<th> Faz uma varredura pela plataforma e armazena as informações do dispositivo na estrutura de dispositivos.
	<tr><th> 1	<th> Imprime todas as informações sobre o dispositivo que está disponível na estrtura. 	
<\table>
*/
int lopcl_ListDevice(devices *X, cl_device_id device, int tipo);

//Cria os buffers, recebe o tamanho do buffer, se é de leitura ou escrita, e o que se deseja colocar no buffer
cl_mem lopcl_CreateBuffer(size_t lopcl_DATASIZE, cl_mem_flags lopcl_FLAGS, cl_bool lopcl_FLAG1, void *a);

//Compila o kernel do programa, recebe o arquivo e o nome do kernel
int lopcl_CreateProgram(const char** source_str, char *kernel);

//Cria os argumentos do kernel, recebe um inteiro, que é a ordem em que o argumento foi criado no código, o tipo, e o que deseja  setar no kernel 
int lopcl_SetKernelArg(cl_int i, size_t tipo, const void* buffer);

//Lista os erros no programa 
void  lopcl_Errors(int i);

//Imprime as informações do dispositivo 
int lopcl_PrintInfo(int lopcl_PLATAFORM_NUMBER);

//Aux Functions
char *DiscStr(char *name);
int isEqual(char *name, char *name2);
void verifica(int index);

/*------------Funções de Get da plataforma-------------*/
//recuperar numero da plataforma
int lopcl_getNumPlatform(int index);

//recuperar nome da plaforma 
char *lopcl_getNamePlatform(int index);

//Recuperar o vendor da plataforma
char *lopcl_getVendorPlatform(int index);

//recuperar as extensões
char *lopcl_getExtensiosPlatform(int index);
/*------------Funções de Get do device-------------*/
//Recuperar número do device
int lopcl_getNumDevice(int index, cl_device_type type);

//Recuperar nome do device
char* lopcl_getNameDevice(int index, cl_device_type type);

//Recuperar vendor do device
char* lopcl_getVendorDevice(int index, cl_device_type type);

//Recuperar vendorId do device
cl_uint* lopcl_getVendorIdDevice(int index, cl_device_type type);

//Recuperar Profile do device
char* lopcl_getProfileDevice(int index, cl_device_type type);

//Recuperar available do device
cl_bool* lopcl_getAvailableDevice(int index, cl_device_type type);

//Recuperar Version do device
char* lopcl_getVersionDevice(int index, cl_device_type type);

//Recuperar Drive Version do device
char* lopcl_getDriveVersionDevice(int index, cl_device_type type);

//Recuperar compilerAvailable do device
cl_bool* lopcl_getCompilerAvailableDevice(int index, cl_device_type type);

//Recuperar Adress Space do device
cl_uint* lopcl_getAdressSpaceDevice(int index, cl_device_type type);

//Recuperar Endian do device
cl_bool* lopcl_getLitleEndianDevice(int index, cl_device_type type);

//Recuperar ErrorCorrection do device
cl_bool* lopcl_getErrorCorrectionDevice(int index, cl_device_type type);

//Recuperar AdressAlingment do device
cl_uint* lopcl_getAdressAlingmentDevice(int index, cl_device_type type);

//Recuperar SmallAlingment do device
cl_uint* lopcl_getSmallAlingmentDevice(int index, cl_device_type type);

//Recuperar ResolutionTimer do device
size_t* lopcl_getResolutionTimerDevice(int index, cl_device_type type);

//Recuperar MaxClock do device
cl_uint* lopcl_getMaxClockDevice(int index, cl_device_type type);

//Recuperar MaxComputeUnits do device
cl_uint* lopcl_getMaxComputeUnitsDevice(int index, cl_device_type type);

//Recuperar MaxConstantArgs do device
cl_uint* lopcl_getMaxConstantArgsDevice(int index, cl_device_type type);

//Recuperar MaxBufferSize do device
cl_ulong* lopcl_getMaxBufferSizeDevice(int index, cl_device_type type);

//Recuperar MaxMemAlocSize do device
cl_ulong* lopcl_getMaxMemAlocSizeDevice(int index, cl_device_type type);

//Recuperar MaxParamSize do device
size_t* lopcl_getMaxParamSizeDevice(int index, cl_device_type type);

//Recuperar exec_capabilities do device
cl_device_exec_capabilities* lopcl_getExecutionCapabilitiesDevice(int index, cl_device_type type);

//Recuperar MaxGlobalMemSize do device
cl_ulong* lopcl_getMaxGlobalMemSizeDevice(int index, cl_device_type type);

//Recuperar MaxGlobalMemCacheSize do device
cl_ulong* lopcl_getMaxGlobalMemCacheSizeDevice(int index, cl_device_type type);

//Recuperar GlobalMemLineChaceSize do device
cl_uint* lopcl_getGlobalMemLineChaceSizeDevice(int index, cl_device_type type);

//Recuperar MaxLocalMemSize do device
cl_ulong* lopcl_getMaxLocalMemSizeDevice(int index, cl_device_type type);

//Recuperar LocalMemType do device
cl_device_local_mem_type* lopcl_getLocalMemTypeDevice(int index, cl_device_type type);


//Recuperar CacheMemType do device
cl_device_mem_cache_type* lopcl_getCacheMemTypeDevice(int index, cl_device_type type);

//Recuperar MaxWorkGroups do device
size_t* lopcl_getMaxWorkGroupsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItem do device
size_t* lopcl_getMaxWorkItemsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItemDimensions do device
cl_uint* lopcl_getMaxWorkItemsDimensionsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItem do device
size_t* lopcl_getMaxWorkItemsDevice(int index, cl_device_type type);

//Recuperar MaxWorkItemSizes do device
size_t* lopcl_getMaxWorkItemSizesDevice(int index, cl_device_type type);

//Recuperar Max2dHeight do device
size_t* lopcl_getMax2dHeightDevice(int index, cl_device_type type);

//Recuperar Max2dWidth do device
size_t* lopcl_getMax2dWidthDevice(int index, cl_device_type type);

//Recuperar Max3dDepth do device
size_t* lopcl_getMax3dDepthDevice(int index, cl_device_type type);

//Recuperar Max3dHeight do device
size_t* lopcl_getMax3dHeightDevice(int index, cl_device_type type);

//Recuperar Max3dWidth do device
size_t* lopcl_getMax3dWidthDevice(int index, cl_device_type type);

//Recuperar MaxReadImageArgs do device
cl_uint* lopcl_getMaxReadImageArgsDevice(int index, cl_device_type type);

//Recuperar MaxWriteImageArgs do device
cl_uint* lopcl_geMaxWriteImageArgsDevice(int index, cl_device_type type);

//Recuperar MaxSamplers do device
cl_uint* lopcl_getMaxSamplersDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthChar do device
cl_uint* lopcl_getPreferredVectorWidthCharDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthShort do device
cl_uint* lopcl_getPreferredVectorWidthShortDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthInt do device
cl_uint* lopcl_getPreferredVectorWidthInttDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthLong do device
cl_uint* lopcl_getPreferredVectorWidthLongDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthFloat do device
cl_uint* lopcl_getPreferredVectorWidthFloatDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_uint* lopcl_getPreferredVectorWidthDoubleDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* lopcl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* lopcl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
char* lopcl_getExtensionsDevice(int index, cl_device_type type);

#endif


