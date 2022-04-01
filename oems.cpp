#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

/* int loadNumbers(){
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
        cout << numbers[i];
    }
    cout << endl << flush;
} */

int main(int argc, char *argv[]){
    int processors[19][5]={
        {},{},{},{},
        {},{},{},{},
        {},{},{},{},
        {},{},{},{},
        {},{},{}
    };
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    cout << "I am" << rank << " of " << size << endl;
    MPI_Finalize();



    return EXIT_SUCCESS;
}