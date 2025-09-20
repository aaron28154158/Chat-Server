#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "../common/protocol.h"

// 連線到伺服器
int client_connect(const char *server_ip, int port);

// 發送訊息到伺服器
int client_send_message(int socket, const char *message);

// 接收伺服器回應
int client_receive_message(int socket, char *buffer, int buffer_size);

// 客戶端主要運行循環
void client_run(int socket);

#endif