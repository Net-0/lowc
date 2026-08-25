#ifndef TOKENP_C
#define TOKENP_C

#include "../token.c"
#include "../types.c"

// Print the tokens from a input fileDescript into a output fileDescriptor
void tokenp(Reader* reader, Writer* writer) {
    TokenReader tokenReader = TokenReader_new(reader);
    while (TokenReader_hasNext(&tokenReader)) {
        const Token token = TokenReader_next(&tokenReader);
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