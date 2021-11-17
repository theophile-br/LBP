//
// Created by teto on 10/11/2021.
//
#include "filesystem"
#include "utils.h"
#include <math.h>

using namespace std;
using namespace filesystem;

int count_image_in_directory(std::string directory) {
    namespace fs = filesystem;
    fs::path Path(directory);
    int Nb_ext = 0;
    fs::directory_iterator end_iter;

    for (fs::directory_iterator iter(Path); iter != end_iter; ++iter)
        if (iter->path().extension() == ".png" || iter->path().extension() == ".jpg" ||
            iter->path().extension() == ".JPEG" ||
            iter->path().extension() == ".jpeg")
            ++Nb_ext;

    return Nb_ext;
}

string string_array_join(string array[], int size, char del) {
    stringstream ss;
    ss.str();
    for (int i = 0; i < size; i++) {
        ss << array[i];
        if (i != size - 1)
            ss << del;
    }
    return ss.str();
}

string double_array_join(double array[], int size, char del) {
    stringstream ss;
    ss.str();
    for (int i = 0; i < size; i++) {
        ss << array[i];
        if (i != size - 1)
            ss << del;
    }
    return ss.str();
}

string int_array_join(int array[], int size, char del) {
    stringstream ss;
    ss.str();
    for (int i = 0; i < size; i++) {
        ss << array[i];
        if (i != size - 1)
            ss << del;
    }
    return ss.str();
}

string float_array_join(float array[], int size, char del) {
    stringstream ss;
    ss.str();
    for (int i = 0; i < size; i++) {
        ss << array[i];
        if (i != size - 1)
            ss << del;
    }
    return ss.str();
}

float float_array_sum(float array[], int size) {
    float total = 0;
    for (int i = 0; i < size; i++) {
        total += array[i];
    }
    return total;
}

float roundoff(float value, unsigned char prec) {
    float pow_10 = pow(10.0f, (float) prec);
    return round(value * pow_10) / pow_10;
}
