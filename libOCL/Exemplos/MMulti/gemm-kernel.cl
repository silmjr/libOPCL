
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void gemm_OpenCL(__global const double *A, __global const double *B, __global double *C) {
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);

    int k, size = get_global_size(0);
    
    double cvalue = C[i*size+j];
    for(k=0; k<size; k++)
        cvalue += A[i*size+k]*B[k*size+j];
    C[i*size+j] = cvalue;    
}
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void gemm_OpenCL_local(__global const double *A, __global const double *B, __global double *C) {
    
    __local double ads[4][4];
    __local double bds[4][4];
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    int qTx = get_local_id(0);
    int qTy = get_local_id(1);
    
    int lSize = get_local_size(0);
    int k, m, size = get_global_size(0);
    
    int quantBlocos = size/lSize;
    
    double cvalue = C[i*size+j];
    
    for(m=0; m<quantBlocos; m++)
    {
        ads[qTx][qTy] = A[i*size + (m*lSize + qTy)];
        bds[qTx][qTy] = B[(m*lSize + qTx)*size + j];
        barrier(CLK_LOCAL_MEM_FENCE);
        for(k=0; k<lSize ; k++)
            cvalue += ads[qTx][k]*bds[k][qTy];
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    C[i*size+j] = cvalue;    
}
