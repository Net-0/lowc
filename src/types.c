#ifndef TYPES_C
#define TYPES_C

////////////////////////////////////////////////////
// Numeric types

typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
typedef signed int i32;
typedef unsigned int u32;

#if __WORDSIZE == 64
typedef signed long int i64;
typedef unsigned long int u64;
#else
__extension__ typedef signed long long int i64;
__extension__ typedef unsigned long long int u64;
#endif

typedef float f32;
typedef double f64;

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Size types

typedef char byte;

typedef long unsigned int usize;
typedef long int isize;

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Boolean type

typedef char boolean;
const boolean true = 1;
const boolean false = 0;
#define true true
#define false false

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Exit status

enum ExitStatus {
    EXIT_SUCCESS = 0,
    EXIT_FAILURE,
    EXIT_STDIN_READ_FAILURE,
    EXIT_STDIN_TOO_BIG,
    EXIT_UNEXPECTED_TOKEN
};

////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Standard IO

const i32 stdin = 0;
const i32 stdout = 1;
const i32 stderr = 2;

////////////////////////////////////////////////////

#endif
