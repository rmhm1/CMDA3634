#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct Array2D_tag {
    // M rows, N columns
    unsigned int M;
    unsigned int N;

    float* data;
} Array2D;

int allocate(Array2D* array, unsigned int M, unsigned int N);

int deallocate(Array2D* array);

int initialize(Array2D* array);

int deep_dopy(Array2D original, Array2D* copy);

int save_to_disk(Array2D* array, char* filename);


#endif
