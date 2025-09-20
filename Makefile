CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread -g
TARGET_DIR = bin
SRC_DIR = src
COMMON_DIR = $(SRC_DIR)/common
SERVER_DIR = $(SRC_DIR)/server
CLIENT_DIR = $(SRC_DIR)/client

SERVER_TARGET = $(TARGET_DIR)/server
CLIENT_TARGET = $(TARGET_DIR)/client

COMMON_SOURCES =
SERVER_SOURCES = $(SERVER_DIR)/main.c $(SERVER_DIR)/server.c
CLIENT_SOURCES = $(CLIENT_DIR)/main.c $(CLIENT_DIR)/client.c

COMMON_OBJECTS = $(COMMON_SOURCES:.c=.o)
SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)
CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)

.PHONY: all clean server client test

all: $(TARGET_DIR) $(SERVER_TARGET) $(CLIENT_TARGET)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

$(SERVER_TARGET): $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

server: $(SERVER_TARGET)

client: $(CLIENT_TARGET)

test:
	@echo "Running tests..."
	@$(MAKE) -C $(SRC_DIR)/test

clean:
	rm -f $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
	rm -rf $(TARGET_DIR)

install: all
	cp $(SERVER_TARGET) /usr/local/bin/chat-server
	cp $(CLIENT_TARGET) /usr/local/bin/chat-client

.SUFFIXES: .c .o