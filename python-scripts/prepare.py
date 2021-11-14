import matplotlib.pyplot as plt
import numpy as np
import os
import csv

program_path = os.path.dirname(os.path.abspath(__file__))

csv_data = []
width = 0.35

with open(os.path.join(program_path, '..', 'PROCESS', 'graph', 'dataset_distribution.csv')) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        csv_data.append(row)

y = np.asarray(csv_data)[:1, 1:][0]  # test / train
x = np.asarray(csv_data)[1:, :1].flatten()  # label
data = np.asarray(csv_data)[1:, 1:].astype(int)  # data

fig, ax = plt.subplots()
for i in range(0, len(x)):
    ax.bar(y, data[i], width, label=x[i])

ax.legend()
plt.title("Dataset distribution")
plt.xlabel("Label")
plt.ylabel("Total")

# plt.show()
plt.savefig(os.path.join(program_path, '..', 'PROCESS', 'graph', 'dataset_distribution.jpg'))
