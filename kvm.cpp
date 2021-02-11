#include "kvm.h"

namespace svm {

    kvm::kvm():
        m_fd(-1),
        m_vmfd(-1),
        m_version() {

        m_fd = open("/dev/kvm", O_RDWR);
        if (m_fd < 0)
            PERROR("open /dev/kvm");

        m_version = ioctl(m_fd, KVM_GET_API_VERSION, 0);
        if (m_version != KVM_API_VERSION) {
            fprintf(stderr, "kvm api version mismatch\n");
            fprintf(stderr, "  kernel version: %d\n", m_version);
            fprintf(stderr, "  header version: %d\n", KVM_API_VERSION);
            abort();
        }

        m_vmfd = ioctl(m_fd, KVM_CREATE_VM, 0);
        if (m_vmfd < 0)
            PERROR("KVM_CREATE_VM");

        // apparently needed for intel kvm...?
        if (ioctl(m_vmfd, KVM_SET_TSS_ADDR, 0xfffbd000) < 0) {
            perror("KVM_SET_TSS_ADDR");
            abort();
        }
    }

    kvm::~kvm() {
        if (m_vmfd >= 0)
            close(m_vmfd);
        if (m_fd >= 0)
            close(m_fd);
    }

    void kvm::new_cpu(int& fd, int& size) {
        fd = ioctl(m_vmfd, KVM_CREATE_VCPU, 0);
        if (fd < 0)
            PERROR("KVM_CREATE_VCPU");

        size = ioctl(m_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
        if (size <= 0)
            PERROR("KVM_GET_VCPU_MMAP_SIZE");
    }

    void kvm::new_mem(u64 addr, u64 size, void* ptr) {
        struct kvm_userspace_memory_region region;
        memset(&region, 0, sizeof(region));

        region.guest_phys_addr = addr;
        region.memory_size = size;
        region.userspace_addr = (uintptr_t)ptr;

        if (ioctl(m_vmfd, KVM_SET_USER_MEMORY_REGION, &region) < 0)
            PERROR("KVM_SET_USER_MEMORY_REGION");
    }

}
