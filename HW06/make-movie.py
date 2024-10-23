#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import struct
import sys
import argparse

def read_binary_file(filename):
    """Read binary file containing all iterations of the stencil data."""
    with open(filename, 'rb') as f:
        # Read metadata (rows and cols as integers)
        rows = struct.unpack('i', f.read(4))[0]
        cols = struct.unpack('i', f.read(4))[0]
        
        # Calculate file size to determine number of frames
        f.seek(0, 2)  # Seek to end of file
        file_size = f.tell()
        f.seek(8)  # Seek back to after metadata
        
        # Calculate number of frames
        data_size_per_frame = rows * cols * 8  # 8 bytes per double
        num_frames = (file_size - 8) // data_size_per_frame
        
        # Read all frames
        frames = []
        f.seek(8)  # Reset to start of data
        for _ in range(num_frames):
            frame = np.zeros((rows, cols))
            for i in range(rows):
                # Read one row of doubles
                row_data = struct.unpack('d' * cols, f.read(8 * cols))
                frame[i,:] = row_data
            frames.append(frame)
            
    return frames

def update_frame(frame_num, frames, img):
    """Update function for animation."""
    img.set_array(frames[frame_num])
    return [img]

def create_movie(input_file, output_file, fps=10):
    """Create movie from binary data file."""
    # Read all frames
    print(f"Reading data from {input_file}...")
    frames = read_binary_file(input_file)
    num_frames = len(frames)
    print(f"Found {num_frames} frames")
    
    # Set up the figure and animation
    fig, ax = plt.subplots()
    ax.set_title('Stencil Operation Animation')
    
    # Calculate min and max values across all frames for consistent colormap
    vmin = min(frame.min() for frame in frames)
    vmax = max(frame.max() for frame in frames)
    
    # Create initial plot
    img = ax.imshow(frames[0], cmap='viridis', vmin=vmin, vmax=vmax)
    plt.colorbar(img)
    
    # Create animation
    print(f"Creating animation with {fps} fps...")
    anim = animation.FuncAnimation(fig, update_frame, frames=num_frames,
                                 fargs=(frames, img),
                                 interval=1000/fps, blit=True)
    
    # Save animation
    print(f"Saving animation to {output_file}...")
    anim.save(output_file, fps=fps, extra_args=['-vcodec', 'libx264'])
    plt.close()
    print("Done!")

def main():
    parser = argparse.ArgumentParser(description='Create movie from stencil data')
    parser.add_argument('input_file', help='Input binary data file')
    parser.add_argument('output_file', help='Output movie file (MP4)')
    parser.add_argument('--fps', type=int, default=20, help='Frames per second (default: 20)')
    
    args = parser.parse_args()
    
    if not args.output_file.endswith('.mp4'):
        args.output_file += '.mp4'
    
    try:
        create_movie(args.input_file, args.output_file, args.fps)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    main()