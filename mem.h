#ifndef SVM_MEM_H
#define SVM_MEM_H

#include "common.h"
#include "kvm.h"

namespace svm {

    class mem
    {
    private:
        kvm& m_vm;
        u8*  m_mem;
        u64  m_size;

        mem() = delete;
        mem(const mem&) = delete;

    public:
        u8* raw() { return m_mem; }
        u64 size() const { return m_size; }

        mem(kvm& vm, u64 size);
        virtual ~mem();

        void load(const char* path, u64 offset);
    };
}

#endif
