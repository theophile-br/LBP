#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <string>

void train(std::string path_to_dataset);

void process_descriptor_for_label(std::string path, int labelId, std::ofstream *file);

void produce_label_file(std::string path_to_test_directory);

#endif