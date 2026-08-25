#ifndef MEMORY_C
#define MEMORY_C

#include "./types.c"

inline boolean memeq(const byte* p1, const byte* p2, const u32 n) {
  for (u32 i = 0; i < n; i++)
    if (p1[i] != p2[i])
      return false;
  return true;
}

byte* memset(byte* dest, const i32 value, const usize n) {
  byte* d = (byte*) dest;
  for (usize i = 0; i < n; i++)
    d[i] = (byte) value;
  return dest;
}

byte* memcpy(byte* dest, const byte* src, const usize n) {
  byte* d = (byte*) dest;
  const byte* s = (const byte*) src;
  for (usize i = 0; i < n; i++)
    d[i] = s[i];
  return dest;
}

byte* memmove(byte* dest, const byte* src, const usize n) {
  byte* d = (byte*) dest;
  const byte* s = (const byte*) src;

  if (d < s) {
    for (usize i = 0; i < n; i++)
      d[i] = s[i];
  } else if (d > s) {
    for (usize i = n; i > 0; i--)
      d[i - 1] = s[i - 1];
  }

  return dest;
}

#endif