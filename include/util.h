#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    inline uint16_t swap_16(uint16_t x)
    {
        return ((x << 8) & 0xFF00) | ((x >> 8) & 0x00FF);
    }

    inline uint32_t swap_32(uint32_t x)
    {
        return ((x & 0x000000FF) << 24) | ((x & 0x0000FF00) << 8) |
               ((x & 0x00FF0000) >> 8) | ((x & 0xFF000000) >> 24);
    }

    inline uint16_t big_endian_16(uint16_t x)
    {
#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
        return swap_16(x);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return x;
#else
#error unsupported endianness
#endif
    }

    inline uint32_t big_endian_32(uint32_t x)
    {
#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
        return swap_32(x);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return x;
#else
#error unsupported endianness
#endif
    }

    inline uint16_t little_endian_16(uint16_t x)
    {
#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
        return x;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap_16(x);
#else
#error unsupported endianness
#endif
    }

    inline uint32_t little_endian_32(uint32_t x)
    {
#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
        return x;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return swap_32(x);
#else
#error unsupported endianness
#endif
    }

    uint16_t swap_16(uint16_t x);
    uint32_t swap_32(uint32_t x);
    uint16_t big_endian_16(uint16_t);
    uint32_t big_endian_32(uint32_t);
    uint16_t little_endian_16(uint16_t);
    uint32_t little_endian_32(uint32_t);

#ifdef __cplusplus
}
#endif

#endif
