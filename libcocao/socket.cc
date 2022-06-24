#include "socket.h"

namespace libcocao {

/**
* 创建TCP Socket(满足地址类型)
* @param address 地址
* @return
*/
Socket::ptr Socket::CreateTCP (libcocao::Address::ptr address){

}

/**
* 创建UDP Socket（满足地址类型）
*/
Socket::ptr Socket::CreateUDP (libcocao::Address::ptr address){

}

/**
* 创建IPv4的TCP Socket
*/
Socket::ptr Socket::CreateTCPSocket (){

}

/**
* 创建IPv4的UDP Socket
* @return
*/
Socket::ptr Socket::CreateUDPSocket (){

}

/**
* 创建IPv6的TCP Socket
*/
Socket::ptr Socket::CreateTCPSocket6 (){

}

/**
* 创建IPv6 的UDP Socket
*/
Socket::ptr Socket::CreateUDPSocket6 (){

}

/**
* 创建Unix的TCP Socket
*/
Socket::ptr Socket::CreateUnixTCPSocket (){

}

/**
* 创建Unix的UDP Socket
*/
Socket::ptr Socket::CreateUnixUDPSocket (){

}

/**
* Socket构造函数
* family 协议族
* type 类型
* protocol 协议
*/
Socket::Socket (int family, int type, int protocol){

}

/**
* 析构函数
*/
Socket::~Socket(){

}

/**
* 获取发送超时时间(毫秒）
* @return
*/
int Socket::getSendTimeout (){

}

/**
* 设置发送超时时间毫秒
* @param v
*/
void Socket::setSendTimeout (int64_t v){

}

/**
 * 获取接受超时时间（毫秒）
 * @return
 */
int64_t Socket::getRecvTimeout (){

}

/**
 * 设置接受超时时间（毫秒）
 * @param v
 */
void Socket::setRecvTimeout (int64_t v){

}

/**
 * 获取socket
 * @param level
 * @param option
 * @param result
 * @param len
 * @return
 */
bool Socket::getOption (int level, int option, void *result, socklen_t *len){

}

/**
 * 设置sockopt @see getsockopt
 */
bool Socket::setOption (int level, int option, const void *result, socklen_t len){

}

/**
 *
 * @return 成功返回新链接的socket，失败返回nullptr
 * Socket必须bind， listen成功
 */
Socket::ptr Socket::accept (){

}

/**
 * 绑定地址
 * @param addr 地址
 * @return
 */
bool Socket::bind (const Address::ptr addr){

}

/**
 * 连接地址
 * @param addr 目标地址
 * @param timeout_ms 超时时间（毫秒）
 * @return
 */
bool Socket::connect (const Address::ptr addr, uint64_t timeout_ms = -1){

}
bool Socket::reconnect (uint64_t timeout_ms = -1){

}

/**
 * 监听socket
 * backlog 未完成连接的最大长度
 * 返回监听是否成功
 * 必须先bind成功
 */
bool Socket::listen (int backlog = SOMAXCONN){

}

/**
 * 关闭socket
 * @return
 */
bool Socket::close (){

}

/**
 * 发送数据
 * @param buffer 待发送数据的内存
 * @param length 待发送数据的长度
 * @param flags 标志字
 * @return
 *  > 0 发送成功对应的大小的数据
 *  socket被关闭
 *  < 0 socket出错
 */
int Socket::send (const void *buffer, size_t length, int flags){

}

/**
 * 发送数据
 * @param buffers 待发送数据内存
 * @param length 待发送数据长度
 * @param flags 标志字
 * @return
 *      > 0 发送成功对应大小的数据
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::send (const iovec *buffers, size_t length, int flags){

}

/**
 * 发送数据
 * @param buffer 待发送数据的内存
 * @param length 待发送数据的长度的
 * @param to 发送的目标地址
 * @param flags 标志字
 * @return
 *      > 0 发送成功对应大小的数据
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::sendTo (const void *buffer, size_t length, const Address::ptr to, int flags){

}

/**
 * 发送数据
 * @param buffers 待发送数据的内存 （iovec数组）
 * @param length 带发送数据的长度
 * @param to 发送的目标地址
 * @param flags 标志字
 * @return
 *      > 0 发送成功对应大小的数据
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::sendTo (const iovec *buffers, size_t length, const Address::ptr to, int flags){

}

/**
 * 接受数据
 * @param buffer 接受数据的内存
 * @param length 接受数据内存的大小j
 * @param flags 标志字
 * @return
 *      > 0 接受到对应大小的数据
 *      socket关闭
 *      < 0 socket出错
 */
int Socket::recv (void *buffer, size_t length, int flags){

}

/**
 * 接受数据
 * @param buffers 接受数据的内存
 * @param length 接受数据的内存大小（iovec数组长度）
 * @param flags flags 标志字
 * @return
 *      > 0 接受对应大小的数据
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::recv (iovec *buffers, size_t length, int flags){

}

/**
 * 接受数据
 * @param buffer 接受数据的内存
 * @param length 接受数据内存的大小
 * @param from 发送端地址
 * @param flags 标志字
 * @return
 *      > 0 接受到数据的大小
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::recvFrom (void *buffer, size_t length, Address::ptr from, int flags){

}

/**
 * 接受数据
 * @param buffers 接受数据内存（iovec数组)
 * @param length 接受数据的内存大小（iovec数组长度）
 * @param from 发送端地址
 * @param flags 标志字
 * @return
 *      > 0 接受到对应数据的大小
 *      socket被关闭
 *      < 0 socket出错
 */
int Socket::recvFrom (iovec *buffers, size_t length, Address::ptr from, int flags){

}

/**
 * 获取远端地址
 * @return
 */
Address::ptr Socket::getRemoteAddress (){

}

/**
 * 获取本地地址
 * @return
 */
Address::ptr Socket::getLocalAddress (){

}

/**
 * 是否有效（m_sock != 1)
 * @return
 */
bool Socket::isValid() const{

}

/**
 * 返回Socket错误
 * @return
 */
int Socket::getError (){

}

/**
 * 输出信息到流中
 * @param os
 * @return
 */
std::ostream &Socket::dunmp (std::ostream &os) const {

}

std::string Socket::toString () const{

}

/**
 * 取消读
 * @return
 */
bool Socket::cancelRead (){

}

/**
 * 取消写
 * @return
 */
bool Socket::cancelWrite(){

}

/**
 * 取消accept
 * @return
 */
bool Socket::cancelAccept (){

}

/**
 * 取消所有事件
 * @return
 */
bool Socket::cancelAll (){

}


/**
 * 初始化socket
 */
void Socket::initSock (){

}

/**
 * 创建socket
 */
void Socket::newSock(){

}

/**
 * 初始化sock
 * @param sock
 * @return
 */
bool Socket::init (int sock){

}

}