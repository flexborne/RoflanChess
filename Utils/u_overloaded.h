#ifndef U_OVERLOADED_H
#define U_OVERLOADED_H

namespace utils {

    template <class... Ts>
    struct Overloaded : Ts...
    {
        using Ts::operator()...;
    };

    template<class... Ts>
    Overloaded(Ts...) -> Overloaded<Ts...>;

} // end of namespace utils

#endif // U_OVERLOADED_H
