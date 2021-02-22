#include "kvm.h"
#include "mem.h"
#include "cpu.h"
#include "helper.h"

int main(int argc, const char** argv) {

    if (argc != 2) {
        printf("Usage: %s <image>\n", argv[0]);
        return -1;
    }

    svm::kvm kvm;
    svm::mem mem(kvm, 2 * svm::MiB);
    svm::cpu cpu(kvm);

    cpu.regs.rip = 0;
    cpu.regs.rsp = mem.size();

    mem.load(argv[1], 0x0);

    helper_setup_page_tables(cpu, mem);
    helper_setup_segment_registers(cpu);

    printf("simulating...\n");
    cpu.execute();

    return 0;
}
