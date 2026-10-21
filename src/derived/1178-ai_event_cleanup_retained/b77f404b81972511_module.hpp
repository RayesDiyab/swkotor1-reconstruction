#pragma once
#include <cstdint>
namespace kotor::module {
using Word=std::uint32_t;
struct Memory {
    virtual ~Memory()=default;
    virtual Word read(Word address) const=0;
    virtual void write(Word address,Word value)=0;
};
void clear_area_cache(Memory&,Word self);
Word is_object_in_limbo(Memory&,Word self,Word object);
void remove_object_from_limbo(Memory&,Word self,Word object);
}
