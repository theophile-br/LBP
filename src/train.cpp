#include <train.h>
#include <lbp.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <utils.h>
#include <fstream>

using namespace std;
using namespace cv;
using namespace filesystem;

int main(int argc, char **argv) {
    string trainingset_path = path(current_path()) / path(PROCESS);
    if (!is_directory(trainingset_path)) {
        cout << "PROCESS FOLDER NOT IN ROOT DIRECTORY" << endl;
        return EXIT_FAILURE;
    }
    train(path(trainingset_path) / path(TRAIN));
    cout << "END";
    return EXIT_SUCCESS;
}

void train(string trainingset_path) {
    cout << "\x1B[32m-- START TRAINING --\033[0m" << endl;
    cout << "\x1B[33mDATASET FOLDER: \033[0m" << trainingset_path << endl;
    produce_label_file(trainingset_path);
    // Read Label (value and length) from text file
    ifstream labelsFile(path(trainingset_path) / path("..") / path(LABEL_FILE));
    if (!labelsFile) {
        cout << "Cant Open Descriptor File" << endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    int labelsLength = 0;
    for (; std::getline(labelsFile, line); ++labelsLength);
    labelsFile.clear();
    labelsFile.seekg(0, ios::beg);
    string *labels = new string[labelsLength];
    for (int i = 0; std::getline(labelsFile, line); ++i)
        labels[i] = line;
    labelsFile.close();
    ofstream outfile;
    string descriptorPath = path(trainingset_path) / path("..") / path(DESCRIPTOR_FILE);
    outfile.open(descriptorPath, ofstream::trunc);
    for (int i = 0; i < labelsLength; i++) {
        cout << "\x1B[32m--      TYPE: " << labels[i] << "      --\033[0m" << endl;
        process_descriptor_for_label(path(trainingset_path) / path(labels[i]), i, &outfile);
    }
    outfile.close();
};

void process_descriptor_for_label(std::string path, int labelId, ofstream *file) {
    stringstream data;
    data.str(string());
    int count = 0;
    int maxfile = count_image_in_directory(path);
    cout << "process " << maxfile << " images" << endl;
    for (auto &p: directory_iterator(path)) {
        count++;
        cout << flush << "\r"
             << (int) ((float) count / (float) maxfile * 100) << "%";
        Mat img = imread(p.path());
        float pblHist[256] = {0};
        data.str(string());
        img_2_lbp_hist(img, pblHist);
        for (int i = 0; i < 256; i++) {
            data << pblHist[i];
            if (i != 256 - 1) {
                data << ",";
            }
        }
        *file << data.str() << ":" << labelId << endl;
    }
    cout << endl << "\x1B[33m --     DONE     -- \033[0m" << endl;
}

void produce_label_file(string path_to_test_directory) {
    stringstream ss;
    ss.str(string());
    int i = 0;
    for (const auto &entry: directory_iterator(path_to_test_directory)) {
        if (is_directory(entry)) {
            if (i != 0)
                ss << endl;
            ss << entry.path().filename().string();
            i++;
        }

    }
    ofstream outfile;
    outfile.open(path(path_to_test_directory) / path("..") / path("label.txt"), ofstream::trunc);
    outfile << ss.str();
    outfile.close();
}