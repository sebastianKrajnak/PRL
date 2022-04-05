/*
    Parallel Odd-even Merge Sort using OpenMPI
    Author: Sebastian Krajnak
    Date: 5.4.2022 
*/
#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

void printNumbers(int *numbers){
    if(sizeof(numbers) == 0){
        cout << "Error numbers not loaded!";
    }
    else{
        for(int i = 0; i < 8; i++){
            cout << numbers[i] << ' ';
        }
    }
    cout << endl;
}

/*
    function which receives a processor's rank, 2D array of receiver procs
    and received numbers from the previous two processors. Sorts the numbers into maximum and minimum
    then sends them out to the next two processors in line based on 2D array position
*/
void oddEvenMergeSort(int rank, int processors[][2], int *receivedNums){
    int minimum = min(receivedNums[0], receivedNums[1]);
    int maximum = max(receivedNums[0], receivedNums[1]);

    MPI_Send(&minimum, 1, MPI_INT, processors[rank][0], 0, MPI_COMM_WORLD);
    MPI_Send(&maximum, 1, MPI_INT, processors[rank][1], 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){

    int rank;
    int numbers[8] = {};
    int receivedNums[2] = {};
    int sortedNums[8] = {};
    /*
        2D array of processors where: pos 0 and 1 are receiving processors of the 
        sender processor (IDed by rank)
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
        numbers would be sorted on the processors but they would be saved 
        randomly, depending on who sent their answer first, in the array
     */
    int outputProcs[8] = {10,16,16,17,17,18,18,13};
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Master processor loads numbers from input binary file a sends them out to
    // the first layer of sorting processors
    if(rank == 0){
        fstream srcFile;
        srcFile.open("numbers", ios::in);

        while(srcFile.good()){
            int number = 0;
            for(int i = 0; i < 8; i++){
                number = srcFile.get();
                if(srcFile.eof())
                    break;
                
                numbers[i] = number;
            }
        }
        srcFile.close();

        printNumbers(numbers);

        for(int i = 0; i < 8; i+= 2){
            MPI_Send(&numbers[i], 1, MPI_INT, i/2, 0, MPI_COMM_WORLD);
            MPI_Send(&numbers[i+1], 1, MPI_INT, i/2, 0, MPI_COMM_WORLD);
        }
    }

    // Receive numbers from previous processors and send the min/max to the next set of processors
    MPI_Recv(&receivedNums[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, nullptr);
    MPI_Recv(&receivedNums[1], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, nullptr);

    oddEvenMergeSort(rank, processors, receivedNums);

    // In the end, master processor will receive sorted numbers and print them out
    if(rank == 0){
        for(int i = 0; i < 8; i++){
            MPI_Recv(&sortedNums[i], 1, MPI_INT, outputProcs[i], 0, MPI_COMM_WORLD, nullptr);
        }
        for(int i = 0; i< 8; i++){
            cout << sortedNums[i] << endl;
        }
        cout << flush;
    }

    MPI_Finalize();    
    return 0;
}