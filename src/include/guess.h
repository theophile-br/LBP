//
// Created by teto on 02/11/2021.
//

#ifndef ROCK_CLASS_AI_GUESS_H
#define ROCK_CLASS_AI_GUESS_H

#include <string>
#include <opencv2/core/mat.hpp>

int guess(cv::Mat img, std::string path_to_descriptor, std::string path_to_label);

void img_process_details(cv::Mat img);

#endif //ROCK_CLASS_AI_GUESS_H
