// lane-menu-tables D1 bodies (supersedes menu_tables_bodies.cpp / menu_tables_gui.cpp /
// menu_tables_options.cpp). Instruction addresses refer to swkotor.exe (GOG K1).
#include "9ea344dff0-d1_bodies.h"
#include "c534281817-startup_string.h"     // kotor_reconstruction::StartupString
#include "231bd565df-constructors.h"       // kotor_list::Internal, kotor_list_dependencies
#include "369787b919-window_shared_cells.h"// kotor_menu_window_cells
#include "604fee70b4-reference.h"          // unresolved_ref_assign_00406290 (ECX Ref, EDX unused, out, text; RET8)
#include "7a7fd591c3-shared_boundary.hpp"  // c2da_ref_00405fe0 (CResRef::GetResRefStr)
#include <windows.h>
#include <excpt.h>
#include <cstring>
#include <exception>
#include <new>
namespace menu_tables_m41 {
using String=kotor_reconstruction::StartupString;
namespace cells {
float keyboard_camera_007a2320=-1.0f;
float keyboard_camera_007a2324=-1.0f;
float keyboard_camera_007a2328=-1.0f;
U32 dynamic_memory_ratio_0078d3f0=0x3f000000u;
const U8 module_type_00746632=0x03;
const U32 invalid_object_00746644=0x7f000000u;
const U32 module_word_0074410c=0x0000053cu;
}
namespace {
inline U8* at(void* p,std::size_t o){return static_cast<U8*>(p)+o;}
inline U32 get32(const void* p,std::size_t o){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
inline void put32(void* p,std::size_t o,U32 v){std::memcpy(static_cast<U8*>(p)+o,&v,4);}
inline void put_bits(float& cell,U32 bits){std::memcpy(&cell,&bits,4);}
template<class F>inline F slot(U32 table,unsigned offset){F f;U32 v=get32(reinterpret_cast<const void*>(table),offset);std::memcpy(&f,&v,4);return f;}
// Original-layout virtual slots read from the object's table (memcpy discipline of M40 gui_mouse.cpp).
using Reference=void(__fastcall*)(void*,void*,const char*,const char*);   // vtable +34h, 2 stack words
using Animation=void(__fastcall*)(void*,void*,const char*,U32,U32,U32);   // vtable +18h, 4 stack words
const char* const cursor_names_0078d240[]={
#include "ce27399e7b-cursor_names.inc"
};
static_assert(sizeof(cursor_names_0078d240)/sizeof(cursor_names_0078d240[0])==94,"93 observed entries + zero word");
const char empty_0073d71c[]="";
const char center_0073df74[]="center";
const char default_0073df6c[]="default";
template<std::size_t N>inline void construct_strings(void* p){for(std::size_t i=0;i<N;++i)::new(at(p,8*i)) String();}
}

// ---- lists (004B25D0 / 0062B510): allocate 0Ch -> 005E9AA0 -> [esi]; unwind funclet 00718620 frees via 006FA390
static void* construct_list(void* self){
    void* storage=kotor_list_dependencies::allocate(12);           // 004B25E7/004B25EB
    kotor_list::Internal* p=nullptr;
    if(storage){                                                   // 004B25F7
        try{p=(::new(storage) kotor_list::Internal)->Construct();} // 004B2605 call 005E9AA0
        catch(...){kotor_list_dependencies::release(storage);throw;}
    }
    std::memcpy(self,&p,4);                                        // 004B260A / 004B2624
    return self;                                                   // EAX=this
}
void* __fastcall ClientListConstruct_004b25d0(void* list,void*){return construct_list(list);}
void* __fastcall StringListConstruct_0062b510(void* list,void*){return construct_list(list);}

// ---- 0061D9E0 CClientOptions::SetDefaultKeyboardCamera: fcomp 00740CBC (-1.0f); test ah,44h; jnp skip
void __fastcall SetDefaultKeyboardCamera_0061d9e0(menu_providers_40_49::Options* o,void*){
    if(!(cells::keyboard_camera_007a2320==-1.0f))put_bits(cells::keyboard_camera_007a2320,0x43480000u); // 0061D9E0..0061D9F3
    o->keyboard_camera_dps94=0x43480000u;                                                               // 0061D9FD 200.0f
    if(!(cells::keyboard_camera_007a2324==-1.0f))put_bits(cells::keyboard_camera_007a2324,0x43fa0000u); // 0061DA07..0061DA1A
    o->keyboard_camera_acceleration98=0x43fa0000u;                                                      // 0061DA24 500.0f
    if(!(cells::keyboard_camera_007a2328==-1.0f))put_bits(cells::keyboard_camera_007a2328,0x44fa0000u); // 0061DA2E..0061DA41
    o->keyboard_camera_deceleration9c=0x44fa0000u;                                                      // 0061DA4B 2000.0f
}

// ---- 005DF5C0 CExoInput::SetMouseCursor: mov ecx,[ecx+4]; jmp 005E0000
U32 __fastcall InputSetMouseCursor_005df5c0(void* input,void*,U32 cursor,U32 unused,U32 adjust){
    void* internal;std::memcpy(&internal,at(input,4),4);
    return InputInternalSetMouseCursor_005e0000(internal,nullptr,cursor,unused,adjust);
}
// ---- 005E0000 CExoInputInternal::SetMouseCursor
U32 __fastcall InputInternalSetMouseCursor_005e0000(void* self,void*,U32 cursor,U32 unused,U32 adjust){
    (void)unused;                                                   // [esp+8] never read
    put32(self,0x3ac,adjust);                                       // 005E000E before the test
    const I32 raw=static_cast<I32>(cursor);
    U32 effective=cursor;                                           // 005E000C
    if(adjust!=0)effective=cursor+static_cast<U32>(raw%2);          // 005E0016..005E0023
    if(get32(self,0x398)!=effective){                               // 005E0025
        put32(self,0x398,cursor);                                   // 005E002D raw EAX, not ESI
        if(*at(self,0x378)==0){                                     // 005E0033..005E003B
            HCURSOR handle=LoadCursorA(kotor_menu_window_cells::instance_007a39c0,
                MAKEINTRESOURCEA(static_cast<WORD>(effective)));    // 005E003D..005E0048 movzx eax,si
            if(handle){                                             // 005E0050
                HCURSOR previous=SetCursor(handle);                 // 005E0055
                if(previous!=handle)DestroyCursor(previous);        // 005E005B..005E0060 (also NULL)
            }
        }
    }
    return effective;                                               // 005E0067 ; ret 0Ch
}

// ---- 0040A270 CSWGuiManager::SetSoftwareMouseCursor (+1C flags, +24 cursor object, +28 CResRef, +38 id)
void __fastcall GuiSetSoftwareMouseCursor_0040a270(void* gui,void*,U32 cursor,U32 visible,U32 adjust){
    put32(gui,0x38,cursor);                                          // 0040A282
    const I32 raw=static_cast<I32>(cursor);
    U32 index=cursor;
    if(adjust!=0)index=cursor+static_cast<U32>(raw%2);               // 0040A27F..0040A296
    alignas(4) menu_resources::Ref temporary;                        // [esp+0Ch]
    unresolved_ref_assign_00406290(std::launder(reinterpret_cast<menu_resources::Ref*>(at(gui,0x28))),nullptr,&temporary,cursor_names_0078d240[index]); // 0040A298..0040A2AA
    const U32 object=get32(gui,0x24);                                // 0040A2AF
    if(object!=0){
        const U32 table=get32(reinterpret_cast<const void*>(object),0);            // 0040A2BB vtable before GetResRefStr
        const char* text=c2da_ref_00405fe0(at(gui,0x28),nullptr);                  // 0040A2BF
        slot<Reference>(table,0x34)(reinterpret_cast<void*>(get32(gui,0x24)),nullptr,empty_0073d71c,text); // 0040A2C4..0040A2CD
        if(visible!=((get32(gui,0x1c)>>2)&1u)){                                    // 0040A2D0..0040A2DC full compare
            const char* name=visible!=0?center_0073df74:default_0073df6c;           // 0040A2DE..0040A2E7
            U32 receiver=get32(gui,0x24);
            slot<Animation>(get32(reinterpret_cast<const void*>(receiver),0),0x18)(reinterpret_cast<void*>(receiver),nullptr,name,0x3f800000u,0,0); // 0040A2EC..0040A2FB
            menu_tables_required::AnimateExternal_0043e150(reinterpret_cast<void*>(get32(gui,0x24)),nullptr,1.0f); // 0040A2FE..0040A306
            receiver=get32(gui,0x24);
            slot<Animation>(get32(reinterpret_cast<const void*>(receiver),0),0x18)(reinterpret_cast<void*>(receiver),nullptr,empty_0073d71c,0,0x80,0); // 0040A30B..0040A31E
        }
    }
    const U32 flags=get32(gui,0x1c);                                  // 0040A321
    put32(gui,0x1c,flags^(((visible*4u)^flags)&4u));                  // 0040A324..0040A335
}

// ---- vector element callbacks used by CGuiInGame 0062FED0 (objSize 16)
void* __fastcall LiveContentConstruct_0059ea90(void* e,void*){construct_strings<1>(e);return e;} // 0059EA93 call 005B3190; EAX=this
void __fastcall LiveContentDestroy_0059ea30(void* e,void*){std::launder(reinterpret_cast<String*>(e))->~String();} // jmp 005E5C20
void* __fastcall DialogMessageConstruct_006285b0(void* e,void*){
    ::new(e) String();                                               // 006285CD
    try{::new(at(e,8)) String();}                                    // 006285DD (state 0)
    catch(...){std::launder(reinterpret_cast<String*>(e))->~String();throw;}
    return e;
}
void __fastcall DialogMessageDestroy_00628600(void* e,void*){
    std::launder(reinterpret_cast<String*>(at(e,8)))->~String();     // 00628628 (state 0; ~String is noexcept)
    std::launder(reinterpret_cast<String*>(e))->~String();           // 00628637
}

// ---- 006FA7F4 __ArrayUnwind / 006FB55F `eh vector constructor iterator'
static int ArrayUnwindFilter(unsigned long code){
    if(code==0xE06D7363ul)std::terminate();                          // 006FA84C call 006FEE5F
    return EXCEPTION_CONTINUE_SEARCH;                                // 006FA83C..006FA846
}
void __stdcall ArrayUnwind_006fa7f4(void* destination,U32 size,U32 count,Destructor dtor){
    __try{
        U32 remaining=count;
        U32 cursor=reinterpret_cast<U32>(destination);
        for(;;){
            --remaining;                                            // 006FA804 DEC wraps at 80000000
            if(remaining&0x80000000u)break;                           // 006FA807 JS
            cursor-=size;                                           // 006FA809..006FA80F, modulo32
            destination=reinterpret_cast<void*>(cursor);
            dtor(destination,nullptr);                               // 006FA812 (ECX=element)
        }
    }__except(ArrayUnwindFilter(GetExceptionCode())){}
}
void __stdcall VectorConstruct_006fb55f(void* destination,U32 size,U32 count,Constructor ctor,Destructor dtor){
    volatile I32 built=0;volatile int completed=0;                   // [ebp-20h], [ebp-1Ch]
    void* volatile cursor=destination;                               // [ebp+8] updated in place
    __try{
        for(;built<static_cast<I32>(count);++built){                 // 006FB576..006FB57C signed
            (void)ctor(cursor,nullptr);                              // 006FB583 EAX ignored
            cursor=static_cast<U8*>(cursor)+size;                    // 006FB586..006FB589
        }
        completed=1;                                                 // 006FB591
    }__finally{
        if(!completed)ArrayUnwind_006fa7f4(cursor,size,static_cast<U32>(built),dtor); // 006FB5A9..006FB5BB
    }
}

// ---- 0061B6F0 CClientOptions::SetCameraMode(byte)
void __fastcall SetCameraMode_0061b6f0(void* options,void*,U32 word){
    const U8 mode=static_cast<U8>(word);                              // 0061B6F0 mov al,[esp+4]
    *at(options,0x6d)=mode;                                           // 0061B6F6
    if(mode==1||mode==0)*at(options,0x6e)=mode;                       // 0061B6F4/0061B6FB..0061B6FF
}

// ---- 005EE150 CClientExoApp::SetMouseMode: mov ecx,[ecx+4]; jmp 005F49B0
void __fastcall ClientAppSetMouseMode_005ee150(void* app,void*,U32 mode){
    void* internal;std::memcpy(&internal,at(app,4),4);
    menu_tables_required::ClientInternalSetMouseMode_005f49b0(internal,nullptr,mode);
}
// ---- 0061D670 CClientOptions::SetEnableHardwareMouseCursor(int)
void __fastcall SetEnableHardwareMouseCursor_0061d670(void* options,void*,I32 enabled){
    void* const manager=kotor_menu_window_cells::manager_007a39fc;   // 0061D670 single load
    if(!manager)return;                                             // 0061D67A
    void* client;std::memcpy(&client,at(manager,4),4);              // 0061D67C
    if(!client)return;                                              // 0061D681
    ClientAppSetMouseMode_005ee150(client,nullptr,enabled!=0?0u:1u); // 0061D688..0061D692 push 0/1
    const U32 word=get32(options,0);                                // 0061D69A after the call
    put32(options,0,word^(((static_cast<U32>(enabled)+static_cast<U32>(enabled))^word)&2u)); // 0061D697..0061D6A5
}

// ---- 0041E9B0 AurTextureSetDynamicMemoryRatio: mov eax,[esp+4]; mov [0078D3F0],eax
void __cdecl SetDynamicMemoryRatio_0041e9b0(float ratio){
    U32 bits;std::memcpy(&bits,&ratio,4);cells::dynamic_memory_ratio_0078d3f0=bits;
}
}
