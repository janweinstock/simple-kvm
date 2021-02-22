CFLAGS = -Os

.PHONY: all clean

all: hello

clean:
	$(RM) hello

hello: hello.c linker.ld
	$(CC) $(CFLAGS) -nostdlib -ffreestanding -fno-pic -o $@ -T linker.ld hello.c

disas: hello
	objdump -D -b binary -m i386:x86-64 $^

