#define _POSIX_C_SOURCE 200809L  // 啟用POSIX.1-2008功能
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "server.h"

// 全域變數：伺服器socket
int g_server_socket = -1;

// 信號處理函數：優雅地關閉伺服器
void signal_handler(int sig) {
    (void)sig;  // 避免unused parameter警告
    printf("\n正在關閉伺服器...\n");
    if (g_server_socket != -1) {
        close(g_server_socket);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;

    // 檢查命令列參數
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            printf("無效的port號: %s\n", argv[1]);
            printf("使用方法: %s [port]\n", argv[0]);
            return 1;
        }
    }

    // 使用sigaction註冊信號處理器（比signal更可靠）
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);  // 清空信號遮罩
    sa.sa_flags = 0;           // 不使用特殊標誌

    // 註冊SIGINT (Ctrl+C) 和 SIGTERM 信號處理器
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("註冊SIGINT處理器失敗");
        return 1;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("註冊SIGTERM處理器失敗");
        return 1;
    }

    // 初始化伺服器
    g_server_socket = server_init(port);
    if (g_server_socket < 0) {
        printf("伺服器初始化失敗\n");
        return 1;
    }

    // 開始運行伺服器
    server_run(g_server_socket);

    // 正常情況下不會到達這裡
    close(g_server_socket);
    return 0;
}