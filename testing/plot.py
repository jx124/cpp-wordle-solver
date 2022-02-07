import matplotlib.pyplot as plt
import numpy as np

# Change path to absolute path
data = np.genfromtxt(r"./testing/random_data.csv", delimiter = ',')
data = data[:-1] # get rid of nan caused by trailing comma

plt.subplots(figsize = (8,6))
plt.hist(data, bins = np.arange(1,11)-0.5)

plt.title("Random strategy, starting guess \"crane\"\n Time elapsed: 8.387s, n = 2314", fontsize = 15)
plt.xlabel("Attempts", fontsize = 15)
plt.ylabel("Frequency", fontsize = 15)
plt.xticks(range(1,11))
plt.axvline(np.mean(data), c = 'k', ls = '-', label = f'Mean: {np.mean(data):.3f}', lw = 1)
plt.legend()
plt.tight_layout()

plt.savefig(r"./testing/random_plot.png")
