#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"

// 連線到指定的伺服器
int client_connect(const char *server_ip, int port) {
    int client_socket;
    struct sockaddr_in server_addr;

    // 參數驗證
    if (server_ip == NULL) {
        printf("錯誤: IP地址不能為空\n");
        return -1;
    }
    if (port <= 0 || port > 65535) {
        printf("錯誤: Port必須在1-65535範圍內，目前為: %d\n", port);
        return -1;
    }

    // 建立TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("建立socket失敗");
        return -1;
    }

    // 設定伺服器地址結構
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;           // IPv4
    server_addr.sin_port = htons(port);         // 轉換port為網路位元組順序

    // 將IP字串轉換為網路地址
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("無效的IP地址: %s\n", server_ip);
        close(client_socket);
        return -1;
    }

    // 連線到伺服器
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("連線失敗");
        close(client_socket);
        return -1;
    }

    printf("已成功連線到伺服器 %s:%d\n", server_ip, port);
    return client_socket;
}

// 發送訊息到伺服器
int client_send_message(int socket, const char *message) {
    int bytes_sent = send(socket, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("發送訊息失敗");
        return -1;
    }
    return bytes_sent;
}

// 接收伺服器回應
int client_receive_message(int socket, char *buffer, int buffer_size) {
    int bytes_received = recv(socket, buffer, buffer_size - 1, 0);
    if (bytes_received < 0) {
        perror("接收訊息失敗");
        return -1;
    } else if (bytes_received == 0) {
        printf("伺服器已關閉連線\n");
        return 0;  // 連線正常結束，不是錯誤
    }

    // 確保字串結尾
    buffer[bytes_received] = '\0';
    return bytes_received;
}

// 客戶端主要運行循環
void client_run(int socket) {
    char input_buffer[MAX_MESSAGE_SIZE];
    char response_buffer[MAX_MESSAGE_SIZE];

    printf("已連線！輸入訊息並按Enter發送，輸入'quit'離開\n");

    while (1) {
        // 提示用戶輸入
        printf("> ");
        fflush(stdout);

        // 讀取用戶輸入
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("讀取輸入失敗\n");
            break;
        }

        // 檢查是否要離開
        if (strncmp(input_buffer, "quit", 4) == 0) {
            printf("正在離開...\n");
            break;
        }

        // 發送訊息到伺服器
        if (client_send_message(socket, input_buffer) < 0) {
            printf("發送訊息失敗\n");
            break;
        }

        // 接收伺服器回應
        int recv_result = client_receive_message(socket, response_buffer, sizeof(response_buffer));
        if (recv_result < 0) {
            printf("接收回應失敗\n");
            break;
        } else if (recv_result == 0) {
            // 伺服器關閉連線，已在函數內顯示訊息
            break;
        }

        // 顯示伺服器回應
        printf("伺服器回應: %s", response_buffer);
    }
}