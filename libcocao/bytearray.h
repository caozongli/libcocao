#ifndef __LIBCOCAO_BYTEARRAY_H__
#define __LIBCOCAO_BYTEARRAY_H__

#include <memory>
#include <string>
#include <vector>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace libcocao {

class ByteArray {
public:
    typedef std::shared_ptr<ByteArray> ptr;
    struct Node {
        /**
         * 构造指定大小的内存块
         * 内存块字节数
         * @param s
         */
        Node (size_t s);

        /**
         * 无参构造函数
         */
        Node();

        /**
         * 析构函数，释放内存
         */
        ~Node();

        /// 内存块地址指针
        char *ptr;
        /// 下一个内存块地址
        Node *next;
        /// 内存块大小
        size_t size;
    };

    /**
     * 使用指定长度的内存块构造ByteArray
     * base_size 内存块大小
     */
    ByteArray (size_t base_size = 4096);

    /**
     * 析构函数`
     */
    ~ByteArray();

    /**
     * 写入固定长度int8_t类型数据
     * m_position += sizeof (value)
     * 如果m_position > m_size 则m_size = m_ppsition
     * @param value
     */
    void writeFint8 (int8_t value);

    /**
     * 写入固定长度uint8_t类型的数据
     * @param value
     */
    void writeFuint8 (uint8_t value);

    /**
     * 写入固定长度int16_t数据
     * @param value
     */
    void writeFint16 (int16_t value);

    /**
     * 写入固定长度uint16_t类型的数据（大端/小端)
     * @param value
     */
    void writeFuint16 (uint16_t value);

    /**
     * 写入固定长度int32_t数据
     * @param value
     */
    void writeFint32 (int32_t value);

    /**
     * 写入固定长度uint32_t类型的数据（大端/小端)
     * @param value
     */
    void writeFuint32 (uint32_t value);

    /**
     * 写入固定长度int16_t数据
     * @param value
     */
    void writeFint64 (int64_t value);

    /**
     * 写入固定长度uint16_t类型的数据（大端/小端)
     * @param value
     */
    void writeFuint64 (uint64_t value);

    /**
     * 写入有符号Varint32类型的数据
     * @param value
     */
    void writeint32 (int32_t value);

    /**
     * 写入无符号Varint32类型的数据
     * @param value
     */
    void writeUint32_t (uint32_t value);

    /**
     * 写入有符号Varint64类型的数据
     * @param value
     */
    void writeint64 (int64_t value);

    /**
     * 写入无符号Varint64类型的数据
     * @param value
     */
    void writeUint64_t (uint64_t value);

    /**
     * 写入float类型的数据
     * @param value
     */
    void writeFloat (float value);

    /**
     * 写入double类型的数据
     * @param value
     */
    void writeDouble (double value);

    /**
     * 写入std::string类型数据，用uint16_t作为长度类型
     * @param value
     */
    void writeStringF16 (const std::string &value);

    /**
     * 写入std::string类型数据，用uint32_T作为长度类型
     * @param value
     */
    void writeStringF32 (const std::string &value);

    /**
     * 写入std::string类型数据,用uint64_t作为长度类型
     * @param value
     */
    void writeStringF64 (const std::string &value);

    /**
     * 写入std:：string类型数据，用无符号Varint64作为长度
     * @param value
     */
    void writeStringVint (const std::string &value);

    /**
     * 写入std::string类型的数据，无长度
     * @param value
     */
    void writeStringWithoutLength (const std::string &value);

    /**
     * 读取int8_t类型的数据
     * @return
     */
    int8_t readint8 ();

    /**
     * 读取uint8类型数据
     * @return
     */
    uint8_t readFuint8 ();

    /**
     * 读取int16类型数据
     * @return
     */
    int16_t readFint16 ();

    /**
     * 读取uint16类型数据
     * @return
     */
    int16_t readFuint16 ();

    /**
     * 读取int32类型数据
     * @return
     */
    int32_t readFint32 ();

    /**
     * 读取uint32类型数据
     * @return
     */
    int32_t readFuint32 ();

    /**
     * 读取int64类型数据
     * @return
     */
    int64_t readFint64 ();

    /**
     * 读取uint64类型数据
     * @return
     */
    int64_t readFuint64 ();

