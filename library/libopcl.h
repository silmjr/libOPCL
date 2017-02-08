/****************************************************************************
 * Copyright (C) 2016 by SilvanO MOreira Junior   							*
 * Email: silvanomoreirajunior@gmail.com 			                        *
 *                                                                          *
 * A libOPCL é uma biblioteca que visa facilitar o uso do OpenCL, 			*
 * ela foi criada em linguagem C e está disponível no github				*
 * @see https://github.com/silmjr/libOPCL									*
*****************************************************************************/
/**
* \mainpage LibOPCL
* \section Introdução Introdução
* A LibOPCL foi criada com o objetivo de fazer uma interface de alto nível para o OpenCL,
* facilitando o desenvolvimento de algoritmos realmente portáveis utilizando desta ferramenta.
* A biblioteca foi desenvolvida na linguagem C e está disponível, como um projeto de software livre,
* via repositório no GitHub. Foram criadas funções para automatizar os processos de inicialização
* e preparação da memória nos dispositivos.  Adicionalmente foram desenvolvidos recursos para permitir
* a utilização mais eficientes das funcionalidades disponíveis na plataforma selecionada.
* \subsection Install Instalação:
* Clone o repositório da biblioteca: \n
* git clone https://github.com/silmjr/libOPCL \n
* cd  libOPCL/library \n
* make \n
* sudo make install
*\subsection uninstall Desinstalação:
* cd  libOPCL/library \n
* sudo make uninstall
*/

/**
*   @example ../Exemplos/Gemm/Arquivo/Sgemm/main.c
*	@brief Exemplo de multiplicação de matrizes utilizando a libOPCL
*/

/**
*   @example ../Exemplos/clinfo/main.c
*	@brief Exemplo de clinfo utilizando a libOPCL
*/


