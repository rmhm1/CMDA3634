#include <stdio.h>
#include "array.h"
#include "wave_simulation.h"

int main(int argc, char** argv){

    if (argc != 1){
        printf("Incorrect number of arguments");
        return 1;
    }

    extra_credit(5, 101, 2, 3, 1.0);
}
