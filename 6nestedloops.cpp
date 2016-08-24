#include <iostream>
#include <ctime>
using namespace std;

#define N 100
#define MIN(a,b) (((a)<(b))?(a):(b))

int X[100][100], Y[100][100], Z[100][100];
int main(int argc, char *argv[])
{
    for(unsigned i = 0; i < N; i++){
        for(unsigned j = 0; j < N; j++){
            X[i][j] = 0;
            Y[i][j] = 0;
            Z[i][j] = 0;
        }
    }

    int blockSize = 10;


    clock_t start = clock();
    for (int I = 0; I < N; I+=blockSize)
        for (int J = 0; J < N; J+=blockSize)
            for (int K = 0; K < N; K+=blockSize)
                for (int i = I; i < MIN(I + blockSize,N); i++)
                    for (int j = J; j < MIN(J + blockSize, N); j++)
                        for (int k = K; k < MIN(K + blockSize, N); k++)
                            Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
    int t = double(clock() - start) / CLOCKS_PER_SEC;
    cout<<"time 6 nested loops "<< t <<endl;

    return 0;
}
