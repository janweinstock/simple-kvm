#include "helper.h"

using svm::u32;
using svm::u64;

enum cr0_bits : u64 {
    CR0_PE         = 1u <<  0,
    CR0_MP         = 1u <<  1,
    CR0_EM         = 1u <<  2,
    CR0_TS         = 1u <<  3,
    CR0_ET         = 1u <<  4,
    CR0_NE         = 1u <<  5,
    CR0_WP         = 1u << 16,
    CR0_AM         = 1u << 18,
    CR0_NW         = 1u << 29,
    CR0_CD         = 1u << 30,
    CR0_PG         = 1u << 31,
};

enum cr4_bits : u64 {
    CR4_VME        = 1u <<  0,
    CR4_PVI        = 1u <<  1,
    CR4_TSD        = 1u <<  2,
    CR4_DE         = 1u <<  3,
    CR4_PSE        = 1u <<  4,
    CR4_PAE        = 1u <<  5,
    CR4_MCE        = 1u <<  6,
    CR4_PGE        = 1u <<  7,
    CR4_PCE        = 1u <<  8,
    CR4_OSFXSR     = 1u <<  9,
    CR4_OSXMMEXCPT = 1u << 10,
    CR4_UMIP       = 1u << 11,
    CR4_VMXE       = 1u << 13,
    CR4_SMXE       = 1u << 14,
    CR4_FSGSBASE   = 1u << 16,
    CR4_PCIDE      = 1u << 17,
    CR4_OSXSAVE    = 1u << 18,
    CR4_SMEP       = 1u << 20,
    CR4_SMAP       = 1u << 21,
};

enum pde64_bits : u64 {
    PDE64_PRESENT  = 1u << 0,
    PDE64_RW       = 1u << 1,
    PDE64_USER     = 1u << 2,
    PDE64_ACCESSED = 1u << 5,
    PDE64_DIRTY    = 1u << 6,
    PDE64_PS       = 1u << 7,
    PDE64_G        = 1u << 8,
};

enum efer_bits : u64 {
    EFER_SCE       = 1u <<  0,
    EFER_LME       = 1u <<  8,
    EFER_LMA       = 1u << 10,
    EFER_NXE       = 1u << 11,
};

// https://david942j.blogspot.com/2018/10/note-learning-kvm-implement-your-own.html

void helper_setup_page_tables(svm::cpu& cpu, svm::mem& mem) {
    // We place the page map at address 0x2000, make sure program ends before!
    u64 pml4_addr = 0x2000;             // page map level 4
    u64 pdpt_addr = pml4_addr + 0x1000; // page directory pointer table
    u64 pd_addr   = pdpt_addr + 0x1000; // page directory

    u64* pml4 = (u64*)(mem.raw() + pml4_addr);
    u64* pdpt = (u64*)(mem.raw() + pdpt_addr);
    u64* pd   = (u64*)(mem.raw() + pd_addr);

    *pml4 = PDE64_PRESENT | PDE64_RW | PDE64_USER | pdpt_addr;
    *pdpt = PDE64_PRESENT | PDE64_RW | PDE64_USER | pd_addr;
    *pd = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS;

    cpu.sysregs.cr3 = pml4_addr;
    cpu.sysregs.cr4 = CR4_PAE;
    cpu.sysregs.cr0 = CR0_PE | CR0_MP | CR0_ET | CR0_NE | CR0_WP | CR0_AM | CR0_PG;
    cpu.sysregs.efer = EFER_LME | EFER_LMA;
}

void helper_setup_segment_registers(svm::cpu& cpu) {
    struct kvm_segment seg;
    memset(&seg, 0, sizeof(seg));

    seg.base = 0;
    seg.limit = 0xffffffff;
    seg.present = 1;
    seg.dpl = 0;
    seg.db = 0;
    seg.s = 1;
    seg.l = 1;
    seg.g = 1;
    seg.selector = 1 << 3;
    seg.type = 11;        // execute, read, accessed
    cpu.sysregs.cs = seg; // code segment

    seg.selector = 2 << 3;
    seg.type = 3;         // read/write, accessed
    cpu.sysregs.ds = seg; // data segment
    cpu.sysregs.ss = seg; // stack segment
    cpu.sysregs.es = seg; // extra segment
    cpu.sysregs.fs = seg; // extra segment f
    cpu.sysregs.gs = seg; // extra segment g
}
