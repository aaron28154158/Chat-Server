#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "../common/protocol.h"

// 伺服器初始化函數
int server_init(int port);

// 處理客戶端連線
void server_handle_client(int client_socket);

// 伺服器主要運行循環
void server_run(int server_socket);

#endif