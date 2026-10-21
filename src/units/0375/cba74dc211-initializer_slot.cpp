#include "48098dbfed-initializer_slot.h"
namespace aurora_context_adoption {
kotor_renderer_init::I32 __fastcall initialize_slot(kotor_renderer_init::Renderer* self,
    void*,void* top,void* render,std::uint32_t width,std::uint32_t height,
    std::uint32_t bits,std::uint32_t frequency,std::int32_t fullscreen,
    std::int32_t color,std::int32_t depth) {



    // Only the original fifth argument consumes a low byte; the other words
    // retain their full payload, including signed color/depth and dimensions.
    return self->Initialize(top,render,width,height,
        static_cast<kotor_renderer_init::U8>(bits),frequency,fullscreen,color,depth);
}
std::uint32_t initializer_slot_word() noexcept {
    return static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(&initialize_slot));
}
bool dispatch_initializer(const kotor_reinit::Call& call,kotor_reinit::Result& result) {
    if(call.site!=0x00403a86 || call.kind!=kotor_reinit::Kind::virtual_slot ||
       call.slot!=8 || call.count!=9 || !call.self ||
       call.target!=initializer_slot_word())return false;
    auto pointer=[](std::uint32_t value){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));};
    const auto value=initialize_slot(static_cast<kotor_renderer_init::Renderer*>(pointer(call.self)),nullptr,
        pointer(call.args[0]),pointer(call.args[1]),call.args[2],call.args[3],call.args[4],call.args[5],
        static_cast<std::int32_t>(call.args[6]),static_cast<std::int32_t>(call.args[7]),static_cast<std::int32_t>(call.args[8]));




    result=static_cast<std::uint32_t>(value);

    return true;
}
}
