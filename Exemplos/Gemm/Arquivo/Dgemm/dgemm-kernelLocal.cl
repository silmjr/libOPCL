#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void dgemm(const int M, const int N, const int K,
                   const double alfa, __global const double *A, const int lda,
                   __global const double *B, const int ldb, 
                   const double beta,__global double *C, const int ldc){  
    
    __local double ads[32][32];
    __local double bds[32][32];
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    int qTx = get_local_id(0);
    int qTy = get_local_id(1);
    
    int lSize = get_local_size(0);
    int k, m;
    
    int quantBlocos = get_num_groups(0);
    
    double cvalue = C[i*lda+j];
    
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