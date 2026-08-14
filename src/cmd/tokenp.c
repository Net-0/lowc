#ifndef TOKENP_C
#define TOKENP_C

#include "../lib.c"
#include "../token.c"
#include "../write.c"
#include "../types.c"

// 1MiB
#define MAX_INPUT 1 * 1024 * 1024

// Print the tokens from a input fileDescript into a output fileDescriptor
void tokenp(const i32 in, const i32 out) {
    byte input[MAX_INPUT];
    isize inputLength = read(in, input, MAX_INPUT);
    if (inputLength <= 0) exit(EXIT_TOKENIZE_IN_READ_FAILURE);
    if (inputLength == MAX_INPUT) exit(EXIT_TOKENIZE_IN_TOO_BIG);

    // Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD

    Token* tokens = (Token*) &input;
    u64 tokensLength = (inputLength / sizeof(Token));

    for (u64 i = 0; i < tokensLength; i++) {
        Token token = tokens[i];
        Token_write(&token, out);
        writeString(out, "\n");
    }
}

// Binary entrypoint if we want only to test the token print
void _start(void) {
    tokenp(stdin, stdout);
    exit(EXIT_SUCCESS);
}

#endif