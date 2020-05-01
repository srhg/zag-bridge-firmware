#include "serialization.h"

void serialize_uint(unsigned int value, uint8_t *target, size_t target_size)
{
    size_t i;
    for (i = 1; i <= target_size; i++) {
        target[target_size - i] = value & 0xFF;
        value = value >> 8;
    }
}

void serialize_int(int value, uint8_t *target, size_t target_size)
{
    serialize_uint((unsigned int)value, target, target_size);
}

unsigned int deserialize_uint(const uint8_t *source, size_t source_size)
{
    unsigned int retval = 0;
    size_t i;

    for (i = 0; i < source_size; i++) {
        retval = (retval << 8) | source[i];
    }

    return retval;
}

int deserialize_int(const uint8_t *source, size_t source_size)
{
    return (int)deserialize_uint(source, source_size);
}

unsigned short deserialize_ushort(const uint8_t *source, size_t source_size)
{
    return (unsigned short)deserialize_uint(source, source_size);
}

short deserialize_short(const uint8_t *source, size_t source_size)
{
    return (short)deserialize_ushort(source, source_size);
}
