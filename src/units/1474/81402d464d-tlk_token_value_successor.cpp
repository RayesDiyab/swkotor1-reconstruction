// lane-m48-traps / f-token: CSWTlkTable::GetTokenValue 00674B90 (vtable 0074E700 slot 0, called by
// CTlkTable::ParseStr 0041E006 for every non-CUSTOM token) and the menu-reachable callees it needs:
//   005EDEB0 CClientExoApp::GetGUIString            005FB540 CClientExoAppInternal::GetGUIString
//   005F7D00 / 005F50F0 / 005F5020 CExoArrayList<CGuiStringStruct>::Insert / Add / Allocate
//   005ED6D0 CClientExoApp::GetPlayerByPlayerID      005F5C70 CClientExoAppInternal::GetPlayerByPlayerID
//   005EE280 CClientExoApp::GetDisplayCharacterForKey 005F4DF0 CClientExoAppInternal::GetDisplayCharacterForKey
//   005DF640 CExoInput::GetDisplayCharacterForKey    005E0320 CExoInputInternal::GetDisplayCharacterForKey
//   005E3D10 CExoRawInputInternal::GetDisplayCharacterDirectInputValue
//   005E6690 CExoBase::WcsToMbs (-> 005E8B20 CExoBaseInternal::WcsToMbs)
// Every original thiscall is exposed as __fastcall(receiver, unused EDX, ...) (D1); stack cleanup matches the
// original RET n. The original object-token branch00674CAC is supplied by the feedback lower closure.
// UCRT wcslen/wctomb stand in for the original CRT 006FD83E/006FD817 (FORK.json equivalence_records).
#include "3f0abd0978-tlk_core.h"
#include "8c34f546dd-native_strings.h"      // WindowCopy005e5b00 (005E5B00)
#include "81f59c47ba-string_priority.h"     // GuiAssign005e5140 (005E5140)
#include "03e67ad358-window_shared_cells.h" // kotor_menu_window_cells::manager_007a39fc / input_007a39e4
#include "6542d604d6-allocator_live.h"      // live_operator_new (006FA7E6), live_free (006FA390)
#include "897d394ced-feedback_token_object.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <new>

namespace menu_tables_m41 {   // identical declaration of menu-tables d1_bodies.h (eh vector constructor iterator 006FB55F)
using Constructor=void*(__fastcall*)(void*,void*);
using Destructor=void(__fastcall*)(void*,void*);
void __stdcall VectorConstruct_006fb55f(void* destination,std::uint32_t size,std::uint32_t count,Constructor ctor,Destructor dtor);
}
extern "C" {
void* __fastcall tlk_token_construct_0041d910(void* element, void*);          // startup46 tlk_custom_token.cpp (0041D910)
void __fastcall tlk_token_destroy_0059ea20(void* element, void*);             // startup46 tlk_custom_token.cpp (0059EA20)
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                // r4_lowers.h: CClientExoApp::GetModule
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                // event_descriptions.h: CClientExoApp::GetClientOptions
void __fastcall frame_LogStringCtor_005e5a90(kotor_reconstruction::StartupString*, void*, const char*) noexcept(false); // CExoString(char const*)
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**); // CExoLinkedListInternal::GetNext
}
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // M41 canonical-assignment-format
extern "C" void* __fastcall gui_m48_input_get_event_005df4c0(void* input, void*, std::uint32_t index, int input_class); // lane-m48-b input_m48.cpp (005DF4C0)

