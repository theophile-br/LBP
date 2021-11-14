import matplotlib.pyplot as plt
import numpy as np
import os
import csv

program_path = os.path.dirname(os.path.abspath(__file__))

csv_data = []
width = 0.35

with open(os.path.join(program_path, '..', 'lbp_process_histogram.csv')) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        csv_data.append(row)

data = np.asarray(csv_data).flatten().astype(float)  # data
plt.figure(figsize=(100, 100));
plt.bar(range(0, 256), data, 1)
plt.title("LBP histogram")
plt.xlabel("Pixel value")
plt.ylim([0, 1])
plt.xlim([0, 255])
plt.ylabel("Normalize quantity")

# plt.show()
plt.savefig(os.path.join(program_path, '..', 'lbp_histogram.jpg'))
