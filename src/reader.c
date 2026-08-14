#ifndef READER_C
#define READER_C

#include "./types.c"
#include "./position.c"

typedef struct Reader Reader;
struct Reader {
    const byte* input;
    const u32 inputLength;
    Position position;

};

Reader Reader_new(const byte* input, const u32 inputLength) {
    return (Reader) { .input = input, .inputLength = inputLength, .position = INITIAL_POSITION };
}

boolean Reader_hasNext(const Reader* reader) {
    return reader->position.index < reader->inputLength;
}

byte Reader_peek(Reader* reader) {
    if (!Reader_hasNext(reader))
        exit(EXIT_FAILURE);

    return reader->input[reader->position.index];
}

void Reader_next(Reader* reader) {
    if (!Reader_hasNext(reader))
        exit(EXIT_FAILURE);

    if ('\n' == reader->input[reader->position.index]) {
        reader->position.line++;
        reader->position.column = 1;
    } else {
        reader->position.column++;
    }
    reader->position.index++;
}

#endif