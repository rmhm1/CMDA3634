#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "array.h"
#include <time.h>

// Solves the wave equation at specific time/spatial points.
float wave_function(float t, float mx, float my, float x, float y){

    float w, wave_solution;
    w = M_PI * (sqrt((mx * mx) + (my * my)));
    wave_solution = sin(mx * x * M_PI) * sin(my * y * M_PI) * cos(w * t);

    return wave_solution;
}

// Solves an entire grid of the wave equation at a time step.
int grid_wave_solution(Array2D* u_k, int mx, int my, unsigned int n, float t){

    float dx = (1.0 - 0) / ((float)n - 1.0);
    // Create linspaced array for x and y
    float xgrid[n];
    float ygrid[n];
    // Checks if n >= 1 so creating linearspaced array of x and y
    // doesn't run into any division by 0.
    if (n <= 1) {
        return 1;
    }
    // Populate the linspaced grids
    for (int i = 0; i < n; ++i){

        xgrid[i] = (float)i*dx;
        ygrid[i] = (float)i*dx;
    }


    // Pass in each x, y point into the wave function and return the output 
    // to the corresponding entry of the 2D array
    for (int j = 0; j < n; ++j){
        for (int i = 0; i < n; ++i){
            // Calculates the linear index of the array
            int linear_index = j*n + i;
            u_k->data[linear_index] = wave_function(t, mx, my, xgrid[i], ygrid[j]);
        }
    }
    return 0;
}
// Computes one time step of the overall standing wave simulation
int time_step(Array2D* u_prev, Array2D* u_curr, Array2D* u_next, unsigned int n, float dt){

    // Checks if the arrays are of the same size
    if (u_prev->N != u_curr->N || u_prev->M != u_curr->M || u_curr->N != u_next->N || 
            u_curr->M != u_next->M){
        return 1;
    }
    // Checking n size to check for division by 0 for
    // computing dx
    if (n <= 1){

        return 1;
    }

    // Compute dx
    float dx = (1.0 - 0.0) / ((float)n - 1);
    // Loop over all grid points
    for (int j = 0; j < n; ++j){
        for (int i = 0; i < n; ++i){
            // Computes index of row-major array.
            int kr = j * n + i;
            // If at a border point set equal to 0 and move on
            if ((j == 0 || i == 0) || (j == (n - 1) || i == (n - 1))){
                u_next->data[kr] = 0;   
            }
            else {
                // Compute the laplacian
                float laplacian = (-4*u_curr->data[kr] + u_curr->data[(j-1)*n + i] + u_curr->data[j*n +(i-1)] 
                        + u_curr->data[j*n + (i+1)] + u_curr->data[(j+1)*n + i]);
                laplacian /= dx * dx;
                // Compute next time step over the grid points
                u_next->data[kr] = 2*u_curr->data[kr] - u_prev->data[kr] + (dt * dt) * laplacian;
            }
        }
    }
    return 0;
}

// Swaps the data held in u_next, u_curr, and u_prev
// Doesn't swap N or M since we're working with square grids
int swap_grids(Array2D* prev, Array2D* curr, Array2D* next){

    float* temp_curr_ptr = curr->data;
    curr->data = next->data;
    next->data = prev->data;
    prev->data = temp_curr_ptr;

    return 0;

}

int save_array(Array2D* array, float curr_time){
    // Calculates current time step in comparison to targets to save at
    // timestep 0.0 is missing as that's calculates as part of initial conditions
    float save0_5 = fabs(0.5 - curr_time);    
    float save1_0 = fabs(1.0 - curr_time);
    float save1_5 = fabs(1.5 - curr_time);
    float save2_0 = fabs(2.0 - curr_time);
    float save2_5 = fabs(2.5 - curr_time);
    float error = .01;


    char save_name[100];
    sprintf(save_name, "../data/array_time_%.3f.bin", curr_time);
    // Checks if the current time step is within error bounds with target time step 
    // values to save the simulation at
    if (save0_5 <= error | save1_0 <= error | save1_5 <= error | save2_0 <= error | save2_5 <= error){
        save_to_disk(array, save_name);
        return 0;
    }
    return 0;
}

