#include <iostream>
#include <ctime>
using namespace std;

int N = 100;
int A[100][100], B[100][100], C[100][1000];
int main(int argc, char *argv[])
{
    for(unsigned i = 0; i < N; i++){
        for(unsigned j = 0; j < N; j++){
            A[i][j] = 0;
            B[i][j] = 0;
            C[i][j] = 0;
        }
    }


    clock_t start = clock();
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            for(int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    double t = double(clock() - start) / CLOCKS_PER_SEC;
    cout<<"time 3 nested loops "<< t <<endl;

    return 0;
}
