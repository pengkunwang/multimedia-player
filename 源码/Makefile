CC=gcc
TARGET=a.out

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lcurses
	rm *.o

$(OBJS):%.o:%.c
	$(CC) -c $^ -o $@

.PHONY : clean
clean:
	rm  $(TARGET)