    /**
     * 读取有符号Varint32类型的数据
     * @return
     */
    int32_t readInt32 ();

    /**
     * 读取无符号Varuint32类型的数据
     * @return
     */
    uint32_t readUint32 ();

    /**
     * 读取有符号Varint64类型的数据
     * @return
     */
    int64_t readInt64 ();

    /**
     * 读取无符号Varint64类型的数据
     * @return
     */
    uint64_t readUint64 ();

    /**
     * 读取float类型的数据
     * @return
     */
    float readFloat ();

    /**
     * 读取double类型数据
     * @return
     */
    double readDouble ();

    /**
     * 读取std::string类型的数据，用uint16_t作为长度
     * @return
     */
    std::string readStringF16 ();

    /**
     * 读取std::string类型数据，用uint32_t作为长度
     * @return
     */
    std::string readStringF32 ();

    /**
     * 读取std::string类型的数据，用uint64_t作为长度
     * @return
     */
    std::string readStringF64 ();

    /**
     * 读取std::string类型的数据，用无符号Varint64作为长度
     * @return
     */
    std::string readStringVint ();

    /**
     * 清空byteArray
     */
    void clear ();

    /**
     * 写入size长度数据
     * @param buf
     * @param size
     */
    void write (const void *buf, size_t size);

    /**
     * 读取size长度数据
     * @param buf 内存缓冲指针
     * @param size 数据大小
     */
    void read (void *buf, size_t size);

    /**
     * 读取size长度数据
     * @param buf
     * @param size
     * @param position
     */
    void read(void *buf, size_t size, size_t position) const ;

    /**
     * 返回ByteArray当前位置
     * @return
     */
    size_t getPosition () const { return m_position; }

    /**
     * 设置ByteArray当前位置
     * @param v
     */
    void setPosition (size_t v);

    /**
     * 把byreArray的数据写入文件中
     * @param name
     * @return
     */
    bool writeToFile (const std::string& name) const;

    /**
     * 从文件中读取数据
     * @param name
     * @return
     */
    bool readFromFile (const std::string &name);

    /**
     * 返回内存块大小
     * @return
     */
    size_t getBaseSize () const {return m_baseSize;}

    /**
     * 返回可读数据大小
     * @return
     */
    size_t getReadSize () const { return m_size - m_position; }

    /**
     * 是否为小端
     * @return
     */
    bool isLittleEndian () const;

    /**
     * 设置是否为小端
     * @param val
     */
    void setIsLittleEndian (bool val);

    /**
     * 将ByteArray里面的数据[m_postion, m_siz]转为std::stirng
     * @return
     */
    std::string toString() const ;

    /**
     * 将ByteArray里面的数据[m_position, m_size)转为176进制的std::string(格式：FF FF FF
     * @return
     */
    std::string toHexString () const;

    /**
     * 获取可读取的缓存，保存成iovec数组
     * @param buffers
     * @param len
     * @return
     */
    uint64_t getReadBuffers (std::vector<iovec>& buffers, uint64_t len = ~0ull) const;

    /**
     * 获取可读取的缓存，保存成iovec数组，从position位置开始
     * @param buffers
     * @param len
     * @param position
     * @return
     */
    uint64_t getReadBuffers (std::vector<iovec> & buffers, uint64_t len, uint64_t position) const;

    /**
     * 获取可写入，保存成iovec数组
     * @param buffers
     * @param len
     * @return
     */
    uint64_t getWriteBUffers (std::vector<iovec>& buffers, uint64_t len);

    /**
     * 返回数据的长度
     * @return
     */
    size_t getSize () const { return m_size; }

private:
    /**
     * 扩容ByteArray，使其可以容纳size个数据（如果原本可以容纳，则不扩容）
     * @param size
     */
    void addCapacity (size_t size);

    /**
     * 获取当前可写入容量
     * @return
     */
    size_t getCapacity() const { return m_capacity - m_position; }

private:
    ///内存块大小
    size_t m_baseSize;
    /// 当前操作位置
    size_t m_position;
    /// 当前总容量
    size_t m_capacity;
    /// 当前数据大小
    size_t m_size;
    /// 字节序，默认大端
    int8_t m_endian;
    /// 第一个内存块的指针
    Node* m_root;
    /// 当前操作的内存块指针
    Node* m_cur;

};

}

#endif