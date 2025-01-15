#include "cpu/exec.h"

void raise_intr(uint32_t NO, vaddr_t epc);

#define REG_A7 17

// Define CSR (Control and Status Register) addresses
enum {
  SSTATUS = 0x100, // Supervisor status register
  STVEC   = 0x105, // Supervisor trap vector base address
  SEPC    = 0x141, // Supervisor exception program counter
  SCAUSE  = 0x142  // Supervisor cause register
};

// Handle system instructions (CSR operations, ecall, sret)
make_EHelper(system) {
  switch (decinfo.isa.instr.funct3) { // Check the funct3 field of the instruction
    case 0: { // CSR Read-Write (csrrw)
      switch (decinfo.isa.instr.csr) { // Check the CSR address
        case SEPC: { // Handle SEPC
          t0 = cpu.sepc; // Save current SEPC value
          cpu.sepc = id_src->val; // Update SEPC with source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SEPC value to destination register
          break;
        }
        case SSTATUS: { // Handle SSTATUS
          t0 = cpu.sstatus; // Save current SSTATUS value
          cpu.sstatus = id_src->val; // Update SSTATUS with source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SSTATUS value to destination register
          break;
        }
        case SCAUSE: { // Handle SCAUSE
          t0 = cpu.scause; // Save current SCAUSE value
          cpu.scause = id_src->val; // Update SCAUSE with source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SCAUSE value to destination register
          break;
        }
        case STVEC: { // Handle STVEC
          t0 = cpu.stvec; // Save current STVEC value
          cpu.stvec = id_src->val; // Update STVEC with source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old STVEC value to destination register
          break;
        }
        default: break; // Ignore unsupported CSRs
      }
      print_asm_template3(csrrw); // Print assembly template for csrrw
      break;
    }
    case 1: { // CSR Read-Set (csrrs)
      switch (decinfo.isa.instr.csr) { // Check the CSR address
        case SEPC: { // Handle SEPC
          t0 = cpu.sepc; // Save current SEPC value
          cpu.sepc = t0 | id_src->val; // Set bits in SEPC using source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SEPC value to destination register
          break;
        }
        case SSTATUS: { // Handle SSTATUS
          t0 = cpu.sstatus; // Save current SSTATUS value
          cpu.sstatus = t0 | id_src->val; // Set bits in SSTATUS using source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SSTATUS value to destination register
          break;
        }
        case SCAUSE: { // Handle SCAUSE
          t0 = cpu.scause; // Save current SCAUSE value
          cpu.scause = t0 | id_src->val; // Set bits in SCAUSE using source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old SCAUSE value to destination register
          break;
        }
        case STVEC: { // Handle STVEC
          t0 = cpu.stvec; // Save current STVEC value
          cpu.stvec = t0 | id_src->val; // Set bits in STVEC using source value
          rtl_sr(id_dest->reg, &t0, 4); // Write old STVEC value to destination register
          break;
        }
        default: break; // Ignore unsupported CSRs
      }
      print_asm_template3(csrrs); // Print assembly template for csrrs
      break;
    }
    case 2: { // Handle ecall and sret
      if (decinfo.isa.instr.simm11_0 == 0x0) { // ecall (environment call)
        raise_intr(reg_l(REG_A7), cpu.pc); // Raise an interrupt with the system call number
        print_asm_template1(ecall); // Print assembly template for ecall
      } else if (decinfo.isa.instr.simm11_0 == 0x102) { // sret (supervisor return)
        rtl_j(cpu.sepc + 4); // Jump to SEPC + 4 (return address)
        print_asm_template3(sret); // Print assembly template for sret
      }
      break;
    }
    default: break; // Ignore unsupported funct3 values
  }
}
