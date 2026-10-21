// Narrow extraction of existing W4 0063D470. No unrelated WIP methods.
#include <cstdint>
#include <cstring>
static_assert(sizeof(void*) == 4, "original x86 layout");
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
unsigned __fastcall kotor_fastcall_004aea30(void*, void*, unsigned);
void __fastcall kotor_fastcall_004aef40(void*, void*, unsigned);
}
namespace w4 {
using U32 = std::uint32_t;
using U8 = std::uint8_t;
inline void* pt(U32 a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U32 lw(const void* o, U32 off) { U32 v; std::memcpy(&v, static_cast<const U8*>(o) + off, 4); return v; }
inline void* lp(const void* o, U32 off) { return pt(lw(o, off)); }
inline void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* server_app() { return lp(app_manager(), 8); }
}
using namespace w4;
extern "C" void __fastcall kotor_fastcall_0063d470(void* object, void*) {
    void* server = server_app();                                                             // 0063D470, 0063D479 [[007A39FC]+8]
    const unsigned sid = kotor_fastcall_004aea30(server, nullptr, lw(object, 4));            // 0063D475..0063D47F
    kotor_fastcall_004aef40(server, nullptr, sid);                                           // 0063D484..0063D487
}

// Strong carrier for W5's existing weak import; same body, no second reconstruction.
extern "C" void __fastcall w5_t2_client_object_clear_all_actions_0063d470(void* object, void*) {
    kotor_fastcall_0063d470(object, nullptr);
}