namespace {
using Word=std::uint32_t;
using I32=std::int32_t;
using String=kotor_reconstruction::StartupString;
Word w(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object)+offset, 4); return v; }
unsigned char b(const void* object, Word offset) { return static_cast<const unsigned char*>(object)[offset]; }
void put(void* object, Word offset, Word value) { std::memcpy(static_cast<unsigned char*>(object)+offset, &value, 4); }
void* p(Word value) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value)); }
void* client_app() { return p(w(kotor_menu_window_cells::manager_007a39fc, 4)); }   // [007A39FC]+4 (CClientExoApp*)
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
String* at_string(void* object, Word offset) { return std::launder(reinterpret_cast<String*>(static_cast<unsigned char*>(object)+offset)); }
// Return-slot CExoString (8 bytes) constructed by the callee; destroyed (005E5C20) when the EH state that owns it
// ends. `live` mirrors the original EH state byte: it is set only once the callee has constructed the slot.
struct Slot {
    alignas(4) unsigned char raw[8];
    bool live=false;
    String* get() { return std::launder(reinterpret_cast<String*>(raw)); }
    kotor_menu_10_19::String* view() { return reinterpret_cast<kotor_menu_10_19::String*>(raw); }
    ~Slot() { if(live) get()->~String(); }
};
// .rdata constants (read-only image words)
constexpr Word kHoursPerDay_0074416c=0x18;     // 006750EA
constexpr I32 kFirstKey_0074d3d8=6;            // 005F4DF5
constexpr I32 kKeyLimit_0074d3dc=0x6E;         // 005F4E06
const char kEmpty_0073d71c[]="";               // 006751CD/006751EA/006751FC push 73D71Ch
const char kCharFormat_0073e2f0[]="%c";        // 00675274
const char kNotAvailable_0074d70c[]="N/A";     // 005E3DA2
}

// CGuiStringStruct (0Ch): +0 strref, +4 CExoString. Passed by value to Insert/Add (caller copy-constructs id then
// text with 005E5B00; the callee destroys the text: 005F7DA4 / 005F515B).
struct GuiStringStruct {
    Word id;
    alignas(1) unsigned char text_raw[8];
    String* text() { return std::launder(reinterpret_cast<String*>(text_raw)); }
    const String* text() const { return std::launder(reinterpret_cast<const String*>(text_raw)); }
    GuiStringStruct() { ::new (static_cast<void*>(text_raw)) String(); }                       // 005B3190 (005FB5F3)
    GuiStringStruct(const GuiStringStruct& other) : id(other.id) {                              // 005FB66C / 005F7D30
        WindowCopy005e5b00(reinterpret_cast<String*>(text_raw), nullptr, other.text());        // 005FB672 / 005F7D3E
    }
    GuiStringStruct& operator=(const GuiStringStruct&)=delete;
    ~GuiStringStruct() { text()->~String(); }                                                  // 005E5C20
};
static_assert(sizeof(GuiStringStruct)==12, "CGuiStringStruct");

