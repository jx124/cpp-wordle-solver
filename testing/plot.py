from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np

random_data = np.genfromtxt(Path(__file__).parent.joinpath("test_data", "random_data.csv"), delimiter = ',')
random_time = random_data[-1]
random_data = random_data[:-1] 

entropy_data = np.genfromtxt(Path(__file__).parent.joinpath("test_data", "entropy_data.csv"), delimiter = ',')
entropy_time = entropy_data[-1]
entropy_data = entropy_data[:-1] 

data = [random_data, entropy_data]
labels = ["Random choice", "Entropy maximizing"]

fig, ax = plt.subplots(figsize = (8,6))
n_bins = 8
plt.hist(data, bins = np.arange(1, n_bins + 2)-0.5, label = labels)

plt.title(f"Wordle Solver Results, Starting Word \"SLATE\", Hard Mode, n = {len(random_data)}", fontsize = 15)
plt.xlabel("Attempts", fontsize = 15)
plt.ylabel("Frequency", fontsize = 15)
plt.xticks(range(1, n_bins + 1))
plt.legend()

random_mean = np.mean(random_data)
plt.axvline(random_mean, c = 'k', ls = '--', lw = 1)
plt.annotate(f"Random mean: {random_mean:.3f}\nElapsed time: {random_time} s",
            (random_mean, 800),
            xytext = (40, 0),
            textcoords="offset points",
            arrowprops = dict(arrowstyle="->", connectionstyle="arc3,rad=0.2", relpos = (0,0.5)),
            fontsize = 11)

entropy_mean = np.mean(entropy_data)
plt.axvline(entropy_mean, c = 'k', ls = '--', lw = 1)
plt.annotate(f"Entropy mean: {entropy_mean:.3f}\nElapsed time: {entropy_time} s",
            (entropy_mean, 900),
            xytext = (-162, 0),
            textcoords="offset points",
            arrowprops = dict(arrowstyle="->", connectionstyle="arc3,rad=-0.2", relpos = (1,0.5)),
            fontsize = 11)

plt.axvline(3.509, c = 'b', ls = '--', lw = 1)
plt.annotate("Optimal mean: 3.509\nElapsed time: 4.5 s",
            (3.509, 800),
            xytext = (-160, 0),
            textcoords="offset points",
            arrowprops = dict(arrowstyle="->", connectionstyle="arc3,rad=-0.2", relpos = (1,0.5)),
            fontsize = 11)

plt.tight_layout()
plt.savefig(Path(__file__).parent.joinpath("plot.png"))
