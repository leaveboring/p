#include "common.h"
#include "syscall.h"

#include "proc.h"

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
void naive_uload(PCB *pcb, const char *filename);


_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
 
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  int ret = 0;

  switch (a[0]) {
    
	case SYS_yield: // Yield CPU
	    _yield(); // Call yield function
	    break;

	  case SYS_exit: // Exit current process
	    naive_uload(NULL, "/bin/init"); // Load the init program
	    break;

	  case SYS_write: // Write to a file
	    ret = fs_write(a[1], (void*)a[2], a[3]); // Call file system write function
	    break;

	  case SYS_read: // Read from a file
	    ret = fs_read(a[1], (void*)a[2], a[3]); // Call file system read function
	    break;

	  case SYS_open: // Open a file
	    ret = fs_open((char*)a[1], a[2], a[3]); // Call file system open function
	    break;

	  case SYS_close: // Close a file
	    ret = fs_close(a[1]); // Call file system close function
	    break;

	  case SYS_lseek: // Move file pointer
	    ret = fs_lseek(a[1], a[2], a[3]); // Call file system seek function
	    break;

	  case SYS_brk: // Change data segment size
	    // No implementation yet
	    break;

	  case SYS_execve: // Execute a program
	    naive_uload(NULL, (const char*)a[1]); // Load the specified program
	    break;
    
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  c->GPRx = ret;

  return NULL;
}
