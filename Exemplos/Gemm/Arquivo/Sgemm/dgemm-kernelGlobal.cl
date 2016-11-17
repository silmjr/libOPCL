__kernel void dgemm(const int M, const int N, const int K,
                   const float alfa, __global const float *A, const int lda,
                   __global  const float *B, const int ldb, 
                   const float beta, __global  float *C, const int ldc){
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    int k;
    
    float cvalue = C[i*ldc+j];
    for(k=0; k < K; k++)
        cvalue += A[i*lda+k]*B[k*ldb+j];
    C[i*ldc+j] = alfa*cvalue + beta*C[i*ldc+j];    
}


