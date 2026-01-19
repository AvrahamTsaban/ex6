CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -g -lm -std=c99 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wstrict-overflow=2 -Wwrite-strings -Wcast-align -Wpointer-arith -Werror -Wconversion -Og

SRCS = main.c game.c bst.c utils.c
OBJS = $(SRCS:.c=.o)

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) 
	# rm $(TARGET)
