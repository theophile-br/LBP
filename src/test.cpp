#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <lbp.h>
#include <formula.h>
#include <test.h>
#include <utils.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;
using namespace cv;
using namespace filesystem;

int main(int argc, char **argv) {
    string dataset_path = path(current_path()) / path(PROCESS);
    if (!is_directory(dataset_path)) {
        cout << "PROCESS FOLDER NOT IN ROOT DIRECTORY" << endl;
        return EXIT_FAILURE;
    }
    test(path(dataset_path) / path(TEST), path(dataset_path) / path(DESCRIPTOR_FILE),
         path(dataset_path) / path(LABEL_FILE));
    cout << "END";
    return EXIT_SUCCESS;
}

void test(string path_to_dataset, std::string path_to_descriptor, std::string path_to_label) {
    cout << "\x1B[32m-- START PREDICTION --\033[0m" << endl;
    cout << "\x1B[33mDATASET FOLDER: \033[0m" << path_to_dataset << endl;
    // Read Label (value and length) from text file
    ifstream labelsFile(path_to_label);
    if (!labelsFile) {
        cout << "Cant Open Label File" << endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    int size = 0;
    for (; std::getline(labelsFile, line); ++size);
    const int labels_length = size;
    labelsFile.clear();
    labelsFile.seekg(0, ios::beg);
    string *labels = new string[labels_length];
    for (int i = 0; std::getline(labelsFile, line); ++i)
        labels[i] = line;
    labelsFile.close();
    // PREPARE VARIABLE FOR DISTANCE FORMULA / CONFUSION MATRICE
    int number_of_distance_formula = 4;
    string label_distance_formula[4] = {"SumOfAbsDif", "Chisquare", "Bhattacharyya", "Correlation"};
    float success[4] = {0};
    int confusion_matrice[4][labels_length][labels_length];
    memset(confusion_matrice, 0, 4 * labels_length * labels_length * sizeof(int));

    time_t start = time(0);
    ifstream descriptorFile(path_to_descriptor);
    line = string();
    int maxFile = 0;
    for (int i = 0; i < labels_length; i++) {
        maxFile += count_image_in_directory(path(path_to_dataset) / path(labels[i]));
    }
    if (!descriptorFile) {
        cout << "Cant Open Descriptor File" << endl;
        exit(EXIT_FAILURE);
    }
    vector<float> result(number_of_distance_formula, 0);
    int numberOfImageProcess = 0;
    cout << "START TESTING " << endl;
    // ITERATE BETWEEN LABELS
    for (int currentLabelId = 0; currentLabelId < labels_length; currentLabelId++) {
        string currentLabel = labels[currentLabelId];
        string pathToTestingSet = path(path_to_dataset) / path(currentLabel);
        // ITERATE FOLDER
        for (auto &p: directory_iterator(pathToTestingSet)) {
            int bestCandidatLabelId[4] = {0};
            vector<float> minError(number_of_distance_formula, 1.79769e+308);
            minError[3] = -1.0f;
            Mat currentImg = imread(p.path());
            float testDescriptorVector[256] = {0};
            img_2_lbp_hist(currentImg, testDescriptorVector);
            while (getline(descriptorFile, line)) {
                float trainDescriptorVector[256] = {0};
                txt_vector_2_vector(line, trainDescriptorVector);
                result[0] = sad(trainDescriptorVector, testDescriptorVector);
                result[1] = chisquare(trainDescriptorVector, testDescriptorVector);
                result[2] = bhattacharyya(trainDescriptorVector, testDescriptorVector);
                for (int i = 0; i < number_of_distance_formula - 1; i++) {
                    if (minError[i] > result[i]) {
                        minError[i] = result[i];
                        bestCandidatLabelId[i] = txt_vector_get_type(line);
                    }
                }
                // CORELATION SPECIAL LOGIC
                result[3] = correlation(trainDescriptorVector, testDescriptorVector);
                if (minError[3] < result[3]) {
                    minError[3] = result[3];
                    bestCandidatLabelId[3] = txt_vector_get_type(line);
                }
            }
            numberOfImageProcess++;
            //cout << " ----- " << endl;
            cout << flush << "\rDATASETS" << " " << currentLabel << " : " << numberOfImageProcess
                 << " images processed >> "
                 << (int) ((double) numberOfImageProcess / (double) maxFile * 100) << "%";
            for (int i = 0; i < number_of_distance_formula; i++) {
                if (bestCandidatLabelId[i] == currentLabelId) {
                    success[i] += 1;
                }
                // [EXPERT][CLASSIFIER]
                confusion_matrice[i][currentLabelId][bestCandidatLabelId[i]]++;
                cout << " --- " << label_distance_formula[i] << " : " << (int) (success[i] / numberOfImageProcess * 100)
                     << " %";
            }
            descriptorFile.clear();
            descriptorFile.seekg(0, ios::beg);
        }
    }
    // CSV RESULT
    ofstream outfile;
    // COMPUTE RATIO
    for (int i = 0; i < number_of_distance_formula; i++) {
        success[i] = success[i] / numberOfImageProcess * 100;
    }

    // DISTANCE SUCCESS RATE CSV
    stringstream ss;
    ss << string_array_join(label_distance_formula, number_of_distance_formula, ',') << endl;
    ss << float_array_join(success, number_of_distance_formula, ',') << endl;
    outfile.open(path(path_to_dataset) / path("..") / path(GRAPH) / path("distance_success_rate.csv"), ofstream::trunc);
    outfile << ss.str();
    outfile.close();

    // CONFUSION MATRICE FOR EACH DISTANCE CSV
    for (int d = 0; d < number_of_distance_formula; d++) {
        ss.str("");
        ss << "EXPERT/CLASSIFIER," << string_array_join(labels, labels_length, ',') << endl;
        for (int i = 0; i < labels_length; i++) {
            ss << labels[i] << ","
               << int_array_join(confusion_matrice[d][i], labels_length, ',') << endl;
        }
        outfile.open(path(path_to_dataset) / path("..") / path(GRAPH) /
                     path(toLowerCase(label_distance_formula[d]) + "_confusion_matrice.csv"),
                     ofstream::trunc);
        outfile << ss.str();
        outfile.close();
    }
    time_t end = time(0);
    double ltmDif = difftime(end, start);
    cout << endl << "Time: " << to_string(ltmDif) << "sec" << endl;
}