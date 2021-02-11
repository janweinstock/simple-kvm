#ifndef SVM_KVM_H
#define SVM_KVM_H

#include "common.h"

namespace svm {

    class kvm
    {
    private:
        int m_fd;
        int m_vmfd;
        int m_version;

    public:
        kvm(const kvm&) = delete;

        int version() const { return m_version; }

        kvm();
        virtual ~kvm();

        void new_cpu(int& fd, int& size);
        void new_mem(u64 addr, u64 size, void* ptr);
    };

}

#endif
