__kernel void dgemm(const int M, const int N, const int K,
                   const float alfa, __global const float *A, const int lda,
                   __global const float *B, const int ldb, 
                   const float beta,__global float *C, const int ldc){  
    
    __local float ads[32][32];
    __local float bds[32][32];
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    int qTx = get_local_id(0);
    int qTy = get_local_id(1);
    
    int lSize = get_local_size(0);
    int k, m;
    
    int quantBlocos = get_num_groups(0);
    
    float cvalue = C[i*lda+j];
    
    for(m=0; m<quantBlocos; m++)
    {
        ads[qTx][qTy] = A[i*lda + (m*lSize + qTy)];
        bds[qTx][qTy] = B[(m*lSize + qTx)*ldb + j];
        barrier(CLK_LOCAL_MEM_FENCE);
        for(k=0; k<lSize ; k++)
            cvalue += ads[qTx][k]*bds[k][qTy];
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    C[i*ldc+j] = alfa*cvalue + beta*C[i*ldc+j];
}