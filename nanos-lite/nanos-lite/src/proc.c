#include "proc.h" // Include process-related definitions

#define MAX_NR_PROC 4 // Maximum number of processes

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {}; // Array of process control blocks (PCBs)
static PCB pcb_boot = {}; // Boot process control block
PCB *current = NULL; // Pointer to the current running process

// Switch to the boot process control block
void switch_boot_pcb() {
  current = &pcb_boot; // Set the current process to the boot PCB
}

// A simple hello function to demonstrate process execution
void hello_fun(void *arg) {
  int j = 1; // Counter for logging
  while (1) { // Infinite loop
    Log("Hello World from Nanos-lite for the %dth time!", j); // Log a message
    j++; // Increment the counter
    _yield(); // Yield control to the scheduler
  }
}

// Initialize processes
void init_proc() {
  switch_boot_pcb(); // Set the current process to the boot PCB

  Log("Initializing processes..."); // Log initialization message

  // Load programs here (to be implemented)
}

// Scheduler function (to be implemented)
_Context* schedule(_Context *prev) {
  return NULL; // Placeholder for scheduling logic
}
