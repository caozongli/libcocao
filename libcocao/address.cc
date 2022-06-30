#include <netdb.h>
#include <ifaddrs.h>
#include "address.h"
#include "log.h"
#include "endian.h"

namespace libcocao {

static libcocao::Logger::ptr g_logger = LIBCOCAO_LOG_NAME("system");

template <class T>
static T CreateMask (uint32_t bits) {
    return (1 << (sizeof (T) * 8 - bits)) - 1;
}

template <class T>
static uint32_t CountBytes (T value) {
    uint32_t result = 0;
    for (; value; ++ result) {
        value &= value - 1
    }
    return result;
}

/**
 * 通过sockaddr指针创建Address
 * addr sockaddr指针
 * addrlen sockaddr长度
 * 返回和sockaddr相匹配的Address，失败返回nullptr
 * */
Address::ptr Address::Create (const sockaddr *addr, socklen_t addrlen){
    if (addr == nullptr) return nullptr;

    Address::ptr result;
    switch (addr->sa_family) {
        case AF_INET:
            result.reset(new IPv4Address(*(const sockaddr_in *) addr));
            break;
        case AF_INET6:
            result.reset(new IPv6Address(*(const sockaddr_in6 *) addr));
            break;
        default:
            result.reset(new UnknowAddress (*addr));
            break;
    }
    return result;
}

/**
 * 通过host地址返回对应条件的所有Addresss
 * result保存满足条件的Addresss
 * host 域名，服务器名等，举例：www.caozong.top[:80]
 * family 协议族（AF_INET，AF_INET6，AF_UNIX）
 * type socketl类型SOCK_STRESM，SOCK_DGRAM等
 * protocol 协议，IPPROTO_TCP/IPPROTO_UDP等
 * */
bool Address::Lookup (std::vector<Address::ptr>& result, const std::string &host,
                    int family, int type, int protocol){
    addrinfo hints, *results, *next;
    hints.ai_flags = 0;
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    std::string node;
    const char *service = NULL;

    //检查node service
    if (node.empty()) {
        service = (const char *)memchr(host.c_str(), ':', host.size());
        if (service) {
            if (!memchr(service + 1, ':', host.c_str() + host.size() - service - 1)) {
                node = host.substr(0, service - host.c_str());
                ++ service;
            }
        }
    }

    if (node.empty()) node = host;
    int error = getaddrinfo(node.c_str(), service, &hints, &results);
    if (error) {
        LIBCOCAO_LOG_DEBUG(g_logger) << "Address::Lookup getaddress(" << host << ", "
                                        << family << ", " << type << ") err=" << error << " errstr="
                                        << gai_strerror(error);
        return false;
    }

    next = results;
    while (next) {
        result.push_back(Create(next->ai_addr, (socklen_t)next->ai_addrlen));
        LIBCOCAO_LOG_DEBUG(g_logger) << "family:" << next->ai_family << ", sock_type:" << next->ai_socktype;
        next = next->ai_next;
    }

    freeaddrinfo(results);
    return !result.empty();
}

/**
 * 通过host返回对应条件的任意Address
 * 返回满足条件的任意Address，失败返回nullptr
 * */
Address::ptr Address::LookupAny (const std::string &host,
                               int family, int type, int protocol){
    std::vector<Address::ptr> result;
    if (Lookup(result, host, family, type, protocol)) {
        return result[0];
    }
    return nullptr;
}

/**
 * 通过host地址返回对应条件的任意IPAddress
 * 返回满足条件的任意IPAddress，失败返回nullptr
 * */
std::shared_ptr<IPAddress> Address::LookupAnyIPAddress (const std::string &host,
                                                      int family, int type, int protocol){
    std::vector<Address::ptr> result;
    if (Lookup(result, host, family, type, protocol)) {
        for (auto &i: result) {
            IPAddress::ptr v = std::dynamic_pointer_cast<IPAddress>(i);
            if (v) return v;
        }
    }
    return nullptr;
}

/**
 * 返回本机所有网卡的<网卡名，地址，子网掩码位数>
 * result保存本机所有地址
 * family 协议（AF_INET, AF_INET6, AF_UNIX）
 * 是否获取成功
 * */
bool Address::GetInterfaceAddresses (std::multimap<std::string, std::pair<Address::ptr, uint32_t>> &result,
                                   int family){
    struct ifaddrs *next, *results;
    if (getifaddrs(&results) != 0) {
        LIBCOCAO_LOG_DEBUG(g_logger) << "Address::GetInterfaceAddresses getifassrs err = "
                                        << errno << " errstr=" << strerror (errno);
        return false;
    }

    try {
        for (next = results; next; next = next->ifa_next) {
            Address::ptr addr;
            uint32_t prefix_len = ~0u;
            if (family != AF_UNSPEC && family != next->ifa_addr->sa_family) continue;
            switch (next->ifa_addr->sa_family) {
                case AF_INET: {
                    addr = Create(next->ifa_addr, sizeof (sockaddr_in));
                    uint32_t netmask = ((sockaddr_in *)next->ifa_netmask)->sin_addr.s_addr;
                    prefix_len = CountBytes(netmask);
                }break;
                case AF_INET6: {
                    addr = Create(next->ifa_addr, sizeof (sockaddr_in6));
                    in6_addr &netmask = ((sockaddr_in6 *)next->ifa_netmask)->sin6_addr;
                    prefix_len = 0;
                    for (int i = 0; i < 16; ++ i) prefix_len += CountBytes(netmask.s6_addr[i]);
                } break;
                default:
                    break;
            }
            if (addr) {
                result.insert(std::make_pair(next->ifa_name, std::make_pair(addr, prefix_len)));
            }
        }
    } catch (...) {
        LIBCOCAO_LOG_ERROR(g_logger) << "Address::GetInterfaceAddresses exception";
        freeifaddrs(results);
        return false;
    }
    freeifaddrs(results);
    return !result.empty();
}

/**
 * 指定获取网卡的地址和子网掩码位数
 * result 保存指定网卡所有地址
 * iface 网卡名称
 * family 协议族(AF_INT, AF_INT6, AF_UNIX){

}
 * 是否获取成功
 * */
bool Address::GetInterfaceAddresses(std::vector<std::pair<Address::ptr, uint32_t>>& result, const std::string &iface, int family){
    if (iface.empty() || iface == "*") {
        if (family == AF_INET || family == AF_UNSPEC) {
            result.push_back(std::make_pair(Address::ptr(new IPv4Address()), 0u));
        }
        if(family == AF_INET6 || family == AF_UNSPEC) {
            result.push_back(std::make_pair(Address::ptr(new IPv6Address()), 0u));
        }
    }

    std::multimap<std::string, std::pair<Address::ptr, uint32_t>> results;

    if (!GetInterfaceAddresses (results, family)) return false;

    auto its = results.equal_range(iface);
    for (; its.first != its.second; ++ its.first) result.push_back(its.first->second);
    return !result.empty();
}

/**
 * 返回协议族
 * */
int Address::getFamily () const{
    return getAddr()->sa_family;
}

/**
 * @return 返回可读性字符串
 */
std::string Address::toString () const{
    std::stringstream ss;
    insert(ss);
    return ss.str();
}

/**
 * 小于号比较函数
 * @param rhs
 */
bool Address::operator<(const Address &rhs) const{
    socklen_t minlen = std::min(getAddrLen(), rhs.getAddrLen());
    int result = memcmp(getAddr(), rhs.getAddr(), minlen);
    if (result < 0) {
        return true;
    } else if (result > 0) {
        return false;
    } else if (getAddrLen() < rhs.getAddrLen()) {
        return true;
    }
    return false;
}

/**
 * 等于函数
 * @param rhs
 * @return
 */
bool Address::operator==(const Address &rhs) const{
    return getAddrLen() == rhs.getAddrLen() && memcmp(getAddr(), rhs.getAddr(), getAddrLen()) == 0;
}

/**
 * 不等于函数
 */
bool Address::operator!=(const Address &rhs) const{
    return !(*this == rhs);
}

/**
 * 通过域名，IP，服务器名创建IPAddress
 * @param address 域名，IP，服务器名等，www.caozong.top
 * @param port 端口号
 * @return 调用成功返回IPAddress，失败返回nullptr
 */
IPAddress::ptr IPAddress::Create (const char *address, uint16_t port){
    addrinfo hints, *results;
    memset (&hints, 0, sizeof (addrinfo));

    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_family = AF_UNSPEC;

    int error = getaddrinfo(address, NULL, &hints, &results);
    if (error) {
        LIBCOCAO_LOG_DEBUG(g_logger) << "IPAddresss:Create(" << address
                                        << ", " << port << ") error=" << error
                                        << " errno=" << errno << " errstr=" << strerror(errno);
        return nullptr;
    }

    try {
        IPAddress::ptr result = std::dynamic_pointer_cast<IPAddress>(Address::Create(results->ai_addr, (socklen_t)results->ai_addrlen));
        if (result) result->setPort(port);
        freeaddrinfo(results);
        return result;
    } catch (...) {
        freeaddrinfo(results);
        return nullptr;
    }
}

/**
 * 使用点分十进制地址创建IPv4Address
 * address 点分十进制，如：192.168.1.1
 * port 端口号
 * 返回IPv4Address，失败返回nullptr
 * */
IPv4Address::ptr IPv4Address::Create (const char* address, uint16_t port){
    IPv4Address::ptr rt(new IPv4Address);
    rt->m_addr.sin_port = byteswapOnLittleEndian(port);
    int result = inet_pton(AF_INET, address, &rt->m_addr.sin_addr);
    if (result <= 0) {
        LIBCOCAO_LOG_DEBUG(g_logger) << "IPv4Address::Create(" << address << ", "
                                        << port << ") rt=" << result << " errno=" << errno
                                        << " errstr=" << strerror (errno);
        return nullptr;
    }
    return rt;
}

/**
 * 通过sockaddr_in构造IPv4Address
 * @param address sockaddr_in结构体
 */
IPv4Address::IPv4Address(const sockaddr_in &address){
    m_addr = address;
}

/**
 * 通过二进制地址构造IPv4Address
 * @param address 二进制地址address
 * @param port 端口号
 */
IPv4Address::IPv4Address(uint32_t address, uint16_t port){
        memset(&m_addr, 0, sizeof (m_addr));
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = byteswapOnLittleEndian(port);
        m_addr.sin_addr.s_addr = byteswapOnLittleEndian(address);
}

const sockaddr* IPv4Address::getAddr() const{
    return (sockaddr *)&m_addr;
}

sockaddr* IPv4Address::getAddr(){
    return (sockaddr *)&m_addr;
}

socklen_t IPv4Address::getAddrLen() const{
    return sizeof m_addr;
}

std::ostream& IPv4Address::insert(std::ostream &os) const{
    uint32_t addr = byteswapOnLittleEndian(m_addr.sin_addr.s_addr);
    os << ((addr >> 24) & 0xff) << "."
        << ((addr >> 16) & 0xff) << "."
        << ((addr >> 8) & 0xff) << "."
        << (addr & 0xff);
    os << ":" << byteswapOnLittleEndian(m_addr.sin_port);
    return os;
}

IPAddress::ptr IPv4Address::broadcastAddress (uint32_t prefix_len){
    if (prefix_len > 32) return nullptr;

    sockaddr_in baddr (m_addr);
    baddr.sin_addr.s_addr |= byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(baddr));
}

