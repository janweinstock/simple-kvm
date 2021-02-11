#include "kvm.h"
#include "mem.h"
#include "cpu.h"
#include "helper.h"

int main() {
    svm::kvm kvm;
    svm::mem mem(kvm, 2 * svm::MiB);
    svm::cpu cpu(kvm);

    cpu.regs.rip = 0;
    cpu.regs.rsp = mem.size();

    mem.load("guest", 0x0);

    helper_setup_page_tables(cpu, mem);
    helper_setup_segment_registers(cpu);

    printf("executing...\n");
    cpu.execute();

    return 0;
}