extern "C" {
void __fastcall kotor_fastcall_005f5020(void* list, void*, I32 capacity) noexcept(false);
void __fastcall kotor_fastcall_005f50f0(void* list, void*, GuiStringStruct element) noexcept(false);
void __fastcall kotor_fastcall_005f7d00(void* list, void*, GuiStringStruct element, I32 index) noexcept(false);
String* __fastcall kotor_fastcall_005fb540(void* internal, void*, String* out, Word strref) noexcept(false);
String* __fastcall kotor_fastcall_005edeb0(void* app, void*, String* out, Word strref) noexcept(false);
void* __fastcall kotor_fastcall_005f5c70(void* internal, void*, Word player_id) noexcept(false);
void* __fastcall kotor_fastcall_005ed6d0(void* app, void*, Word player_id) noexcept(false);
int __fastcall kotor_fastcall_005e8b20(void* base_internal, void*, char* destination, const wchar_t* source, Word limit) noexcept(false);
int __fastcall kotor_fastcall_005e6690(void* base, void*, char* destination, const wchar_t* source, Word limit) noexcept(false);
String* __fastcall kotor_fastcall_005e3d10(void* raw_input, void*, String* out, Word object) noexcept(false);
String* __fastcall kotor_fastcall_005e0320(void* input_internal, void*, String* out, I32 key) noexcept(false);
String* __fastcall kotor_fastcall_005df640(void* input, void*, String* out, I32 key) noexcept(false);
String* __fastcall kotor_fastcall_005f4df0(void* internal, void*, String* out, I32 key) noexcept(false);
String* __fastcall kotor_fastcall_005ee280(void* app, void*, String* out, I32 key) noexcept(false);

// ---- 005F5020 CExoArrayList<CGuiStringStruct>::Allocate(int) RET 4.
void __fastcall kotor_fastcall_005f5020(void* list, void*, I32 capacity) noexcept(false) {
    unsigned char* const old=static_cast<unsigned char*>(p(w(list,0)));               // 005F503F ebx=[edi]
    put(list,8,static_cast<Word>(capacity));                                             // 005F504C
    void* const block=live_operator_new(static_cast<Word>(capacity)*12u+4u);           // 005F5041..005F504F (006FA7E6)
    unsigned char* next=nullptr;                                                         // 005F505B
    if(block) {                                                                          // 005F505D..005F5063
        put(block,0,static_cast<Word>(capacity));                                       // 005F5076 count cookie
        next=static_cast<unsigned char*>(block)+4;                                       // 005F5070
        menu_tables_m41::VectorConstruct_006fb55f(next,12,static_cast<Word>(capacity),
            &tlk_token_construct_0041d910,&tlk_token_destroy_0059ea20);                // 005F5065..005F5078 (006FB55F, 0041D910, 0059EA20)
    }
    const I32 count=static_cast<I32>(w(list,4));                                         // 005F507D
    put(list,0,static_cast<Word>(reinterpret_cast<std::uintptr_t>(next)));              // 005F5080
    for(I32 i=0;i<count;) {                                                              // 005F5084..005F50B1
        unsigned char* const to=static_cast<unsigned char*>(p(w(list,0)))+i*12;        // 005F5092..005F5097 ([edi] reloaded)
        put(to,0,w(old+i*12,0));                                                         // 005F5094/005F50A1
        at_string(to,4)->Assign005e5c50(at_string(old+i*12,4));                          // 005F5099..005F50A3
        ++i;                                                                             // 005F50AB
        if(i>=static_cast<I32>(w(list,4)))break;                                         // 005F50A8/005F50AF ([edi+4] reloaded)
    }
    if(old) {                                                                            // 005F50B3..005F50B5
        const Word n=w(old-4,0);                                                         // 005F50B7
        for(Word k=n;k!=0;--k)tlk_token_destroy_0059ea20(old+(k-1)*12,nullptr);         // 005F50C6 006FA852(old,0Ch,n,0059EA20): last to first
        live_free(old-4);                                                                // 005F50CC (006FA390)
    }
}

// ---- 005F50F0 CExoArrayList<CGuiStringStruct>::Add(CGuiStringStruct) RET 0Ch.
void __fastcall kotor_fastcall_005f50f0(void* list, void*, GuiStringStruct element) noexcept(false) {
    const Word capacity=w(list,8);                                                       // 005F5108
    if(w(list,4)==capacity)                                                              // 005F510B..005F5116
        kotor_fastcall_005f5020(list,nullptr,static_cast<I32>(capacity?capacity+capacity:0x10u)); // 005F5118..005F5128
    const Word n=w(list,4);                                                              // 005F512D
    unsigned char* const slot=static_cast<unsigned char*>(p(w(list,0)))+n*12;           // 005F5130..005F5136
    put(list,4,n+1);                                                                     // 005F5135/005F5139
    put(slot,0,element.id);                                                              // 005F513C/005F5144
    at_string(slot,4)->Assign005e5c50(element.text());                                   // 005F5140..005F514A
}                                                                                        // 005F515B element.text destroyed

// ---- 005F7D00 CExoArrayList<CGuiStringStruct>::Insert(CGuiStringStruct, int) RET 10h.
void __fastcall kotor_fastcall_005f7d00(void* list, void*, GuiStringStruct element, I32 index) noexcept(false) {
    const I32 old_count=static_cast<I32>(w(list,4));                                     // 005F7D20
    kotor_fastcall_005f50f0(list,nullptr,element);                                       // 005F7D23..005F7D45 (copy via 005E5B00)
    if(old_count>index) {                                                                // 005F7D4E..005F7D50
        Word offset=static_cast<Word>(old_count)*12u;                                    // 005F7D52..005F7D55
        for(I32 left=old_count-index;left!=0;--left) {                                   // 005F7D58 / 005F7D79..005F7D7A
            unsigned char* const data=static_cast<unsigned char*>(p(w(list,0)));        // 005F7D60 ([ebx] reloaded)
            put(data+offset,0,w(data+offset-12,0));                                      // 005F7D62..005F7D6B
            at_string(data+offset,4)->Assign005e5c50(at_string(data+offset-12,4));      // 005F7D68..005F7D71
            offset-=12;                                                                  // 005F7D76
        }
    }
    unsigned char* const slot=static_cast<unsigned char*>(p(w(list,0)))+index*12;       // 005F7D7C..005F7D86
    put(slot,0,element.id);                                                              // 005F7D7E/005F7D91
    at_string(slot,4)->Assign005e5c50(element.text());                                   // 005F7D89..005F7D93
}                                                                                        // 005F7DA4 element.text destroyed

// ---- 005FB540 CClientExoAppInternal::GetGUIString(CExoString* out, ulong strref) RET 8.
// Cache: CExoArrayList<CGuiStringStruct> at +358h (data), +35Ch (count). The search keeps the original's
// exclusive upper bound count-1 (005FB563..005FB570) and its comparison direction (005FB5D6 jae -> lo=mid+1).
String* __fastcall kotor_fastcall_005fb540(void* internal, void*, String* out, Word strref) noexcept(false) {
    Word found=0;                                                                        // 005FB571 [esp+10h]
    I32 high=static_cast<I32>(w(internal,0x35c))-1;                                      // 005FB563..005FB570 ebp
    I32 low=0;                                                                           // 005FB56A edi
    I32 middle=0;                                                                        // 005FB55B esi
    String result;                                                                       // 005FB56C..005FB575 (005B3190) EH 1
    if(high>0) {                                                                         // 005FB57A..005FB584
        do {
            if(found)break;                                                              // 005FB590..005FB596
            middle=(high-low)/2+low;                                                     // 005FB5A2..005FB5B1 (cdq/sub/sar)
            unsigned char* const element=static_cast<unsigned char*>(p(w(internal,0x358)))+middle*12; // 005FB59C..005FB5B6
            const Word key=w(element,0);                                                 // 005FB5B9
            if(key==strref) { result.Assign005e5c50(at_string(element,4)); found=1; }   // 005FB5BB..005FB5CC
            else if(key>=strref) low=middle+1;                                           // 005FB5D6 jae -> 005FB5DC
            else high=middle;                                                            // 005FB5D8
        } while(low<high);                                                               // 005FB5DF..005FB5E1
    }
    if(!found) {                                                                         // 005FB5E3..005FB5E9
        GuiStringStruct entry;                                                           // 005FB5EF..005FB5F3 text (005B3190) EH 2
        {
            Slot fetched;                                                                // [esp+10h] reused as return slot
            tlk()->simple_0041e8f0(fetched.view(),strref); fetched.live=true;           // 005FB5FC..005FB60D (0041E8F0) EH 3
            result.Assign005e5c50(fetched.get());                                        // 005FB612..005FB61C
        }                                                                                // 005FB621..005FB62A
        entry.id=strref;                                                                 // 005FB638
        entry.text()->Assign005e5c50(&result);                                           // 005FB62F..005FB63C
        if(static_cast<I32>(w(internal,0x35c))>0 &&                                      // 005FB641..005FB649
           w(static_cast<unsigned char*>(p(w(internal,0x358)))+middle*12,0)<strref)      // 005FB64B..005FB657 (jae skips)
            ++middle;                                                                    // 005FB659
        kotor_fastcall_005f7d00(static_cast<unsigned char*>(internal)+0x358,nullptr,entry,middle); // 005FB65A..005FB67D
    }                                                                                    // 005FB682..005FB68B
    tlk_core::ParseStr0041dd30(tlk(),nullptr,&result);                                   // 005FB690..005FB69B
    WindowCopy005e5b00(out,nullptr,&result);                                             // 005FB6A0..005FB6AB
    return out;                                                                          // 005FB6C1 result destroyed; EAX=out
}

// ---- 005EDEB0 CClientExoApp::GetGUIString(CExoString* out, ulong strref) RET 8: forwards to [this+4].
String* __fastcall kotor_fastcall_005edeb0(void* app, void*, String* out, Word strref) noexcept(false) {
    kotor_fastcall_005fb540(p(w(app,4)),nullptr,out,strref);                             // 005EDEB1..005EDEC7
    return out;                                                                          // 005EDECC
}

// ---- 005F5C70 CClientExoAppInternal::GetPlayerByPlayerID(ulong) RET 4: walk the player list at [+1Ch].
void* __fastcall kotor_fastcall_005f5c70(void* internal, void*, Word player_id) noexcept(false) {
    void* const list=p(w(internal,0x1c));                                                // 005F5C74
    void* position=p(w(list,0));                                                         // 005F5C77..005F5C79 head node
    Word data=w(list,0);                                                                 // 005F5C7D
    if(data)data=w(p(data),8);                                                           // 005F5C7F..005F5C84 node->data
    while(data) {                                                                        // 005F5C87..005F5C89
        if(w(p(data),4)==player_id)return p(data);                                       // 005F5C90..005F5C93
        data=unresolved_list_next_005e9b90(p(w(internal,0x1c)),nullptr,&position);       // 005F5C95..005F5C9D
    }                                                                                    // 005F5CA2..005F5CA4
    return nullptr;                                                                      // 005F5CA6
}
// ---- 005ED6D0 CClientExoApp::GetPlayerByPlayerID: mov ecx,[ecx+4]; jmp 005F5C70.
void* __fastcall kotor_fastcall_005ed6d0(void* app, void*, Word player_id) noexcept(false) {
    return kotor_fastcall_005f5c70(p(w(app,4)),nullptr,player_id);
}

// ---- 005E8B20 CExoBaseInternal::WcsToMbs(char*, wchar_t const*, ulong) RET 0Ch (ECX unused), EAX=1.
int __fastcall kotor_fastcall_005e8b20(void*, void*, char* destination, const wchar_t* source, Word limit) noexcept(false) {
    Word written=0;                                                                      // 005E8B29 ebx
    const Word length=static_cast<Word>(std::wcslen(source));                           // 005E8B28..005E8B33 (006FD83E -> UCRT)
    for(Word i=0;;) {                                                                    // 005E8B37 edi
        const Word bound=limit<length?limit:length;                                      // 005E8B40..005E8B48
        if(i>=bound)break;                                                               // 005E8B4A..005E8B4C
        char* const at=destination+written;                                              // 005E8B59 esi
        const int r=std::wctomb(at,source[i]);                                           // 005E8B54..005E8B5E (006FD817 -> UCRT)
        const wchar_t c=source[i];                                                       // 005E8B6B
        if(r==-1||(c>=1&&c<=0x1f)) { ++written; *at=0x7f; }                              // 005E8B66..005E8B7A / 005E8B85..005E8B8A
        else written+=static_cast<Word>(r);                                              // 005E8B7C
        ++i;                                                                             // 005E8B82 / 005E8B8D
    }
    return 1;                                                                            // 005E8B93
}
// ---- 005E6690 CExoBase::WcsToMbs: mov ecx,[ecx+14h]; jmp 005E8B20.
int __fastcall kotor_fastcall_005e6690(void* base, void*, char* destination, const wchar_t* source, Word limit) noexcept(false) {
    return kotor_fastcall_005e8b20(p(w(base,0x14)),nullptr,destination,source,limit);
}

// ---- 005E3D10 CExoRawInputInternal::GetDisplayCharacterDirectInputValue(CExoString* out, ulong) RET 8.
String* __fastcall kotor_fastcall_005e3d10(void* raw_input, void*, String* out, Word object) noexcept(false) {
    DIPROPSTRING property;                                                               // [esp+58h] 0x218 bytes
    property.diph.dwObj=object;                                                          // 005E3D23
    property.diph.dwSize=0x218;                                                          // 005E3D2C
    property.diph.dwHeaderSize=0x10;                                                     // 005E3D34
    property.diph.dwHow=DIPH_BYOFFSET;                                                   // 005E3D3C (1)
    IDirectInputDevice8A* const device=static_cast<IDirectInputDevice8A*>(p(w(raw_input,0x20))); // 005E3D27
    const HRESULT hr=device->GetProperty(DIPROP_KEYNAME,&property.diph);                 // 005E3D1E..005E3D4F vslot +14h, prop 14h
    if(hr<0) {                                                                           // 005E3D52..005E3D54
        frame_LogStringCtor_005e5a90(out,nullptr,kNotAvailable_0074d70c);                // 005E3DA2..005E3DA9 (005E5A90)
        return out;
    }
    char text[0x50];                                                                     // [esp+0Ch]
    std::memset(text,0,sizeof text);                                                     // 005E3D57..005E3D62 rep stos 14h dwords
    kotor_fastcall_005e6690(kotor_menu_10_19::dependencies::base_007a39e0,nullptr,text,property.wsz,0x4f); // 005E3D64..005E3D76
    frame_LogStringCtor_005e5a90(out,nullptr,text);                                      // 005E3D7B..005E3D89
    return out;
}
// ---- 005E0320 CExoInputInternal::GetDisplayCharacterForKey(CExoString*, int) RET 8.
String* __fastcall kotor_fastcall_005e0320(void* input_internal, void*, String* out, I32 key) noexcept(false) {
    const Word object=w(input_internal,0x164+static_cast<Word>(key)*4u);                 // 005E0325 key map
    kotor_fastcall_005e3d10(p(w(input_internal,0x140)),nullptr,out,object);              // 005E032C..005E0341
    return out;
}
// ---- 005DF640 CExoInput::GetDisplayCharacterForKey(CExoString*, int) RET 8: forwards to [this+4].
String* __fastcall kotor_fastcall_005df640(void* input, void*, String* out, I32 key) noexcept(false) {
    kotor_fastcall_005e0320(p(w(input,4)),nullptr,out,key);                              // 005DF641..005DF657
    return out;
}
// ---- 005F4DF0 CClientExoAppInternal::GetDisplayCharacterForKey(CExoString*, int) RET 8.
String* __fastcall kotor_fastcall_005f4df0(void*, void*, String* out, I32 key) noexcept(false) {
    if(key>=kFirstKey_0074d3d8&&key<kKeyLimit_0074d3dc) {                                // 005F4DF1..005F4E0C (signed)
        kotor_fastcall_005df640(kotor_menu_window_cells::input_007a39e4,nullptr,out,key); // 005F4E0E..005F4E1A
        return out;
    }
    tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out),0xbc10);     // 005F4E26..005F4E36
    return out;
}
// ---- 005EE280 CClientExoApp::GetDisplayCharacterForKey(CExoString*, int) RET 8: forwards to [this+4].
String* __fastcall kotor_fastcall_005ee280(void* app, void*, String* out, I32 key) noexcept(false) {
    kotor_fastcall_005f4df0(p(w(app,4)),nullptr,out,key);                                // 005EE281..005EE297
    return out;
}
}   // extern "C"

