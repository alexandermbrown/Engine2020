#pragma once

#include "yojimbo/yojimbo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

const uint64_t ProtocolId = 0x11223344556677ULL;

const int ClientPort = 30000;
const int ServerPort = 40000;

inline int GetNumBitsForMessage(uint16_t sequence)
{
    static int messageBitsArray[] = { 1, 320, 120, 4, 256, 45, 11, 13, 101, 100, 84, 95, 203, 2, 3, 8, 512, 5, 3, 7, 50 };
    const int modulus = sizeof(messageBitsArray) / sizeof(int);
    const int index = sequence % modulus;
    return messageBitsArray[index];
}

struct TestMessage : public yojimbo::Message
{
    uint16_t sequence;

    TestMessage()
    {
        sequence = 0;
    }

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_bits(stream, sequence, 16);

        int numBits = GetNumBitsForMessage(sequence);
        int numWords = numBits / 32;
        uint32_t dummy = 0;
        for (int i = 0; i < numWords; ++i)
            serialize_bits(stream, dummy, 32);
        int numRemainderBits = numBits - numWords * 32;
        if (numRemainderBits > 0)
            serialize_bits(stream, dummy, numRemainderBits);

        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct TestBlockMessage : public yojimbo::BlockMessage
{
    uint16_t sequence;

    TestBlockMessage()
    {
        sequence = 0;
    }

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_bits(stream, sequence, 16);
        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct TestSerializeFailOnReadMessage : public yojimbo::Message
{
    template <typename Stream> bool Serialize(Stream& /*stream*/)
    {
        return !Stream::IsReading;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct TestExhaustStreamAllocatorOnReadMessage : public yojimbo::Message
{
    template <typename Stream> bool Serialize(Stream& stream)
    {
        if (Stream::IsReading)
        {
            const int NumBuffers = 100;

            void* buffers[NumBuffers];

            memset(buffers, 0, sizeof(buffers));

            for (int i = 0; i < NumBuffers; ++i)
            {
                buffers[i] = YOJIMBO_ALLOCATE(stream.GetAllocator(), 1024 * 1024);
            }

            for (int i = 0; i < NumBuffers; ++i)
            {
                YOJIMBO_FREE(stream.GetAllocator(), buffers[i]);
            }
        }

        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

enum TestMessageType
{
    TEST_MESSAGE,
    TEST_BLOCK_MESSAGE,
    TEST_SERIALIZE_FAIL_ON_READ_MESSAGE,
    TEST_EXHAUST_STREAM_ALLOCATOR_ON_READ_MESSAGE,
    NUM_TEST_MESSAGE_TYPES
};

YOJIMBO_MESSAGE_FACTORY_START(TestMessageFactory, NUM_TEST_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE(TEST_MESSAGE, TestMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(TEST_BLOCK_MESSAGE, TestBlockMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(TEST_SERIALIZE_FAIL_ON_READ_MESSAGE, TestSerializeFailOnReadMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(TEST_EXHAUST_STREAM_ALLOCATOR_ON_READ_MESSAGE, TestExhaustStreamAllocatorOnReadMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

enum SingleTestMessageType
{
    SINGLE_TEST_MESSAGE,
    NUM_SINGLE_TEST_MESSAGE_TYPES
};

YOJIMBO_MESSAGE_FACTORY_START(SingleTestMessageFactory, NUM_SINGLE_TEST_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE(SINGLE_TEST_MESSAGE, TestMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

enum SingleBlockTestMessageType
{
    SINGLE_BLOCK_TEST_MESSAGE,
    NUM_SINGLE_BLOCK_TEST_MESSAGE_TYPES
};

YOJIMBO_MESSAGE_FACTORY_START(SingleBlockTestMessageFactory, NUM_SINGLE_BLOCK_TEST_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE(SINGLE_BLOCK_TEST_MESSAGE, TestBlockMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class TestAdapter : public yojimbo::Adapter
{
public:

    yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator)
    {
        return YOJIMBO_NEW(allocator, TestMessageFactory, allocator);
    }
};

static TestAdapter adapter;