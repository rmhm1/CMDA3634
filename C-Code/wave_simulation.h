#ifndef __WAVE_SIMULATION_H__
#define __WAVE_SIMULATION_H__

float wave_function(float t, float mx, float my, float x, float y);
int grid_wave_solution(Array2D* u_k, int mx, int my, unsigned int n, float t);
int time_step(Array2D* u_prev, Array2D* u_curr, Array2D* u_next, unsigned int n, float dt);
int swap_grids(Array2D* prev, Array2D* curr, Array2D* next);
int save_array(Array2D* array, float curr_time);
int simulate_wave(float T, unsigned int n, int mx, int my, float alpha);
int extra_credit(float T, unsigned int n, int mx, int my, float alpha);
#endif
