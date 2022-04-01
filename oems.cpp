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

    unsigned int arr[size];

    if (rank == 0) {
        int number;
        int arr_idx = 0;
        fstream fin;
        fin.open("numbers", ios::in);

        fin.seekg(0, ios::end);
        int fileSize = fin.tellg();
        fin.seekg(0, ios::beg);

        while (fin.good()) {
            number = fin.get();
            if (!fin.good()) break;
            array[arr_idx++] = number;
        }
        fin.close();

        for (int i = 0; i < fileSize; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    MPI_Finalize();



    return EXIT_SUCCESS;
}