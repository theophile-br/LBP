//
// Created by teto on 10/11/2021.
//
#include "filesystem"
#include "utils.h"

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
