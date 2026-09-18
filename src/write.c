#ifndef WRITE_C
#define WRITE_C

#include "./lib.c"

#define writeString(fileDescriptor, str) write(fileDescriptor, str, sizeof(str)-1)

i32 writeU32(const i32 fileDescriptor, u32 value) {
    byte buf[10]; // max for u32: 4294967295 (10 digits)
    u32 i = 0;

    // Special case: 0
    if (value == 0) {
        buf[i++] = '0';
        return write(fileDescriptor, buf, i);
    }

    // Build digits in reverse
    while (value > 0) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Reverse in-place
    for (u32 j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i-1-j] = tmp;
    }

    return write(fileDescriptor, buf, i);
}

#endif