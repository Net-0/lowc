#ifndef LINUX_C
#define LINUX_C

#include "./types.c"

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