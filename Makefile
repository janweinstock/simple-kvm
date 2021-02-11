CFLAGS = -Wall -Werror -O0 -ggdb

.PHONY: all run clean disas 

all: svm guest

run: svm guest
	./svm

clean:
	$(RM) svm guest


svm: kvm.cpp mem.cpp cpu.cpp main.cpp helper.cpp
	$(CXX) $(CFLAGS) -o $@ $^

guest: guest.c guest.ld
	$(CC) -nostdlib -ffreestanding -fno-pic -o $@ -T guest.ld guest.c

disas: guest
	objdump -D -b binary -m i386:x86-64 $^