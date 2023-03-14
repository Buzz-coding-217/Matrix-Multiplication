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

#define NUM_THREADS 8
#define MAX 1000000
pthread_mutex_t mutx;

int a[10][10], b[10][10], mult[10][10], r1, c1, r2, c2, i, j, k;

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
  
    

    cout << "Enter rows and columns for first matrix: ";
    cin >> r1 >> c1;
    cout << "Enter rows and columns for second matrix: ";
    cin >> r2 >> c2;

    // If column of first matrix in not equal to row of second matrix,
    // ask the user to enter the size of matrix again.
    while (c1!=r2)
    {
        cout << "Error! column of first matrix not equal to row of second.";

        cout << "Enter rows and columns for first matrix: ";
        cin >> r1 >> c1;

        cout << "Enter rows and columns for second matrix: ";
        cin >> r2 >> c2;
    }

    // Storing elements of first matrix.
    cout << endl << "Enter elements of matrix 1:" << endl;
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c1; ++j)
        {
            cout << "Enter element a" << i + 1 << j + 1 << " : ";
            cin >> a[i][j];
        }

    // Storing elements of second matrix.
    cout << endl << "Enter elements of matrix 2:" << endl;
    for(i = 0; i < r2; ++i)
        for(j = 0; j < c2; ++j)
        {
            cout << "Enter element b" << i + 1 << j + 1 << " : ";
            cin >> b[i][j];
        }

    // Initializing elements of matrix mult to 0.
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c2; ++j)
        {
            mult[i][j]=0;
        }

    // Multiplying matrix a and b and storing in array mult.
    auto start = std::chrono::high_resolution_clock::now();

    pthread_t threads[NUM_THREADS];
    for (int rowt = 0; rowt < r1; rowt++) {
        pthread_create(&threads[rowt], NULL, mat_mul, (void *)rowt);
    }

   for(int rowt = 0; rowt < r1; rowt++) {
       pthread_join(threads[rowt], NULL);
   }
  
    // for(i = 0; i < r1; ++i)
    //     for(j = 0; j < c2; ++j)
    //         for(k = 0; k < c1; ++k)
    //         {
    //             mult[i][j] += a[i][k] * b[k][j];
    //         }

  
    auto end = std::chrono::high_resolution_clock::now(); // end time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // duration in nanoseconds
    std::cout << "Task duration: " << std::fixed << std::setprecision(9) << duration << " nanoseconds" << std::endl;
    // Displaying the multiplication of two matrix.
    cout << endl << "Output Matrix: " << endl;
    for(i = 0; i < r1; ++i)
    for(j = 0; j < c2; ++j)
    {
        cout << " " << mult[i][j];
        if(j == c2-1)
            cout << endl;
    }

    return 0;
}
