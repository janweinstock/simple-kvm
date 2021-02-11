#include "mem.h"

namespace svm {

    mem::mem(kvm& vm, u64 size):
        m_vm(vm),
        m_mem((u8*)MAP_FAILED),
        m_size(size) {

        const int prot = PROT_READ | PROT_WRITE;
        const int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;

        m_mem = (u8*)mmap(NULL, m_size, prot, flags, -1, 0);
        if (m_mem == MAP_FAILED)
            PERROR("mmap memory");

        madvise(m_mem, m_size, MADV_MERGEABLE);

        m_vm.new_mem(0x0, m_size, m_mem);
    }

    mem::~mem() {
        if (m_mem != MAP_FAILED)
            munmap(m_mem, m_size);
    }

    void mem::load(const char* path, u64 offset) {
        printf("loading '%s'... ", path);

        FILE* image = fopen(path, "rb");
        if (image == nullptr)
            ERROR("cannot open: %s", path);

        fseek(image, 0, SEEK_END);
        size_t size = ftell(image);
        fseek(image, 0, SEEK_SET);

        if (offset + size > m_size)
            ERROR("file too large: %s", path);

        fread(m_mem + offset, 1, size, image);
        fclose(image);

        printf("%zu bytes ok\n", size);
        fflush(stdout);
    }

}
