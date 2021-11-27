#include <segmentation.h>
#include <lbp.h>
#include <formula.h>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <filesystem>

using namespace std;
using namespace cv;
using namespace filesystem;

// COLOR
Scalar red = Scalar(57, 0, 199);
Scalar yellow = Scalar(0, 195, 255);
Scalar blue = Scalar(255, 221, 51);
Scalar deep_blue = Scalar(200, 91, 0);
Scalar green = Scalar(51, 255, 153);
Scalar orange = Scalar(51, 165, 255);
Scalar purple = Scalar(255, 51, 156);
Scalar pink = Scalar(150, 51, 255);
Scalar brown = Scalar(0, 94, 164);

vector<Scalar> palette = {red, yellow, blue, green, orange, purple, pink, brown, deep_blue};

int main(int argc, char **argv) {
    string file_path;
    string descriptor_path;
    string label_path;
    if (argc == 1) {
        cout << "No arguments" << endl;
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--file" || string(argv[i]) == "-f") {
            file_path = current_path().string() + "/" + argv[i + 1];
            ifstream f(file_path);
            if (!f.good()) {
                cout << file_path << " isn't a file" << endl;
                return EXIT_FAILURE;
            }
        } else if (string(argv[i]) == "--descriptor" || string(argv[i]) == "-m") {
            descriptor_path = current_path().string() + "/" + argv[i + 1];
        } else if (string(argv[i]) == "--label" || string(argv[i]) == "-l") {
            label_path = current_path().string() + "/" + argv[i + 1];
        }
        i++;
    }

    if (empty(label_path) || empty(descriptor_path) || empty(file_path)) {
        cout << "Wrong arguments" << endl;
        return EXIT_FAILURE;
    }
    Mat my_image = imread(file_path);
    cout << "\x1B[32m-- START SEGMENTATION --\033[0m" << endl;
    cout << "\x1B[33mSEGMENT IMAGE: \033[0m" << file_path << endl;
    segment(my_image, descriptor_path, label_path);
    return EXIT_SUCCESS;
}

void segment(Mat src, string path_to_descriptor, string path_to_label) {

    int width = src.cols;
    int height = src.rows;
    int GRID_SIZE = 60;
    //int GRID_SIZE = 30;
    //int GRID_SIZE = 15;

    vector<Mat> data;
    cout << endl;
    Mat dest = src.clone();
    for (int y = 0; y < height - GRID_SIZE; y += GRID_SIZE) {
        for (int x = 0; x < width - GRID_SIZE; x += GRID_SIZE) {
            Rect grid_rect(x, y, GRID_SIZE, GRID_SIZE);
            cout << flush << "\r" << grid_rect;
            int label = guess(src(grid_rect), path_to_descriptor, path_to_label);
            rectangle(dest, grid_rect, palette[label], -1);
            data.push_back(dest(grid_rect));
        }
    }
    dest.data = data.data()->data;
    //imshow("MASK", dest);
    //waitKey();
    imwrite(current_path() / path("segmentation_mask.jpg"), dest);
    addWeighted(src, 0.5, dest, 0.5, 0.0, src);
    imwrite(current_path() / path("segmentation.jpg"), src);
    //imshow("SRC", src);
    //waitKey();

    //PRODUCE LABEL COLOR CORRESPONDENCE
    ifstream labelsFile(path_to_label);
    if (!labelsFile) {
        cout << "Cant Open Label File" << endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    int labels_length = 0;
    for (; std::getline(labelsFile, line); ++labels_length);
    labelsFile.clear();
    labelsFile.seekg(0, ios::beg);
    string *labels = new string[labels_length];
    for (int i = 0; std::getline(labelsFile, line); ++i)
        labels[i] = line;
    labelsFile.close();
    int rect_size = 25;
    int padding_bottom = 50;
    int padding_left = 20;
    Mat img_label(500, 500, CV_8UC3);
    for (int i = 0; i < labels_length; i++) {
        rectangle(img_label, Rect(0, i * padding_bottom, rect_size, rect_size), palette[i], -1);
        putText(img_label, labels[i], Point(rect_size + padding_left, (i * padding_bottom) + rect_size / 2),
                FONT_HERSHEY_PLAIN,
                1,
                palette[i],
                1);
    }
    imwrite(path(current_path()) / path("segmentatin_label.jpg"), img_label);
}

int guess(Mat my_image, string path_to_descriptor, string path_to_label) {
    // READ LABEL FROM LABEL.TXT
    ifstream labelsFile(path_to_label);
    if (!labelsFile) {
        cout << "Cant Open Label File" << endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    int labels_length = 0;
    for (; std::getline(labelsFile, line); ++labels_length);
    labelsFile.clear();
    labelsFile.seekg(0, ios::beg);
    string *labels = new string[labels_length];
    for (int i = 0; std::getline(labelsFile, line); ++i)
        labels[i] = line;
    labelsFile.close();
    //OPEN DESCRIPTOR FILE
    ifstream descriptorFile(path_to_descriptor);
    if (!descriptorFile) {
        cout << "Cant Open Descriptor File" << endl;
        exit(EXIT_FAILURE);
    }
    float result;
    int bestCandidatLabelId = 0;
    float minError = 3.40282347E+38f;
    float img_descriptor_vector[256] = {0};
    img_2_lbp_hist(my_image, img_descriptor_vector);
    while (getline(descriptorFile, line)) {
        float trainDescriptorVector[256] = {0};
        txt_vector_2_vector(line, trainDescriptorVector);
        result = sad(trainDescriptorVector, img_descriptor_vector);
        if (minError > result) {
            minError = result;
            bestCandidatLabelId = txt_vector_get_type(line);
        }
    }

    descriptorFile.close();
    cout << " I think is : " << labels[bestCandidatLabelId];
    return bestCandidatLabelId;
}
