#include "nemu.h"
#include "monitor/diff-test.h"

// Compare NEMU registers with reference registers (e.g., QEMU)
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool ret = true; // Assume registers match by default
  for (size_t i = 0; i < 32; i++) { // Check all 32 general-purpose registers
    if (cpu.gpr[i]._32 != ref_r->gpr[i]._32) { // Compare register values
      // Print mismatch details
      printf("reg_name:%-4s nemu:0x%-8x qemu:0x%-8x\n", reg_name(i, 4), cpu.gpr[i]._32, ref_r->gpr[i]._32);
      ret = false; // Set return value to false if mismatch found
    }
  }
  return ret; // Return true if all registers match, false otherwise
}

// Attach to differential testing (currently empty)
void isa_difftest_attach(void) {
  // This function can be implemented to set up differential testing
}
