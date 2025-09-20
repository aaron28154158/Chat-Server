# 測試指南 (Testing Guide)

## 如何建置專案

```bash
# 編譯所有程式
make

# 清理編譯檔案
make clean

# 只編譯server
make server

# 只編譯client
make client
```

## Phase 1 測試 - 基本 TCP Echo 功能

### 測試 1: 基本 Echo 功能

**步驟 1: 啟動伺服器**
```bash
# 使用預設port (8888)
./bin/server

# 使用指定port
./bin/server 9999
```

**預期輸出:**
```
伺服器在port 8888 上監聽中...
等待客戶端連線...
```

**步驟 2: 啟動客戶端（另開終端）**
```bash
# 連線到預設server (127.0.0.1:8888)
./bin/client

# 連線到指定server
./bin/client 192.168.1.100 9999
```

**預期輸出:**
```
正在連線到伺服器 127.0.0.1:8888...
已成功連線到伺服器 127.0.0.1:8888
客戶端已連線  # 這個會在server端顯示
已連線！輸入訊息並按Enter發送，輸入'quit'離開
>
```

**步驟 3: 測試訊息傳送**
```bash
# 在客戶端輸入任何訊息
> Hello Server
伺服器回應: Hello Server

> 測試中文
伺服器回應: 測試中文

> quit
正在離開...
已斷線
```

**server端應該顯示:**
```
客戶端 127.0.0.1 已連線
收到訊息: Hello Server
收到訊息: 測試中文
客戶端已斷線
等待客戶端連線...  # 繼續等待下一個客戶端
```

### 測試 2: 錯誤處理測試

**測試無效port:**
```bash
./bin/client 127.0.0.1 99999
# 預期: 無效的port號: 99999

./bin/server 0
# 預期: 無效的port號: 0
```

**測試無效IP:**
```bash
./bin/client 999.999.999.999
# 預期: 無效的IP地址: 999.999.999.999
```

**測試連線失敗:**
```bash
# server沒啟動時執行client
./bin/client
# 預期: 連線失敗: Connection refused
```

### 測試 3: 信號處理測試

**測試優雅關閉:**
```bash
# 啟動server後按 Ctrl+C
./bin/server
^C
# 預期: 正在關閉伺服器...
```

### 測試 4: 多次連線測試

**測試連續連線:**
```bash
# 啟動server
./bin/server

# 第一個client連線，發送訊息後quit
./bin/client
> test1
> quit

# 第二個client連線，發送訊息後quit
./bin/client
> test2
> quit
```

**預期**: server應該能處理多次連線，但一次只能處理一個客戶端

## 已知限制 (Phase 1)

1. **單一客戶端**: 一次只能處理一個客戶端連線
2. **阻塞式處理**: server在處理一個客戶端時，其他客戶端無法連線
3. **簡單echo**: 只能回傳相同訊息，沒有其他功能

## 快速測試腳本

建立測試用的腳本：

```bash
# 建立 test_basic.sh
cat > test_basic.sh << 'EOF'
#!/bin/bash
echo "啟動server..."
./bin/server &
SERVER_PID=$!
sleep 1

echo "測試echo功能..."
echo -e "Hello World\nquit" | ./bin/client

echo "關閉server..."
kill $SERVER_PID
echo "測試完成"
EOF

chmod +x test_basic.sh
./test_basic.sh
```

## 除錯技巧

**檢查port是否被佔用:**
```bash
netstat -ln | grep 8888
# 或
lsof -i :8888
```

**查看系統日誌:**
```bash
dmesg | tail
```

**使用netcat測試:**
```bash
# 啟動server後，用netcat連線測試
nc 127.0.0.1 8888
```

## 下一階段測試準備

Phase 2 將需要測試：
- 多客戶端同時連線
- 客戶端列表管理
- 連線/斷線通知
- 並發處理

---

**注意**: 這個文檔會隨著新功能的加入持續更新。