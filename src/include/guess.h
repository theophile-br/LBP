//
// Created by teto on 02/11/2021.
//

#ifndef ROCK_CLASS_AI_GUESS_H
#define ROCK_CLASS_AI_GUESS_H

#include <string>
#include <opencv2/core/mat.hpp>

int guess(cv::Mat path_to_dataset, std::string path_to_descriptor, std::string path_to_label);

#endif //ROCK_CLASS_AI_GUESS_H
