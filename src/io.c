#ifndef IO_C
#define IO_C

#include "./types.c"
#include "./linux.c"
#include "./math.c"
#include "./memory.c"

#define IO_BUFFER_CAPACITY 8192

////////////////////////////////////////////////////

typedef struct Writer Writer;
struct Writer {
    const i32 fileDescriptor;
    byte buffer[IO_BUFFER_CAPACITY];
    u16 bufferIndex;
};

#define Writer_new(fd) (Writer) { .fileDescriptor = fd, .bufferIndex = 0 }

void Writer_flush(Writer* writer) {
    const i32 result = write(writer->fileDescriptor, writer->buffer, writer->bufferIndex);
    if (result < 0)
        exit(EXIT_IO_WRITER_BAD_FLUSH);
    writer->bufferIndex = 0;
}

void Writer_write(Writer* writer, const byte b) {
    if (IO_BUFFER_CAPACITY < writer->bufferIndex)
        Writer_flush(writer);
    writer->buffer[writer->bufferIndex++] = b;
}

void Writer_writeN(Writer* writer, const byte* buffer, const u32 count) {
    for (u32 i = 0; i < count;) {
        for (; writer->bufferIndex < IO_BUFFER_CAPACITY && i < count;)
            writer->buffer[writer->bufferIndex++] = buffer[i++];
        if (IO_BUFFER_CAPACITY < writer->bufferIndex)
            Writer_flush(writer);
    }
}

void Writer_writeU32(Writer* writer, u32 value) {
    // Special case: 0
    if (value == 0)
        Writer_write(writer, '0');

    byte buf[10]; // max for u32: 4294967295 (10 digits)
    u32 i = 0;

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

    return Writer_writeN(writer, buf, i);
}

#define Writer_writeString(writer, str) Writer_writeN(writer, str, sizeof(str)-1)

////////////////////////////////////////////////////

#include "./position.c"

typedef struct Reader Reader;
struct Reader {
    const i32 fileDescriptor;
    byte buffer[IO_BUFFER_CAPACITY];
    u16 bufferLength;
    u16 bufferIndex;
    Position position;
};

#define Reader_new(fd) (Reader) { .fileDescriptor = fd, .bufferLength = 0, .bufferIndex = 1, .position = INITIAL_POSITION }

void Reader_fill(Reader* reader) {
    // 1. Move unread buffered data to the begin of the buffer
    const u16 remaining = reader->bufferLength - min(reader->bufferLength, reader->bufferIndex);
    memmove(reader->buffer, reader->buffer+reader->bufferIndex, remaining);

    // 2. Fill the remaining space in the buffer
    reader->bufferLength = remaining + read(reader->fileDescriptor, reader->buffer+remaining, IO_BUFFER_CAPACITY - remaining);
    reader->bufferIndex = 0;
}

boolean Reader_hasNext(Reader* reader) {
    // Ensure that the buffer is loaded
    if (reader->bufferLength < reader->bufferIndex)
        Reader_fill(reader);

    return reader->bufferIndex < reader->bufferLength;
}

byte Reader_peek(Reader* reader) {
    if (!Reader_hasNext(reader)) // check and load buffer if needed!
        exit(EXIT_IO_READER_NO_REMAINING_DATA);

    return reader->buffer[reader->bufferIndex];
}

byte Reader_next(Reader* reader) {
    if (!Reader_hasNext(reader)) // check and load buffer if needed!
        exit(EXIT_IO_READER_NO_REMAINING_DATA);

    const byte b = reader->buffer[reader->bufferIndex++];

    if ('\n' == b) {
        reader->position.line++;
        reader->position.column = 1;
    } else {
        reader->position.column++;
    }
    reader->position.index++;
    return b;
}

u32 Reader_nextN(Reader* reader, byte* buffer, u32 count) {
    u32 i = 0;
    for (; i < count && Reader_hasNext(reader); i++)
        buffer[i] = Reader_next(reader);
    return i;
}

////////////////////////////////////////////////////
// Standard IO

Reader __stdin = Reader_new(0);
Writer __stdout = Writer_new(1);
Writer __stderr = Writer_new(2);

#define stdin &__stdin
#define stdout &__stdout
#define stderr &__stderr

////////////////////////////////////////////////////

#endif