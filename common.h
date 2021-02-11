#ifndef SVM_COMMON_H
#define SVM_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/kvm.h>

#define ERROR(...)                                     \
    do {                                               \
        fprintf(stderr, "%s:%d ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                  \
        fprintf(stderr, "\n");                         \
        abort();                                       \
    } while (0)

#define PERROR(s)                                      \
    do {                                               \
        fprintf(stderr, "%s:%d ", __FILE__, __LINE__); \
        perror(s);                                     \
        abort();                                       \
    } while (0)

namespace svm {

    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    const size_t KiB = 1024;
    const size_t MiB = 1024 * KiB;

}

#endif
