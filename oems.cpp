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
void oddEvenMergeSort(int rank, int processors[][4], int *receivedNums){
    int minimum = min(receivedNums[0], receivedNums[1]);
    int maximum = max(receivedNums[0], receivedNums[1]);

    MPI_Send(&minimum, 1, MPI_INT, processors[rank][0], processors[rank][2], MPI_COMM_WORLD);
    MPI_Send(&maximum, 1, MPI_INT, processors[rank][1], processors[rank][3], MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){

    int rank;
    int numbers[8] = {};
    int sortedNums[8] = {};
    /*
        2D array of processors where: pos 0 and 1 are receiving processors of the 
        sender processor (IDed by rank), pos 2 and 3 are MPI tags used to identify the order of
        final sorted numbers (most important is the last layer where both receivers are the master proc)
    */
    int processors[19][4]={
        {4,5,0,0}, {4,5,1,1}, {6,7,0,0}, {6,7,1,1},
        {10,8,0,0}, {8,13,1,0}, {10,9,1,0}, {9,13,1,1},
        {12,11,0,0}, {12,11,1,1}, {0,14,0,0}, {14,18,1,0},
        {16,15,0,0}, {15,0,1,7}, {16,17,1,0}, {17,18,1,1},
        {0,0,1,2}, {0,0,3,4}, {0,0,5,6}
    };
    
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
            MPI_Send(&numbers[i+1], 1, MPI_INT, i/2, 1, MPI_COMM_WORLD);
        }
    }

    int receivedNums[2] = {};

    MPI_Recv(&receivedNums[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, nullptr);
    MPI_Recv(&receivedNums[1], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, nullptr);

    oddEvenMergeSort(rank, processors, receivedNums);

    // In the end, master processor will receive sorted numbers and print them out
    if(rank == 0){
        for(int i = 0; i < 8; i++){
            MPI_Recv(&sortedNums[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, nullptr);
        }
        for(int i = 0; i< 8; i++){
            cout << sortedNums[i] << endl;
        }
        cout << flush;
    }

    MPI_Finalize();    
    return 0;
}