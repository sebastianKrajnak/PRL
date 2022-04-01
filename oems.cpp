#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

int loadNumbers(){
    int numbers[8] = {};

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
    return *numbers;
}

void printNumbers(int numbers[8]){
    for(int i = 0; i < 8; i++){
        cout << numbers[i] << ' ';
    }
    cout << endl << flush;
}

int main(int argc, char *argv[]){
    int processors[19][2]={
        {4,5},{4,5},{6,7},{6,7},
        {10,8},{8,13},{10,9},{9,13},
        {12,11},{12,11},{0,14},{14,18},
        {16,15},{15,0},{16,17},{17,18},
        {0,0},{0,0},{0,0}
    };

    int rank, size;
    int numbers[8] = {};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        numbers = loadNumbers();
        printNumbers(numbers);
    }
    
    //MPI_Comm_size(MPI_COMM_WORLD, &size);
    //cout << "I am" << rank << " of " << size << endl;
    MPI_Finalize();



    return EXIT_SUCCESS;
}
