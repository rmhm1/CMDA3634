#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "wave_simulation.h"

int main(int argc, char** argv){
    if (argc != 6){
        printf("Not enough arguments given");
        return 1;
    }

    float T = atof(argv[1]);
    unsigned int n = atoi(argv[2]);
    int mx = atoi(argv[3]);
    int my = atoi(argv[4]);
    float alpha = atof(argv[5]);

    simulate_wave(T, n, mx, my, alpha);
}
