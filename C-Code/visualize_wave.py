#!/usr/bin/env python
# coding: utf-8

# In[233]:


import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import mpl_toolkits.mplot3d.axes3d as p3
get_ipython().run_line_magic('matplotlib', 'notebook')


# In[3]:


def save_data(filename, arr):
    """
    Saves a 2D floating point array in a simple format.

    * The first 4 bytes are an unsigned integer with the number of rows.
    * The second 4 bytes are an unsigned integer with the number of columns.
    * The remaining data are rows*cols 32-bit floats.

    Parameters
    ----------
    filename: string or path
       The filename to write to
    arr: np.ndarray
       Array of float32s to write out.

    """

    # Open a file handle for binary writing
    with open(filename, "wb") as outfile:

        # Get the arrays shape as unsigned integers and write to file
        sh = np.asarray(arr.shape).astype(np.uint32)
        sh.tofile(outfile)

        # Convert the data to float32 and write it to file
        arr = arr.astype(np.float32)
        arr.tofile(outfile)


# In[223]:


def animate_wave_simulation_3D(U, dt):
    """
    Creates a 3D surface animation of a wave simulation.

    Automatically selects color-scale.  In the event of instability or
    a wild range of values, some components might not be easily seen.
    Consider tweaking this scale if you need to.

    Adapted from https://stackoverflow.com/a/45713451 under the 
    CC BY-SA 3.0 license.

    Parameters
    ----------
    U: list-like
       A list of 2D wavefields to animate.

    Returns
    -------
    Matplotlib animation class instance.

    """
    
    grid_y = np.linspace(0, 1, U[0].shape[0])
    grid_x = np.linspace(0, 1, U[0].shape[1])
    YY, XX = np.meshgrid(grid_y, grid_x)

    fig = plt.figure()
    ax = p3.Axes3D(fig)


    cmin, cmax = U[0].min(), U[0].max()
    for u in U:
        cmin = min(cmin, u.min())
        cmax = max(cmax, u.max())

    cmin = max(-1, cmin)
    cmax = min(1, cmax)

    surf = [ax.plot_surface(YY, XX, U[0], cmap="viridis", clim=(cmin, cmax))]
    ax.set_zlim(1.1*cmin, 1.1*cmax)

    time_text = ax.text2D(0.25, 0.95, 'Time:', horizontalalignment='center',
                        verticalalignment='center', transform=ax.transAxes)
    
    def animate(i, U, surf):
        surf[0].remove()
        surf[0] = ax.plot_surface(YY, XX, U[i], cmap="viridis", clim=(-1,1))
        time_text.set_text('Current Time = {:.3f}'.format(i * dt))
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    
    ani = animation.FuncAnimation(fig, animate, fargs=(U, surf), interval=50, repeat_delay=1000)

    return ani


# In[222]:


def animate_single_step(U):
    """
    Creates a 3D surface animation of a wave simulation.

    Automatically selects color-scale.  In the event of instability or
    a wild range of values, some components might not be easily seen.
    Consider tweaking this scale if you need to.

    Adapted from https://stackoverflow.com/a/45713451 under the 
    CC BY-SA 3.0 license.

    Parameters
    ----------
    U: list-like
       A list of 2D wavefields to animate.

    Returns
    -------
    Matplotlib animation class instance.

    """
    
    grid_y = np.linspace(0, 1, U[0].shape[0])
    grid_x = np.linspace(0, 1, U[0].shape[1])
    YY, XX = np.meshgrid(grid_y, grid_x)

    fig = plt.figure()
    ax = p3.Axes3D(fig)


    cmin, cmax = U[0].min(), U[0].max()
    for u in U:
        cmin = min(cmin, u.min())
        cmax = max(cmax, u.max())

    cmin = max(-1, cmin)
    cmax = min(1, cmax)

    surf = [ax.plot_surface(YY, XX, U[0], cmap="viridis", clim=(cmin, cmax))]
    ax.set_zlim(1.1*cmin, 1.1*cmax)

    
    def animate(i, U, surf):
        surf[0] = ax.plot_surface(YY, XX, U[i], cmap="viridis", clim=(-1,1))
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')


# In[5]:


def load_data(filename):
    """
    Reads a 2D floating point array in a simple format.

    * The first 4 bytes are an unsigned integer with the number of rows.
    * The second 4 bytes are an unsigned integer with the number of columns.
    * The remaining data are rows*cols 32-bit floats.

    Parameters
    ----------
    filename: string or path
       The filename to write to

    Returns
    -------
    arr: np.ndarray
       Array of float32s read in.

    """

    # Open a file handle for binary reading
    with open(filename, "rb") as infile:

        # Read 2 unsigned integers to get the array's shape
        sh = np.fromfile(infile, dtype=np.uint32, count=2)
        print(sh)

        # Read the remaining data
        arr = np.fromfile(infile, dtype=np.float32, count=np.prod(sh))

        # Reshape the array to the expected shape
        arr.shape = sh

    return arr


# In[225]:


def load_all(nt, dt):
    """
    Reads a 2D floating point array in a simple format.

    * The first 4 bytes are an unsigned integer with the number of rows.
    * The second 4 bytes are an unsigned integer with the number of columns.
    * The remaining data are rows*cols 32-bit floats.

    Parameters
    ----------
    filename: string or path
       The filename to write to

    Returns
    -------
    arr: np.ndarray
       Array of float32s read in.

    """
    arrays = []
    for i in range(nt):
        fl_string = "{:.3f}".format(i*dt)
        name = "ec_" + fl_string + ".bin"
        arr = load_data(name)
        arrays.append(arr)
    return arrays, dt

