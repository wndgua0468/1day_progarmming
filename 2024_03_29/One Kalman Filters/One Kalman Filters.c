#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


float kalmanFilter(float measurement, float* estimate, float* errorCovariance, float q, float r) {
    float kalmanGain = *errorCovariance / (*errorCovariance + r);
    *estimate += kalmanGain * (measurement - *estimate); // 업데이트
    *errorCovariance = (1 - kalmanGain) * (*errorCovariance) + q; // 오차 공분산 업데이트
    return *estimate;
}

int main() {
    FILE* inputFile = fopen("D:/chrome down/position_1.txt", "r");
    FILE* outputFile = fopen("D:/chrome down/filtered_1.txt", "w");
    if (!inputFile || !outputFile) {
        printf("File opening failed.\n");
        return 1;
    }

    // 파일 헤더 건너뛰기
    fscanf(inputFile, "%*[^\n]\n");

    float x, y, z;
    float estimateX = 0.0, estimateY = 0.0, estimateZ = 0.0;
    float errorCovX = 1.0, errorCovY = 1.0, errorCovZ = 1.0;
    float q = 0.1, r = 0.1; // 필터 파라미터

    // 데이터를 읽고 필터링한 뒤 결과 파일에 쓰기
    while (fscanf(inputFile, "%f %f %f\n", &x, &y, &z) == 3) {
        float filteredX = kalmanFilter(x, &estimateX, &errorCovX, q, r);
        float filteredY = kalmanFilter(y, &estimateY, &errorCovY, q, r);
        float filteredZ = kalmanFilter(z, &estimateZ, &errorCovZ, q, r);

        fprintf(outputFile, "%f   %f   %f\n", filteredX, filteredY, filteredZ);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Data filtering complete. Output written to filtered_position_data.txt\n");
    return 0;
}
