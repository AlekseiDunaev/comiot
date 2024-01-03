CC = gcc
CFLAGS = -O2 -s
SRC = $(wildcard *.c ./cJSON/*.c)
# SRC = main.c com.c connection.c uciio.c entity.c sqlitedb.c ./cJSON/cJSON.c
OBJ = $(patsubst %.c, %.o, $(SRC))
LDFLAGS = -lsqlite3 -luci
# LDFLAGS = -lmosquitto -luci -lsqlite3 -lconfig

PROJECT_PATH = ~/RCIoT/projects/comiot/current/comiot
OPENWRT_SRC_PATH = ~/OpenWRTSource/openwrt
OPENWRT_PACKAGE_PATH = package/utils

TARGET=comiot

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) -c $< -o $@

include Makefile.deploy

print:
	echo $(SRC)
	echo $(OBJ)

clean:
	rm $(TARGET)

