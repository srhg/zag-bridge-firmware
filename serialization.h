#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <stdint.h>
#include <string.h>

void serialize_uint(unsigned int value, uint8_t *target, size_t target_size);
void serialize_int(int value, uint8_t *target, size_t target_size);
unsigned int deserialize_uint(const uint8_t *source, size_t source_size);
int deserialize_int(const uint8_t *source, size_t source_size);
unsigned short deserialize_ushort(const uint8_t *source, size_t source_size);
short deserialize_short(const uint8_t *source, size_t source_size);

#endif // _SERIALIZATION_H_