/**
* @file libopcl.h
*
* @author Silvano Moreira Junior
* @date 21 novembro 2016
* @brief Esse arquivo contém as definições e declarações das funções que compõe a biblioteca libOPCL
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
///Vetor responsável pelo armazenamento dos números dos dispositivos em uma máquina específica (Default é -1).
int listDevices[3];
///Identificador do dispositivos CPU.
#define lopcl_DEVICE_CPU 0
///Identificador do dispositivos ACCELERATOR.
#define lopcl_DEVICE_ACCELERATOR 1
///Identificador do dispositivos GPU.
#define lopcl_DEVICE_GPU 2

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
* @struct devices
* A estrutura é responsável por armazenar todas as informações sobre um dispositivo especificado.
*/
typedef struct{
	int numDevice;///< Número do dispositivo
	char *Name;	///< Nome do dispositivo
	char *Vendor; ///< Vendor do dispositivo
	cl_device_type Type; ///< Tipo do dispositivo
	cl_uint *VendorId;///< Vendor Id do dispositivo
	char *Profile; ///< Profile do dispositivo
	cl_bool *Available; ///< Disponibilidade do dispositivo
	char *Version;	///< Versão do OpenCL do dispositivo
	char *DriverVersion; ///< Versão do Driverdo dispositivo
	cl_bool *CompilerAvailable; ///< Disponibilidade do compilador do dispositivo
	cl_uint *AdressSpace;///< Tamanho da palavra de memória do dispositivo
	cl_bool *LitleEndian;///< Endian do dispositivo
	cl_bool *ErrorCorrection;///< Disponibilidade de correção de erros do dispositivo
	cl_uint *AdressAligment; ///< do dispositivo
	cl_uint *SmallestAlignment; ///< Maior tipo de dados suportados do dispositivo
	size_t *ResolutionTimer; ///< Resolução do timer do dispositivo
	cl_uint *MaxClock; ///< Clock máximo do dispositivo
	cl_uint * MaxComputeUnits; ///< Quantidade máxima de unidades de cômputo do dispositivo
	cl_uint *MaxConstantArgs; ///< Máximo de argumentos __constant no kernel do dispositivo
	cl_ulong *MaxBufferSize; ///< Tamanho máximo do buffer do dispositivo
	cl_ulong *MaxMemAlocSize;///< Tamanho máximo de alocação do dispositivo
	size_t *MaxParamSize;	///< Máximo de parametros que podem ser usados no kernel do dispositivo
	cl_device_exec_capabilities *ExecutionCapabilities; ///< recursos de execução do dispositivo
	cl_ulong *MaxGlobalMemSize;	///< Máximo de memória Global do dispositivo
	cl_ulong *MaxGlobalMemCacheSize; ///< Máximo de memória cache global do dispositivo
	cl_uint *GlobalMemLineCacheSize; ///< Tamanho da linha de mória chache globaldo dispositivo
	cl_ulong *MaxLocalMemSize;	///< Máximo de memória local do dispositivo
	cl_device_local_mem_type *MemType;	///< Tipo de memória local do dispositivo
	cl_device_mem_cache_type *GlobalMemCahcheType; ///< Tipo de memória global do dispositivo
	size_t *MaxWorkGroupSize; ///< Tamanho máximo de Work Groups do dispositivo
	size_t *MaxWorkItem; ///< Tamanho máximo de work items do dispositivo
	cl_uint *MaxWorkItemDimensions; ///< Máximo de work items por dimensão do dispositivo
	size_t *MaxWorkItemSizes; ///< Tamanho máximo de work itemsdo dispositivo
	size_t *Max2dHeight; ///< Altura máxima de uma imagem 2D no dispositivo
	size_t *Max2dWidth; ///< Largura máxima de uma imgagem 2D no dispositivo
	size_t *Max3dDepth;///< Profundidade máxima de uma imagem 3D no dispositivo
	size_t *Max3dHeight;///< Altura máxima de uma imagem 3D no dispositivo
	size_t *Max3dWidth;///< Largura máxima de uma imagem 3D no dispositivo
	cl_uint *MaxReadImageArgs; ///< Máximo de imagens lidas simultaneamente no dispositivo
	cl_uint *MaxWriteImageArgs; ///< Máximo de imagens escritas simultaneamente no dispositivo
	cl_uint *MaxSamplers;///< Número máximo de modelos que podem ser usados no Kernel do dispositivo
	cl_uint *PreferredVectorWidthChar; ///<  tamanho preferencial de um vetor de char no dispositivo
	cl_uint *PreferredVectorWidthShort;///<  tamanho preferencial de um vetor de Short no dispositivo
	cl_uint *PreferredVectorWidthInt;///<  tamanho preferencial de um vetor de Int no dispositivo
	cl_uint *PreferredVectorWidthLong;///<  tamanho preferencial de um vetor de Long no dispositivo
	cl_uint *PreferredVectorWidthFloat;///<  tamanho preferencial de um vetor de float no dispositivo
	cl_uint *PreferredVectorWidthDouble;///<  tamanho preferencial de um vetor de double no dispositivo
	cl_device_fp_config *SinglePrecisionFlatCapability;///<  recursos de precisão simples do dispositivo
	cl_device_fp_config *DoublePrecisionFlatCapability;///<  recursos de precisão dupla do dispositivo
	char *Extensions;///< Extensões do dispositivo

}devices;
/**
* @struct plataforms
* A estrutura é responsável por armazenar todas as informações sobre uma plataforma especificada.
*/
typedef struct{
	int numPlat;///< Número da plataforma
	char *Name;	///< Nome da plataforma
	char *Vendor;///< Vendor da plataforma
	char *Version;///< Versão da plataforma
	char *Extensions;///< Extensões da plataforma
	devices *MyDevices;	///< Ponteiro para dispositivos da plataforma
}plataforms;

///Vetor onde será armazenado todos os dados sobre plataformas e dispositivos presentes em uma máquina.
plataforms *lopcl_DispPlats;

/*!A função é responsável por inicializar as plataformas, os dispositivos,
preencher as estruturas com as informações disponíveis na máquina, criar a fila de comando e o contexto para a plataforma e o dispositivo específicados.
Caso o index seja lopcl_ALL e 0, não criará a fila de comando e o contexto.
@param [in] lopcl_PLATFORM_NUM - Número inteiro que representa a plataforma escolhida.
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
</table>

@param [in] lopcl_DEVICE_NUM - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_DEVICE_CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
	<tr><th> -1	<th> Usado em conjunto com o lopcl_ALL para que  se incialize todas plataformas e dispositivos presentes na máquina desejada.
</table>
@return int - Usado para verficação de erros
*/
int lopcl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM);

