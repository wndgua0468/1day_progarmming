#include "stdafx.h"
#include "CAN_DLL/CAN_Access.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

void SendCanMessage(CAN_HANDLE h) {
    long sid = 0;
    unsigned char sdata[8] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11 };

    int ret1 = CAN_Send(h, sid, 8, (char*)sdata, 0, 0);
    if (!ret1) {
        printf("Send failed.\n");
    }
}

void RecvCanMessage(CAN_HANDLE h, SOCKET sock) {
    char rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    long rid;
    int rlen, ext, rtr;

    int ret = CAN_Recv(h, &rid, &rlen, rdata, &ext, &rtr);
    printf("CAN %d\n",ret);
    if (ret) {
        // 수신된 메시지 포맷 지정
        char recv_message[100];
        sprintf(recv_message,
            "%03X %d %02X %02X %02X %02X %02X %02X %02X %02X",
            rid, rlen,
            (int)(unsigned char)rdata[0],
            (int)(unsigned char)rdata[1],
            (int)(unsigned char)rdata[2],
            (int)(unsigned char)rdata[3],
            (int)(unsigned char)rdata[4],
            (int)(unsigned char)rdata[5],
            (int)(unsigned char)rdata[6],
            (int)(unsigned char)rdata[7]
        );

        // 소켓을 통해 메시지를 전송
        if (send(sock, recv_message, strlen(recv_message), 0) < 0) {
            printf("Send to server failed : %d\n", WSAGetLastError());
        }
    }
    else { printf("CAN READ FAIL\n"); }
}

int _tmain(int argc, _TCHAR* argv[]) {
    // Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return -1;
    }

    // 소켓 생성 및 서버 연결
    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connect error : %d\n", WSAGetLastError());
        return -1;
    }

    // CAN 핸들 설정
    int noDevice = CAN_Fifo_ScanSerialNumber();
    if (noDevice <= 0) return -1;

    CAN_HANDLE h = CAN_OpenFifo(CAN_Fifo_GetSerialNumber(0));
    if (h < 0) {
        printf("USB2CAN open failed.\n");
        return -1;
    }

    HANDLE hEvent = CreateEvent(NULL, false, false, "");
    CAN_SetRxEventNotification(h, hEvent);
    CAN_SetTransferMode(h, 1);
    while (1) {
        //send(sock, "790 8 35 99 EC 18 99 19 20 7D", strlen("790 8 35 99 EC 18 99 19 20 7D"), 0);
        if (CAN_CountRxQueue(h) > 0) {
            RecvCanMessage(h, sock);
        }
        //else { printf("N R\n"); }
        //DWORD dwWaitResult = WaitForSingleObject(hEvent, 2);
    }
    /*
    
    // 메시지 전송 및 수신 루프
    for (int i = 0; i < 1000; i++) {
        //SendCanMessage(h);

        if (CAN_CountRxQueue(h) > 0) {
            RecvCanMessage(h, sock);
            printf("SESE\n");
        }
        else {
            printf("%d\n",i);
            DWORD dwWaitResult = WaitForSingleObject(hEvent, 1000);
            if (dwWaitResult == WAIT_OBJECT_0) {
                RecvCanMessage(h, sock);
                
            }
        }
    }
   */
    // 정리
    CAN_Close(h);
    CloseHandle(hEvent);

    closesocket(sock);
    WSACleanup();

    getchar();

    return 0;
}