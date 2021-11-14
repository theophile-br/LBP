#include <segmentation.h>
#include <lbp.h>
#include <formula.h>
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
    cout << "\x1B[32m-- START GUESSING --\033[0m" << endl;
    cout << "\x1B[33mGUESSING IMAGE: \033[0m" << file_path << endl;
    segment(my_image, descriptor_path, label_path);
    return EXIT_SUCCESS;
}

void segment(Mat my_image, string path_to_descriptor, string path_to_label) {

    int width = src.cols;
    int height = src.rows;
    int GRID_SIZE = 100;

    vector<Rect> mCells;

    for (int y = 0; y < height - GRID_SIZE; y += GRID_SIZE) {
        for (int x = 0; x < width - GRID_SIZE; x += GRID_SIZE) {
            int k = x * y + x;
            Rect grid_rect(x, y, GRID_SIZE, GRID_SIZE);
            cout << grid_rect << endl;
            mCells.push_back(grid_rect);
            rectangle(src, grid_rect, Scalar(0, 255, 0), 1);
            imshow("src", src);
            imshow(format("grid%d", k), src(grid_rect));
            waitKey();
        }
    }
}