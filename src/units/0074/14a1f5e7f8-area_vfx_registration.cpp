// Narrow adapter to existing0884 object_register004D80E0 body; no copied original-body credit.
#include "9136b2d0c5-area_vfx.hpp"
#include "d1ddc501e0-object_registration.hpp"
#if !defined(_M_IX86)
#error Object registration adapter requires x86 direct memory
#endif
namespace {
using kotor::server_mainloop::Word;
void* ptr(Word value) {return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));}
Word word(void* value) {return static_cast<Word>(reinterpret_cast<std::uintptr_t>(value));}
struct Direct final:kotor::server_mainloop::Memory {
    Word read(Word address)override {Word value;std::memcpy(&value,ptr(address),4);return value;}
    void write(Word address,Word value)override {std::memcpy(ptr(address),&value,4);}
    Word read16(Word address)override {std::uint16_t value;std::memcpy(&value,ptr(address),2);return value;}
    void write16(Word address,Word value)override {const std::uint16_t half=static_cast<std::uint16_t>(value);std::memcpy(ptr(address),&half,2);}
    Word read8(Word address)override {return *static_cast<const unsigned char*>(ptr(address));}
};
struct Heap final:kotor::server_mainloop::ModuleAllocation {
    Word allocate(Word size)override {return word(live_operator_new(size));}
};
}
extern "C" std::uint32_t __fastcall area_vfx_register_existing_004d80e0(void* array,void*,
    std::uint32_t* id,void* object,std::uint32_t reverse) {
    Direct memory;Heap heap;
    return kotor::server_mainloop::object_register_004d80e0(memory,heap,word(array),word(id),word(object),reverse);
}
