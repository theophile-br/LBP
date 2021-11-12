#include <opencv2/core/core.hpp>
#include <iostream>
#include <filesystem>
#include "lbp.h"

using namespace std;
using namespace cv;
using namespace filesystem;


int lbp_encode_pixel(Mat img, int x, int y) {
    uchar bits[8];
    int center_pixel = img.at<uchar>(y, x);

    bits[0] = (center_pixel >= img.at<uchar>(y - 1, x - 1)) ? 1 : 0;
    bits[1] = (center_pixel >= img.at<uchar>(y - 1, x)) ? 1 : 0;
    bits[2] = (center_pixel >= img.at<uchar>(y - 1, x + 1)) ? 1 : 0;
    bits[3] = (center_pixel >= img.at<uchar>(y, x + 1)) ? 1 : 0;
    bits[4] = (center_pixel >= img.at<uchar>(y + 1, x + 1)) ? 1 : 0;
    bits[5] = (center_pixel >= img.at<uchar>(y + 1, x)) ? 1 : 0;
    bits[6] = (center_pixel >= img.at<uchar>(y + 1, x - 1)) ? 1 : 0;
    bits[7] = (center_pixel >= img.at<uchar>(y, x - 1)) ? 1 : 0;

    int decimal_value = 0;
    for (int i = 0; i < 8; i++)
        decimal_value += pow(2, i) * bits[i];
    return decimal_value;
}

Mat gray2LBPMat(Mat &img) {
    vector<uchar> pblImg;
    for (int y = 1; y < img.rows - 1; y++) {
        for (int x = 1; x < img.cols - 1; x++) {
            pblImg.push_back(lbp_encode_pixel(img, x, y));
        }
    }
    Mat my_mat(img.rows - 2, img.cols - 2, CV_8UC1, pblImg.data());
    return my_mat;
}

void gray_img_2_lbp_hist(Mat &img, float lbpHist[256]) {
    for (int y = 1; y < img.rows - 1; y++) {
        for (int x = 1; x < img.cols - 1; x++) {
            int decimal_value = lbp_encode_pixel(img, x, y);
            lbpHist[decimal_value] = lbpHist[decimal_value] + 1;
        }
    }
    normalize_hist(img, lbpHist);
}

void gray_txt_vector_2_vector(string descriptor, float v[256]) {
    descriptor = gray_txt_vector_get_txt_histogram(descriptor);
    string delimiter = ",";
    size_t pos = 0;
    string token;
    int i = 0;
    while ((pos = descriptor.find(delimiter)) != std::string::npos) {
        token = descriptor.substr(0, pos);
        v[i] = stof(token.c_str());
        descriptor.erase(0, pos + delimiter.length());
        i++;
    }
    token = descriptor.substr(0, pos);
    v[255] = stof(token.c_str());
}

int gray_txt_vector_get_type(string descriptor) {
    string delimiterType = ":";
    size_t pos = descriptor.find(delimiterType);
    string type = descriptor.erase(0, pos + delimiterType.length());
    return atoi(type.c_str());
}

string gray_txt_vector_get_txt_histogram(string descriptor) {
    string delimiterType = ":";
    size_t pos = descriptor.find(delimiterType);
    return descriptor.substr(0, pos);
}

void normalize_hist(Mat &img, float lbpHist[256]) {
    float float_lbp_hist[256];
    int imgSurface = img.cols * img.rows;
    for (int i = 0; i < 256; i++) {
        lbpHist[i] = (float) lbpHist[i] / (float) imgSurface;
    };
}

