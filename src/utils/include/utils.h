//
// Created by teto on 10/11/2021.
//

#ifndef ROCK_CLASS_AI_UTILS_H
#define ROCK_CLASS_AI_UTILS_H

#include <string>

static const std::string TRAIN("TRAIN");
static const std::string PROCESS("PROCESS");
static const std::string TEST("TEST");
static const std::string GRAPH("graph");
static const std::string DESCRIPTOR_FILE("descriptor.txt");
static const std::string LABEL_FILE("label.txt");

int count_image_in_directory(std::string directory);

std::string string_array_join(std::string *array, int size, char del);

std::string double_array_join(double array[], int size, char del);

std::string int_array_join(int array[], int size, char del);

std::string float_array_join(float array[], int size, char del);

float float_array_sum(float array[], int size);

#endif //ROCK_CLASS_AI_UTILS_H
