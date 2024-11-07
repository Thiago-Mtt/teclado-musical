#include "CircularBuffer.h"
#include <stdlib.h>


typedef struct CircularBufferStruct
{
    unsigned int capacity;
    char * values;
    unsigned int index;
    unsigned int outdex;
} CircularBufferStruct;

enum guardValue { OK = 100};

CircularBuffer CircularBuffer_Create(unsigned int capacity)
{
    if (capacity == 0)
    {
        return 0;
    }

    CircularBuffer buffer = (CircularBuffer) calloc(1, sizeof(CircularBufferStruct));
    /* +1 para ser capaz de encher buffer de [capacity] bytes e manter index e outdex diferentes*/
    buffer->capacity = capacity + 1;
    /* +1 para servir de 'guarda', conferir se houve escrita excedendo limite do buffer*/
    buffer->values = (char *) calloc(buffer->capacity +1, sizeof(char)); 
    buffer->index = 0;
    buffer->outdex = 0;

    /* Definindo valor padrão para byte de guarda de integridade*/
    buffer->values[buffer->capacity] = OK;

    return buffer;
}

void CircularBuffer_Destroy(CircularBuffer buffer)
{
    free(buffer->values);
    free(buffer);
}

static bool isEmpty(CircularBuffer buffer)
{
    if(buffer->index != buffer->outdex)
    {
        return false;
    }
    return true;
}

bool CircularBuffer_IsEmpty(CircularBuffer buffer)
{
    return isEmpty(buffer);
}

static bool isFull(CircularBuffer buffer)
{
    if ((buffer->index + 1) % buffer->capacity == buffer->outdex)
    {
        return true;
    }
    return false;    
}

bool CircularBuffer_IsFull(CircularBuffer buffer)
{
    return isFull(buffer);
}

static void moveOutdex(CircularBuffer buffer)
{
    buffer->outdex++;
    buffer->outdex = buffer->outdex % buffer->capacity;
}

bool CircularBuffer_Read(CircularBuffer buffer, char * byte)
{
    if(isEmpty(buffer))
    {
        return false;
    }

    *byte = buffer->values[buffer->outdex];
    moveOutdex(buffer);
    
    return true;
}

static void moveIndex(CircularBuffer buffer)
{
    buffer->index++;
    buffer->index = buffer->index % buffer->capacity;
}

bool CircularBuffer_Write(CircularBuffer buffer, char byte)
{
    if (isFull(buffer))
    {
        return false;
    }

    buffer->values[buffer->index] = byte;
    moveIndex(buffer);
    
    return true;
}

bool CircularBuffer_CheckIntegrity(CircularBuffer buffer)
{
    if (buffer->values[buffer->capacity] == OK)
    {
        return true;
    } 
    return false;
}