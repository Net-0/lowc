#ifndef LINUX_C
#define LINUX_C

#include "./types.c"

////////////////////////////////////////////////////
// file flags

#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR   0x0002
#define O_CREAT  0x0040
#define O_TRUNC  0x0200

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// mmap-family flags

#define PROT_NONE  0x0
#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_FIXED     0x10
#define MAP_ANONYMOUS 0x20

#define MREMAP_MAYMOVE 0x1
#define MREMAP_FIXED   0x2

#define MS_ASYNC      0x1
#define MS_INVALIDATE 0x2
#define MS_SYNC       0x4

////////////////////////////////////////////////////

// Linux 'read' syscall
isize read(const i32 fileDescriptor, const byte* buffer, const usize count) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(0), "D"(fileDescriptor), "S"(buffer), "d"(count)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'write' syscall
isize write(const i32 fileDescriptor, const byte* buffer, const usize count) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(1), "D"(fileDescriptor), "S"(buffer), "d"(count)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'open' syscall
isize open(const byte* path, const i32 flags, const i32 mode) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(2), "D"(path), "S"(flags), "d"(mode)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'close' syscall
isize close(const i32 fileDescriptor) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(3), "D"(fileDescriptor)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'lseek' syscall
isize lseek(const i32 fileDescriptor, const isize offset, const i32 whence) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(8), "D"(fileDescriptor), "S"(offset), "d"(whence)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'pread64' syscall
isize pread(const i32 fileDescriptor, const byte* buffer, const usize count, const isize offset) {
  isize ret;
  register isize r10 __asm__("r10") = offset;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(17), "D"(fileDescriptor), "S"(buffer), "d"(count), "r"(r10)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'pwrite64' syscall
isize pwrite(const i32 fileDescriptor, const byte* buffer, const usize count, const isize offset) {
  isize ret;
  register isize r10 __asm__("r10") = offset;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(18), "D"(fileDescriptor), "S"(buffer), "d"(count), "r"(r10)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'mmap' syscall
isize mmap(byte* addr, const usize length, const i32 prot, const i32 flags, const i32 fileDescriptor, const isize offset) {
  isize ret;
  register i32 r10 __asm__("r10") = flags;
  register i32 r8 __asm__("r8") = fileDescriptor;
  register isize r9 __asm__("r9") = offset;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(9), "D"(addr), "S"(length), "d"(prot), "r"(r10), "r"(r8), "r"(r9)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'munmap' syscall
isize munmap(byte* addr, const usize length) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(11), "D"(addr), "S"(length)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'mprotect' syscall
isize mprotect(byte* addr, const usize length, const i32 prot) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(10), "D"(addr), "S"(length), "d"(prot)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'mremap' syscall
isize mremap(byte* oldAddress, const usize oldSize, const usize newSize, const i32 flags) {
  isize ret;
  register i32 r10 __asm__("r10") = flags;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(25), "D"(oldAddress), "S"(oldSize), "d"(newSize), "r"(r10)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'msync' syscall
isize msync(byte* addr, const usize length, const i32 flags) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(26), "D"(addr), "S"(length), "d"(flags)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'exit' syscall
void __attribute__((noreturn)) exit(const u8 code) {
  __asm__ volatile (
    "syscall"
    :
    : "a"(60), "D"((int) code)
    : "rcx", "r11", "memory"
  );
  __builtin_unreachable();
}

#endif