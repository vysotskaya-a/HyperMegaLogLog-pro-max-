import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_results(filename, title_suffix):
    if not os.path.exists(filename):
        print(f"File {filename} not found.")
        return

    df = pd.read_csv(filename)
    
    plt.figure(figsize=(10, 6))
    plt.plot(df['processed'], df['true_cardinality'], 'g-', label='True Cardinality', linewidth=2)
    plt.plot(df['processed'], df['est_mean'], 'b--', label='HLL Estimate (Mean)', linewidth=2)
    plt.xlabel('Processed Elements')
    plt.ylabel('Unique Elements')
    plt.title(f'True vs Estimated Cardinality ({title_suffix})')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'plot_comparison_{title_suffix}.png')
    
    plt.figure(figsize=(10, 6))
    plt.plot(df['processed'], df['est_mean'], 'b-', label='Mean Estimate')
    plt.fill_between(df['processed'], 
                     df['est_mean'] - df['est_stddev'], 
                     df['est_mean'] + df['est_stddev'], 
                     color='b', alpha=0.2, label='Sigma Range')
    plt.plot(df['processed'], df['true_cardinality'], 'r--', label='True Cardinality')
    plt.xlabel('Processed Elements')
    plt.ylabel('Estimate')
    plt.title(f'Statistical Analysis ({title_suffix})')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'plot_stats_{title_suffix}.png')

plot_results('results_std.csv', 'Standard')
plot_results('results_imp.csv', 'Improved')