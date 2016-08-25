#include <iostream>
#include <ctime>
using namespace std;

#define N 10000
#define MIN(a,b) (((a)<(b))?(a):(b))

int A[N][N], B[N][N], C[N][N];
int X[N][N], Y[N][N], Z[N][N];
int main(int argc, char *argv[])
{
    for(unsigned i = 0; i < N; i++){
        for(unsigned j = 0; j < N; j++){
            A[i][j] = 0;
            B[i][j] = 0;
            C[i][j] = 0;
            X[i][j] = 0;
            Y[i][j] = 0;
            Z[i][j] = 0;
        }
    }


    clock_t start = clock();
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            for(int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    double t = double(clock() - start) / CLOCKS_PER_SEC;
    cout<<"time 3 nested loops "<< t <<endl;


    int blockSize = 100;


    start = clock();
    for (int I = 0; I < N; I+=blockSize)
        for (int J = 0; J < N; J+=blockSize)
            for (int K = 0; K < N; K+=blockSize)
                for (int i = I; i < MIN(I + blockSize,N); i++)
                    for (int j = J; j < MIN(J + blockSize, N); j++)
                        for (int k = K; k < MIN(K + blockSize, N); k++)
                            Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
    t = double(clock() - start) / CLOCKS_PER_SEC;
    cout<<"time 6 nested loops "<< t <<endl;

    return 0;
}
