// This program implements a GEMM using OpenCL

// System includes

#pragma OPENCL EXTENSION cl_khr_fp64 : enable // Usar variáveis Double 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h> 
#include "libocl.h"
#include <omp.h>


// OpenCL includes
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

#define BSIZE 4

void gemm_CPU(double *a, double* b, double *c, int size);// Serial 
void gemm_OpenCL(double *a, double* b, double *c, int size, int t); //OpenCl
double comparaMatrizes(double* a, double* b, int size);
void printMatrix(double *a, int size);

int main(int argc, char *argv[])
{
    
    // Host data
    double *A = NULL;  // Input array
    double *B = NULL;  // Input array
    double *C_host = NULL;  // Output array
    double *C_device = NULL;  // Output array
    
    double start, stop, t_s, t_p;
    double error;
    
    // Matriz size (square matrix)
    int mSize = 512;
    
    if(argc >= 2)//Receber tamanho da matriz por linha de comando 
        mSize = atoi(argv[1]);
    
    // Elements in each array
    const int elements = mSize*mSize;   
    
    // Compute the size of the data 
    size_t datasize = sizeof(double)*elements;
    
    // Allocate space for input/output data
    A = (double*)malloc(datasize);
    B = (double*)malloc(datasize);
    C_host = (double*)malloc(datasize);
    C_device = (double*)malloc(datasize);
    
    // Initialize the input data
    for(int i = 0; i < elements; i++) {
        A[i] = 1.0;
        B[i] = 2.0;
        C_host[i] = 3.0;
        C_device[i] = 3.0;
    }

    start = omp_get_wtime();
    gemm_CPU(A, B, C_host, mSize);
    stop = omp_get_wtime();
    t_s = stop - start;
    printf("Result Ok! time - %f\n", t_s);
    //printMatrix(C_host, mSize);
    
    start = omp_get_wtime();
    gemm_OpenCL(A, B, C_device, mSize, 0);

    stop = omp_get_wtime();
    t_p = stop - start;
    
    //printMatrix(C_device, mSize);
    error = comparaMatrizes(C_host, C_device, mSize);
    printf("Error - %f\n", error);
    printf("Result Ok! time - %f\n", t_p);
    printf("Result Ok! speedup - %f\n", t_s/t_p);
    
    // Free host resources
    free(A);
    free(B);
    free(C_host);
    free(C_device);
}


void gemm_CPU(double *a, double* b, double *c, int size)
{
    int i, j, k;
    double cValue;
    
    for(i=0; i<size; i++) 
        for(j=0; j<size; j++){
            cValue = c[i*size+j];
            for(k=0; k<size; k++)
                cValue += a[i*size+k]*b[k*size+j];
            c[i*size+j] = cValue;
        }
        
        return;
}

