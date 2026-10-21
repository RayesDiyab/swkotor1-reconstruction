#include "eb639d366c-sound_lifecycle.h"
#include "53783c93b9-allocator_live.h"
#include <cstring>



namespace {
void* pointer(void* object) { void* value;std::memcpy(&value,object,4);return value; }
void publish(void* object,void* value) { std::memcpy(object,&value,4); }
// 00723FF0: the saved allocation is freed on unwind from state zero. Native
// tests below exercise normal return only; this is not unwind acceptance.
struct AllocationGuard {
    void* value;
    ~AllocationGuard() { if(value)live_free(value); }
};
}
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void* outer,void*) {

    if(*sound_lifecycle29_required::word_00832894) {
        publish(outer,nullptr);return outer;
    }

    void* raw=live_operator_new(0x84);
    AllocationGuard guard{raw};
    void* result=nullptr;
    if(raw) {
        void* source=pointer(table29_canonical_required::sound_007a39ec);
        result=sound_lifecycle29_required::construct_005dbbe0(raw,nullptr,source);
    }
    guard.value=nullptr;
    publish(outer,result);
    return outer;
}
void __fastcall gui_sound_destroy_005d6200(void* outer,void*) {
    void* raw=pointer(outer);
    if(raw) {
        sound_lifecycle29_required::destroy_005dbca0(raw,nullptr);
        live_free(raw);

        publish(outer,nullptr);

    }
}
}
