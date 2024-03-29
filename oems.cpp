/*
    Parallel Odd-even Merge Sort using OpenMPI
    Author: Sebastian Krajnak
    Date: 8.4.2022 
*/
#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

// Function to check if numbers are loaded and if so, print them out
void checkNumbers(int *numbers){
    if(sizeof(numbers) == 0){
        cout << "Error numbers not loaded!";
    }
    else{
        for(int i = 0; i < 8; i++){
            cout << numbers[i] << ' ';
        }
        cout << endl;
    }
}

/*
    Function which receives a processor's rank and 2D array of receiver procs. Sorts the numbers into maximum and minimum
    then sends them out to the next two processors in line based on the position in said 2D array 
*/
void oddEvenMergeSort(int rank, int processors[][2]){
    int receivedNums[2] = {};

    MPI_Recv(&receivedNums[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&receivedNums[1], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int minimum = min(receivedNums[0], receivedNums[1]);
    int maximum = max(receivedNums[0], receivedNums[1]);

    MPI_Send(&minimum, 1, MPI_INT, processors[rank][0], 0, MPI_COMM_WORLD);
    MPI_Send(&maximum, 1, MPI_INT, processors[rank][1], 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){
    
    int rank;
    int numbers[8] = {};
    int sortedNums[8] = {};
    /*
        2D array of processors where: col 0 and 1 are the next two receiving processors of the 
        sender processor whose rank = row number
    */
    int processors[19][2]={
        {4,5}, {4,5}, {6,7}, {6,7},
        {10,8}, {8,13}, {10,9}, {9,13},
        {12,11}, {12,11}, {0,14}, {14,18},
        {16,15}, {15,0}, {16,17}, {17,18},
        {0,0}, {0,0}, {0,0}
    };
    /* 
        Array of processors outputting correct sequence of sorted numbers, otherwise 
        numbers would be sorted on the processors but they would be saved into the array 
        randomly, due to the asynchronity of sending order
     */
    int outputProcs[8] = {10,16,16,17,17,18,18,13};
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Master processor loads numbers from input binary file a sends them out to the first layer of sorting processors
    if(rank == 0){
        fstream srcFile;
        srcFile.open("numbers", ios::in);

        if(srcFile.is_open()){
            while(srcFile.good()){
                int number;
                for(int i = 0; i < 8; i++){
                    number = srcFile.get();
                    if(srcFile.eof())
                        break;
                    
                    numbers[i] = number;
                }
            }
        }
        srcFile.close();

        checkNumbers(numbers);

        for(int i = 0; i < 8; i+= 2){
            MPI_Send(&numbers[i], 1, MPI_INT, i/2, 0, MPI_COMM_WORLD);
            MPI_Send(&numbers[i+1], 1, MPI_INT, i/2, 0, MPI_COMM_WORLD);
        }
    }

    // Receive numbers from previous processors and send the two numbers sorted to the next set of processors
    oddEvenMergeSort(rank, processors);

    // In the end, master processor will receive sorted numbers and print them out
    if(rank == 0){
        for(int i = 0; i < 8; i++){
            MPI_Recv(&sortedNums[i], 1, MPI_INT, outputProcs[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for(int i = 0; i< 8; i++){
            cout << sortedNums[i] << endl;
        }
        cout << flush;
    }

    MPI_Finalize();    
    return 0;
}