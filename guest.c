#define NORETURN __attribute__((noreturn));
#define INITCODE __attribute__((section(".init")))

typedef unsigned char u8;
typedef unsigned short u16;

void _start() INITCODE NORETURN;
void exit(int code) NORETURN;

static inline void outb(u8 val, u16 port) {
    asm("outb %0, %1"
        : : "a" (val), "Nd" (port) : "memory");
}

void putchar(int c) {
    outb(c, 0x66);
}

void puts(const char* s) {
    while (s && *s)
        putchar(*s++);
    putchar('\n');
}

void exit(int code) {
    while (1) {
        asm("hlt" : : "a" (code) : "memory");
    }
}

void _start() {
    puts("> Hello World!");
    exit(42);
}
