#ifndef __LIBCOCAO_ENDIAN_H__
#define __LIBCOCAO_ENDIAN_H__

#define LIBCOCAO_LITTLE_ENDIAN 1
#define LIBCOCAO_BIG_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>
#include <type_traits>

namespace libcocao {

/**
 * 8字节类型的字节序列化
 */
template <class T>
    typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
    byteswap (T value) {
        return (T) bswap_64((uint64_t)value);
    }

/**
 * 4字节类型的字节序列化
 */
template <class T>
    typename std::enable_if<sizeof (T) == sizeof(uint32_t), T>::type
    byteswap (T value) {
        return (T) bswap_32((uint32_t)value);
    }

/**
 * 2字节类型的字节序列化
 */
template <class T>
    typename std::enable_if<sizeof (T) == sizeof(uint16_t), T>::type
    byteswap (T value) {
        return (T) bswap_16((uint16_t)value);
    }

#if BYTE_ORDER == BIG_ENDIAN
#define LIBCOCAO_BYTE_ORDER LIBCOCAO_BIG_ENDIAN
#else
#define LIBCOCAO_BYTE_ORDER LIBCOCAO_LITTLE_ENDIAN
#endif

#if LIBCOCAO_BYTE_ORDER == LIBCOCAO_BIG_ENDIAN

/**
 * 只有小端机器上执行byteswap，在大端机器上什么都不做
 */
template <class T>
T byteswapOnLittleEndian (T t) {
    return t;
}

/**
 * 只有在大端机器上执行byteswap,在小端口机器上什么都不做
 */
template <class T>
T byteswapOnBigEndain (T t) {
    return byteswap(t);
}
#else

/**
 * 只有小端机器上执行byteswap，在大端机器上什么都不做
 */
template <class T>
T byteswapOnLittleEndian (T t) {
    return t;
}

/**
 * 只有在大端机器上执行byteswap,在小端口机器上什么都不做
 */
template <class T>
T byteswapOnBigEndain (T t) {
    return byteswap(t);
}
#endif

}

#endif