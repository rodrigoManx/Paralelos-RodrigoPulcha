#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <Windows.h>
#include <stdio.h>
 
__global__ void vecAdd(double *a, double *b, double *c, int n)
{
    int id = blockIdx.x*blockDim.x+threadIdx.x;
 
    if (id < n)
        c[id] = a[id] + b[id];
}
 
int main( int argc, char* argv[] )
{

    int n = 100000;
 
    double *h_a;
    double *h_b;
    double *h_c;
 
    double *d_a;
    double *d_b;
    double *d_c;
 
    size_t bytes = n*sizeof(double);
 
    h_a = (double*)malloc(bytes);
    h_b = (double*)malloc(bytes);
    h_c = (double*)malloc(bytes);
 
    cudaMalloc(&d_a, bytes);
    cudaMalloc(&d_b, bytes);
    cudaMalloc(&d_c, bytes);
 
    int i;
    for( i = 0; i < n; i++ ) {
        h_a[i] = sin(i)*sin(i);
        h_b[i] = cos(i)*cos(i);
    }
 
    cudaMemcpy( d_a, h_a, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy( d_b, h_b, bytes, cudaMemcpyHostToDevice);
 
    int blockSize, gridSize;

    vecAdd<<<ceil(n/1024.0), 1024>>>(d_a, d_b, d_c, n);
 
    cudaMemcpy( h_c, d_c, bytes, cudaMemcpyDeviceToHost );
 
    double sum = 0;
    for(i=0; i<n; i++)
        sum += h_c[i];
    printf("final result: %f\n", sum/n);
 
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
 
    free(h_a);
    free(h_b);
    free(h_c);
 
    return 0;
}