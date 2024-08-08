import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# After clustering
plt.figure()
df = pd.read_csv("clusters/basic5_3-clusters.csv")
sns.scatterplot(x=df["1° coordinate"], y=df["2° coordinate"],
                hue=df["clusterId"],
                palette=sns.color_palette("hls", n_colors=5))
plt.xlabel("x")
plt.ylabel("y")
plt.title("Kmeans")

plt.show()