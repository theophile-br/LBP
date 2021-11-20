#include <augmentation.h>
#include <lbp.h>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <utils.h>

using namespace std;
using namespace cv;
using namespace filesystem;

int main(int argc, char **argv) {
    string trainingset_path = path(current_path()) / path(PROCESS) / path(TRAIN);
    if (!is_directory(trainingset_path)) {
        cout << "PROCESS FOLDER NOT IN ROOT DIRECTORY" << endl;
        return EXIT_FAILURE;
    }
    augment(trainingset_path);
    cout << "\x1B[33m --     DONE     -- \033[0m" << endl;
    return EXIT_SUCCESS;
}

void augment(string path_to_dataset) {
    int labelLength = 0;
    for (const auto &entry: directory_iterator(path_to_dataset)) {
        if (is_directory(entry))
            labelLength++;
    }
    string labels[labelLength];
    int i = 0;
    for (const auto &entry: directory_iterator(path_to_dataset)) {
        if (is_directory(entry)) {
            labels[i] = entry.path().filename().string();
            i++;
        }
    }
    for (int i = 0; i < labelLength; i++) {
        int c = 0;
        string dataset_src = path(path_to_dataset) / path(labels[i]);
        int maxfile = count_image_in_directory(dataset_src);
        cout << "\x1B[33mPROCESS : " << labels[i] << "\033[0m" << endl;
        for (auto &p: directory_iterator(dataset_src)) {
            Mat img = imread(p.path());
            if (img.empty()) {
                c++;
                continue;
            }
            // FLIP
            Mat flip_img = img.clone();
            flip(img, flip_img, 0);
            imwrite(path(dataset_src) / path(to_string(i) + "_flip" + p.path().extension().string()), flip_img);
            // NOISE
            Mat noise_img = img.clone();
            Mat mSrc_16SC;
            Mat mGaussian_noise = Mat(img.size(), CV_16SC3);
            randn(mGaussian_noise, Scalar::all(0.0), Scalar::all(10.0));
            img.convertTo(mSrc_16SC, CV_16SC3);
            addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
            mSrc_16SC.convertTo(noise_img, img.type());
            imwrite(path(dataset_src) / path(to_string(i) + "_noise" + p.path().extension().string()), noise_img);
            // BLUR
            Mat blur_img = img.clone();
            GaussianBlur(img, blur_img, Size(5, 5), 0);
            imwrite(path(dataset_src) / path(to_string(i) + "_blur" + p.path().extension().string()), blur_img);
            c++;
            cout << "\r" << (int) ((float) c / (float) maxfile * 100) << "% : " << p.path().filename();
        }
        cout << endl;
    }
}
