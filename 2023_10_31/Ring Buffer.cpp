#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define BUFFER_SIZE 5

int main() {
    int ringBuffer[BUFFER_SIZE] = { 0 };
    int index = 0;

    while (1) {
        int value;
        printf("값을 입력하세요: ");
        scanf("%d", &value);

        if (value == 0) {
            break;
        }

        ringBuffer[index] = value;

        // 버퍼 값을 출력
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", ringBuffer[i]);
        }
        printf("\n");

        index = (index + 1) % BUFFER_SIZE; // 다음 인덱스로 이동
    }

    return 0;
}
