#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <memory>

namespace utils {

    template <class... T>
    auto add(T... args)
    {
        return (... + args);
    }

    template <class... T>
    auto add(const char* a, T... args)
    {
        return (std::string{a} + ... + args);
    }

    template <class F, class... R>
    auto compose(F&& f, R&&... r)
    {
        return [=] (auto x) {
            return f(compose(r...)(x));
        };
    }

    /// @brief in order to get const T& or const T* from unique_ptr etc
    template <class T>
    struct add_const_ref
    {
        typedef const T& type;
    };

    template <class T>
    struct add_const_ref<std::unique_ptr<T>>
    {
        typedef const T* type;
    };

    template <class T>
    struct add_const_ref<std::shared_ptr<T>>
    {
        typedef const T* type;
    };

    template <class T>
    bool isAnyOf(T x, T y)
    {
        return x == y;
    }

    template <class T, class ...Args>
    bool isAnyOf(T x, T y, Args... args)
    {
        return x == y || isAnyOf(x, args...);
    }

}   // end of namespace utils
#endif // UTILS_H
