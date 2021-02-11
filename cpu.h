#ifndef SVM_CPU_H
#define SVM_CPU_H

#include "common.h"
#include "kvm.h"

namespace svm {

    class cpu
    {
    private:
        kvm& m_vm;
        int m_fd;
        int m_runsz;
        struct kvm_run* m_run;

        cpu() = delete;
        cpu(const cpu&) = delete;

        void pop_regs();
        void push_regs();

        void rd_io(u16 port, u8* dest, u64 size);
        void wr_io(u16 port, const u8* src, u64 size);

        void rd_mmio(u64 addr, u8* dest, u64 size);
        void wr_mmio(u64 addr, const u8* src, u64 size);

    public:
        struct kvm_regs regs;
        struct kvm_sregs sysregs;

        cpu(kvm& vm);
        virtual ~cpu();

        void execute();
    };

}

#endif
