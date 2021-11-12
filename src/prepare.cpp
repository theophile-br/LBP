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
            dataset_path = current_path().string() + "/" + argv[i + 1];
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
    prepare(dataset_path);
    analysis_dataset(dataset_path);
    cout << "\x1B[33m --     DONE     -- \033[0m" << endl;
    return 0;
}

void prepare(string path_to_dataset) {
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
    remove_all(path(path_to_dataset) / path("..") / path(PROCESS));
    for (int i = 0; i < labelLength; i++) {
        process_data(path_to_dataset, labels[i]);
    }
}

void process_data(string path_to_dataset, string label) {
    string completePath = path(path_to_dataset) / path(label);
    int maxfile = count_image_in_directory(completePath);
    int i = 0;
    cout << "\x1B[33mPROCESS : " << label << "\033[0m" << endl;
    for (auto &p: directory_iterator(completePath)) {
        string writePath = "";
        if (i <= maxfile * TEST_RATIO) {
            writePath = path(path_to_dataset) / path("..") / path(PROCESS) / path(TEST) / path(label);
        } else {
            writePath = path(path_to_dataset) / path("..") / path(PROCESS) / path(TRAIN) / path(label);
        }

        Mat img = imread(p.path());
        if (!img.cols) {
            continue;
        }
        /*Mat greyMat;
        if (greyMat.channels() == 1) {
            greyMat = img;
        } else {
            cvtColor(img, greyMat, COLOR_BGR2GRAY);
        }*/
        create_directories(writePath);
        imwrite(path(writePath) / path(to_string(i) + p.path().extension().string()), img);
        i++;
        cout << "\r" << (int) ((float) i / (float) maxfile * 100) << "% : " << p.path().filename();
    }
    cout << endl;
}

void analysis_dataset(string path_to_dataset) {
    int labelLength = 0;
    for (const auto &entry: directory_iterator(path_to_dataset))
        labelLength++;
    string labels[labelLength];
    int i = 0;
    for (const auto &entry: directory_iterator(path_to_dataset)) {
        labels[i] = entry.path().filename();
        i++;
    }
    string test[labelLength];
    string train[labelLength];
    string total[labelLength];

    string pathToProcess = path(path_to_dataset) / path("..") / path(PROCESS);
    for (int i = 0; i < labelLength; i++) {
        int ntest = count_image_in_directory(
                path(pathToProcess) / path(TEST) / path(labels[i]));
        int ntrain = count_image_in_directory(
                path(pathToProcess) / path(TRAIN) / path(labels[i]));
        test[i] = to_string(ntest);
        train[i] = to_string(ntrain);
        total[i] = to_string(ntest + ntrain);
    }
    stringstream ss;
    ss.str();
    ss << "DATA," << string_array_join(labels, labelLength, ',') << "\n";
    ss << "TRAIN," << string_array_join(train, labelLength, ',') << "\n";
    ss << "TEST," << string_array_join(test, labelLength, ',') << "\n";
    ss << "TOTAL," << string_array_join(total, labelLength, ',') << "\n";
    ofstream outfile;
    outfile.open(path(path_to_dataset) / path("..") / path(PROCESS) / path("dataset_distribution.csv"),
                 ofstream::trunc);
    outfile << ss.str().c_str();
    outfile.close();
}