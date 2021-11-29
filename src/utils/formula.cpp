#include "lbp.h"
#include "utils.h"

using namespace std;

float sad(float v1[256], float v2[256]) {
    float sumOfDifference = 0;
    float ratioV1 = float_array_sum(v1, 256);
    float ratioV2 = float_array_sum(v2, 256);
    for (int i = 0; i < 256; i++) {
        sumOfDifference += abs(v1[i] / ratioV1 - v2[i] / ratioV2);
    }
    return sumOfDifference;
}

float correlation(float v1[256], float v2[256]) {
    float ratioV1 = float_array_sum(v1, 256);
    float ratioV2 = float_array_sum(v2, 256);
    float mean_v1 = 0, mean_v2 = 0, dividend = 0, diviseur1 = 0, diviseur2 = 0;
    for (int i = 0; i < 256; i++) {
        mean_v1 += v1[i] / ratioV1;
        mean_v2 += v2[i] / ratioV1;
    }
    mean_v1 = mean_v1 / 256;
    mean_v2 = mean_v2 / 256;
    for (int i = 0; i < 256; i++) {
        dividend += (v1[i] / ratioV1 - mean_v1) * (v2[i] / ratioV2 - mean_v2);
        diviseur1 += (v1[i] / ratioV1 - mean_v1) * (v1[i] / ratioV1 - mean_v1);
        diviseur2 += (v2[i] / ratioV2 - mean_v2) * (v2[i] / ratioV2 - mean_v2);
    }
    return dividend / sqrt(diviseur1 * diviseur2);
}

float chisquare(float v1[256], float v2[256]) {
    float ratioV1 = float_array_sum(v1, 256);
    float ratioV2 = float_array_sum(v2, 256);
    float sumChiSquare = 0;
    for (int i = 0; i < 256; i++) {
        if (v2[i] == 0) {
            continue;
        }
        sumChiSquare += ((v1[i] / ratioV1 - v2[i]) / ratioV2 * ((v1[i] / ratioV1 - v2[i] / ratioV2))) / v2[i] / ratioV2;
    }
    return sumChiSquare;
}

float bhattacharyya(float v1[256], float v2[256]) {
    float ratioV1 = float_array_sum(v1, 256);
    float ratioV2 = float_array_sum(v2, 256);
    float mean_v1 = 0, mean_v2 = 0, coefBhattacharyaSum = 0;
    for (int i = 0; i < 256; i++) {
        mean_v1 += v1[i] / ratioV1;
        mean_v2 += v2[i] / ratioV2;
        coefBhattacharyaSum += sqrt(v1[i] / ratioV1 * v2[i] / ratioV2);
    }
    mean_v1 = mean_v1 / 256;
    mean_v2 = mean_v2 / 256;
    return sqrt(1 -
                (1 / sqrt(mean_v1 * mean_v2 * 256 * 256))
                * coefBhattacharyaSum);
}