import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def plot_2d_graphs(df):
    A_values = [0.01, 0.1, 0.25, 0.5]
    for A in A_values:
        df_subset = df[df['A'].round(2) == A]
        plt.figure(figsize=(10, 6))
        plt.plot(df_subset['H'], df_subset['Reflected'], label='Reflected')
        plt.plot(df_subset['H'], df_subset['Absorbed'], label='Absorbed')
        plt.plot(df_subset['H'], df_subset['Transmitted'], label='Transmitted')
        plt.xlabel('H (Thickness)')
        plt.ylabel('Percentage')
        plt.title(f'Neutron Behavior for A = {A}')
        plt.legend()
        plt.savefig(f'2d_plot_A_{A}.png')
        plt.close()

def plot_3d_graphs(df):
    for behavior in ['Reflected', 'Absorbed', 'Transmitted']:
        fig = plt.figure(figsize=(10, 8))
        ax = fig.add_subplot(111, projection='3d')
        ax.plot_trisurf(df['A'], df['H'], df[behavior], cmap='viridis')
        ax.set_xlabel('A')
        ax.set_ylabel('H (Thickness)')
        ax.set_zlabel(f'{behavior} (%)')
        ax.set_title(f'3D Surface Plot of {behavior} Neutrons')
        plt.savefig(f'3d_plot_{behavior}.png')
        plt.close()

def plot_heatmaps(df):
    for behavior in ['Reflected', 'Absorbed', 'Transmitted']:
        pivot_df = df.pivot_table(values=behavior, index='A', columns='H', aggfunc='mean')
        plt.figure(figsize=(12, 8))
        sns.heatmap(pivot_df, cmap='YlOrRd')
        plt.title(f'Heatmap of {behavior} Neutrons')
        plt.xlabel('H (Thickness)')
        plt.ylabel('A')
        plt.savefig(f'heatmap_{behavior}.png')
        plt.close()

def main(input_file):
    df = pd.read_csv(input_file)
    print("Plotting graphs...")
    plot_2d_graphs(df)
    plot_3d_graphs(df)
    plot_heatmaps(df)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plot_sweep_data.py <input_file_name>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    main(input_file)
    print("Plotting complete.")