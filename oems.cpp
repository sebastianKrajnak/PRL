#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
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