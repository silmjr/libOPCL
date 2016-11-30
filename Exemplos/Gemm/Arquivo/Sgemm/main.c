#pragma OPENCL EXTENSION cl_khr_fp64 : enable // Usar variáveis do tipo Double 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h> 
#include <libopcl.h>
#include <omp.h>


// includes do OpenCL 
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)
//Tamanho do work-Group
#define BSIZE 4

//Função para multiplicação de matrizes na CPU
void gemm_CPU(float *a, float* b, float *c, int size, float alfa, float beta);
//Função para multiplicação de matrizes usando OpenCL 
void gemm_OpenCL(float *a, float* b, float *c, int size, int t, float alfa, float beta); 
//Função para comparação das matrizes geradas
float comparaMatrizes(float* a, float* b, int size);
void printMatrix(float *a, int size);

int main(int argc, char *argv[])
{
    
    // Dados do Host 
    float *A = NULL;  // Array de entrada
    float *B = NULL;  // Array de entrada
    float *C_host = NULL;  // Array de saída
    float *C_device = NULL;  // Array de saída
    
    float start, stop, t_s, t_p;
    float error;
    
    float alfa = 0.3, beta = 0.4;
    // Tamanho da Matriz(square matrix)
    int mSize = 1024;
    if(argc >= 2)//Receber tamanho da matriz por linha de comando, caso não receber nada o tamanho padrão é 1024x1024
        mSize = atoi(argv[1]);
    
    // Elementos em cada array 
    const int elements = mSize*mSize;   
    
    // Calcula o tamanho dos dados 
    size_t datasize = sizeof(float)*elements;
    
    //Aloca espaço para os dados de Entrada/Saída
    A = (float*)malloc(datasize);
    B = (float*)malloc(datasize);
    C_host = (float*)malloc(datasize);
    C_device = (float*)malloc(datasize);
    
    // Incializa os dados de entrada
    for(int i = 0; i < elements; i++) {
        A[i] = 1.0;
        B[i] = 2.0;
        C_host[i] = 3.0;
        C_device[i] = 3.0;
    }

    //Execução do código Serial
    start = omp_get_wtime();
    gemm_CPU(A,B,C_host,mSize, alfa, beta);
    stop = omp_get_wtime();
    t_s = stop - start;
    printf("Result Ok! time - %f\n", t_s);
    
    //Execução do código Paralelo 
    start = omp_get_wtime();
    gemm_OpenCL(A, B, C_device, mSize, 0,  alfa,  beta);

    stop = omp_get_wtime();
    t_p = stop - start;
    
    //Comparação dos dados e de tempo dos dois códigos
    error = comparaMatrizes(C_host, C_device, mSize);
    printf("Error - %f\n", error);
    printf("Result Ok! time - %f\n", t_p);
    printf("Result Ok! speedup - %f\n", t_s/t_p);
    
    // Libera os recursos do Host
    free(A);
    free(B);
    free(C_host);
    free(C_device);
}


