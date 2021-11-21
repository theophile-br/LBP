#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>

void img_2_lbp_hist(cv::Mat &img, float lbpHist[256]);

cv::Mat img_2_lbp_mat(cv::Mat &img);

int lbp_encode_pixel(cv::Mat img, int x, int y);

void normalize_hist(cv::Mat &img, float lbpHist[256]);

void txt_vector_2_vector(std::string descriptor, float v[256]);

int txt_vector_get_type(std::string descriptor);

std::string txt_vector_get_txt_histogram(std::string descriptor);

#endif