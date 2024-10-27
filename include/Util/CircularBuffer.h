#ifndef D_CircularBuffer_H
#define D_CircularBuffer_H

#include <stdbool.h>

typedef struct CircularBufferStruct * CircularBuffer;

CircularBuffer  CircularBuffer_Create              (unsigned int capacity);
void            CircularBuffer_Destroy             (CircularBuffer buffer);
bool            CircularBuffer_IsEmpty             (CircularBuffer buffer);
bool            CircularBuffer_IsFull              (CircularBuffer buffer);
bool            CircularBuffer_Write               (CircularBuffer buffer, char     byte);
bool            CircularBuffer_Read                (CircularBuffer buffer, char *   byte);
bool            CircularBuffer_CheckIntegrity       (CircularBuffer buffer);

#endif  /* D_CircularBuffer_H */