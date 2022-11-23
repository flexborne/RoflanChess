#ifndef ENUMARRAY_H
#define ENUMARRAY_H

#include <array>
#include <bits/ranges_algo.h>
#include <optional>
#include <utility>


template<typename En> requires std::is_enum_v<En>
    constexpr static uint8_t enumSize() {
        return 0;
}

/// @brief simple define for enum class creation with compiler-time evaluated size
#define ENUM_DEFINE(EnumName, ...)\
\
enum class EnumName;\
\
template<>\
[[maybe_unused]] constexpr uint8_t enumSize<EnumName>() {\
    enum EnumName { __VA_ARGS__ }; \
    EnumName enumArray[]{ __VA_ARGS__ }; \
    return sizeof(enumArray) / sizeof(enumArray[0]); \
}\
\
enum class EnumName : int { __VA_ARGS__ }

template <class En>
struct DefaultEnumIndexConverter
{
    static uint8_t enToIndex(En en) noexcept(false) { return static_cast<uint8_t>(en); }
    static En indexToEn(uint8_t idx) noexcept { return static_cast<En>(idx); }
};

template <class T, class R>
using kv = std::pair<T, R>;

/// @brief class that holds key-value pairs of enum & any type T
/// ordinarily is used for enum names
template <class En, class T> requires std::is_enum_v<En>
class EnumArray
{
public:
    template <std::size_t N>
    constexpr EnumArray(kv<En, T>(&&list)[N]) noexcept {
        static_assert(N == enumSize<En>() && "Incomplete EnumArray initialization!");

        for (auto [key, value] : list) {
            buf[idxConverter.enToIndex(key)] = value;
        }
    }

    T operator[](En en) const {
        return buf[idxConverter.enToIndex(en)];
    }

    uint8_t size() const noexcept {
        return buf.size();
    }

    /// @return key if such a key-value pair exists in buf
    std::optional<En> parseEnum(T value) const noexcept {
        if (auto found = std::ranges::find(buf, value); found != buf.end()) {
            return idxConverter.indexToEn(std::distance(buf.begin(), found));
        }

        return std::nullopt;
    }

private:
    std::array<T, enumSize<En>()> buf;
    DefaultEnumIndexConverter<En> idxConverter;
};

template <class Enum, class Func, class... Args> requires std::is_enum_v<Enum>
void iterateOverEnum(Func&& func, Args&& ...args)
{
    for (size_t i = 0; i < enumSize<Enum>(); ++i) {
        func(static_cast<Enum>(i), std::forward<Args>(args)...);
    }
}


#endif // ENUMARRAY_H