IPAddress::ptr IPv4Address::networkAddress (uint32_t prefix_len){
    if (prefix_len > 32) return nullptr;

    sockaddr_in baddr (m_addr);
    baddr.sin_addr.s_addr &= byteswapOnLittleEndian(~CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr(new IPv4Address(baddr));
}

IPAddress::ptr IPv4Address::subnetMask (uint32_t prefix_len){
    sockaddr_in subnet;
    memset (&subnet, 0, sizeof subnet);
    subnet.sin_family = AF_INET;
    subnet.sin_addr.s_addr = ~byteswapOnLittleEndian(CreateMask<uint32_t>(prefix_len));
    return IPv4Address::ptr (new IPv4Address(subnet));
}

uint32_t IPv4Address::getPort() const{
    return byteswapOnLittleEndian(m_addr.sin_port);
}

void IPv4Address::setPort(uint16_t v){
    m_addr.sin_port = byteswapOnLittleEndian(v);
}

/**
 * 通过ipv6地址字符串构造IPv6Address
 * address Ipv
 * port端口号
 */
IPv6Address::ptr IPv6Address::Create(const char* address, uint16_t port){
    IPv6Address::ptr rt(new IPv6Address);
    rt->m_addr.sin6_port = byteswapOnLittleEndian(port);
    int result = inet_pton(AF_INET6, address, &rt->m_addr.sin6_addr);
    if (result <= 0) {
        LIBCOCAO_LOG_DEBUG(g_logger) << "IPv6Address::Create(" << address << ", "
                                        << port << ") rt=" << result << " errno=" << errno
                                        << " errstr=" << strerror(errno);
        return nullptr;
    }
    return rt;
}

/**
 * 无参构造函数
 */
IPv6Address::IPv6Address(){
    memset (&m_addr, 0, sizeof (m_addr));
    m_addr.sin6_family = AF_INET6;
}

/**
 * 通过sockaddr_in6构造IPv6Address
 * @param address address sockaddr_in6结构体
 */
IPv6Address::IPv6Address(const sockaddr_in6 &address){
    m_addr = address;
}

/**
 * 通过Ipv6二进制地址构造IPv6Address
 * address IPv6的二进制地址
 */
IPv6Address::IPv6Address (const uint8_t address[16], uint16_t port){
    memset(&m_addr, 0, sizeof m_addr);
    m_addr.sin6_family = AF_INET6;
    m_addr.sin6_port = byteswapOnLittleEndian(port);
    memcpy (&m_addr.sin6_addr, address, 16);
}

const sockaddr* IPv6Address::getAddr () const{
    return (sockaddr*)&m_addr;
}

sockaddr* IPv6Address::getAddr (){
    return (sockaddr*)&m_addr;
}

socklen_t IPv6Address::getAddrLen () const{
    return sizeof (m_addr);
}

std::ostream& IPv6Address::insert (std::ostream &os) const{
    os << "[";
    uint16_t *addr = (uint16_t*)m_addr.sin6_addr.s6_addr;
    bool used_zeros = false;
    for (size_t i = 0; i < 8; ++ i) {
        if (addr[i] == 0 && !used_zeros) continue;

        if(i && addr[i - 1] == 0 && !used_zeros) {
            os << ":";
            used_zeros = true;
        }
        if (i) {
            os << ":";
        }
        os << std::hex << (int)byteswapOnLittleEndian(addr[i]) << std::dec;
    }
    if (!used_zeros && addr[7] == 0) os << "::";
    os << "]:" << byteswapOnLittleEndian(m_addr.sin6_port);
    return os;
}

IPAddress::ptr IPv6Address::broadcastAddress (uint32_t prefix_len){
    sockaddr_in6 baddr(m_addr);
    baddr.sin6_addr.s6_addr[prefix_len / 8] |= CreateMask<uint8_t>(prefix_len % 8);
    for (int i = prefix_len / 8 + 1; i < 16; ++ i)
        baddr.sin6_addr.s6_addr[i] = 0xff;
    return IPv6Address::ptr (new IPv6Address(baddr));
}

IPAddress::ptr IPv6Address::networkAddress (uint32_t prefix_len){
    sockaddr_in6 baddr(m_addr);
    baddr.sin6_addr.s6_addr[prefix_len / 8] &= CreateMask<uint8_t>(prefix_len % 8);
    for (int i = prefix_len / 8 + 1; i < 16; ++ i) {
        baddr.sin6_addr.s6_addr[i] = 0x00;
    }
    return IPv6Address::ptr(new IPv6Address(baddr));
}

IPAddress::ptr IPv6Address::subnetMask (uint32_t prefix_len){
    sockaddr_in6 subnet;
    memset (&subnet, 0, sizeof subnet);
    subnet.sin6_family = AF_INET6;
    subnet.sin6_addr.s6_addr[prefix_len / 8] = ~CreateMask<uint8_t>(prefix_len % 8);

    for (uint32_t i = 0; i < prefix_len / 8; ++ i)
        subnet.sin6_addr.s6_addr[i] = 0xff;

    return IPv6Address::ptr (new IPv6Address(subnet));
}

uint32_t IPv6Address::getPort() const{
    return byteswapOnLittleEndian(m_addr.sin6_port);
}

void IPv6Address::setPort(uint16_t v){
    m_addr.sin6_port = byteswapOnLittleEndian(v);
}

static const size_t MAX_PATH_LEN = sizeof (((sockaddr_un *)0)->sun_path) - 1;

/**
 * 无参构造函数
 */
UnixAddress::UnixAddress(){
    memset (&m_addr, 0, sizeof m_addr);
    m_addr.sun_family = AF_UNIX;
    m_length = offsetof (sockaddr_un, sun_path) + MAX_PATH_LEN;
}

/**
 * 通过路径构造UnixAddress
 * @param path UnixSocket路径（长度小于UNIX_PATH_MAX)
 */
UnixAddress::UnixAddress (const std::string &path){
    memset (&m_addr, 0, sizeof m_addr);
    m_addr.sun_family = AF_UNIX;
    m_length = path.size() + 1;

    if (!path.empty() && path[0] == '\0') --m_length;

    if (m_length > sizeof m_addr.sun_path)
        throw std::logic_error("path too long");

    memcpy(m_addr.sun_path, path.c_str(), m_length);
    m_length += offsetof(sockaddr_un, sun_path);
}

const sockaddr *UnixAddress::getAddr () const{
    return (sockaddr*)&m_addr;
}

sockaddr* UnixAddress::getAddr (){
    return (sockaddr*)&m_addr;
}

socklen_t UnixAddress::getAddrLen() const{
    return m_length;
}

void UnixAddress::setAddrlen(uint32_t v) {
    m_length = v;
}

std::string UnixAddress::getPath() const{
    std::stringstream ss;
    if (m_length > offsetof (sockaddr_un, sun_path) && m_addr.sun_path[0] == '\0') {
        ss << "\\0" << std::string (m_addr.sun_path + 1, m_length - offsetof (sockaddr_un, sun_path) - 1);
    } else {
        ss << m_addr.sun_path;
    }
    return ss.str();
}

std::ostream& UnixAddress::insert(std::ostream &os) const{
    if (m_length > offsetof (sockaddr_un, sun_path) && m_addr.sun_path[0] == '\0') {
        return os << "\\0" << std::string (m_addr.sun_path + 1, m_length - offsetof(sockaddr_un, sun_path) - 1);
    }
    return os << m_addr.sun_path;
}

UnknowAddress::UnknowAddress(int family) {
    memset (&m_addr, 0, sizeof m_addr);
    m_addr.sa_family = family;
}

UnknowAddress::UnknowAddress(const sockaddr &addr) {
    m_addr = addr;
}

const sockaddr* UnknowAddress::getAddr () const{
    return (sockaddr*)&m_addr;
}

sockaddr* UnknowAddress::getAddr (){
    return (sockaddr*)&m_addr;
}

socklen_t UnknowAddress::getAddrLen () const{
    return sizeof m_addr;
}

std::ostream& UnknowAddress::insert (std::ostream &os) const{
    os << "[UnknowAddress family=" << m_addr.sa_family << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Address &addr){
    return addr.insert(os);
}

}
