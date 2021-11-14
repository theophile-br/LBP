import numpy as np
import seaborn as sn
import pandas as pd
import matplotlib.pyplot as plt
import os
import csv

program_path = os.path.dirname(os.path.abspath(__file__))

width = 0.35
for subdir, dirs, files in os.walk(os.path.join(program_path, "..", "PROCESS", "graph")):
    for file in files:
        if file.endswith("_confusion_matrice.csv"):
            csv_data = []
            with open(os.path.join(program_path, "..", "PROCESS", "graph", file)) as csv_file:
                csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                csv_data.append(row)
            y = np.asarray(csv_data)[:1, 1:][0]  # label
            label = csv_data[0][0].split("/")  # Expert/Classifier
            data = np.asarray(csv_data)[1:, 1:].astype(int)  # data
            df_cm = pd.DataFrame(data, index=[i for i in y],
                                 columns=[i for i in y])
            plt.figure(figsize=(10, 7))
            plt.title("Matrice de confusion " + file.split("_")[0])
            c1 = sn.heatmap(df_cm, annot=True, fmt="1")
            c1.set(ylabel="True label", xlabel="Predicted label")
            # plt.show()
            plt.savefig('../dataset/PROCESS/graph/' + file + '.jpg')

            csv_data = []
            with open('../dataset/PROCESS/graph/distance_success_rate.csv') as csv_file:
                csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                csv_data.append(row)

            x = np.asarray(csv_data)[:1].flatten()  # label
            data = np.asarray(csv_data)[1:].flatten().astype(float)  # data
            fig, ax = plt.subplots()
            for i in range(0, len(x)):
                ax.bar(x[i], data[i], width)

            plt.title("Distance success rate")
            plt.xlabel("Distance formula")
            plt.ylabel("Pourcent (%)")
            plt.ylim([0, 100])

            for index, value in enumerate(data):
                plt.text(index - 0.15, value + 0.5, str(value) + "%")

            plt.savefig(os.path.join(program_path, '..', 'PROCESS', 'graph', 'distance_success_rate.jpg'))