/**lopcl_Finaliza, função libera a memória usada durante a execução do programa.
@return int - Usado para verficação de erros
*/
int lopcl_Finalize(void);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todas plataformas disponiveis,
* atribuindo os valores correspondentes aos índices que correspondem as plataformas. Essa função é usada internamente pela função
* locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@return int - Usado para verficação de erros
*/
int lopcl_Initialize_Platforms(void);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todos os dispositivos de uma plataforma especificada,
* atribuindo os valores correspondentes aos índices que correspondem a esses dispostivos. Essa função é usada internamente pela função
* locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@param [in] index - Número inteiro que representa a plataforma escolhida.
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
</table>
@return int - Usado para verficação de erros

*/
int lopcl_Initialize_Device(int index);

/**
*Função responsável por fazer a varredura na máquina usada, descobrindo todas as informações sobre as plataformas e dispositivos especificados,
* e armazenando esses dados nas estruturas correspondentes aos dispositivos e plataformas, tornando possível recuperar esses dados futuramente.
*	Essa função é usada internamente pela função locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@param [in] lopcl_PLATAFORM_NUMBER - Número inteiro que representa a plataforma escolhida.
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
</table>
@return int - Usado para verficação de erros

*/
int lopcl_Explore(int lopcl_PLATAFORM_NUMBER);

/*!A função é responsável por criar o contexto OpenCL e a fila de comando.
Essa função é usada internamente pela função locl_Init(int lopcl_PLATFORM_NUM, int lopcl_DEVICE_NUM).
@param [in] lopcl_PLATAFORM_NUMBER - Número inteiro que representa a plataforma escolhida.
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
</table>

@param [in] lopcl_DEVICE_NUMBER - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_DEVICE_CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th>lopcl_DEVICE_ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return int - Usado para verficação de erros

*/
int lopcl_CreateCmdQueue(int lopcl_PLATAFORM_NUMBER ,int lopcl_DEVICE_NUMBER);

/** Função responsável por recuperar as informações dos dispositivos de uma plataforma especifica.
* A função é usada internenamente pela lopcl_Explore(int lopcl_PLATFORMA_NUMBER) e pela função int lopcl_PrintInfo(int lopcl_PLATAFORM_NUMBER);
@param [in] *X - Um ponteiro para uma estrutura de dispositivos.
@param [in] device - A lista dos dispositivos OpenCL encontrados em uma plataforma.
@param [in] tipo - Escolhe qual operação será feita
<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> 0	<th> Faz uma varredura pela plataforma e armazena as informações do dispositivo na estrutura de dispositivos.
	<tr><th> 1	<th> Imprime todas as informações sobre o dispositivo que está disponível na estrtura.
</table>
@return int - Usado para verficação de erros

*/
int lopcl_ListDevice(devices *X, cl_device_id device, int tipo);


/** Função responsável por criar os buffers de memória, empilhar, e tornar disponível ao dispositivo os dados do hospedeiro.
@param [in] lopcl_DATASIZE - Tamanho em bytes do objeto de memória que será usado.
@param [in] lopcl_FLAGS - Campo de bits que é usado pra especificar informações sobre alocação e uso, como a área de memória deve ser usada para alocar o elemento.
<table>
	<caption id="lopcl_FLAGS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CL_MEM_READ_WRITE	<th> Essa flag especifica que a memória será de leitura e escrita para o kernel. Essa é o padrão.
	<tr><th> CL_MEM_WRITE_ONLY	<th> Essa flag especifica que a memória será somente de escrita para o kernel. CL_MEM_READ_WRITE e CL_MEM_WRITE_ONLY são mutuamente exclusivas.
	<tr><th> CL_MEM_READ_ONLY	<th> Essa flag especica que a memória será somente de leitura para o kernel. CL_MEM_READ_WRITE e CL_MEM_READ_ONLY são mutuamente exclusivas.
	<tr><th> CL_MEM_USE_HOST_PTR	<th> Essa flag só é valida se o host_ptr não for NULL. Se usada, a flag especifica que a memória referencida será usada
	como bits de armazenamento para um objeto de memória.
	<tr><th> CL_MEM_ALLOC_HOST_PTR	<th> Essa flag especifica que a aplicação deseja que o OpenCL aloque a memória de modo acessível ao host. CL_MEM_USE_HOST_PTR e CL_MEM_ALLOC_HOST_PTR são mutuamente exclusivas.
	<tr><th> CL_MEM_COPY_HOST_PTR	<th> Essa flag só é valida se o host_ptr não for NULL. Se usada, a flag especifica que a aplicação deseja que
	a memória seja alocada e copiada para o objeto de memória.
	<tr><th> CL_MEM_HOST_WRITE_ONLY	<th> Essa flag especifica que somente o host pode escrever no objeto de memória.
	<tr><th> CL_MEM_HOST_READ_ONLY	<th> > Essa flag especifica que somente o host pode ler o objeto de memória.
	<tr><th> CL_MEM_HOST_NO_ACCESS	<th> > Essa flag especifica que o host não poderá ler ou escrever no objeto de memória.
</table>
@param lopcl_FLAG1 - Indica se a operação de escrita está bloqueada ou não
@param [in] *host_ptr - Um ponteiro para um buffer de dados que já foi alocado pela aplicação.
@return cl_mem - Que é o onjeto de memória que será usado pela aplicação
@see https://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/
*/

