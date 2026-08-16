CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
	LIBS = -lws2_32
	EXE = .exe
else
	LIBS =
	EXE =
endif

all: server client

server:
	$(CC) $(CFLAGS) server/server_TCP.c -o server_TCP$(EXE) $(LIBS)

client:
	$(CC) $(CFLAGS) client/client_TCP.c -o client_TCP$(EXE) $(LIBS)

clean:
	rm -f server_TCP server_TCP.exe client_TCP client_TCP.exe
