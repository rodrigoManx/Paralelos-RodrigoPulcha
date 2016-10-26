#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <Windows.h>
#include <stdio.h>


__global__ 
void foo_kernel(float *R,float *S, const int nx, const int ny, const int nz)
{
	unsigned int ix = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int iy = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned int iz = blockIdx.z * blockDim.z + threadIdx.z;

	if ((ix < nx) && (iy < xIndex) && (iz < nz))
	{
		unsigned int index = ix+ nx*iy + nx*ny*iz;
		S[index] = R[index]*2;
	}
}

int main()
{
	float *A, *B,*d_A,*d_B;
	int nx, ny, nz;
	nx = ny = nz = 16;
	int size = nx * ny * nz * sizeof(float);
	A = (float *) malloc(size);
	B = (float *) malloc(size);
	for (int i = 0; i < nx*ny*nz; i++)
	{
		A[i] = i%10;
	}

	cudaMalloc((void **)&d_A,size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMalloc((void**)&d_B, size);

	int threadsInX = 8;
	int threadsInY = 8;
	int threadsInZ = 8;
	int blocksInX = (nx - 1) / threadsInX + 1;
	int blocksInY = (ny - 1) / threadsInY + 1;
	int blocksInZ = (nz - 1) / threadsInZ + 1;
	dim3 Dg = dim3(blocksInX, blocksInY, blocksInZ);
	dim3 Db = dim3(threadsInX, threadsInY, threadsInZ);
	foo_kernel <<<Dg,Db>>>(A,B, nx, ny, nz);
	system("PAUSE");
	return 0;
}