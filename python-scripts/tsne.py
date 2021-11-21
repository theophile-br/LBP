from sklearn.manifold import TSNE
import cv2.cv2 as cv
import sys
import os
import numpy as np
from numpy import reshape
import seaborn as sns
import pandas as pd

path = os.path.dirname(os.path.realpath(__file__))
dest_dir = os.path.join(path, "..", "dataset", 'etrit')
X = []
Y = []
for subdir, labels, files in os.walk(dest_dir):
    for label in labels:
        path_to_data_label = os.path.join(dest_dir, label)
        for subdir, dirs, files in os.walk(path_to_data_label):
            file_count = len(files)
            for i in range(file_count):
                sys.stdout.write("\r" + str(i) + "/" + str(file_count))
                sys.stdout.flush()
                img_path = os.path.join(subdir, files[i])
                X.append(cv.imread(img_path, cv.IMREAD_GRAYSCALE));
                Y.append(label);
                break;
X = np.ndarray(X).astype(object)
Y = np.ndarray(Y).astype(object)
# tsne = TSNE(n_components=2, random_state=0).fit_transform(X)
