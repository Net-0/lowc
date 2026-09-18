#ifndef LINUX_C
#define LINUX_C

#include "./types.c"

////////////////////////////////////////////////////
// file flags

const i32 O_RDONLY    = 0b00000000000000000000000000000000; // open for reading only
const i32 O_WRONLY    = 0b00000000000000000000000000000001; // open for writing only
const i32 O_RDWR      = 0b00000000000000000000000000000010; // open for reading and writing
const i32 O_CREAT     = 0b00000000000000000000000001000000; // create the file if it doesn't exist
const i32 O_EXCL      = 0b00000000000000000000000010000000; // with O_CREAT, fail if the file already exists
const i32 O_NOCTTY    = 0b00000000000000000000000100000000; // don't make this file the process's controlling terminal
const i32 O_TRUNC     = 0b00000000000000000000001000000000; // truncate the file to zero length if it exists
const i32 O_APPEND    = 0b00000000000000000000010000000000; // always write at the end of the file
const i32 O_NONBLOCK  = 0b00000000000000000000100000000000; // open/operate in non-blocking mode
const i32 O_DSYNC     = 0b00000000000000000001000000000000; // writes complete once the data (not necessarily metadata) is flushed
const i32 O_ASYNC     = 0b00000000000000000010000000000000; // enable signal-driven I/O
const i32 O_DIRECT    = 0b00000000000000000100000000000000; // minimize cache effects of I/O to/from this file
const i32 O_LARGEFILE = 0b00000000000000001000000000000000; // legacy 32-bit large-file flag, no-op on 64-bit
const i32 O_DIRECTORY = 0b00000000000000010000000000000000; // fail unless path is a directory
const i32 O_NOFOLLOW  = 0b00000000000000100000000000000000; // fail if path is a symbolic link
const i32 O_NOATIME   = 0b00000000000001000000000000000000; // don't update the file's last access time on read
const i32 O_CLOEXEC   = 0b00000000000010000000000000000000; // close this fd automatically on execve
const i32 O_SYNC      = 0b00000000000100000001000000000000; // writes complete once both data and metadata are flushed
const i32 O_PATH      = 0b00000000001000000000000000000000; // obtain a fd for path-based operations without opening the file itself
const i32 O_TMPFILE   = 0b00000000010000010000000000000000; // create an unnamed temporary file within the given directory

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// file mode (permission bits, used with O_CREAT/O_TMPFILE)

const i32 S_IRUSR = 0b00000000000000000000000100000000; // owner read permission
const i32 S_IWUSR = 0b00000000000000000000000010000000; // owner write permission
const i32 S_IXUSR = 0b00000000000000000000000001000000; // owner execute permission
const i32 S_IRGRP = 0b00000000000000000000000000100000; // group read permission
const i32 S_IWGRP = 0b00000000000000000000000000010000; // group write permission
const i32 S_IXGRP = 0b00000000000000000000000000001000; // group execute permission
const i32 S_IROTH = 0b00000000000000000000000000000100; // other read permission
const i32 S_IWOTH = 0b00000000000000000000000000000010; // other write permission
const i32 S_IXOTH = 0b00000000000000000000000000000001; // other execute permission
const i32 S_ISUID = 0b00000000000000000000100000000000; // set-user-ID on execute
const i32 S_ISGID = 0b00000000000000000000010000000000; // set-group-ID on execute
const i32 S_ISVTX = 0b00000000000000000000001000000000; // sticky bit (restrict deletion within a directory)

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// seek constants

typedef enum SeekWhence SeekWhence;
enum SeekWhence {
    SEEK_SET = 0, // offset is absolute, measured from the start of the file
    SEEK_CUR = 1, // offset is relative to the current file position
    SEEK_END = 2, // offset is relative to the end of the file
};

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// mmap-family flags