cl_mem lopcl_CreateBuffer(size_t lopcl_DATASIZE, cl_mem_flags lopcl_FLAGS, cl_bool lopcl_FLAG1, void *host_ptr);

/**Cria um objeto Program para um contexto previamente criado, e carrega o código fonte que é especificado a partir de um array de strings.
*Esta função também constroi (linka e compila) um programa executável para o código fonte do kernel ou seu binário.
@param  [in] **source_str - Um array de ponteiros para sequências de caracteres,  opcionalmente terminadas por null que compõe o código fonte do kernel.
@param kernel - Uma string que será o nome que foi dado a função de kernel
@return int - Usado para verficação de erros
@see https://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/

*/
int lopcl_CreateProgram(const char** source_str, char *kernel);


/** Função usada pra setar o valor de um argumento para um kernel específico.
@param [in] i - Ordem em que o argumento é listado na função de kernel
@param [in] tipo - tamanho em bytes do argumento
@param [in] *buffer - ponteiro para o argumento que se deseja setar na função de kernel.

@return int - Usado para verficação de erros
*/
int lopcl_SetKernelArg(cl_int i, size_t tipo, const void* buffer);
/**Função responsável por enviar o kernel para execução, ler o resultado da operação, e transferir os dados de volta ao hospedeiro.
@param [in] work_dim - Dimensões do espaço de índices.
@param [in] global_offset - Array de deslocamentos para valores dos índices em cada dimensão.
@param [in] globalWorkSize - tamanho para cada dimensão do espaço de índices.
@param [in] localWorkSize - Arrau de tamanhos dos grupos de trabalho. 
@param [in] buffer - objeto de memória do tipo buffer a ser lido.
@param [in] blocking_read - Define se a leitura é blockeada ou não. 
<table>
	<caption id="blocking_read"></caption>
	<tr><th>Operador                      <th>	Descrição
	<tr><th>cl_TRUE	<th> Booleano correspondete a verdadeiro.
	<tr><th>cl_FALSE <th> Booleano correspondete a falso.
</table>
@param [in] datasize - comprimentos em bytes dos dados a serem transferidos
@param [in] ptr - região do host onde os dados serão escritos
@return int - Usado para verficação de erros
*/
int lopcl_EnqueueNDRangeKernel(cl_uint work_dim, const size_t* global_offset, const size_t* globalWorkSize, const size_t*  localWorkSize, cl_mem buffer, cl_bool blocking_read, size_t datasize, void* ptr);

/** Função lista os erros que podem acontecer usando as funções da biblioteca
@param [in] i - Inteiro com o numero do erro retornado, por alguma função.
*/
void  lopcl_Errors(int i);

/*!A função é responsável Imprimir todas as informações disponíveis sobre a(s) plataformas especificadas,
@param [in] lopcl_PLATAFORM_NUMBER - Número inteiro que representa a plataforma escolhida.
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
</table>
@return int - Usado para verficação de erros
*/
int lopcl_PrintInfo(int lopcl_PLATAFORM_NUMBER);

///Função auxiliar de uso interno da biblioteca
char *DiscStr(char *name);
///Função auxiliar de uso interno da biblioteca
int isEqual(char *name, char *name2);
///Função auxiliar de uso interno da biblioteca
void verifica(int index);

/*------------Funções de Get da plataforma-------------*/
/**Função responsável por recuperar o número da plataforma especificada
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@return int - retorna a informação solicitada
*/
int lopcl_getNumPlatform(int index);

