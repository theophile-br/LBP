//
// Created by teto on 14/11/2021.
//

#ifndef ROCK_CLASS_AI_SEGMENTATION_H
#define ROCK_CLASS_AI_SEGMENTATION_H

#include <string>
#include <opencv2/core/mat.hpp>

void segment(cv::Mat my_image, std::string path_to_descriptor, std::string path_to_label);

int guess(cv::Mat img, std::string path_to_descriptor, std::string path_to_label);

#endif //ROCK_CLASS_AI_SEGMENTATION_H
