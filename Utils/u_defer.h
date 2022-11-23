#ifndef U_DEFER_H
#define U_DEFER_H

#include <utility>

template <class F>
class Deferrer
{
    F f;
public:
    Deferrer(F&& f) : f{std::forward<F>(f)} {}
    ~Deferrer() { f(); }
};

#define defer(function) auto def = Deferrer{function}

#endif // U_DEFER_H
