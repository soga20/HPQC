import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys
import os

def load_data(filename):
    try:
        data = np.loadtxt(filename, delimiter=',')
        return data
    except Exception as e:
        print(f"Error loading data from {filename}: {e}")
        sys.exit(1)

def animate_line(filename, output_dir=".", output_filename="wave_animation.mp4"):
    data = load_data(filename)

    if len(data.shape) == 1:
        data = data.reshape(1, -1)

    print(f"Data shape after reshape: {data.shape}")

    fig, ax = plt.subplots()
    x = np.arange(len(data[0]))
    line, = ax.plot(x, data[0, :], 'b-', label='Wave')
    ax.set_ylim(np.min(data) - 1, np.max(data) + 1)
    ax.set_title('Wave Oscillation')
    ax.set_xlabel('Point on String')
    ax.set_ylabel('Displacement')

    def update_plot(frame):
        line.set_ydata(data[frame, :])
        return line,

    ani = animation.FuncAnimation(fig, update_plot, frames=len(data), interval=50, blit=True)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    output_path = os.path.join(output_dir, output_filename)

    try:
        ani.save(output_path, writer='ffmpeg', fps=30)
        print(f"Animation saved as '{output_path}'")
    except Exception as e:
        print(f"Failed to save as .mp4: {e}")
        gif_path = os.path.splitext(output_path)[0] + ".gif"
        ani.save(gif_path, writer='pillow', fps=20)
        print(f"Saved animation as GIF instead: '{gif_path}'")

def parse_arguments():
    if len(sys.argv) < 2:
        print("Usage: python3 animate_line_file.py <input_file> [<output_dir> <output_filename>]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_dir = sys.argv[2] if len(sys.argv) > 2 else "."
    output_filename = sys.argv[3] if len(sys.argv) > 3 else "wave_animation.mp4"
    return input_file, output_dir, output_filename

if __name__ == "__main__":
    input_file, output_dir, output_filename = parse_arguments()
    animate_line(input_file, output_dir, output_filename)
