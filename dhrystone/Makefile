CFLAGS = -Os -DTIME -Wno-implicit-function-declaration -Wno-implicit-int

SOURCES = init.c dhry_1.c dhry_2.c ee_printf.c

.PHONY: all clean

all: dhrystone dhrystone_host

clean:
	$(RM) dhrystone dhrystone_native

dhrystone: $(SOURCES) dhry.h linker.ld
	$(CC) $(CFLAGS) -DBAREMETAL -nostdlib -ffreestanding -fno-pic -o $@ -T linker.ld $(SOURCES)

dhrystone_host: $(SOURCES) dhry.h
	$(CC) $(CFLAGS) -nostdlib -ffreestanding -o $@ $(SOURCES)

disas: dhrystone
	objdump -D -b binary -m i386:x86-64 $^