/**Função responsável por recuperar o nome da plataforma especificada
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char *lopcl_getNamePlatform(int index);

/**Função responsável por recuperar o Vendor da plataforma especificada
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char *lopcl_getVendorPlatform(int index);

/**Função responsável por recuperar as extensões da plataforma especificada
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char *lopcl_getExtensiosPlatform(int index);
/*------------Funções de Get do device-------------*/
/**Função responsável por recuperar o número do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return int - retorna a informação solicitada
*/
int lopcl_getNumDevice(int index, cl_device_type type);

/**Função responsável por recuperar o nome do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getNameDevice(int index, cl_device_type type);

/**Função responsável por recuperar o Vendor do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getVendorDevice(int index, cl_device_type type);

/**Função responsável por recuperar o Vendor ID do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getVendorIdDevice(int index, cl_device_type type);

/**Função responsável por recuperar o Profile do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getProfileDevice(int index, cl_device_type type);

/**Função responsável por recuperar se o dispositivo especificado está disponível
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_bool - retorna a informação solicitada
*/
cl_bool* lopcl_getAvailableDevice(int index, cl_device_type type);

/**Função responsável por recuperar a vVersão do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getVersionDevice(int index, cl_device_type type);

/**Função responsável por recuperar a versão do drive do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getDriveVersionDevice(int index, cl_device_type type);

/**Função responsável por recuperar se o compilado do dispositivo especificado está
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_bool - retorna a informação solicitada
*/
cl_bool* lopcl_getCompilerAvailableDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho da palavra de memória do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getAdressSpaceDevice(int index, cl_device_type type);

/**Função responsável por recuperar se o dispositivo especificado é litlle ou big endian
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_bool - retorna a informação solicitada
*/cl_bool* lopcl_getLitleEndianDevice(int index, cl_device_type type);

/**Função responsável por recuperar se a correção de erros está disponível no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_bool - retorna a informação solicitada
*/
cl_bool* lopcl_getErrorCorrectionDevice(int index, cl_device_type type);

/**Função responsável por recuperar o valor do maior tipo de dados suportado no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in]  type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getAdressAlingmentDevice(int index, cl_device_type type);

/**Função responsável por recuperar o menor alinhamento de dados que pode ser usado no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in]  type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getSmallAlingmentDevice(int index, cl_device_type type);

/**Função responsável por recuperar a resolução do timer do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getResolutionTimerDevice(int index, cl_device_type type);

/**Função responsável por recuperar o clock máximo do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxClockDevice(int index, cl_device_type type);

/**Função responsável por recuperar o máximo de unidades de cômputo do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxComputeUnitsDevice(int index, cl_device_type type);

/**Função responsável por recuperar a quantidade máxima de argumentos __constant no kernel em um dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxConstantArgsDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho máximo (em bytes) disponível para alocação __constant buffers no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_ulong - retorna a informação solicitada
*/
cl_ulong* lopcl_getMaxBufferSizeDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho máximo (em bytes) disponível para alocação de dados no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>

@return cl_ulong - retorna a informação solicitada
*/
cl_ulong* lopcl_getMaxMemAlocSizeDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho máximo (em bytes) de argumentos que podem ser enviados para o kernel no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMaxParamSizeDevice(int index, cl_device_type type);

/**Função responsável por recuperar os recursos de execução do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_device_exec_capabilities - retorna a informação solicitada
*/
cl_device_exec_capabilities* lopcl_getExecutionCapabilitiesDevice(int index, cl_device_type type);

/**Função responsável por recuperar o máximo de memória global do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_ulong - retorna a informação solicitada
*/
cl_ulong* lopcl_getMaxGlobalMemSizeDevice(int index, cl_device_type type);


/**Função responsável por recuperar o máximo de memória global cache do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_ulong - retorna a informação solicitada
*/
cl_ulong* lopcl_getMaxGlobalMemCacheSizeDevice(int index, cl_device_type type);


/**Função responsável por recuperar o tamanho da linha de cache de memória global (em bytes) do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_ulong - retorna a informação solicitada
*/
cl_uint* lopcl_getGlobalMemLineChaceSizeDevice(int index, cl_device_type type);

/**Função responsável por recuperar o máximo de memória local do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_ulong - retorna a informação solicitada
*/
cl_ulong* lopcl_getMaxLocalMemSizeDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tipo de memória suportada no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_device_local_mem_type - retorna a informação solicitada
*/
cl_device_local_mem_type* lopcl_getLocalMemTypeDevice(int index, cl_device_type type);



