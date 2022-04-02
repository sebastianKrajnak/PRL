#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

void printNumbers(int *numbers){
    for(int i = 0; i < 8; i++){
        cout << numbers[i] << ' ';
    }
    cout << endl << flush;
}

void oddEvenMerge(int rank, int processors[][2], int *receivedNums){
    int minimum = min(receivedNums[0], receivedNums[1]);
    int maximum = max(receivedNums[0], receivedNums[1]);

    MPI_Send(&minimum, 1, MPI_INT, processors[rank][0], 0, MPI_COMM_WORLD);
    MPI_Send(&maximum, 1, MPI_INT, processors[rank][1], 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){
    int rank, size;
    int numbers[8] = {};
    int sortedNums[8] = {};
    int processors[19][2]={
        {4,5},{4,5},{6,7},{6,7},
        {10,8},{8,13},{10,9},{9,13},
        {12,11},{12,11},{0,14},{14,18},
        {16,15},{15,0},{16,17},{17,18},
        {0,0},{0,0},{0,0}
    };
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
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
            MPI_Send(&(numbers[i]), 1, MPI_INT, i-2, 0, MPI_COMM_WORLD);
            MPI_Send(&(numbers[i+1]), 1, MPI_INT, i-2, 0, MPI_COMM_WORLD);
        }

        int receivedNums[2] = {};
        MPI_Recv(&receivedNums[0], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&receivedNums[1], 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        oddEvenMerge(rank, processors, receivedNums);
    }

    for(int i = 0; i < 8; i++){
        MPI_Recv(&sortedNums[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << sortedNums[i] << endl;
    }
    
    //MPI_Comm_size(MPI_COMM_WORLD, &size);
    //cout << "I am" << rank << " of " << size << endl;
    MPI_Finalize();



    return EXIT_SUCCESS;
}
