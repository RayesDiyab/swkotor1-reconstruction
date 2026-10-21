// 00684DF0 CSWGuiMainInterfaceAction::HitCheckMouse(x,y), RET8.
// Complete original code boundary [00684DF0,00684E6B), 123 bytes.
#include "a3b6729268-w5_server_env.hpp"
#include <cstdint>
#include <initializer_list>
extern "C" unsigned __fastcall kotor_fastcall_00684df0(void* action, void*, int x, int y) {
    for (unsigned offset : {0x388u, 0x54cu, 0x1c4u}) {
        void* child = static_cast<unsigned char*>(action) + offset;
        if (w5::vcall<unsigned>(child, 0x44, x, y) != 0)
            return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(child));
    }
    if (w5::vcall<unsigned>(action, 0x44, x, y) != 0)
        return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(action));
    return 0;
}
