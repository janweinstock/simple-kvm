#define NORETURN __attribute__((noreturn));
#define INITCODE __attribute__((section(".init")))

#define _NR_write 1
#define _NR_exit  60

void _start() INITCODE NORETURN;
void exit(int code) NORETURN;

typedef unsigned char u8;
typedef unsigned short u16;

void* malloc(unsigned int sz) {

    static char HEAP[4096];
    static char* heap = HEAP;

    char* buf = heap;
    heap += (((sz + 7) / 8) * 8);
    return buf;
}

void strcpy(char* dst, const char* src) {
    for (; (*dst = *src) != '\0'; ++dst, ++src);
}

int strcmp(const char* p1, const char* p2) {
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;
    do {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

static inline void outb(u8 val, u16 port) {
    asm("outb %0, %1"
        : : "a" (val), "Nd" (port) : "memory");
}

long syscall(long nr, long arg0, long arg1, long arg2) {

    long ret;
    asm volatile
    (
     "syscall"
     : "=a" (ret)
     : "0"(nr), "D"(arg0), "S"(arg1), "d"(arg2)
     : "rcx", "r11", "memory"
    );

    return ret;
}

void putchar(int c) {
#ifdef BAREMETAL
    outb(c, 0x66);
#else
    char buf[8];
    buf[0] = c;
    syscall(_NR_write, 1, (long)buf, 1);
#endif
}

unsigned long time() {
    unsigned int hi, lo;
    asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long) hi << 32 | lo);
}

void _start() {
    main();
    exit(0);
}

void exit(int code) {
    while (1) {
#ifdef BAREMETAL
        asm("hlt" : : "a" (code) : "memory");
#else
        syscall(_NR_exit, code, 0, 0);
#endif
    }
}

