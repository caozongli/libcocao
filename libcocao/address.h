#ifndef __LIBCOCAO_ADDRESS_H__
#define __LIBCOCAO_ADDRESS_H__
#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <vector>
#include <map>

namespace libcocao {

class IPAddress;

class Address {
public:
    typedef std::shared_ptr<Address> ptr;

    /**
     * 通过sockaddr指针创建Address
     * addr sockaddr指针
     * addrlen sockaddr长度
     * 返回和sockaddr相匹配的Address，失败返回nullptr
     * */
    static Address::ptr Create (const sockaddr *addr, socklen_t addrlen);

    /**
     * 通过host地址返回对应条件的所有Addresss
     * result保存满足条件的Addresss
     * host 域名，服务器名等，举例：www.caozong.top[:80]
     * family 协议族（AF_INET，AF_INET6，AF_UNIX）
     * type socketl类型SOCK_STRESM，SOCK_DGRAM等
     * protocol 协议，IPPROTO_TCP/IPPROTO_UDP等
     * */
    static bool Lookup (std::vector<Address::ptr>& result, const std::string &host,
                        int family = AF_INET, int type = 0, int protocol = 0);

    /**
     * 通过host返回对应条件的任意Address
     * 返回满足条件的任意Address，失败返回nullptr
     * */
    static Address::ptr LookupAny (const std::string &host,
                                   int family = AF_INET, int type = 0, int protocol = 0);

    /**
     * 通过host地址返回对应条件的任意IPAddress
     * 返回满足条件的任意IPAddress，失败返回nullptr
     * */
    static std::shared_ptr<IPAddress> LookupAnyIPAddress (const std::string &host,
                                                          int family = AF_INET, int type = 0, int protocol = 0);

    /**
     * 返回本机所有网卡的<网卡名，地址，子网掩码位数>
     * result保存本机所有地址
     * family 协议（AF_INET, AF_INET6, AF_UNIX）
     * 是否获取成功
     * */
    static bool GetInterfaceAddresses (std::multimap<std::string, std::pair<Address::ptr, uint32_t>> &result,
                                     int family = AF_INET);

    /**
     * 指定获取网卡的地址和子网掩码位数
     * result 保存指定网卡所有地址
     * iface 网卡名称
     * family 协议族(AF_INT, AF_INT6, AF_UNIX);
     * 是否获取成功
     * */
    static bool GetInterfaceAddresses(std::vector<std::pair<Address::ptr, uint32_t>>& result, const std::string &iface, int family = AF_INET);

    /**
     * 虚析构函数
     * */
    virtual ~Address() {}

    /**
     * 返回协议族
     * */
    int getFamily () const;

    /**
     * 返回sockaddr指针
     * */
    virtual const sockaddr *getAddr() const = 0;

    /**
     * @return 返回sockaddr指针，读写
     */
    virtual sockaddr *getAddr() = 0;

    /**
     * @return  返回sockaddr长度
     */
    virtual socklen_t getAddrLen() const = 0;

    /**
     * @param os
     * @return 可读性输出地址
     */
    virtual std::ostream &insert(std::ostream &os) const = 0;

    /**
     * @return 返回可读性字符串
     */
    std::string toString () const;

    /**
     * 小于号比较函数
     * @param rhs
     */
    bool operator<(const Address &rhs) const;

    /**
     * 等于函数
     * @param rhs
     * @return
     */
    bool operator==(const Address &rhs) const;

    /**
     * 不等于函数
     */
     bool operator!=(const Address &rhs) const;
};

class IPAddress:public Address {
public:
    typedef std::shared_ptr<IPAddress> ptr;

    /**
     * 通过域名，IP，服务器名创建IPAddress
     * @param address 域名，IP，服务器名等，www.caozong.top
     * @param port 端口号
     * @return 调用成功返回IPAddress，失败返回nullptr
     */
    static IPAddress::ptr Create (const char *address, uint16_t port = 0);

    /**
     * 获取该地址的广播地址
     * prefix_len 子网掩码数
     * @return 调用成功返回IPAddress，失败返回nullptr
     */
    virtual IPAddress::ptr broadcastAddress (uint32_t prefix_len) = 0;

