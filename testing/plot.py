from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

random_data = np.genfromtxt(Path("./test_data/random_data.csv"), delimiter = ',')
random_time = random_data[-1]
random_data = random_data[:-1] 

entropy_data = np.genfromtxt(Path("./test_data/entropy_data.csv"), delimiter = ',')
entropy_time = entropy_data[-1]
entropy_data = entropy_data[:-1] 

data = [random_data, entropy_data]
labels = ["Random choice", "Entropy maximizing"]

fig, ax = plt.subplots(figsize = (8,6))
n_bins = 8
plt.hist(data, bins = np.arange(1, n_bins + 2)-0.5, label = labels)

plt.title(f"Wordle Solver Results, Starting Word \"slate\", n = {len(random_data)}", fontsize = 15)
plt.xlabel("Attempts", fontsize = 15)
plt.ylabel("Frequency", fontsize = 15)
plt.xticks(range(1, n_bins + 1))
plt.legend()

random_mean = np.mean(random_data)
plt.axvline(random_mean, c = 'k', ls = '--', lw = 1)
plt.text(random_mean + 0.7, 800, f"Random mean: {random_mean:.3f}\nElapsed time: {random_time} s", 
         transform = ax.transData, fontsize =11)

entropy_mean = np.mean(entropy_data)
plt.axvline(entropy_mean, c = 'k', ls = '--', lw = 1)
plt.text(entropy_mean - 2.8, 800, f"Entropy mean: {entropy_mean:.3f}\nElapsed time: {entropy_time} s", 
         transform = ax.transData, fontsize =11)

plt.tight_layout()
plt.savefig(Path("./plot.png"))
