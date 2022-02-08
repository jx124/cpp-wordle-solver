import matplotlib.pyplot as plt
import numpy as np

# Change path to absolute path
random_data = np.genfromtxt(r"./testing/random_data.csv", delimiter = ',')
random_time = random_data[-1]
random_data = random_data[:-1] 

entropy_data = np.genfromtxt(r"./testing/entropy_data.csv", delimiter = ',')
entropy_time = entropy_data[-1]
entropy_data = entropy_data[:-1] 

data = [random_data, entropy_data]
labels = ["Random choice", "Entropy maximizing"]

fig, ax = plt.subplots(figsize = (8,6))
n_bins = 7 # 1 to 8
plt.hist(data, bins = np.arange(1, n_bins + 3)-0.5, label = labels)

plt.title(f"Wordle Solver Results, n = {len(random_data)}", fontsize = 15)
plt.xlabel("Attempts", fontsize = 15)
plt.ylabel("Frequency", fontsize = 15)
plt.xticks(range(1, n_bins + 2))
plt.legend()

random_mean = np.mean(random_data)
plt.axvline(random_mean, c = 'k', ls = '--', lw = 1)
plt.text(random_mean + 0.3, 1100, f"Random mean: {random_mean:.3f}\nElapsed time: {random_time} s", 
         transform = ax.transData, fontsize =11)

entropy_mean = np.mean(entropy_data)
plt.axvline(entropy_mean, c = 'k', ls = '--', lw = 1)
plt.text(entropy_mean - 2.25, 1100, f"Entropy mean: {entropy_mean:.3f}\nElapsed time: {entropy_time} s", 
         transform = ax.transData, fontsize =11)

plt.tight_layout()
plt.savefig(r"./testing/plot.png")
