/*
 * Lista de Testes do Buffer Circular
 * Objetivo: Facilitar troca de dados assíncrona (Ex: entre interrupções e módulos encarregados
 * da comunicação externa)
 * 
 * Buffer aponta estar vazio ao iniciar                     OK
 * Buffer retorna falso ao tentar ler com buffer vazio      OK
 * Buffer retorna ponteiro nulo caso seja passado capacidade zero como parâmetro de criação OK(?)
 * Buffer guarda e retorna o mesmo valor                    OK
 * Buffer aponta não-vazio após inserir valor               OK
 * Buffer aponta que está vazio após inserir e retirar valor
 * Buffer guarda e retorna um valor duas vezes              OK
 * Buffer apontar que está cheio corretamente               OK
 * 
 * Buffer aponta vazio após encher e esvaziar completamente OK
 * Buffer aponta integridade positiva ao ser criado (confere byte de guarda) OK
 * Buffer aponta integridade positiva quando cheio          OK
 * Buffer aponta integridade positiva após wrap-around      OK
 * Testar funcionalidades e estado (vazio/com elementos/cheio) após de inserir e retirar
 *      o equivalente à capacidade (wrap-around do buffer circular)   
*/

#include "unity.h"
#include "unity_fixture.h"
#include "CircularBuffer.h"
#include <stdbool.h>

TEST_GROUP(CircularBuffer);

static CircularBuffer buffer;
static unsigned int capacity = 10;

TEST_SETUP(CircularBuffer)
{
    buffer = CircularBuffer_Create(capacity);
}

TEST_TEAR_DOWN(CircularBuffer)
{
    CircularBuffer_Destroy(buffer);
}

TEST(CircularBuffer, CreateAndDestroy)
{
    CircularBuffer myBuffer = CircularBuffer_Create(10);
    TEST_ASSERT(myBuffer);

    CircularBuffer_Destroy(myBuffer);
}

TEST(CircularBuffer, ReturnNullIfCapacityIsZero)
{
    CircularBuffer myBuffer = CircularBuffer_Create(0);
    
    TEST_ASSERT(!myBuffer);
}

TEST(CircularBuffer, BufferIsEmptyWhenCreated)
{
    bool isEmpty = CircularBuffer_IsEmpty(buffer);

    TEST_ASSERT(isEmpty);
}

TEST(CircularBuffer, BufferReturnsFalseWhenReadingWhileEmpty)
{
    char byte = 100;
    
    bool byteReceived = CircularBuffer_Read(buffer, &byte);

    TEST_ASSERT(!byteReceived);
    TEST_ASSERT_EQUAL_INT(100, byte);
}

TEST(CircularBuffer, WriteAndRead)
{
    bool written = CircularBuffer_Write(buffer, 5);

    char byte;
    bool read =    CircularBuffer_Read(buffer, &byte);

    TEST_ASSERT(written);
    TEST_ASSERT(read);
    TEST_ASSERT_EQUAL_INT(5, byte);
}

TEST(CircularBuffer, BuffertNotEmptyAfterWrite)
{
    CircularBuffer_Write(buffer, 5);
    bool isEmpty = CircularBuffer_IsEmpty(buffer);

    TEST_ASSERT(!isEmpty);
}

TEST(CircularBuffer, BuffertEmptyAfterWriteAndRead)
{
    CircularBuffer_Write(buffer, 5);
    char byte;
    CircularBuffer_Read(buffer, &byte);

    bool isEmpty = CircularBuffer_IsEmpty(buffer);

    TEST_ASSERT(isEmpty);
}

TEST(CircularBuffer, WriteAndReadTwice)
{
    bool written1 = CircularBuffer_Write(buffer, 5);
    bool written2 = CircularBuffer_Write(buffer, 6);

    char byte1; char byte2;
    bool read1 =    CircularBuffer_Read(buffer, &byte1);
    bool read2 =    CircularBuffer_Read(buffer, &byte2);

    TEST_ASSERT(written1);
    TEST_ASSERT(written2);
    TEST_ASSERT(read1);
    TEST_ASSERT(read2);
    TEST_ASSERT_EQUAL_INT(5, byte1);
    TEST_ASSERT_EQUAL_INT(6, byte2);
}

TEST(CircularBuffer, IsBufferFull)
{
    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
    }

    bool isFull = CircularBuffer_IsFull(buffer);
    
    TEST_ASSERT(isFull);
    TEST_ASSERT(CircularBuffer_Write(buffer,0) == false);
}

TEST(CircularBuffer, BufferIsEmptyAfterFull)
{
    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
    }
    char byte;
    for (char i = 0; i < capacity; i++)
    {
        TEST_ASSERT(CircularBuffer_Read(buffer, &byte));
    }
    
    
    TEST_ASSERT(CircularBuffer_IsEmpty(buffer));
    TEST_ASSERT(CircularBuffer_Read(buffer, &byte) == false);
}

TEST(CircularBuffer, IntegrityAfterCreate)
{
    TEST_ASSERT(CircularBuffer_CheckIntegrity(buffer));
}

TEST(CircularBuffer, IntegrityWhenFull)
{
    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
    }

    TEST_ASSERT(CircularBuffer_IsFull(buffer));
    TEST_ASSERT(CircularBuffer_CheckIntegrity(buffer));
}

TEST(CircularBuffer, IntegrityAfterWrapAround)
{
    char readStore;
    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
        CircularBuffer_Read(buffer, &readStore);
    }

    CircularBuffer_Write(buffer, 0);
    TEST_ASSERT(CircularBuffer_CheckIntegrity(buffer));
}

TEST(CircularBuffer, WrapAround)
{
    char readStore;
    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
        CircularBuffer_Read(buffer, &readStore);
    }

    for (char i = 0; i < capacity; i++)
    {
        CircularBuffer_Write(buffer, i);
    }
    char byte;
    for (char i = 0; i < capacity; i++)
    {
        TEST_ASSERT(CircularBuffer_Read(buffer, &byte));
        TEST_ASSERT(CircularBuffer_CheckIntegrity(buffer));
    }
}