const i32 PROT_NONE     = 0b00000000000000000000000000000000; // the mapping cannot be accessed
const i32 PROT_READ     = 0b00000000000000000000000000000001; // the mapping can be read
const i32 PROT_WRITE    = 0b00000000000000000000000000000010; // the mapping can be written
const i32 PROT_EXEC     = 0b00000000000000000000000000000100; // the mapping can be executed

const i32 MAP_SHARED    = 0b00000000000000000000000000000001; // writes go back to the file and are visible to other mappers
const i32 MAP_PRIVATE   = 0b00000000000000000000000000000010; // writes are copy-on-write, private to this mapping
const i32 MAP_FIXED     = 0b00000000000000000000000000010000; // place the mapping at exactly the given address
const i32 MAP_ANONYMOUS = 0b00000000000000000000000000100000; // the mapping isn't backed by a file

const i32 MREMAP_MAYMOVE = 0b00000000000000000000000000000001; // allow the kernel to relocate the mapping if it can't grow in place
const i32 MREMAP_FIXED   = 0b00000000000000000000000000000010; // move the mapping to exactly the given address (requires MREMAP_MAYMOVE)

const i32 MS_ASYNC       = 0b00000000000000000000000000000001; // schedule the flush and return immediately
const i32 MS_INVALIDATE  = 0b00000000000000000000000000000010; // invalidate other mappings' cached copies of the flushed pages
const i32 MS_SYNC        = 0b00000000000000000000000000000100; // block until the flush completes

////////////////////////////////////////////////////

// Linux 'read' syscall
//
// Read up to count bytes from fileDescriptor into buffer, returning the number of bytes actually read
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
//
// Write count bytes from buffer to fileDescriptor, returning the number of bytes actually written
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
//
// Open (or create) the file at path with the given flags/mode, returning a new file descriptor
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
//
// Close fileDescriptor, releasing it
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
//
// Move fileDescriptor's file position to offset relative to whence, returning the resulting position
isize lseek(const i32 fileDescriptor, const isize offset, const SeekWhence whence) {
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
//
// Read up to count bytes from fileDescriptor into buffer starting at offset, without moving the file position
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
//
// Write count bytes from buffer to fileDescriptor starting at offset, without moving the file position
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
//
// Map length bytes of fileDescriptor (or anonymous memory, if MAP_ANONYMOUS) at offset into memory with the given prot/flags;
// hint is only a placement suggestion unless flags includes MAP_FIXED. Returns the mapped address
isize mmap(byte* hint, const usize length, const i32 prot, const i32 flags, const i32 fileDescriptor, const isize offset) {
  isize ret;
  register i32 r10 __asm__("r10") = flags;
  register i32 r8 __asm__("r8") = fileDescriptor;
  register isize r9 __asm__("r9") = offset;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(9), "D"(hint), "S"(length), "d"(prot), "r"(r10), "r"(r8), "r"(r9)
    : "rcx", "r11", "memory"
  );
  return ret;
}

// Linux 'munmap' syscall
//
// Unmap the length-byte mapping starting at addr
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
//
// Change the access permissions of the length-byte mapping starting at addr to prot
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
//
// Resize the oldSize-byte mapping at oldAddress to newSize, relocating it if it can't grow in place and flags includes MREMAP_MAYMOVE
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
//
// Flush changes made to the length-byte mapping starting at addr back to its backing file
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
//
// Terminate the process immediately with the given exit code
void __attribute__((noreturn)) exit(const u8 code) {
  __asm__ volatile (
    "syscall"
    :
    : "a"(60), "D"((int) code)
    : "rcx", "r11", "memory"
  );
  __builtin_unreachable();
}

// Linux 'ftruncate' syscall
//
// Resize fileDescriptor's file to exactly length bytes, extending it with zeros or discarding trailing data as needed
isize ftruncate(const i32 fileDescriptor, const isize length) {
  isize ret;
  __asm__ volatile (
    "syscall"
    : "=a"(ret)
    : "a"(77), "D"(fileDescriptor), "S"(length)
    : "rcx", "r11", "memory"
  );
  return ret;
}

#endif