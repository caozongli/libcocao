#include "bytearray.h"
#include "endian.h"
#include "log.h"

namespace libcocao {

static libcocao::Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");

ByteArray::Node::Node()
    : ptr (nullptr)
    , next (nullptr)
    , size (0){
}

ByteArray::Node::Node(size_t s)
    : ptr (nullptr)
    , next (nullptr)
    , size (s){
}

ByteArray::Node::~Node() {
    if (ptr)
        delete [] ptr;
}

/**
 * 使用指定长度的内存块构造ByteArray
 * base_size 内存块大小
 */
ByteArray::ByteArray (size_t base_size){

}

/**
 * 析构函数`
 */
ByteArray::~ByteArray(){

}

/**
 * 写入固定长度int8_t类型数据
 * m_position += sizeof (value)
 * 如果m_position > m_size 则m_size = m_ppsition
 * @param value
 */
void ByteArray::writeFint8 (int8_t value){

}

/**
 * 写入固定长度uint8_t类型的数据
 * @param value
 */
void ByteArray::writeFuint8 (uint8_t value){

}

/**
 * 写入固定长度int16_t数据
 * @param value
 */
void ByteArray::writeFint16 (int16_t value){

}

/**
 * 写入固定长度uint16_t类型的数据（大端/小端)
 * @param value
 */
void ByteArray::writeFuint16 (uint16_t value){

}

/**
 * 写入固定长度int32_t数据
 * @param value
 */
void ByteArray::writeFint32 (int32_t value){

}

/**
 * 写入固定长度uint32_t类型的数据（大端/小端)
 * @param value
 */
void ByteArray::writeFuint32 (uint32_t value){

}

/**
 * 写入固定长度int16_t数据
 * @param value
 */
void ByteArray::writeFint64 (int64_t value){

}

/**
 * 写入固定长度uint16_t类型的数据（大端/小端)
 * @param value
 */
void ByteArray::writeFuint64 (uint64_t value){

}

/**
 * 写入有符号Varint32类型的数据
 * @param value
 */
void ByteArray::writeint32 (int32_t value){

}

/**
 * 写入无符号Varint32类型的数据
 * @param value
 */
void ByteArray::writeUint32_t (uint32_t value){

}

/**
 * 写入有符号Varint64类型的数据
 * @param value
 */
void ByteArray::writeint64 (int64_t value){

}

/**
 * 写入无符号Varint64类型的数据
 * @param value
 */
void ByteArray::writeUint64_t (uint64_t value){

}

/**
 * 写入float类型的数据
 * @param value
 */
void ByteArray::writeFloat (float value){

}

/**
 * 写入double类型的数据
 * @param value
 */
void ByteArray::writeDouble (double value){

}

/**
 * 写入std::string类型数据，用uint16_t作为长度类型
 * @param value
 */
void ByteArray::writeStringF16 (const std::string &value){

}

/**
 * 写入std::string类型数据，用uint32_T作为长度类型
 * @param value
 */
void ByteArray::writeStringF32 (const std::string &value){

}

/**
 * 写入std::string类型数据,用uint64_t作为长度类型
 * @param value
 */
void ByteArray::writeStringF64 (const std::string &value){

}

/**
 * 写入std:：string类型数据，用无符号Varint64作为长度
 * @param value
 */
void ByteArray::writeStringVint (const std::string &value){

}

/**
 * 写入std::string类型的数据，无长度
 * @param value
 */
void ByteArray::writeStringWithoutLength (const std::string &value){

}

/**
 * 读取int8_t类型的数据
 * @return
 */
int8_t ByteArray::readint8 (){

}

/**
 * 读取uint8类型数据
 * @return
 */
uint8_t ByteArray::readuint8 (){

}

/**
 * 读取int16类型数据
 * @return
 */
int16_t ByteArray::readFint16 (){

}

/**
 * 读取uint16类型数据
 * @return
 */
int16_t ByteArray::readFuint16 (){

}

/**
 * 读取int32类型数据
 * @return
 */
int32_t ByteArray::readFint32 (){

}

/**
 * 读取uint32类型数据
 * @return
 */
int32_t ByteArray::readFuint32 (){

}

/**
 * 读取int64类型数据
 * @return
 */
int64_t ByteArray::readFint64 (){

}

/**
 * 读取uint64类型数据
 * @return
 */
int64_t ByteArray::readFuint64 (){

}

/**
 * 读取有符号Varint32类型的数据
 * @return
 */
int32_t ByteArray::readInt32 (){

}

/**
 * 读取无符号Varuint32类型的数据
 * @return
 */
uint32_t ByteArray::readUint32 (){

}

/**
 * 读取有符号Varint64类型的数据
 * @return
 */
int64_t ByteArray::readInt64 (){

}

/**
 * 读取无符号Varint64类型的数据
 * @return
 */
uint64_t ByteArray::readUint64 (){

}

/**
 * 读取float类型的数据
 * @return
 */
float ByteArray::readFloat (){

}

/**
 * 读取double类型数据
 * @return
 */
double ByteArray::readDouble (){

}

/**
 * 读取std::string类型的数据，用uint16_t作为长度
 * @return
 */
std::string ByteArray::readStringF16 (){

}

/**
 * 读取std::string类型数据，用uint32_t作为长度
 * @return
 */
std::string ByteArray::readStringF32 (){

}

/**
 * 读取std::string类型的数据，用uint64_t作为长度
 * @return
 */
std::string ByteArray::readStringF64 (){

}

/**
 * 读取std::string类型的数据，用无符号Varint64作为长度
 * @return
 */
std::string ByteArray::readStringVint (){

}

/**
 * 清空byteArray
 */
void ByteArray::clear (){

}

/**
 * 写入size长度数据
 * @param buf
 * @param size
 */
void ByteArray::write (const void *buf, size_t size){

}

/**
 * 读取size长度数据
 * @param buf 内存缓冲指针
 * @param size 数据大小
 */
void ByteArray::read (void *buf, size_t size){

}

/**
 * 读取size长度数据
 * @param buf
 * @param size
 * @param position
 */
void ByteArray::read(void *buf, size_t size, size_t position) const {

}

/**
 * 设置ByteArray当前位置
 * @param v
 */
void ByteArray::setPosition (size_t v){

}

/**
 * 把byreArray的数据写入文件中
 * @param name
 * @return
 */
bool ByteArray::writeToFile (const std::string& name) const{

}

/**
 * 从文件中读取数据
 * @param name
 * @return
 */
bool ByteArray::readFromFile (const std::string &name){

}

/**
 * 是否为小端
 * @return
 */
bool ByteArray::isLittleEndian () const{

}

/**
 * 设置是否为小端
 * @param val
 */
void ByteArray::setIsLittleEndian (bool ByteArray::val){

}

/**
 * 将ByteArray里面的数据[m_postion, m_siz]转为std::stirng
 * @return
 */
std::string ByteArray::toString() const {

}

/**
 * 将ByteArray里面的数据[m_position, m_size)转为176进制的std::string(格式：FF FF FF
 * @return
 */
std::string ByteArray::toHexString () const{

}

/**
 * 获取可读取的缓存，保存成iovec数组
 * @param buffers
 * @param len
 * @return
 */
uint64_t ByteArray::getReadBuffers (std::vector<iovec>& buffers, uint64_t len) const{

}

/**
 * 获取可读取的缓存，保存成iovec数组，从position位置开始
 * @param buffers
 * @param len
 * @param position
 * @return
 */
uint64_t ByteArray::getReadBuffers (std::vector<iovec> & buffers, uint64_t len, uint64_t position) const{

}

/**
 * 获取可写入，保存成iovec数组
 * @param buffers
 * @param len
 * @return
 */
uint64_t ByteArray::getWriteBUffers (std::vector<iovec>& buffers, uint64_t len){

}

}