import numpy as np
import seaborn as sn
import pandas as pd
import matplotlib.pyplot as plt
import os
import csv

width = 0.35
for subdir, dirs, files in os.walk("../dataset/PROCESS/graph/"):
    for file in files:
        if "_confusion_matrice.csv" in file:
            csv_data = []
            with open('../dataset/PROCESS/graph/' + file) as csv_file:
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
