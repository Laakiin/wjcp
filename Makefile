CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lcjson -lcurl
TARGET = wjcp
SRCS = main.c api.c geo.c parser.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
