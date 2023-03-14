#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <string>
using namespace std;
#include <iomanip>

#define NUM_THREADS 100
pthread_mutex_t mutx;

int a[100][100], b[100][100], mult[100][100], r1 = 100, c1 = 100, r2 = 100, c2 = 100, i, j, k;

void *mat_mul(void *rowt)
{

  long row = (long)rowt;
  
   for(int i = 0; i < c2; i++)
     for (int j = 0; j < c1; j++)
       {
         mult[row][i] += a[row][j] * b[j][i];
       }
   pthread_exit(NULL);
}

int main()
{

    int cores = std::thread::hardware_concurrency();
    pthread_mutex_init(&mutx, NULL);

    cout<<" the number of cores on this machine = "<<cores<<endl;

    while (c1!=r2)
    {
        cout << "Error! column of first matrix not equal to row of second.";

        cout << "Enter rows and columns for first matrix: ";
        cin >> r1 >> c1;

        cout << "Enter rows and columns for second matrix: ";
        cin >> r2 >> c2;
    }
    cout << endl << "Enter elements of matrix 1:" << endl;
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c1; ++j)
        {
            a[i][j] = rand() % 20;
        }
    cout << endl << "Enter elements of matrix 2:" << endl;
    for(i = 0; i < r2; ++i)
        {
            b[i][j] = rand() % 20;
        }

    for(i = 0; i < r1; ++i)
        for(j = 0; j < c2; ++j)
        {
            mult[i][j]=0;
        }
    auto start = std::chrono::high_resolution_clock::now();

    pthread_t threads[NUM_THREADS];
    for (int rowt = 0; rowt < r1; rowt++) {
        pthread_create(&threads[rowt], NULL, mat_mul, (void *)rowt);
    }
auto end = std::chrono::high_resolution_clock::now(); // end time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // duration in nanoseconds
    std::cout << "Task duration: " << std::fixed << std::setprecision(9) << duration << " nanoseconds" << std::endl;
   for(int rowt = 0; rowt < r1; rowt++) {
       pthread_join(threads[rowt], NULL);
   }

    return 0;
}
