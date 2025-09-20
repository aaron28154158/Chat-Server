#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"

int main(int argc, char *argv[]) {
    const char *server_ip = "127.0.0.1";  // 預設連線到本機
    int port = DEFAULT_PORT;
    int client_socket;

    // 檢查命令列參數
    if (argc > 1) {
        server_ip = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            printf("無效的port號: %s\n", argv[2]);
            printf("使用方法: %s [server_ip] [port]\n", argv[0]);
            return 1;
        }
    }

    printf("正在連線到伺服器 %s:%d...\n", server_ip, port);

    // 連線到伺服器
    client_socket = client_connect(server_ip, port);
    if (client_socket < 0) {
        printf("無法連線到伺服器\n");
        return 1;
    }

    // 開始客戶端主要循環
    client_run(client_socket);

    // 關閉連線
    close(client_socket);
    printf("已斷線\n");

    return 0;
}