#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

void printNumbers(int *numbers){
    for(int i = 0; i < 8; i++){
        cout << numbers[i] << ' ';
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
    int outputProcs[8] = {10,16,16,17,17,18,18,13};
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        // Load numbers from input binary file a send send them out to the first layer of sorting processors
        fstream srcFile;
        srcFile.open("numbers", ios::in);

        if(!srcFile.is_open()){
            cout << "Error: unable to open file numbers";
            return {};
        }

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

    int receivedNums[2] = {};

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