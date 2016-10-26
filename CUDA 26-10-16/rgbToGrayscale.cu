__global__
void rgba_to_greyscale(const uchar4* const rgbaImage, unsigned char* const greyImage, int numRows, int numCols)
{
	int x = (blockIdx.x * blockDim.x) + threadIdx.x;
 	int y = (blockIdx.y * blockDim.y) + threadIdx.y;

  	if (x < rows && y < cols) 
    	{ 
        	int idx = c + cols * r;
        	uchar4 pixel    = rgba[idx]; 
        	float intensity = 0.2126f * pixel.x + 0.7152f * pixel.y + 0.0722f * pixel.z; 
        	gray[idx]        = (unsigned char)intensity; 
    	} 

}

int main(int argc, char ** argv)
{
	if (argc > 2) 
    	{ 
        	imagePath = string(argv[1]); 
        	outputPath = string(argv[2]); 
    	} 

}
void your_rgba_to_greyscale(const uchar4 * const h_rgbaImage, uchar4 * const d_rgbaImage, unsigned char* const d_greyImage, size_t numRows, size_t numCols)
{
 	const dim3 blockSize(numCols/32, numCols/32 , 1);
  	const dim3 gridSize(numRows/12, numRows/12 , 1);
  	rgba_to_greyscale<<<gridSize, blockSize>>>(d_rgbaImage, d_greyImage, numRows, numCols);

  	cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
}