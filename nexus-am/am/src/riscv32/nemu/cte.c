#include <am.h>
#include <riscv32.h>

// Define system call numbers
enum {
  SYS_exit,        // Exit a process
  SYS_yield,       // Yield CPU
  SYS_open,        // Open a file
  SYS_read,        // Read from a file
  SYS_write,       // Write to a file
  SYS_kill,        // Kill a process
  SYS_getpid,      // Get process ID
  SYS_close,       // Close a file
  SYS_lseek,       // Move file pointer
  SYS_brk,         // Change data segment size
  SYS_fstat,       // Get file status
  SYS_time,        // Get current time
  SYS_signal,      // Handle signals
  SYS_execve,      // Execute a program
  SYS_fork,        // Create a child process
  SYS_link,        // Create a hard link
  SYS_unlink,      // Delete a file
  SYS_wait,        // Wait for a child process
  SYS_times,       // Get process times
  SYS_gettimeofday // Get time of day
};

// Pointer to user-defined event handler
static _Context* (*user_handler)(_Event, _Context*) = NULL;

// Interrupt handler for system calls and events
_Context* __am_irq_handle(_Context *c) {
  _Context *next = c; // Default to the current context
  if (user_handler) { // If a user handler is registered
    _Event ev = {0};  // Initialize an event
    switch (c->cause) { // Check the cause of the interrupt
      case -1: ev.event = _EVENT_YIELD; break; // Yield event
      case SYS_exit:
      case SYS_yield:
      case SYS_open:
      case SYS_read:
      case SYS_write:
      case SYS_close:
      case SYS_lseek:
      case SYS_brk:
      case SYS_execve: ev.event = _EVENT_SYSCALL; break; // System call event

      default: ev.event = _EVENT_ERROR; break; // Error event
    }

    // Call the user handler with the event and context
    next = user_handler(ev, c);
    if (next == NULL) { // If the handler returns NULL, keep the current context
      next = c;
    }
  }

  return next; // Return the next context to execute
}

extern void __am_asm_trap(void);

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("li a7, -1; ecall");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
