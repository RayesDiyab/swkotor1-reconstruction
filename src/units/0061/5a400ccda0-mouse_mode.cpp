// lane-startup-misc: the 005F49B0 mouse-mode chain reached through 0061D670 -> 005EE150
// (lane-menu-tables d1_bodies.cpp) and CAurObject::AnimateExternal 0043E150 (reached from
// 0040A270 when the GUI cursor object +24 exists). swkotor.exe GOG K1, base 0x400000.
// D1 form: original __thiscall bodies as __fastcall(receiver, unused EDX, stack words).
// Receivers are opaque original objects addressed by observed offsets only.
//
// Reused providers (not re-authored here):
//   0061F9C0 HideMouse, 005DF5A0 CExoInput::SetMouseMode  -> lane-routes window_leaves.cpp (@kotor_fastcall_<addr>)
//   0061FA00 ShowMouse                                    -> lane-prepare stage client_callees.cpp (@kotor_fastcall_0061fa00@12)
//   0040B060 CSWGuiManager::ActivateSoftwareMouse          -> M41 service-gui_0040B060-26.obj (gui_mouse::Gui)
//   005E0000 CExoInputInternal::SetMouseCursor             -> lane-menu-tables d1_bodies.cpp
// Authored here under the names those packets declare as missing:
//   005E1740, 005DF5E0, 0040A260, 005E0930 (lane-routes @kotor_fastcall_<addr>), 005E00B0 (lane-prepare),
//   and their lowers 005E0070, 0040A230.
#include "9ea344dff0-d1_bodies.h"           // menu_tables_m41::InputInternalSetMouseCursor_005e0000, menu_tables_required decls
#include "c2de7d895c-gui_mouse.h"           // gui_mouse::Gui::activate_0040b060 (M41)
#include "369787b919-window_shared_cells.h" // kotor_menu_window_cells::{input_007a39e4, instance_007a39c0}
#include <windows.h>
#include <cstdint>
#include <cstring>
#include <new>

