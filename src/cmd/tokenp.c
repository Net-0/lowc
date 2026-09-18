#ifndef TOKENP_C
#define TOKENP_C

#include "../token.c"
#include "../types.c"

// Print the tokens from a input fileDescript into a output fileDescriptor
void tokenp(Reader* reader, Writer* writer) {
    // Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD
    Token token;
    for (;Reader_hasNext(reader);) {
        Reader_nextN(reader, (byte*) &token, sizeof(token));
        Token_write(&token, writer);
        Writer_write(writer, '\n');
    }
    Writer_flush(writer);
}

// Binary entrypoint if we want only to test the token print
void _start(void) {
    __asm__ __volatile__ ("and $-16, %%rsp" ::: "memory"); // Fix the stack for the System V constraints
    tokenp(stdin, stdout);
    exit(EXIT_SUCCESS);
}

#endif