#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <prepare.h>
#include <lbp.h>
#include <utils.h>
#include <filesystem>

using namespace std;
using namespace filesystem;
using namespace cv;

const float TEST_RATIO = 0.2;

int main(int argc, char **argv) {
    string dataset_path;
    if (argc == 1) {
        cout << "No arguments" << endl;
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--dataset" || string(argv[i]) == "-d") {
            dataset_path = path(current_path().string()) / path(argv[i + 1]);
            if (!is_directory(dataset_path)) {
                cout << current_path().string() + "/" + dataset_path << " isn't a directory" << endl;
                return EXIT_FAILURE;
            }
        }
    }
    if (empty(dataset_path)) {
        cout << "Wrong arguments" << endl;
        return EXIT_FAILURE;
    }
    cout << dataset_path << endl;
    prepare(dataset_path);
    analysis_dataset(path(current_path()) / path(PROCESS));
    cout << "\x1B[33m --     DONE     -- \033[0m" << endl;
    return 0;
}

void prepare(string path_to_dataset) {
    string path_to_process = path(current_path()) / path(PROCESS);
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
    remove_all(path_to_process);
    for (int i = 0; i < labelLength; i++) {
        process_data(path_to_dataset, labels[i]);
    }
}

void process_data(string path_to_dataset, string label) {
    string dataset_dest = path(current_path()) / path(PROCESS);
    string dataset_src = path(path_to_dataset) / path(label);
    int maxfile = count_image_in_directory(dataset_src);
    int i = 0;
    cout << "\x1B[33mPROCESS : " << label << "\033[0m" << endl;
    for (auto &p: directory_iterator(dataset_src)) {
        string writePath = "";
        if (i <= maxfile * TEST_RATIO) {
            writePath = path(dataset_dest) / path(TEST) / path(label);
        } else {
            writePath = path(dataset_dest) / path(TRAIN) / path(label);
        }

        Mat img = imread(p.path());
        if (img.empty()) {
            continue;
        }
        create_directories(writePath);
        imwrite(path(writePath) / path(to_string(i) + p.path().extension().string()), img);
        bool augmentation_du_jeu_de_donnees = false;
        if (augmentation_du_jeu_de_donnees) {
            // FLIP
            Mat flip_img = img.clone();
            flip(img, flip_img, 0);
            imwrite(path(writePath) / path(to_string(i) + "_flip" + p.path().extension().string()), img);
            // NOISE
            Mat noise_img = img.clone();
            Mat mSrc_16SC;
            Mat mGaussian_noise = Mat(img.size(), CV_16SC3);
            randn(mGaussian_noise, Scalar::all(0.0), Scalar::all(10.0));
            img.convertTo(mSrc_16SC, CV_16SC3);
            addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
            mSrc_16SC.convertTo(noise_img, img.type());
            imwrite(path(writePath) / path(to_string(i) + "_noise" + p.path().extension().string()), noise_img);
            // BLUR
            Mat blur_img = img.clone();
            GaussianBlur(img, blur_img, Size(5, 5), 0);
            imwrite(path(writePath) / path(to_string(i) + "_blur" + p.path().extension().string()), blur_img);
        }
        i++;
        cout << "\r" << (int) ((float) i / (float) maxfile * 100) << "% : " << p.path().filename();
    }
    cout << endl;
}

void analysis_dataset(string path_to_process_dataset) {
    int labelLength = 0;
    for (const auto &entry: directory_iterator(path(path_to_process_dataset) / path(TRAIN)))
        labelLength++;
    string labels[labelLength];
    int i = 0;
    for (const auto &entry: directory_iterator(path(path_to_process_dataset) / path(TRAIN))) {
        labels[i] = entry.path().filename();
        i++;
    }
    string test[labelLength];
    string train[labelLength];
    for (int i = 0; i < labelLength; i++) {
        int ntest = count_image_in_directory(
                path(path_to_process_dataset) / path(TEST) / path(labels[i]));
        int ntrain = count_image_in_directory(
                path(path_to_process_dataset) / path(TRAIN) / path(labels[i]));
        test[i] = to_string(ntest);
        train[i] = to_string(ntrain);
    }
    stringstream ss;
    ss.str();
    ss << "label," << string_array_join(labels, labelLength, ',') << "\n";
    ss << toLowerCase(TRAIN) << "," << string_array_join(train, labelLength, ',') << "\n";
    ss << toLowerCase(TRAIN) << "," << string_array_join(test, labelLength, ',') << "\n";
    ofstream outfile;
    create_directories(path(path_to_process_dataset) / path(GRAPH));
    outfile.open(path(path_to_process_dataset) / path(GRAPH) / path("dataset_distribution.csv"),
                 ofstream::trunc);
    outfile << ss.str().c_str();
    outfile.close();
}