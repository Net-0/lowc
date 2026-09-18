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
void exit(const u8 code) {
  __asm__ volatile (
    "syscall"
    :
    : "a"(60), "D"((int) code)
    : "rcx", "r11", "memory"
  );
  __builtin_unreachable();
}

inline boolean memeq(const byte* p1, const byte* p2, const u32 n) {
  for (u32 i = 0; i < n; i++)
    if (p1[i] != p2[i])
      return false;
  return true;
}

#endif