    /**
     * 获取改地址网段
     * prefix_len 子网掩码数
     * @return 调用成功返回IPAddress，失败返回nullptr
     */
    virtual IPAddress::ptr networkAddress (uint32_t prefix_len) = 0;

    /**
     * 获取子网掩码地址
     * prefix_len 子网掩码位数
     * 调用成功返回IPAddress，失败返回nullptr
     */
    virtual IPAddress::ptr subnetMask(uint32_t prefix_len) = 0;

    /**
     * 返回端口号
     */
    virtual uint32_t getPort() const = 0;

    /**
     * 设置端口号
     * @param v
     */
    virtual void setPort(uint16_t v) = 0;
};

class IPv4Address: public IPAddress {
public:
    typedef std::shared_ptr<IPv4Address> ptr;

    /**
     * 使用点分十进制地址创建IPv4Address
     * address 点分十进制，如：192.168.1.1
     * port 端口号
     * 返回IPv4Address，失败返回nullptr
     * */
    static IPv4Address::ptr Create (const char* address, uint16_t port = 0);

    /**
     * 通过sockaddr_in构造IPv4Address
     * @param address sockaddr_in结构体
     */
    IPv4Address(const sockaddr_in &address);

    /**
     * 通过二进制地址构造IPv4Address
     * @param address 二进制地址address
     * @param port 端口号
     */
    IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

    const sockaddr *getAddr() const override;
    sockaddr *getAddr() override;
    socklen_t getAddrLen() const override;
    std::ostream &insert(std::ostream &os) const override;

    IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networkAddress (uint32_t prefix_len) override;
    IPAddress::ptr subnetMask (uint32_t prefix_len) override;
    uint32_t getPort() const override;
    void setPort(uint16_t v) override;

private:
    sockaddr_in m_addr;
};

class IPv6Address : public IPAddress {
public:
    typedef std::shared_ptr<IPv6Address> ptr;

    /**
     * 通过ipv6地址字符串构造IPv6Address
     * address Ipv
     * port端口号
     */
    static IPv6Address::ptr Create(const char* address, uint16_t port = 0);

    /**
     * 无参构造函数
     */
    IPv6Address();

    /**
     * 通过sockaddr_in6构造IPv6Address
     * @param address address sockaddr_in6结构体
     */
    IPv6Address(const sockaddr_in6 &address);

    /**
     * 通过Ipv6二进制地址构造IPv6Address
     * address IPv6的二进制地址
     */
     IPv6Address (const uint8_t address[16], uint16_t port = 0);

     const sockaddr *getAddr () const override;
     sockaddr *getAddr () override;
     socklen_t getAddrLen () const override;
     std::ostream &insert (std::ostream &os) const override;

     IPAddress::ptr broadcastAddress (uint32_t prefix_len) override;
     IPAddress::ptr networkAddress (uint32_t prefix_len) override;
     IPAddress::ptr subnetMask (uint32_t prefix_len) override;
     uint32_t getPort() const override;
     void setPort(uint16_t v) override;

private:
    sockaddr_in6 m_addr;
};

class UnixAddress : public Address {
public:
    typedef std::shared_ptr<UnixAddress> ptr;

    /**
     * 无参构造函数
     */
    UnixAddress();

    /**
     * 通过路径构造UnixAddress
     * @param path UnixSocket路径（长度小于UNIX_PATH_MAX)
     */
    UnixAddress (const std::string &path);

    const sockaddr *getAddr () const override;
    sockaddr *getAddr () override;
    socklen_t getAddrLen() const override;
    void setAddrlen(uint32_t v);
    std::string getPath() const;
    std::ostream &insert(std::ostream &os) const override;

private:
    sockaddr_un m_addr;
    socklen_t m_length;
};

class UnknowAddress : public Address {
public:
    typedef std::shared_ptr<UnknowAddress> ptr;
    UnknowAddress(int family);
    UnknowAddress(const sockaddr &addr);
    const sockaddr *getAddr () const override;
    sockaddr *getAddr () override;
    socklen_t getAddrLen () const override;
    std::ostream &insert (std::ostream &os) const override;

private:
    sockaddr m_addr;
};

std::ostream &operator<<(std::ostream &os, const Address &addr);
}

#endif