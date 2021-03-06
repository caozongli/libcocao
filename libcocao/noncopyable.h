#ifndef __LIBCOCAO_NONCOPYABLE_H__
#define __LIBCOCAO_NONCOPYABLE_H__

namespace libcocao {
    class Noncopyable {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;
        Noncopyable(const Noncopyable & ) = delete;
        Noncopyable& operator= (const Noncopyable&) = delete;
    };
}

#endif