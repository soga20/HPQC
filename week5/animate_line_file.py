import numpy as np
import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend for headless environments
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

def load_data(filename):
    """
    Load data from a CSV file. The CSV is expected to have a 2D array
    where each row is a frame, and each column is a point on the wave.

    Arguments:
    filename : str : The path to the CSV file containing the wave data.

    Returns:
    np.ndarray : A 2D array of wave data.
    """
    try:
        data = np.genfromtxt(filename, delimiter=',', filling_values=np.nan)
        if data.ndim == 1:
            data = data.reshape(1, -1)  # Make sure we have 2D
        data = data[~np.isnan(data).any(axis=1)]  # Remove rows with NaNs
        return data
    except Exception as e:
        print(f"Error loading data from {filename}: {e}")
        sys.exit(1)

def animate_line(filename):
    """
    Create an animated line plot from a CSV file containing wave data.

    Arguments:
    filename : str : The path to the CSV file containing the wave data.
    """
    data = load_data(filename)

    print(f"Data shape after load: {data.shape}")  # Debugging

    # Set up the figure and axis
    fig, ax = plt.subplots()
    x = np.arange(data.shape[1])  # X-axis points
    line, = ax.plot(x, data[0, :], 'b-', label='Wave')

    ax.set_ylim(np.min(data) - 10, np.max(data) + 10)
    ax.set_title('Wave Oscillation')
    ax.set_xlabel('Point on String')
    ax.set_ylabel('Displacement')

    def update_plot(frame):
        """
        Update the plot for each frame in the animation.

        Arguments:
        frame : int : The current frame of the animation.
        """
        line.set_ydata(data[frame, :])
        return line,

    ani = animation.FuncAnimation(fig, update_plot, frames=data.shape[0], interval=100, blit=True)

    # Save the animation
    ani.save('wave_animation.mp4', writer='ffmpeg', fps=30)

    print("Animation saved as 'wave_animation.mp4'")

# Example usage
# animate_line("wave_data.csv")import numpy as np
import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend for headless environments
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys

def load_data(filename):
    """
    Load data from a CSV file. The CSV is expected to have a 2D array
    where each row is a frame, and each column is a point on the wave.

    Arguments:
    filename : str : The path to the CSV file containing the wave data.

    Returns:
    np.ndarray : A 2D array of wave data.
    """
    try:
        data = np.genfromtxt(filename, delimiter=',', filling_values=np.nan)
        if data.ndim == 1:
            data = data.reshape(1, -1)  # Make sure we have 2D
        data = data[~np.isnan(data).any(axis=1)]  # Remove rows with NaNs
        return data
    except Exception as e:
        print(f"Error loading data from {filename}: {e}")
        sys.exit(1)

def animate_line(filename):
    """
    Create an animated line plot from a CSV file containing wave data.

    Arguments:
    filename : str : The path to the CSV file containing the wave data.
    """
    data = load_data(filename)

    print(f"Data shape after load: {data.shape}")  # Debugging

    # Set up the figure and axis
    fig, ax = plt.subplots()
    x = np.arange(data.shape[1])  # X-axis points
    line, = ax.plot(x, data[0, :], 'b-', label='Wave')

    ax.set_ylim(np.min(data) - 10, np.max(data) + 10)
    ax.set_title('Wave Oscillation')
    ax.set_xlabel('Point on String')
    ax.set_ylabel('Displacement')

    def update_plot(frame):
        """
        Update the plot for each frame in the animation.

        Arguments:
        frame : int : The current frame of the animation.
        """
        line.set_ydata(data[frame, :])
        return line,

    ani = animation.FuncAnimation(fig, update_plot, frames=data.shape[0], interval=100, blit=True)

    # Save the animation
    ani.save('wave_animation.gif', writer='pillow', fps=30)

    print("Animation saved as 'wave_animation.mp4'")
if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python animate_line_file.py path/to/wave_data.csv")
    else:
        animate_line(sys.argv[1])
# Example usage
# animate_line("wave_data.csv")