namespace {
// value = GetGUIString(strref) through a return slot destroyed after the assignment (e.g. 00674C17..00674C34).
void assign_gui_string(String* value, Word strref) {
    Slot slot;
    kotor_fastcall_005edeb0(client_app(),nullptr,slot.get(),strref); slot.live=true;
    value->Assign005e5c50(slot.get());
}
void assign_int(String* value, Word number) {                                           // 005E5BC0 then 005E5C50
    Slot slot;
    resman_string_from_int_005e5bc0(slot.get(),nullptr,static_cast<int>(number)); slot.live=true;
    value->Assign005e5c50(slot.get());
}
void assign_simple(String* value, Word strref) {                                        // 0041E8F0 then 005E5C50
    Slot slot;
    tlk()->simple_0041e8f0(slot.view(),strref); slot.live=true;
    value->Assign005e5c50(slot.get());
}
void assign_key(String* value, I32 key) {                                               // 005EE280 then 005E5C50
    Slot slot;
    kotor_fastcall_005ee280(client_app(),nullptr,slot.get(),key); slot.live=true;
    value->Assign005e5c50(slot.get());
}
Word token_word(const tlk_core::Token* token, Word offset) { return w(token,offset); }
// CExoInputEventDesc virtual slot +4 (int (void)) read through a typed table (D1 slot form).
struct EventDescMethods { void* slot0; int (__fastcall* slot4)(void*, void*); };
// 00675460: token type -> case (0..0Fh); 00675420: case -> target.
const unsigned char kCaseOfType_00675460[0x1b]={0,1,2,2,2,2,2,2,2,2,2,2,2,15,2,3,4,5,6,7,8,9,10,11,12,13,14};
// 006754B0 / 006754C0: control-option string tables for type 1Ah (targets 0067539D=BD63, 006753A4=BD62,
// 006753AB=BD65, 006753B2=BD64).
const Word kKeyStrref_006754b0[4]={0xbd63,0xbd62,0xbd65,0xbd64};
const Word kKeyStrref_006754c0[4]={0xbd62,0xbd63,0xbd64,0xbd65};
}