/**Função responsável por recuperar o tipo de memória cache suportada no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_device_mem_cache_type - retorna a informação solicitada
*/
cl_device_mem_cache_type* lopcl_getCacheMemTypeDevice(int index, cl_device_type type);


/**Função responsável por recuperar o máximo de work groups do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMaxWorkGroupsDevice(int index, cl_device_type type);


/**Função responsável por recuperar o máximo de work items do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMaxWorkItemsDevice(int index, cl_device_type type);

/**Função responsável por recuperar a dimensão máxima dos work items do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxWorkItemsDimensionsDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho máximo dos work items do dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMaxWorkItemSizesDevice(int index, cl_device_type type);

/**Função responsável por recuperar a altura máxima de uma imagem em 2D no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMax2dHeightDevice(int index, cl_device_type type);

/**Função responsável por recuperar a largura máxima de uma imagem em 2D no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMax2dWidthDevice(int index, cl_device_type type);

/**Função responsável por recuperar a profundidade máxima de uma imagem em 2D no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMax3dDepthDevice(int index, cl_device_type type);

/**Função responsável por recuperar a altura máxima de uma imagem em 2D no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMax3dHeightDevice(int index, cl_device_type type);

/**Função responsável por recuperar a largura máxima de uma imagem em 2D no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
size_t* lopcl_getMax3dWidthDevice(int index, cl_device_type type);

/**Função responsável por recuperar a número máximo de uma imagem que podem ser lidas simultaneamente no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxReadImageArgsDevice(int index, cl_device_type type);

/**Função responsável por recuperar a número máximo de uma imagem que podem ser escritas simultaneamente no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/

cl_uint* lopcl_geMaxWriteImageArgsDevice(int index, cl_device_type type);

/**Função responsável por recuperar a número máximo de modelos que podem ser usados em um kernel no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return size_t - retorna a informação solicitada
*/
cl_uint* lopcl_getMaxSamplersDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de char no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getPreferredVectorWidthCharDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de short no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/

cl_uint* lopcl_getPreferredVectorWidthShortDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de int no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getPreferredVectorWidthInttDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de long no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getPreferredVectorWidthLongDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de float no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getPreferredVectorWidthFloatDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de double no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_uint* lopcl_getPreferredVectorWidthDoubleDevice(int index, cl_device_type type);

/**Função responsável por recuperar o tamanho preferencial de um vetor de short no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return cl_uint - retorna a informação solicitada
*/
cl_device_fp_config* lopcl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

//Recuperar PreferredVectorWidthDouble do device
cl_device_fp_config* lopcl_getSinglePrecisionFlatCapabilityDevice(int index, cl_device_type type);

/**Função responsável por recuperar as extensões disponíves no dispositivo especificado
	@param [in] index - Número inteiro que representa a plataforma escolhida.
	<table>
	<caption id="lopcl_PLATFORMS"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th>lopcl_INTEL	<th> Seleciona e incializa a plataforma Intel, se disponível na máquina.
	<tr><th>lopcl_INTEL_GPU	<th> Seleciona e incializa a plataforma Intel GPU, se disponível na máquina.
	<tr><th>lopcl_AMD	<th> Seleciona e incializa a plataforma AMD, se disponível na máquina.
	<tr><th>lopcl_NVIDIA	<th> Seleciona e incializa a plataforma NVIDIA, se disponível na máquina.
	<tr><th>lopcl_POCL	<th> Seleciona e incializa a plataforma POCL, se disponível na máquina.
	<tr><th>lopcl_COPRTHR	<th> Seleciona e incializa a plataforma COPRTHR, se disponível na máquina.
</table>
@param [in] type - Número inteiro que representa o dispositivo escolhido.
<table>
	<caption id="lopcl_DEVICES"></caption>
	<tr><th>Identificador                      <th>	Descrição
	<tr><th> CPU	<th> Seleciona e incializa o dispositivo do tipo CPU, se disponível na máquina.
	<tr><th> GPU	<th> Seleciona e incializa o dispositivo do tipo GPU, se disponível na máquina.
	<tr><th> ACCELERATOR	<th> Seleciona e incializa o dispositivo do tipo ACCELERATOR, se disponível na máquina.
</table>
@return char - retorna a informação solicitada
*/
char* lopcl_getExtensionsDevice(int index, cl_device_type type);

#endif