void gemm_CPU(float *a, float* b, float *c, int size, float alfa, float beta)
{
    int i, j, k;
    float cValue;
    
    for(i=0; i<size; i++)
        for(j=0; j<size; j++){
            cValue = c[i*size+j];
            for(k=0; k<size; k++)
                cValue += a[i*size+k]*b[k*size+j];
            c[i*size+j] = alfa* cValue + beta*c[i*size+j];
        }
        
        return;
}
void gemm_OpenCL(float *a, float* b, float *c, int size, int t, float alfa, float beta)
{
    // Elementos em cada array
    const int elements = size*size;   
    int error = 0;
    // Calcula o tamanho dos elementos 
    size_t datasize = sizeof(float)*elements;
    
    // Usado para checar a saída do OpenCL 
    cl_int status;  
      
     
    //Leitura do arquivo onde está o kernel 
    FILE *fp;
    char *source_str;
    size_t source_size;
    
    //Carregando o arquivo com o Kernel 
    fp = fopen("dgemm-kernelGlobal.cl", "r");
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
    
    //----------------------------------------------------------------------------------------------------------
    // STEP 1: Descobrir e inicializar as plataformas e Devices, e criar contexto e Fila de Comando 
    //----------------------------------------------------------------------------------------------------------

    lopcl_Init(lopcl_NVIDIA, lopcl_DEVICE_GPU);

    //----------------------------------------------------------------------------------------------------------
    // STEP 2: Criar buffers do device e Escrever os dados do host para os lopcl_DEVICES de buffers
    //----------------------------------------------------- -----------------------------------------------------
    
       
    cl_mem bufferA;  // Array de entrada do device
    cl_mem bufferB;  // Array de entrada do device
    cl_mem bufferC;  // Array de saída do device
    
    // Use lopcl_CreateBuffer() para criar o objeto (d_A) 
    // Isso contêm os dados do Array A do Host.
    bufferA = lopcl_CreateBuffer(datasize, CL_MEM_READ_ONLY, CL_FALSE, a);

    // Use lopcl_CreateBuffer() para criar o objeto (d_B) 
    // Isso contêm os dados do Array B do Host.
    bufferB = lopcl_CreateBuffer(datasize, CL_MEM_READ_ONLY, CL_FALSE, b);
    
    // Use lopcl_CreateBuffer() para criar o objeto (d_B) 
    // com espaço suficiente para "segurar" os dados de saída.
    bufferC = lopcl_CreateBuffer(datasize, CL_MEM_READ_WRITE, CL_FALSE, c);
        
    //---------------------------------------------------------------
    // STEP 3: Cria e compila o programa, e cria o kernel 
    //----------------------------------------------------- ---------

    if(t==0)
        lopcl_CreateProgram((const char**)&source_str, "dgemm");
    
        
    //-----------------------------------------------------
    // STEP 4: Define os argumentos do kernel 
    //----------------------------------------------------- 
        
    // Associa os buffers de entrada e saida ao kernel 
    // usando lopcl_SetKernelArg()
      
    lopcl_SetKernelArg(0, sizeof( int), &size);
    
    lopcl_SetKernelArg(1, sizeof(int), &size);
    
    lopcl_SetKernelArg(2, sizeof(int), &size);
    
    lopcl_SetKernelArg(3, sizeof(float), &alfa);

    lopcl_SetKernelArg(4, sizeof(cl_mem), &bufferA);

    lopcl_SetKernelArg(5, sizeof(int), &size);

    lopcl_SetKernelArg(6, sizeof(cl_mem), &bufferB);

    lopcl_SetKernelArg(7, sizeof(int), &size);

    lopcl_SetKernelArg(8, sizeof(float), &beta);

    lopcl_SetKernelArg(9, sizeof(cl_mem), &bufferC);

    lopcl_SetKernelArg(10, sizeof(int), &size);

    
    //--------------------------------------------------------------
    // STEP 5: Configurar a estrutura dos work itens e work groups
    //----------------------------------------------------- --------
    
    size_t globalWorkSize[2];    
    // Esses são os elementos de work-items 
    globalWorkSize[0] = size/32;
    globalWorkSize[1] = size/32;
    
    size_t localWorkSize[2];    
    
    localWorkSize[0] = BSIZE;
    localWorkSize[1] = BSIZE;
    
    //-----------------------------------------------------
    // STEP 6: Empilha o Kernel para execução
    //----------------------------------------------------- 
    
    // Execute the kernel by using 
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize' is the 1D dimension of the 
    // work-items
    status = clEnqueueNDRangeKernel(lopcl_CMDQUEUE, lopcl_KERNEL , 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
        
    //-----------------------------------------------------
    // STEP 7: Lê a saida do buffer de saída e salva no host
    //----------------------------------------------------- 
    
    status = clEnqueueReadBuffer(lopcl_CMDQUEUE, bufferC, CL_TRUE, 0, datasize, c, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        printf ("Unable to read the C buffer\n");
        exit(1);
    }
     
    //-----------------------------------------------------
    // STEP 8: libera os recursos 
    //----------------------------------------------------- 
    
    //Free OpenCL resources
    clReleaseKernel(lopcl_KERNEL );
    clReleaseProgram(lopcl_PROGRAM);
    clReleaseCommandQueue(lopcl_CMDQUEUE);
    clReleaseContext(lopcl_CONTEXT);
    clReleaseMemObject(bufferA); 
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    lopcl_Finalize();
    free(source_str);
    
    
    return;
     
}

float comparaMatrizes(float* a, float* b, int size)
{
    float max = 0;
    int i, tSize = size*size;
    for(i=0; i<tSize; i++)
        if(max < fabs(a[i] - b[i]))
            max =  fabs(a[i] - b[i]);
        
        return max;
    
}

void printMatrix(float *a, int size)
{
    int i,j;
    for(i=0;i<size;i++){
        for(j=0; j<size; j++)
            printf("%f ", a[i*size +j]);
        printf("\n");
    }
}
