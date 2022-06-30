#ifndef __LIBCOCAO_STREAM_H__
#define __LIBCOCAO_STREAM_H__

#include <memory>
#include "bytearray.h"

namespace libcocao {

class Stream {
public:
    typedef std::shared_ptr<Stream> ptr;

    /**
     * 析构函数
     */
    virtual ~Stream() {}

    /**
     * 读数据
     * @param buffer 接收数据内存
     * @param length 接受数据内存的大小
     * @return
     *      > 0 返回接收到的数据实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int read (void *buffer, size_t length) = 0;

    /**
     * 读数据
     * @param ba 接收数据的ByteArray
     * @param length 接收数据内存大小
     * @return
     *      > 0 返回接收到的数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int read (ByteArray::ptr  ba, size_t length = 0);

    /**
     * 读固定长度的数据
     * @param buffer 接收数据的内存
     * @param length 接受数据的内存大小
     * @return
     *      > 0 返回接收到的数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int readFixSize (void *buffer, size_t length);

    /**
     * 读固定长度数据
     * @param ba 接收数据的ByteArray
     * @param length 接受数据内存大小
     * @return
     *      > 0 返回接收到数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int readFixSize (ByteArray::ptr ba, size_t length);

    /**
     * 写数据
     * @param buffer 写数据的内存
     * @param length 写入数据的内存大小
     * @return
     *      > 0 返回写入到的数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int write (const void* buffer, size_t length) = 0;

    /**
     * 写数据
     * @param ba 写数据的ByteArray
     * @param length 写入数据的内存大小
     * @return
     *      > 0 返回写入到的数据的实际大小
     *      = 0 被关闭
     *      < 0 流错误
     */
    virtual int write (ByteArray::ptr ba, size_t length) = 0;

    /**
     * 写入固定长度的数据
     * @param buffer 写入数据的内存
     * @param length 写入数据的内存大小
     * @return
     *      > 0 返回写入的数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int writeFixSize (const void* buffer, size_t length);

    /**
     * 写入固定长度的数据
     * @param ba 写数据的ByteArray
     * @param length 写入数据的内存大小
     * @return
     *      > 0 返回写入到的数据的实际大小
     *      = 0 被关闭
     *      < 0 出现流错误
     */
    virtual int writeFixSize (ByteArray::ptr ba, size_t length);

    virtual int close() = 0;
};

}

#endif