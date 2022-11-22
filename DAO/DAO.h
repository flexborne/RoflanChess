#ifndef DAO_H
#define DAO_H

// std
#include <cstdint>
#include <functional>
#include <vector>

#include "utils.h"


template <class DTO>
class DAO
{
protected:
    using const_DTO_T = typename utils::add_const_ref<DTO>::type;
    using containerT = std::vector<const_DTO_T>;
public:
    DAO() noexcept = default;
    virtual ~DAO() = default;

    virtual void create(DTO&&) = 0;

    virtual void update(uint8_t idx, DTO&&) = 0;

    virtual auto get(uint8_t idx) const -> const_DTO_T = 0;

    virtual auto getAll() const -> containerT = 0;

    virtual void remove(uint8_t idx) = 0;
};

#endif // DAO_H

