#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <sys/types.h>

// 基本常數定義
#define MAX_MESSAGE_SIZE 1024  // 訊息最大長度
#define DEFAULT_PORT 8888      // 預設伺服器port

// 簡單的訊息結構
typedef struct {
    char data[MAX_MESSAGE_SIZE];  // 訊息內容
} message_t;

#endif