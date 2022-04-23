#ifndef LIBCOCAO_SINGLETON_H
#define LIBCOCAO_SINGLETON_H

namespace libcocao {

template <class T, class X = void, int N = 0>
class Singleton {
public:
    static T* GetInstance() {
        static T V;
        return &V;
    }
};

template<class T, class X = void, int N = 0>
class SingletonPtr {
public:
    static std::shared_ptr<T> GetInstance () {
        std::shared_ptr<T> v(new T);
        return v;
    }
};

}

#endif