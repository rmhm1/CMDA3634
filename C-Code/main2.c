#include "array.h"
#include <string.h>
#include <stdio.h>
#include "wave_simulation.h"

int main(int argc, char** argv){

    if (argc != 1){
        printf("Incorrect number of arguments given");
    }

    simulate_wave(2.5, 501, 3, 4, 1.0);

    Array2D true_solution;
    allocate(&true_solution, 501, 501);
    initialize(&true_solution);

    char savename[26] = "../data/true_solution_0_0";
    grid_wave_solution(&true_solution, 3, 4, 501, 0.0);
    save_to_disk(&true_solution, savename);

    strcpy(savename, "../data/true_solution_0_5");
    grid_wave_solution(&true_solution, 3, 4, 501, 0.5);
    save_to_disk(&true_solution, savename);

    strcpy(savename, "../data/true_solution_1_0");
    grid_wave_solution(&true_solution, 3, 4, 501, 1.0);
    save_to_disk(&true_solution, savename);

    strcpy(savename, "../data/true_solution_1_5");
    grid_wave_solution(&true_solution, 3, 4, 501, 1.5);
    save_to_disk(&true_solution, savename);

    strcpy(savename, "../data/true_solution_2_0");
    grid_wave_solution(&true_solution, 3, 4, 501, 2.0);
    save_to_disk(&true_solution, savename);

    strcpy(savename, "../data/true_solution_2_5");
    grid_wave_solution(&true_solution, 3, 4, 501, 2.5);
    save_to_disk(&true_solution, savename);
}