// Simulates the standing wave over all nt grid points
int simulate_wave(float T, unsigned int n, int mx, int my, float alpha){   


    float dx = 1.0 / ((float)n - 1.0);
    float dt = (alpha * dx) / sqrt(2.0);
    int nt = T / dt;
    // Creates the grids for prev, curr, and next
    Array2D u_prev, u_curr, u_next;
    allocate(&u_prev, n, n);
    allocate(&u_curr, n, n);
    allocate(&u_next, n, n);
    // Initializes all of the grids
    initialize(&u_prev);
    initialize(&u_curr);
    initialize(&u_next);
    // Sets prev and curr to the first two entries of t = 0,
    // and t = -dt
    clock_t start_sim, end_sim;
    clock_t start_init_save, end_init_save;
    clock_t start_save, end_save;

    start_sim = clock();
    grid_wave_solution(&u_prev, mx, my, n, -1 * dt);
    grid_wave_solution(&u_curr, mx, my, n, 0);

    // Saves grid at timestep 0.0
    char initial_save[100] = "../data/array_time_0.00.bin";
    start_init_save = clock();
    save_to_disk(&u_curr, initial_save);
    end_init_save = clock();

    double total_save_time = (double)(end_init_save - start_init_save);

    for (int i = 0; i < nt; ++i){
        // Calculates the current timestep
        float curr_step = i * dt;   
        // Run the next timestep of the simulation
        time_step(&u_prev, &u_curr, &u_next, n, dt);
        
        // Checks if u_next being calculated is at a timestep
        // to save at. Saves if it is, skips if not
        // Also times teh save and adds it to total save time
        start_save = clock();
        save_array(&u_next, curr_step);
        end_save = clock();
        total_save_time += (double)(end_save - start_save);
        // Swap grids around
        swap_grids(&u_prev, &u_curr, &u_next);
    }
    end_sim = clock();
    deallocate(&u_prev);
    deallocate(&u_curr);
    deallocate(&u_next);

    double total_time = (double)(end_sim - start_sim) / (double)CLOCKS_PER_SEC;
    //double overall_save_time = (double)(total_save_time) / (double)CLOCKS_PER_SEC;
    double simulation_time = (double)((end_sim - start_sim) - total_save_time) / (double)CLOCKS_PER_SEC;
    
    printf("Total runtime for n = %d: %f ", n, total_time);
    printf("Total Simulation time for n = %d: %f\n", n, simulation_time);
    return 0;
}

int extra_credit(float T, unsigned int n, int mx, int my, float alpha){
    float dx = 1.0 / ((float)n - 1.0);
    float dt = (alpha * dx) / sqrt(2.0);
    int nt = T / dt;
    // Creates the grids for prev, curr, and next
    Array2D u_prev, u_curr, u_next;
    allocate(&u_prev, n, n);
    allocate(&u_curr, n, n);
    allocate(&u_next, n, n);
    // Initializes all of the grids
    initialize(&u_prev);
    initialize(&u_curr);
    initialize(&u_next);
    // Sets prev and curr to the first two entries of t = 0,
    // and t = -dt

    grid_wave_solution(&u_prev, mx, my, n, -1 * dt);
    grid_wave_solution(&u_curr, mx, my, n, 0);

    char initsave[] = "../data/ec_0.00";
    save_to_disk(&u_curr, initsave);
    // Saves grid at timestep 0.0


    for (int i = 0; i < nt; ++i){
        // Calculates the current timestep
        float curr_step = i * dt;   
        // Run the next timestep of the simulation
        time_step(&u_prev, &u_curr, &u_next, n, dt);
        char savename[80];
        sprintf(savename, "../data/ec_%.3f.bin", curr_step);
        save_to_disk(&u_next, savename);
        // Checks if u_next being calculated is at a timestep
        // to save at. Saves if it is, skips if not
        // Also times teh save and adds it to total save time
        

        // Swap grids around
        swap_grids(&u_prev, &u_curr, &u_next);
    }
    deallocate(&u_prev);
    deallocate(&u_curr);
    deallocate(&u_next);

    
    return 0;

}

