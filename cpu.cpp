#include "cpu.h"

namespace svm {

    void cpu::pop_regs() {
        if (ioctl(m_fd, KVM_GET_SREGS, &sysregs) < 0)
            PERROR("KVM_GET_SREGS");
        if (ioctl(m_fd, KVM_GET_REGS, &regs) < 0)
            PERROR("KVM_GET_REGS");
    }

    void cpu::push_regs() {
        if (ioctl(m_fd, KVM_SET_SREGS, &sysregs) < 0)
            PERROR("KVM_SET_SREGS");
        if (ioctl(m_fd, KVM_SET_REGS, &regs) < 0)
            PERROR("KVM_SET_REGS");
    }

    void cpu::rd_io(u16 port, u8* dest, u64 size) {
        ERROR("unhandled read from ioport %hu", port);
    }

    void cpu::wr_io(u16 port, const u8* src, u64 size) {
        switch (port) {
        case 0x66:
            for (u64 i = 0; i < size; i++)
                fwrite(src + i, 1, 1, stdout);
            fflush(stdout);
            break;

        default:
            ERROR("unhandled write to ioport %hu", port);
        }
    }

    void cpu::rd_mmio(u64 addr, u8* dest, u64 size) {
        ERROR("unhandled load from 0x%016lx", addr);
    }

    void cpu::wr_mmio(u64 addr, const u8* src, u64 size) {
        ERROR("unhandled store to 0x%016lx", addr);
    }

    cpu::cpu(kvm& vm):
        m_vm(vm),
        m_fd(-1),
        m_runsz(),
        m_run(nullptr),
        regs(),
        sysregs() {
        m_vm.new_cpu(m_fd, m_runsz);

        const int prot = PROT_READ | PROT_WRITE;
        m_run = (kvm_run*)mmap(NULL, m_runsz, prot, MAP_SHARED, m_fd, 0);
        if (m_run == MAP_FAILED)
            PERROR("mmap kvm_run");

        pop_regs();
    }

    cpu::~cpu() {
        if (m_run && m_run != MAP_FAILED)
            munmap(m_run, m_runsz);
    }

    void cpu::execute() {
        while (true) {
            push_regs();

            if (ioctl(m_fd, KVM_RUN, 0) < 0)
               PERROR("KVM_RUN");

            pop_regs();

            switch (m_run->exit_reason) {
            case KVM_EXIT_HLT: {
               printf("simulation exit with code %lld\n", regs.rax);
               return;
            }

            case KVM_EXIT_IO: {
               u8* p = (u8*)m_run + m_run->io.data_offset;
               if (m_run->io.direction == KVM_EXIT_IO_OUT)
                   wr_io(m_run->io.port, p, m_run->io.size);
               if (m_run->io.direction == KVM_EXIT_IO_IN)
                   rd_io(m_run->io.port, p, m_run->io.size);
               break;
            }

            case KVM_EXIT_MMIO: {
               u8* p = m_run->mmio.data;
               if (m_run->mmio.is_write)
                   wr_mmio(m_run->mmio.phys_addr, p, m_run->mmio.len);
               else
                   rd_mmio(m_run->mmio.phys_addr, p, m_run->mmio.len);
               break;
            }

            case KVM_EXIT_INTERNAL_ERROR:
                ERROR("kvm internal error");

            case KVM_EXIT_SHUTDOWN:
                ERROR("kvm exit shutdown");

            default:
                ERROR("unknown exit reason: %d", m_run->exit_reason);
            }
        }
    }

}
