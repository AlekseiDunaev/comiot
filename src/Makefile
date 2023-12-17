CC = gcc
PROFILE = -O2 -s
CFLAGS = $(PROFILE)
SRC = main.c com.c connection.c uciio.c entity.c sqlitedb.c ./cJSON/cJSON.c
LDFLAGS = -lsqlite3 -lmosquitto -luci
#LDFLAGS = -lsqlite3 -lconfig

TARGET=comiot

all: main

main:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm $(TARGET)

