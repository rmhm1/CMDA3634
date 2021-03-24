# CMDA3634

## Makefile
The Makfile has 3 targets: main1, main2, and main3. Both main2 and main3 don't take any arguments, and just run the simulation of the standing wave equation while saving to disk the grid at certain timesteps. Main1 accepts 5 arguments to specify the differnt paramaters for the simulation of the wave equation which will be detailed below.

## Arguments
float T: The end time for the simulation, int n: The number of grid points in the x and y direction to make the grid (The struct can hold rectangular grids, but only square are used), 
int mx and int my: the number of standing wave nodes in their respective dimension (used in the true solution of the wave equation), float alpha: The coefficient to determine deltat with, it more or less specifies the stability of the system (1.0 being stable).
