const char *source_global = 
"__kernel void dgemm(const int M, const int N, const int K,\n"
"                   const float alfa, __global const float *A, const int lda,\n"
                   "__global  const float *B, const int ldb, \n"
                   "const float beta, __global  float *C, const int ldc){\n"
    
    "// Get the index of the current element to be processed\n"
    "int i = get_global_id(0);\n"
    "int j = get_global_id(1);\n"
    "int k;\n"
    
    "float cvalue = C[i*ldc+j];\n"
    "for(k=0; k < K; k++)\n"
        "cvalue += A[i*lda+k]*B[k*ldb+j];\n"
    "C[i*ldc+j] = alfa*cvalue + beta*C[i*ldc+j];\n"    
"}\n";



const char *source_local = 
"__kernel void dgemm(const int M, const int N, const int K,\n"
                   "const float alfa, __global const float *A, const int lda,\n"
                   "__global const float *B, const int ldb, \n"
                   "const float beta,__global float *C, const int ldc){  \n"
    
    "__local float ads[32][32];\n"
    "__local float bds[32][32];\n"
    
    "// Get the index of the current element to be processed\n"
    "int i = get_global_id(0);\n"
    "int j = get_global_id(1);\n"
    "int qTx = get_local_id(0);\n"
    "int qTy = get_local_id(1);\n"
    
    "int lSize = get_local_size(0);\n"
    "int k, m;\n"
    
    "int quantBlocos = get_num_groups(0);\n"
    
    "float cvalue = C[i*lda+j];\n"
    
"    for(m=0; m<quantBlocos; m++)\n"
"    {\n"
"        ads[qTx][qTy] = A[i*lda + (m*lSize + qTy)];\n"
"        bds[qTx][qTy] = B[(m*lSize + qTx)*ldb + j];\n"
"        barrier(CLK_LOCAL_MEM_FENCE);\n"
"        for(k=0; k<lSize ; k++)\n"
"            cvalue += ads[qTx][k]*bds[k][qTy];\n"
"        barrier(CLK_LOCAL_MEM_FENCE);\n"
"    }\n"
"    C[i*ldc+j] = alfa*cvalue + beta*C[i*ldc+j];\n"
"}\n";

const char *source_private = 
"__kernel void dgemm(const int M, const int N, const int K,\n"
"                   const float alfa, const __global float *A, const int lda,\n"
"                   __global const float *B, const int ldb, \n"
"                   const float beta, __global float *C, const int ldc){\n"
"    __private float ath[32][32];\n"
"    __private float bth[32][32];\n"
"    __private float cth[32][32];\n"

"    // Get the index of the current element to be processed\n"
"    int i = 32*get_global_id(0);\n"
"    int j = 32*get_global_id(1);\n"
"    int k,m;\n"

"    int il,jl, ig, jg;\n"

"    for(il=0, ig = i; il<32; il++, ig++)\n"
"        for(jl = 0, jg = j; jl<32; jl++, jg++)\n"
"            cth[il][jl] = C[ig*ldc + jg];\n"
    
"    for(m=0; m < K; m+=32){\n"
        
"        for(il=0, ig = i; il<32; il++, ig++)\n"
"            for(jl = 0, jg = j; jl<32; jl++, jg++){\n"
"                ath[il][jl] = A[ig*lda + m +jl];\n"
"                bth[il][jl] = B[(m+il)*ldb + jg];\n"
"            }\n"

"        for(il = 0; il < 32; il++)\n"
"            for(jl = 0; jl < 32; jl++)\n"
"                for(k = 0; k < 32; k++)\n"
"                    cth[il][jl] += ath[il][k]*bth[k][jl];\n"

"    }         \n"

"    for(il=0, ig = i; il<32; il++, ig++){\n"
"        for(jl = 0, jg = j; jl<32; jl++, jg++){\n"
"            C[ig*ldc + jg] = alfa*cth[il][jl] + beta*C[ig*ldc + jg];   \n"
"        }\n"
"    }\n"
 
"    return; \n"
"        \n"
"}\n";