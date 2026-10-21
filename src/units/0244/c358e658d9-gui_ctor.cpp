#include "8d13f4ad0e-gui_ctor.h"
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit service-gui_0040BAD0-24 (base sha256
// 601785ca264a7b413a82e2f243a2da490903fc7006f64813985a5ee52d1cabc3).
// D76 FIX (FIRST FRAME): gui_ctor::bindings is never populated (m47-compose D76-CHECK.json: 16/16 null slots), and
// StartServices calls CSWGuiManager::CSWGuiManager 0040BAD0 at 005F93D7, so the first binding call was a null call.
// Every binding is replaced by a direct call of the genuine provider (D1 fastcall form, same arity); the never-filled
// `bindings` object is no longer defined (gui_ctor.h only declares it; no other reader in the stage):
//   00405ED0 c2da_default_ref_00405ed0 (c2da-default-ref)      005B3190 kotor_menu20::ConstructString005b3190 (menu20-bindings)
//   00416770 exo_array_list_construct_00416770 (3d-render)     005E5140 GuiAssign005e5140 (ini-string-priority)
//   005E5D10 GuiConcat005e5d10 (ini-string-priority)           005E5C50 StartupString::Assign005e5c50
//   005E5C20 StartupString::~StartupString                     00406290 unresolved_ref_assign_00406290 (resource-ref-assignment)
//   006FA7E6 live_operator_new                                 006277C0 gui_tooltip_panel_construct_006277c0 (rev2 tooltip_panel.cpp)
//   005ED700 kotor_menu20::ClientApp::get_options_005ed700     007A39F4 kotor_input_cells::gui_manager_007a39f4
//   007A39FC kotor_menu_window_cells::manager_007a39fc         007A3D50 kotor_cells_gui_suffix_007a3d50 (gui-load rev2 owner)
//   0073DF7C "800x600", 0073D71C "" (.rdata literals of identical bytes, tools/rd.py).
// EH (handler 007147EA): the stage body had none; the original unwind states are reproduced: state 0 string +74,
// 1 list +88, 2 list +94, 3 the temporary string, 4 the 1A4h allocation (freed when 006277C0 throws).
#include "f9f7ab0530-startup_string.h"
#include <initializer_list>
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*);
                         struct ClientApp { __declspec(noinline) void* __thiscall get_options_005ed700(); }; }
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace menu_resources { struct Ref; }
extern "C" {
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref*, const char*);
void* __fastcall exo_array_list_construct_00416770(void*, void*, std::uint32_t);
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);
kotor_reconstruction::StartupString* __fastcall GuiAssign005e5140(kotor_reconstruction::StartupString*, void*, const char*);
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
void* __fastcall gui_tooltip_panel_construct_006277c0(void* self, void*, void* manager) noexcept(false);
extern unsigned char kotor_cells_gui_suffix_007a3d50[40];
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*, void*,
    kotor_reconstruction::StartupString*, const kotor_reconstruction::StartupString*);
namespace gui_ctor {
using kotor_reconstruction::StartupString;
static Word get(const void* p,unsigned o){Word v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,4);return v;}
static void put(void* p,unsigned o,Word v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,4);}
static Word x87_copy(const void* p){Word v;__asm{mov eax,p}__asm{fld dword ptr [eax]}__asm{fstp v}return v;}
static const char k_resolution_0073df7c[]="800x600";
static const char k_empty_0073d71c[]="";
static kotor_menu20::ClientApp* client(){void* c;std::memcpy(&c,static_cast<const unsigned char*>(kotor_menu_window_cells::manager_007a39fc)+4,4);
    return static_cast<kotor_menu20::ClientApp*>(c);}
Gui* __thiscall Gui::construct_0040bad0(){
    StartupString* const name=reinterpret_cast<StartupString*>(bytes+0x74);
    c2da_default_ref_00405ed0(reinterpret_cast<menu_resources::Ref*>(bytes+0x28),nullptr);             // 0040BAF7
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(bytes+0x74),nullptr); // 0040BB01 (state 0)
    try {
    exo_array_list_construct_00416770(bytes+0x88,nullptr,0);                                            // 0040BB13 (state 1)
    try {
    exo_array_list_construct_00416770(bytes+0x94,nullptr,0);                                            // 0040BB24 (state 2)
    try {
    const std::uint16_t zero=0;std::memcpy(bytes+0x6c,&zero,2);std::memcpy(bytes+0x6e,&zero,2);
    bytes[0x70]=0;bytes[0x71]=0;put(this,0x64,0);bytes[0x72]=0;put(this,0x80,0);put(this,0x84,0);put(this,0x7c,0);
    GuiAssign005e5140(name,nullptr,k_resolution_0073df7c);                                             // 0040BB29..58
    alignas(4) unsigned char temporary[16];
    StartupString* const returned=GuiConcat005e5d10(name,nullptr,reinterpret_cast<StartupString*>(temporary),
        reinterpret_cast<const StartupString*>(kotor_cells_gui_suffix_007a3d50));                       // 0040BB5D..69
    try { name->Assign005e5c50(returned); }                                                             // 0040BB6E..76 (state 3)
    catch(...) { reinterpret_cast<StartupString*>(temporary)->~StartupString(); throw; }
    reinterpret_cast<StartupString*>(temporary)->~StartupString();                                      // 0040BB7B..84
    for(unsigned o:{8u,0x10u,0xcu})put(this,o,0);



    put(this,0x1c,get(this,0x1c)&0xfffffffcu);

    for(unsigned o:{0x14u,0x18u,0x24u,0x20u})put(this,o,0);
    unresolved_ref_assign_00406290(reinterpret_cast<menu_resources::Ref*>(bytes+0x28),nullptr,
        reinterpret_cast<menu_resources::Ref*>(temporary),k_empty_0073d71c);                            // 0040BB8C..B3
    put(this,0x1c,get(this,0x1c)&0xfffffffbu);put(this,0x38,1);
    void* allocation=live_operator_new(0x1a4);Word auxiliary=0;                                        // 0040BBBE..CD
    if(allocation){                                                                                     // state 4
        try { auxiliary=static_cast<Word>(reinterpret_cast<std::uintptr_t>(
                  gui_tooltip_panel_construct_006277c0(allocation,nullptr,this))); }                    // 0040BBE2..E5
        catch(...) { live_free(allocation); throw; }
    }
    put(this,0x3c,auxiliary);put(this,0x44,0);put(this,0x1c,get(this,0x1c)&0xfffffff7u);
    for(unsigned o:{0x40u,0x4cu,0x50u,0x54u,0xa0u})put(this,o,0);bytes[0xa4]=0;

    kotor_input_cells::gui_manager_007a39f4=this;                                                      // 0040BC15

    put(this,0x68,0);
    kotor_menu20::ClientApp* app=client();                                                              // 0040BC1E..24
    void* options=app->get_options_005ed700();                                                          // 0040BC2C
    if(options){

        app=client();                                                                                   // 0040BC35..3A

        options=app->get_options_005ed700();put(this,0x48,x87_copy(static_cast<unsigned char*>(options)+0xc)); // 0040BC3D..45
    }
    put(this,0x58,0xffffffffu);put(this,0x5c,0xffffffffu);return this;
    } catch(...) { exo_array_list_destruct_0056b6f0(bytes+0x94,nullptr); throw; }
    } catch(...) { exo_array_list_destruct_0056b6f0(bytes+0x88,nullptr); throw; }
    } catch(...) { name->~StartupString(); throw; }
}
}
