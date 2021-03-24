#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "array.h"

// Allocates memory to an array of size M * N
int allocate(Array2D* array, unsigned int M, unsigned int N){

    array->data = malloc(M * N * sizeof(float));

    if (array->data == NULL){
        return 1;
    }
    array->M = M;
    array->N = N;
    return 0;
}

// Deallocates an array
int deallocate(Array2D* array){

    if (array->data == NULL){
        array->M = 0;
        array->N = 0;
        return 0;
    }
    free(array->data);
    array->data = NULL;
    array->M = 0;
    array->N = 0;

    return 0;
}

// Initializes an array 
int initialize(Array2D* array){

    if (array->data == NULL){
        return 1;
    }
    memset(array->data, 0, array->N * array->M * sizeof(float));
    return 0;
}

// Creates a deep copy of an array with an initialized
// array of correct sized passed in as the second paramater 
int deep_copy(Array2D* original, Array2D* copy){

    if (copy->N != original->N || copy->M != original->M){
    
        return 1;
    }
    memcpy(copy->data, original->data, original->N * original->M * sizeof(float));
    return 0;  
}

// Saves array to disk with unsigned ints M and N as the first two entries
// of the binary file
int save_to_disk(Array2D* array, char* filename){
  
    FILE* f;
    int n_data, n_written_data;
    //n_data = array->M * array->N;

    // open specified file
    f = fopen(filename, "wb");

    // Checking if file is opened correctly
    if (f == NULL){
        fprintf(stderr, "Error opening %s\n", filename);
        return 1;
    }

    unsigned int dimensions[2] = {array->M, array->N};
    //printf("N = %u", N);
    // Add ny, nx header
    fwrite(dimensions, sizeof(unsigned int), 2, f);
    
    // Write the array to filename
    n_written_data = fwrite(array->data, sizeof(float), dimensions[0] * dimensions[1], f);

    // Error checking how much data was written vs. expected
    if (dimensions[0] * dimensions[1] != n_written_data){
        fprintf(stderr, "Error writing file: %s. Expected %d bytes, but got %d bytes", 
                filename, dimensions[0] * dimensions[1], n_written_data);
        return 1;
    }
    fclose(f);
    return 0;

}