void gemm_OpenCL(double *a, double* b, double *c, int size, int t)
{
    // Elements in each array
    const int elements = size*size;   
    int error = 0;
    // Compute the size of the data 
    size_t datasize = sizeof(double)*elements;
    
    // Use this to check the output of each API call
    cl_int status;  
      
     
    /*Fazer Função para leitura do arquivo  Nome do Arquivo, e String onde vai ficar o código fonte*/
    FILE *fp;
    char *source_str;
    size_t source_size;
    
    //Carregando o arquivo com o Kernel 
    fp = fopen("gemm-kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    
    fseek(fp, 0, SEEK_END);//Envia o cabeçote de leitura para o fim do arquivo.  
    long fSize = ftell(fp);//Retorna o tamanho do arquivo em bytes. 
    rewind(fp);//Retorna a leitura para o inicio do arquivo.
    
    source_str = (char*)malloc((fSize+1)*sizeof(char));// Cria uma string com o tamanho do arquivo
    source_size = fread( source_str, 1, (fSize+1)*sizeof(char), fp);//Copia o conteúdo do kernel para a variável pré alocada
    source_str[fSize] = '\0';//Marca o fim do arquivo.
    fclose( fp );//Fecha o arquivo.
    
    //printf("%F\n", c[0]);
    
    //-----------------------------------------------------
    // STEP 1: Descobrir e inicializar as plataformase  e Devices.
    //-----------------------------------------------------
    locl_Init(locl_INTEL_GPU); 

    //-----------------------------------------------------
    // STEP 2: Create a locl_CONTEXT e Fila de Comando 
    //----------------------------------------------------- 
        /*Criar index dos devices*/
        
    error = locl_CreateCmdQueue(locl_DEVICE_GPU);
    locl_Errors(error);
    
    //-----------------------------------------------------
    // STEP 3: Criar buffers do device e Escrever os dados do host para os locl_DEVICES de buffers
    //----------------------------------------------------- 
    
    /*cl_mem clCreateBuffer(cl_locl_CONTEXT locl_CONTEXT,
                            cl_memflags flags, size_t size,
                            void* host_ptr,     
                            cl_int* errcode_ret) */
     
    cl_mem bufferA;  // Array de entrada do device
    cl_mem bufferB;  // Array de entrada do device
    cl_mem bufferC;  // Array de saída do device
    
    // Use clCreateBuffer() para criar o objeto (d_A) 
    // Isso contêm os dados do Array A do Host.
    bufferA = locl_CreateBuffer(datasize, CL_MEM_READ_ONLY, CL_FALSE, a);

    // Use clCreateBuffer() para criar o objeto (d_B) 
    // Isso contêm os dados do Array B do Host.
    bufferB = locl_CreateBuffer(datasize, CL_MEM_READ_ONLY, CL_FALSE, b);
    
    // Use clCreateBuffer() para criar o objeto (d_B) 
    // com espaço suficiente para "segurar" os dados de saída.
    bufferC = locl_CreateBuffer(datasize, CL_MEM_READ_ONLY, CL_FALSE, c);
        
    
    //-----------------------------------------------------
    // STEP 4: Create and compile the program
    //----------------------------------------------------- 
    locl_CreateProgram((const char**)&source_str);

    //-----------------------------------------------------
    // STEP 5: Create the kernel
    //----------------------------------------------------- 
    /*cl_kernel clCreateKernel(cl_program program,(Objeto do programa)
                               const char* kernel_name,(Nome da função definida no Codigo Fonte)
                               cl_int* errcode_ret) (local para armazenar erro de chamada)*/
    cl_kernel kernel = NULL;
    
    // Use clCreateKernel() to create a kernel from the 
    // vector addition function (named "vecadd")
    if(t==0)
        kernel = clCreateKernel(locl_program, "gemm_OpenCL", &status);
    else
        kernel = clCreateKernel(locl_program, "gemm_OpenCL_local", &status);
    if (status != CL_SUCCESS) {
        printf ("Unable to set a kernel from program\n");
        exit(1);
    }
    
    
    //-----------------------------------------------------
    // STEP 6: Set the kernel arguments
    //----------------------------------------------------- 
        
    // Associate the input and output buffers with the 
    // kernel 
    // using clSetKernelArg()
      
    /*cl_int clSetKernelArg(cl_kernel kernel,(Kernel cuja o argumento deve ser configurado)
                          cl_uint arg_index,(posição do arumento de acordo que foi definido no codigo fonte. Inicia em 0)
                          size_t arg_size,(comprimento de cada dado do argumento)
                          const void* arg_value)ponteiro para os dados do argumento.*/

    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    if (status != CL_SUCCESS) {
        printf ("Unable to set first kernel argument\n");
        exit(1);
    }
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    if (status != CL_SUCCESS) {
        printf ("Unable to set second kernel argument\n");
        exit(1);
    }
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    if (status != CL_SUCCESS) {
        printf ("Unable to set third kernel argument\n");
        exit(1);
    }
    
    //-----------------------------------------------------
    // STEP 7: Configure the work-item structure
    //----------------------------------------------------- 
    
    // Define an index space (global work size) of work 
    // items for execution.     A workgroup size (local work size) is not 
    // required, but can be used.
    size_t globalWorkSize[2];    
    // There are 'elements' work-items 
    globalWorkSize[0] = size;
    globalWorkSize[1] = size;
    
    size_t localWorkSize[2];    
    // There are 'elements' work-items 
    localWorkSize[0] = BSIZE;
    localWorkSize[1] = BSIZE;
    
    //-----------------------------------------------------
    // STEP 8: Enqueue the kernel for execution
    //----------------------------------------------------- 
    /*cl_int clEnqueueNDRangeKernel(cl_command_queue command_queue,(Fila de comandos)
                                    cl_kernel kernel,(Kernel a ser executado)
                                    cl_uint work_dim, (Dimensões dos espaços de índices)
                                    const size_t* global_work_offset, (array de deslocamentos para valores dos índices em cada dimensão)
                                    const size_t* global_work_size, (tamanho para cada dimensão do espaço de índices)
                                    const size_t* local_work_size, (array de tamanhos dos grupos de trabalho.)
                                    cl_uint events_in_wait_list,(número de eventos na lista de eventos)
                                    const cl_event* event_wait_list, 
                                    cl_event* event)*/

    // Execute the kernel by using 
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize' is the 1D dimension of the 
    // work-items
    status = clEnqueueNDRangeKernel(locl_CMDQUEUE, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);
    if (status != CL_SUCCESS) {
        printf ("Unable to run the kernel on NDRange\n");
        exit(1);
    }
    
    //-----------------------------------------------------
    // STEP 9: Read the output buffer back to the host
    //----------------------------------------------------- 
    /*cl_int clEnqueueReadBuffer(cl_queue queue,(fila de comandos)
                                 cl_mem buffer, (objeto de memória do tipo buffer a ser lido)
                                 cl_bool blocking_read, 
                                 size_t offset,(offset a partir do qual os dados serão transfferidos)
                                 size_t cb, (comprimentos em bytes dos dados a serem transferidos)
                                 const void* ptr, (região do host onde os dados serão escritos)
                                 cl_uint events_in_wait_list, 
                                 const cl_event* event_wait_list, 
                                 cl_event* event)*/
    
    // Use clEnqueueReadBuffer() to read the OpenCL output  
    // buffer (bufferC) 
    // to the host output array (C)
    status = clEnqueueReadBuffer(locl_CMDQUEUE, bufferC, CL_TRUE, 0, datasize, c, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf ("Unable to read the C buffer\n");
        exit(1);
    }
     
    //-----------------------------------------------------
    // STEP 10: Release OpenCL resources
    //----------------------------------------------------- 
    
    // Free OpenCL resources
    clReleaseKernel(kernel);
    clReleaseProgram(locl_program);
    clReleaseCommandQueue(locl_CMDQUEUE);
    clReleaseMemObject(bufferA); 
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseContext(locl_CONTEXT);
    
    free(source_str);
    free(locl_PLATFORMS);
    free(locl_DEVICES);
    
    return;
     
}

double comparaMatrizes(double* a, double* b, int size)
{
    double max = 0;
    int i, tSize = size*size;
    for(i=0; i<tSize; i++)
        if(max < fabs(a[i] - b[i]))
            max =  fabs(a[i] - b[i]);
        
        return max;
    
}

void printMatrix(double *a, int size)
{
    int i,j;
    for(i=0;i<size;i++){
        for(j=0; j<size; j++)
            printf("%f ", a[i*size +j]);
        printf("\n");
    }
}