namespace tlk_core {
// ---- 00674B90 CSWTlkTable::GetTokenValue(CTlkTableToken*, CExoString*) RET 8.
// Token: +0Ch type (00674BDC), +10h..+1Ch per-variant strrefs, +20h argument (tlk_core::Token).
// CSWTlkTable: +3Ch gender byte, +50h/+5Ch flags, +58h game object (Fetch 005EE3A0), +60h player id (005F89A1).
void __fastcall SwTokenValue00674b90(Tlk* self, void*, Token* token, String* value) {
    String unused_string;                                                                // 00674BB6..00674BBD (005B3190) [esp+CCh]
    kotor_menu_10_19::ResRef unused_ref; unused_ref.default_00405ed0();                  // 00674BC2..00674BD4 (00405ED0)
    const Word type=token_word(token,0x0c);                                              // 00674BD9..00674BDC
    if(type>0x1a)return;                                                                 // 00674BDF..00674BEE -> 006753EF
    switch(kCaseOfType_00675460[type]) {                                                 // 00674BF4..00674BFB
    case 0:                                                                              // 00674C02
        assign_gui_string(value,token_word(token,0x10));                                 // 00674C02..00674C34
        return;
    case 1: {                                                                            // 00674C39
        const Word gender=b(self,0x3c);                                                  // 00674C39
        assign_gui_string(value,token_word(token,0x10+gender*4));                        // 00674C3D..00674C6A
        return;
    }
    case 2: {                                                                            // 00674C6F
        if(w(self,0x58)==0) {                                                            // 00674C6F..00674C74
            assign_gui_string(value,token_word(token,0x20));                             // 00674C76..00674CA7
            return;
        }
        feedback_lowers::token_object(self,token,value);
        return;
    }
    case 3: {                                                                            // 00675065 module +A4h
        void* const module=kotor_fastcall_005ed530(client_app(),nullptr);               // 00675065..0067506E
        assign_int(value,w(module,0xa4));                                                // 00675073..0067509C
        return;
    }
    case 5: {                                                                            // 006750A1 module +A8h
        void* const module=kotor_fastcall_005ed530(client_app(),nullptr);               // 006750A1..006750A9
        assign_int(value,w(module,0xa8));                                                // 006750AE..006750D7
        return;
    }
    case 4: {                                                                            // 006750DC 12-hour clock
        void* const module=kotor_fastcall_005ed530(client_app(),nullptr);               // 006750DC..006750E5
        const Word half=kHoursPerDay_0074416c>>1;                                        // 006750EA..006750F6
        const Word hour=(half+w(module,0xb0)-1u)%half+1u;                                // 006750F0..00675104 (unsigned div)
        assign_int(value,hour);                                                          // 00675100..00675124
        return;
    }
    case 6: {                                                                            // 00675129 time-of-day phase
        const Word dawn=b(kotor_fastcall_005ed530(client_app(),nullptr),0xc0);          // 00675129..00675137
        const Word dusk=b(kotor_fastcall_005ed530(client_app(),nullptr),0xc1);          // 0067513D..0067514E
        const Word hour=w(kotor_fastcall_005ed530(client_app(),nullptr),0xb0);          // 00675154..0067516B
        unsigned char phase=0;                                                           // 0067518E
        if(hour>=dawn&&hour<=dusk)                                                       // 00675171..0067517C (unsigned)
            phase=static_cast<unsigned char>(static_cast<unsigned char>(((hour-dawn)*3u)/(dusk-dawn+1u))+1u); // 0067517E..0067518A
        assign_gui_string(value,token_word(token,0x10+static_cast<Word>(phase)*4));      // 00675190..006751C0
        return;
    }
    case 7:                                                                              // 006751C5
        put(self,0x50,w(self,0x5c)&1u);                                                  // 006751C5..006751D2
        GuiAssign005e5140(value,nullptr,kEmpty_0073d71c);                                // 006751C8..006751D5
        return;
    case 8:                                                                              // 006751DF
        put(self,0x50,w(self,0x5c)&1u);                                                  // 006751DF..006751E4
        GuiAssign005e5140(value,nullptr,kEmpty_0073d71c);                                // 006751E7..006751EF
        return;
    case 9:                                                                              // 006751E7 (enters case 8's tail)
        GuiAssign005e5140(value,nullptr,kEmpty_0073d71c);                                // 006751E7..006751EF
        return;
    case 10:                                                                             // 006751F9
        GuiAssign005e5140(value,nullptr,kEmpty_0073d71c);                                // 006751F9..00675201
        put(self,0x50,0);                                                                // 00675206
        return;
    case 11: {                                                                           // 00675212 player name
        void* const player=kotor_fastcall_005ed6d0(client_app(),nullptr,w(self,0x60));   // 00675212..0067521F
        if(!player) { assign_gui_string(value,token_word(token,0x20)); return; }         // 00675224..00675257
        value->Assign005e5c50(at_string(player,0x0c));                                   // 0067525C..00675263
        return;
    }
    case 12:                                                                             // 0067526D
        kotor_reconstruction::StartupFormat005e5680(value,kCharFormat_0073e2f0,token_word(token,0x20)); // 0067526D..0067527F
        return;
    case 13: {                                                                           // 00675287 key binding
        const I32 variant=static_cast<I32>(token_word(token,0x10));                      // 00675287/0067529D [esp+10h]
        const Word key=token_word(token,0x20);                                           // 0067528A
        assign_simple(value,0);                                                          // 0067528D..006752C7 (strref pushed as 0)
        void* const event=gui_m48_input_get_event_005df4c0(kotor_menu_window_cells::input_007a39e4,nullptr,key,-1); // 006752CC..006752D5
        if(!event)return;                                                                // 006752DA..006752DE
        const EventDescMethods* methods; std::memcpy(&methods,event,4);                  // 006752E4
        Word code;
        if(methods->slot4(event,nullptr)!=0&&variant>1) code=w(event,0x30);              // 006752E6..006752F5
        else code=w(event,0x1c);                                                         // 00675332
        if(!code)return;                                                                 // 006752F8..006752FA / 00675335..00675337
        assign_key(value,static_cast<I32>(code));                                        // 00675300..0067532D / 0067533D..0067536A
        return;
    }
    case 14: {                                                                           // 0067536C control option
        const Word option=token_word(token,0x20);                                        // 00675374
        Word strref=0;                                                                   // 00675377
        void* const options=kotor_fastcall_005ed700(client_app(),nullptr);               // 0067536C..00675379
        if(option<=3) strref=w(options,0x54)==0?kKeyStrref_006754b0[option]:kKeyStrref_006754c0[option]; // 0067537E..006753B2
        assign_simple(value,strref);                                                     // 006753B7..006753E3
        return;
    }
    default:                                                                             // 15 (type 0Dh) -> 006753EF
        return;
    }
}
}
