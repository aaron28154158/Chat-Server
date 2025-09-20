#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"

// 初始化伺服器socket並綁定到指定port
int server_init(int port) {
    int server_socket;
    struct sockaddr_in server_addr;

    // 建立TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("建立socket失敗");
        return -1;
    }

    // 設定socket選項，允許重複使用地址
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("設定socket選項失敗");
        close(server_socket);
        return -1;
    }

    // 設定伺服器地址結構
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;           // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 接受任何IP的連線
    server_addr.sin_port = htons(port);         // 轉換port為網路位元組順序

    // 綁定socket到地址
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("綁定失敗");
        close(server_socket);
        return -1;
    }

    // 開始監聽連線請求，最多排隊5個
    if (listen(server_socket, 5) < 0) {
        perror("監聽失敗");
        close(server_socket);
        return -1;
    }

    printf("伺服器在port %d 上監聽中...\n", port);
    return server_socket;
}

// 處理單一客戶端的連線（簡單的echo功能）
void server_handle_client(int client_socket) {
    char buffer[MAX_MESSAGE_SIZE];
    int bytes_received;

    printf("客戶端已連線\n");

    // 持續接收並回傳訊息，直到客戶端斷線
    while (1) {
        // 清空緩衝區
        memset(buffer, 0, sizeof(buffer));

        // 接收客戶端訊息
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            // 客戶端斷線或發生錯誤
            printf("客戶端已斷線\n");
            break;
        }

        // 確保字串結尾
        buffer[bytes_received] = '\0';
        printf("收到訊息: %s", buffer);

        // 回傳相同訊息（echo功能）
        send(client_socket, buffer, bytes_received, 0);
    }

    // 關閉客戶端socket
    close(client_socket);
}

// 伺服器主要運行循環
void server_run(int server_socket) {
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("等待客戶端連線...\n");

    // 簡單的單一客戶端處理循環
    while (1) {
        // 接受客戶端連線
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            perror("接受連線失敗");
            continue;
        }

        printf("客戶端 %s 已連線\n", inet_ntoa(client_addr.sin_addr));

        // 處理客戶端（目前只能處理一個客戶端）
        server_handle_client(client_socket);
    }
}