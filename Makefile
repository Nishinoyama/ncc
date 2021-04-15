CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

ncc: $(OBJS)
	$(CC) -o ncc $(OBJS) $(LDFLAGS)

all: ncc

test: ncc
	sh ./test.sh

clean:
	rm -f ncc *.o *~ tmp*

.PHONY: test clean