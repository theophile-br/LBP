import matplotlib.pyplot as plt
import numpy as np
import os
import csv

program_path = os.path.dirname(os.path.abspath(__file__))

csv_data = []
width = 0.35

with open(os.path.join(program_path, '..', 'PROCESS', 'graph', 'dataset_distribution_augmented.csv')) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        csv_data.append(row)

y = np.asarray(csv_data)[:1, 1:][0]  # test / train
x = np.asarray(csv_data)[1:, :1].flatten()  # label
data = np.asarray(csv_data)[1:, 1:].astype(int)  # data

fig, ax = plt.subplots()
ax.bar(y, data[0], width, label=x[0])
ax.bar(y, data[1], width, label=x[1], bottom=data[0])

ax.legend()
plt.title("Dataset distribution")
plt.xlabel("Label")
plt.ylabel("Total")

# plt.show()
plt.savefig(os.path.join(program_path, '..', 'PROCESS', 'graph', 'dataset_distribution_augmented.jpg'))
