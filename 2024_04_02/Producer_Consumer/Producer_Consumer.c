#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 7

int items[SIZE]; 
int front = -1, rear = -1; 
int total = 0; 

// 큐가 가득 찼는지 확인
int isFull() {
    if ((front == 0 && rear == SIZE - 1) || (rear == front - 1))
        return 1; 
    return 0; 
}

// 큐가 비었는지 확인
int isEmpty() {
    if (front == -1)
        return 1; 
    return 0; 
}

// 큐에 요소를 추가하는 함수
void enqueue(int element) {
    if (isFull()) {
        printf("\n Overflow! Cannot insert element %d\n", element); 
        return;
    }
    else {
        if (front == -1)
            front = 0; // 큐가 비어있을 때 front 초기화
        rear = (rear + 1) % SIZE; //rear을 다음 위치로 이동
        items[rear] = element; // rear에 요소 추가
        printf("\n inserted -> %d\n", element); 
    }
}

// 큐에서 요소를 제거하고 반환하는 함수
int dequeue() {
    int element;
    if (isEmpty()) {
        printf("\n Queue is empty \n"); 
        return -1;
    }
    else {
        element = items[front]; // 큐의 front 요소 제거하고 반환
        if (front == rear) {
            front = -1;
            rear = -1; // 큐가 비었을 때 front, reaar 초기화
        }
        else {
            front = (front + 1) % SIZE; // front를 다음 위치로 이동
        }
        printf("\n Deleted element -> %d \n", element);
        return element;
    }
}

// 큐의 내용을 출력하는 함수
void display() {
    int i;
    if (isEmpty())
        printf("\n Empty Queue\n"); 
    else {
        printf("\n Front -> %d", front); 
        printf("\n Items -> ");
        for (i = front; i != rear; i = (i + 1) % SIZE) {
            printf("%d ", items[i]); // front부터 rear 까지의 요소 출력
        }
        printf("%d ", items[i]); // rear 요소 출력
        printf("\n Rear -> %d \n", rear); // rear 위치 출력
    }
}

// 생산자의 역할을 하는 함수
void producer() {
    int processer = rand() % 4; // 0~3 랜덤 수를 생성
    printf("Producer processes: %d\n", processer); 
    for (int i = 0; i < processer; i++) {
        int node = rand() % 11 + 10; // 10~20 랜덤 수 생성
        enqueue(node);
    }
}

// 소비자의 역할을 하는 함수
void consumer() {
    int processer = rand() % 4; // 0~3 랜덤 수를 생성
    printf("Consumer processes: %d\n", processer); 
    for (int i = 0; i < processer; i++) {
        int node = dequeue(); // 큐에서 아이템 제거
        if (node != -1)
            total += node; // 제거된 아이템의 값을 총합에 추가
    }
}

int main() {
    srand(time(NULL)); 
    while (total < 100) { 
        producer(); 
        consumer(); 
        printf("Total = %d\n", total); 
        if (isFull()) {
            printf("Overflow occurred!\n"); 
            break;
        }
    }
    return 0;
}
