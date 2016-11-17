__kernel void dgemm(const int M, const int N, const int K,
                   const float alfa, const __global float *A, const int lda,
                   __global const float *B, const int ldb, 
                   const float beta, __global float *C, const int ldc){
    __private float ath[32][32];
    __private float bth[32][32];
    __private float cth[32][32];

    // Get the index of the current element to be processed
    int i = 32*get_global_id(0);
    int j = 32*get_global_id(1);
    int k,m;

    int il,jl, ig, jg;
    
    for(il=0, ig = i; il<32; il++, ig++)
        for(jl = 0, jg = j; jl<32; jl++, jg++)
            cth[il][jl] = C[ig*ldc + jg];
    
    for(m=0; m < K; m+=32){
        
        for(il=0, ig = i; il<32; il++, ig++)
            for(jl = 0, jg = j; jl<32; jl++, jg++){
                ath[il][jl] = A[ig*lda + m +jl];
                bth[il][jl] = B[(m+il)*ldb + jg];
            }
            
        for(il = 0; il < 32; il++)
            for(jl = 0; jl < 32; jl++)
                for(k = 0; k < 32; k++)
                    cth[il][jl] += ath[il][k]*bth[k][jl];
            
    }         
    
    for(il=0, ig = i; il<32; il++, ig++){
        for(jl = 0, jg = j; jl<32; jl++, jg++){
            C[ig*ldc + jg] = alfa*cth[il][jl] + beta*C[ig*ldc + jg];   
        }
    }
    
    return; 
        
}