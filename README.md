# CMDA3634

## Makefile
The Makfile has 3 targets: main1, main2, and main3. Both main2 and main3 don't take any arguments, and just run the simulation of the standing wave equation while saving to disk the grid at certain timesteps. Main1 accepts 5 arguments to specify the differnt paramaters for the simulation of the wave equation which will be detailed below.

## Arguments
int T: The end time for the simulation, int n: The number of grid points in the x and y direction to make the grid (The struct can hold rectangular grids, but only square are used), 
int mx and int my: the number of standing wave nodes in their respective dimension (used in the true solution of the wave equation), float alpha: The coefficient to determine deltat with, it more or less specifies the stability of the system (1.0 being stable).

### Example
The following commands will be an example of a simulation for a standing wave on a squre grid of space over time, with the end point being 5 seconds, and the grid being 10x10:

Make main1
.main1/ 5 10 2 3 1.0