namespace {
using U8=std::uint8_t; using U32=std::uint32_t;
inline U8* at(void* p,std::size_t o){return static_cast<U8*>(p)+o;}
inline U32 get32(const void* p,std::size_t o){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
inline void put32(void* p,std::size_t o,U32 v){std::memcpy(static_cast<U8*>(p)+o,&v,4);}
inline void* ptr32(const void* p,std::size_t o){void* v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
// Original-layout virtual slot read from the object's table (memcpy discipline of lane-menu-tables d1_bodies.cpp).
template<class F>inline F slot(const void* object,unsigned offset){const void* table=ptr32(object,0);F f;std::memcpy(&f,static_cast<const U8*>(table)+offset,4);return f;}
using DeletingDestructor=void*(__fastcall*)(void*,void*,U32);   // [vtable+00], one stack word (0040A23C push 1), inferred EAX
using AnimateSlot=void(__fastcall*)(void*,void*,float);         // [vtable+14], one stack word (0043E163/0043E179), inferred void
}

// ---- providers owned by other lanes (identical declarations) ----
extern "C" {
void __fastcall kotor_fastcall_0061f9c0(void* internal,void*,std::uint32_t flags);  // lane-routes: HideMouse RET 4
void __fastcall kotor_fastcall_005df5a0(void* input,void*,std::uint32_t mode);       // lane-routes: CExoInput::SetMouseMode RET 4
}
extern "C" {
void __fastcall kotor_fastcall_0061fa00(void* internal,void*,int flags);              // lane-prepare (game_prepare_producers.h:88, extern "C"): ShowMouse RET 4
}

// ---- authored bodies ----
extern "C" {
// 005E0070 CExoInputInternal::HideMouse: CURSORINFO local (cbSize 14h at 005E007B); when +378 is 0 and
// GetCursorInfo succeeds with CURSOR_SHOWING, ShowCursor(FALSE) until the display count is negative.
__declspec(noinline) void __fastcall kotor_fastcall_005e0070(void* internal,void*){
    CURSORINFO info;info.cbSize=0x14;                                         // 005E0070/005E007B
    static_assert(sizeof(CURSORINFO)==0x14,"cbSize 14h");
    if(*at(internal,0x378)!=0)return;                                          // 005E0073..005E0083
    if(!GetCursorInfo(&info))return;                                           // 005E0085..005E0092 [0073D3CC]
    if(!(static_cast<U8>(info.flags)&1u))return;                               // 005E0094 test byte [esp+4],1
    while(ShowCursor(FALSE)>=0){}                                              // 005E009C..005E00A8 [0073D3D0]
}
// 005E00B0 CExoInputInternal::ShowMouse: as 005E0070, but when the cursor is NOT showing,
// ShowCursor(TRUE) until the display count is non-negative (005E00D9 jne, 005E00E8 jl).
__declspec(noinline) void __fastcall kotor_fastcall_005e00b0(void* internal,void*){
    CURSORINFO info;info.cbSize=0x14;                                         // 005E00B0/005E00BB
    if(*at(internal,0x378)!=0)return;                                          // 005E00B3..005E00C3
    if(!GetCursorInfo(&info))return;                                           // 005E00C5..005E00D2
    if(static_cast<U8>(info.flags)&1u)return;                                  // 005E00D4..005E00D9
    while(ShowCursor(TRUE)<0){}                                                // 005E00DB..005E00E8
}
// 005DF5E0 CExoInput::HideMouse: mov ecx,[ecx+4]; jmp 005E0070.
__declspec(noinline) void __fastcall kotor_fastcall_005df5e0(void* input,void*){
    kotor_fastcall_005e0070(ptr32(input,4),nullptr);
}
// 005E1740 CExoInputInternal::SetMouseMode(BYTE mode), RET 4, EAX=1 on every path (005E17CB).
// Declared void by its lane-routes consumer (kotor_original_calls.h:47); no consumer reads EAX.
__declspec(noinline) void __fastcall kotor_fastcall_005e1740(void* internal,void*,std::uint32_t word){
    const U8 mode=static_cast<U8>(word);                                       // 005E1740 mov al,[esp+4]
    if(mode==*at(internal,0x378))return;                                       // 005E1747..005E174D
    *at(internal,0x378)=mode;                                                  // 005E1751
    if(mode==1){while(ShowCursor(FALSE)>=0){}}                                 // 005E174F/005E1757..005E1766 [0073D3D0]
    U32 cursor=get32(internal,0x398);                                          // 005E1769
    const bool was_unset=cursor==0xffffffffu;                                  // 005E176F (flags before the store)
    put32(internal,0x398,0xffffffffu);                                         // 005E1772
    if(was_unset)cursor=1;                                                     // 005E177C..005E177E
    put32(internal,0x3ac,0);                                                   // 005E1786
    if(cursor==0xffffffffu)return;                                             // 005E1783/005E1790 (cannot hold after 005E177E)
    const U8 current=*at(internal,0x378);                                      // 005E1792 reload
    put32(internal,0x398,cursor);                                              // 005E179A
    if(current!=0)return;                                                      // 005E1798/005E17A0
    HCURSOR handle=LoadCursorA(kotor_menu_window_cells::instance_007a39c0,
                               MAKEINTRESOURCEA(static_cast<WORD>(cursor)));   // 005E17A2..005E17AD movzx eax,ax [0073D420]
    if(!handle)return;                                                         // 005E17B5
    HCURSOR previous=SetCursor(handle);                                        // 005E17BA [0073D3D4]
    if(previous!=handle)DestroyCursor(previous);                               // 005E17C0..005E17C5 [0073D3D8] (also NULL)
}
// 005E0930 CExoInputInternal::RefreshMouseCursor(adjust), RET 4: cursor=[+398] (-1 -> 1), [+398]=-1,
// then 005E0000(cursor, [+39C], adjust). Original EAX = 005E0000's result; both consumer declarations
// (lane-routes, lane-prepare) are void and no caller reads it.
__declspec(noinline) void __fastcall kotor_fastcall_005e0930(void* internal,void*,std::uint32_t adjust){
    U32 cursor=get32(internal,0x398);                                          // 005E0930
    const bool was_unset=cursor==0xffffffffu;                                  // 005E0936
    put32(internal,0x398,0xffffffffu);                                         // 005E0939
    if(was_unset)cursor=1;                                                     // 005E0943..005E0945
    const U32 second=get32(internal,0x39c);                                    // 005E094F (after the store)
    (void)menu_tables_m41::InputInternalSetMouseCursor_005e0000(internal,nullptr,cursor,second,adjust); // 005E094A..005E0957
}
// 0040A230 CSWGuiManager::HideSoftwareMouse: delete the +20 object through [vtable+0](1), clear +20;
// clear +24 when it is nonzero (no delete).
__declspec(noinline) void __fastcall kotor_fastcall_0040a230(void* gui,void*){
    void* const object=ptr32(gui,0x20);                                        // 0040A233
    if(object){
        (void)slot<DeletingDestructor>(object,0)(object,nullptr,1);            // 0040A23A..0040A23E
        put32(gui,0x20,0);                                                     // 0040A240
    }
    if(get32(gui,0x24)!=0)put32(gui,0x24,0);                                   // 0040A247..0040A24E
}
// 0040A260: jmp 0040A230.
__declspec(noinline) void __fastcall kotor_fastcall_0040a260(void* gui,void*){kotor_fastcall_0040a230(gui,nullptr);}
}
// lane-prepare declares 005E00B0/005E0930 with the same extern "C" names and types
// (game_prepare_producers.h:106-107); lane-routes declares 005E0930/005E1740/005DF5E0/0040A260
// (kotor_original_calls.h:46-49). One body each, here.

namespace menu_tables_required {
// 005F49B0 CClientExoAppInternal::SetMouseMode(BYTE mode), RET 4.
void __fastcall ClientInternalSetMouseMode_005f49b0(void* internal,void*,std::uint32_t mode){
    kotor_fastcall_0061f9c0(internal,nullptr,4);                               // 005F49B2..005F49B6 push 4
    const U32 word=mode;                                                       // 005F49BB full DWORD of the byte argument
    kotor_fastcall_005df5a0(kotor_menu_window_cells::input_007a39e4,nullptr,word); // 005F49BF..005F49C6 (cell read after HideMouse)
    void* const gui=ptr32(internal,0x274);                                     // 005F49CB
    if(gui){                                                                   // 005F49D1
        if(static_cast<U8>(word)==1){                                          // 005F49D5 cmp bl,1
            (void)std::launder(static_cast<gui_mouse::Gui*>(gui))->activate_0040b060(); // 005F49DA
        }else{
            kotor_fastcall_0040a230(gui,nullptr);                              // 005F49ED
        }
    }
    kotor_fastcall_0061fa00(internal,nullptr,4);           // 005F49DF..005F49E3 / 005F49F2..005F49F6
}
// 0043E150 CAurObject::AnimateExternal(float), RET 4: when +68 is set, its +50 counter is incremented
// before the [vtable+14] call and +68 is re-read and incremented again after it.
void __fastcall AnimateExternal_0043e150(void* object,void*,float weight){
    void* const first=ptr32(object,0x68);                                      // 0043E153
    if(first){                                                                 // 0043E156
        put32(first,0x50,get32(first,0x50)+1u);                                // 0043E15A inc dword [eax+50h]
        slot<AnimateSlot>(object,0x14)(object,nullptr,weight);                 // 0043E15D..0043E166 (vtable read after the increment)
        void* const second=ptr32(object,0x68);                                 // 0043E169 reload
        put32(second,0x50,get32(second,0x50)+1u);                              // 0043E16C
        return;
    }
    slot<AnimateSlot>(object,0x14)(object,nullptr,weight);                     // 0043E173..0043E17C
}
}
