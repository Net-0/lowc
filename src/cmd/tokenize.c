#ifndef TOKENIZER_C
#define TOKENIZER_C

#include "../lib.c"
#include "../lexer.c"
#include "../write.c"
#include "../types.c"

// 1MiB
#define MAX_INPUT 1 * 1024 * 1024

// Tokenize from a input fileDescriptor and write the binary content of the tokens into a output fileDescriptr.
void tokenize(const i32 in, const i32 out) {
    byte input[MAX_INPUT];
    isize inputLength = read(in, input, MAX_INPUT);
    if (inputLength <= 0) exit(EXIT_TOKENIZE_IN_READ_FAILURE);
    if (inputLength == MAX_INPUT) exit(EXIT_TOKENIZE_IN_TOO_BIG);

    // Note: this shit is dangerous as fuck, unpacked structs have variadic final size (based on the compiler output), but guess we're going to deal with it xD

    Lexer lexer = Lexer_new(input, inputLength);

    for (;;) {
        const Token token = Lexer_next(&lexer);

        // Stop if no token left
        if (token.type == NO_TOKEN) break;

        if (token.type == UNEXPECTED) {
            writeString(stderr, "Unexpected token: ");
            Token_write(&token, stderr);
            writeString(stderr, "\n");
            exit(EXIT_TOKENIZE_UNEXPECTED_TOKEN);
        }

        write(out, (byte*) &token, sizeof(token));
    }
}

// Binary entrypoint if we want only to test the tokenization
void _start(void) {
    tokenize(stdin, stdout);
    exit(EXIT_SUCCESS);
}

#endif