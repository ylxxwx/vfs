C_SOURCES = $(wildcard disk/*.c ext2fs/*.c utils/*.c vfs/*.c *.c)
HEADERS = $(wildcard disk/*.h ext2fs/*.h utils/*.h vfs/*.h *.h)
INC = disk ext2fs utils vfs
# Nice syntax for file extension replacement
OBJS = ${C_SOURCES:.c=.o }

all: test

test: main.o $(OBJS)
	gcc -o $@ $^

%.o: %.c
	gcc -g $(INC:%=-I %) -o $@ -c $^

clean:
	rm -f test *.o disk/*.o ext2fs/*.o utils/*.o vfs/*.o