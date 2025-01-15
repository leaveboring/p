#include "nemu.h"

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};


// Display register information
void isa_reg_display() {
  printf("---information of reg---\n");
  for (int i = 0; i < 32; i++) {
    printf("%-3s = 0x%08x\n", reg_name(i, 8), reg_l(i)); // Print register name and value
  }
}

// Convert register name string to its value
uint32_t isa_reg_str2val(const char *s, bool *success) {
  *success = true; // Assume success by default
  if (strcmp(s, "pc") == 0) { // Check if the input is "pc"
    return cpu.pc; // Return the program counter value
  }
  for (int i = 0; i < 32; i++) { // Check all 32 registers
    if (strcmp(s, regsl[i]) == 0) { // Compare with register name
      return reg_l(i); // Return the register value
    }
  }
  *success = false; // If no match, set success to false
  return 0; // Return 0 if the register name is invalid
}

