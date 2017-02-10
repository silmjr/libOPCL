#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void dgemm(const int M, const int N, const int K,
                   const double alfa, __global const double *A, const int lda,
                   __global  const double *B, const int ldb, 
                   const double beta, __global  double *C, const int ldc){
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    int k;
    
    double cvalue = 0;
    for(k=0; k < K; k++)
        cvalue += A[i*lda+k]*B[k*ldb+j];
    C[i*ldc+j] = alfa*cvalue + beta*C[i*ldc+j];    
}


