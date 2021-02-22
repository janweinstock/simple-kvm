CFLAGS = -Os -DTIME -DBAREMETAL -Wno-implicit-function-declaration -Wno-implicit-int

SOURCES = dhry_0.c dhry_1.c dhry_2.c ee_printf.c

.PHONY: all clean

all: dhrystone

clean:
	$(RM) dhrystone

dhrystone: $(SOURCES) dhry.h linker.ld
	$(CC) $(CFLAGS) -nostdlib -ffreestanding -fno-pic -o $@ -T linker.ld $(SOURCES)

disas: dhrystone
	objdump -D -b binary -m i386:x86-64 $^

