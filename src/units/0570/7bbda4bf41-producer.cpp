#include "10d1301a0a-table_contract.h"
#include "7b8c81fcff-allocator_live.h"
#include "c7e191db10-native_strings.h"
#include "689940810d-string_priority.h"
#include <initializer_list>

using namespace c2da_current;

namespace c2da_current {
namespace {
// Exactly the two observed slot0 identities. There are no guessed extra slots.
const HelperSlot0 helper_methods{&c2da_current_helper_delete_00413ca0};
const TableSlot0 table_methods{&c2da_current_delete_00414a30};
}
const HelperSlot0* initial_methods() noexcept {return &helper_methods;}
const TableSlot0* final_methods() noexcept {return &table_methods;}
}

extern "C" __declspec(noinline) Table* __cdecl
c2da_current_construct_from_stack_00413cc0(void* storage,Ref* incoming,Word flag) {
    Table* self=::new(storage) Table;
    Helper& helper=self->helper;
    helper.methods=initial_methods();
    (void)c2da_default_ref_00405ed0(&helper.reference,nullptr);
    Ref result;
    helper.resource=nullptr;
    (void)unresolved_ref_assign_00406290(&helper.reference,nullptr,&result,c2da_empty_0073d71c());
    helper.requested=0;
    helper.methods=final_methods();
    String* text=::new(self->string_storage) String();
    for(unsigned offset: {0x38u,0x30u,0x34u,0x24u,0x28u})store_word(self,offset,0);
    (void)GuiAssign005e5140(text,nullptr,c2da_empty_0073d71c());
    for(unsigned offset: {0x2cu,0x3cu,0x40u,0x44u,0x4cu,0x50u,0x48u})store_word(self,offset,0);
    helper.SetResRef(incoming,flag);
    return self;
}

// Ref remains the actual by-value object in entryESP+4..+19. The bridge creates
// no detached parameter copy and leaves the original flag's full32 bits intact.
extern "C" __declspec(naked) Table* __fastcall
c2da_current_construct_00413cc0(void*,void*,Ref,Word) {
    __asm {
        mov eax,esp
        push dword ptr[eax+14h]
        lea edx,[eax+4]
        push edx
        push ecx
        call c2da_current_construct_from_stack_00413cc0
        add esp,12
        ret 20
    }
}

extern "C" __declspec(noinline) void __fastcall
c2da_current_helper_destroy_00413ae0(Helper* self,void*) {
    void* first=self->resource; // Original resource load precedes vtable write.
    self->methods=initial_methods();
    if(!first || !c2da_not_equal_text_00406060(&self->reference,nullptr,c2da_empty_0073d71c()))return;
    if(self->requested) {
        c2da_cancel_00409b00(self->resource,nullptr);
        self->requested=0;
    }
    void* releasing=self->resource;
    menu_resources::Manager* manager=c2da_manager_007a39e8();
    if(!c2da_release_00409cf0(manager,nullptr,releasing)) {
        void* current=self->resource;
        if(current) {
            using Delete=void(__thiscall*)(void*,Word);
            const unsigned char* methods=nullptr;std::memcpy(&methods,current,4);
            Delete deleting=nullptr;std::memcpy(&deleting,methods,4);
            deleting(current,1);
        }
        self->resource=nullptr;
    }
}

namespace {
void destroy_array(void* array) {
    unsigned char* header=static_cast<unsigned char*>(array)-4;
    Word count;std::memcpy(&count,header,4);
    c2da_destroy_array_006fa852(array,8,count,&c2da_current_string_destroy_005e5c20);
    live_free(header);
}
bool signed_less(Word left,Word right) {
    std::int32_t a,b;std::memcpy(&a,&left,4);std::memcpy(&b,&right,4);return a<b;
}
}

extern "C" __declspec(noinline) void __fastcall
c2da_current_unload_004139e0(Table* self,void*) {
    if(!load_word(self,0x2c))return;
    if(load_word(self,0x38)) {
        Word row=0;
        if(signed_less(row,load_word(self,0x24))) {
            do {
                unsigned char* rows=load_pointer<unsigned char>(self,0x38);
                void* array;std::memcpy(&array,rows+row*4u,4);
                if(array)destroy_array(array);
                ++row;
            } while(signed_less(row,load_word(self,0x24)));
        }
        live_free(load_pointer<void>(self,0x38));
        store_word(self,0x38,0);
    }
    for(unsigned offset:{0x34u,0x30u}) {
        void* array=load_pointer<void>(self,offset);
        if(array){destroy_array(array);store_word(self,offset,0);}
    }
    store_word(self,0x24,0);store_word(self,0x28,0);
    (void)GuiAssign005e5140(default_string(self),nullptr,c2da_empty_0073d71c());
    c2da_release_demand_00409b80(self->helper.resource,nullptr);
    void* first=load_pointer<void>(self,0x4c);
    for(unsigned offset:{0x2cu,0x3cu,0x40u,0x44u})store_word(self,offset,0);
    live_free(first);
    void* second=load_pointer<void>(self,0x50);
    store_word(self,0x4c,0);
    live_free(second);
    store_word(self,0x50,0);store_word(self,0x48,0);
}

extern "C" __declspec(noinline) void __fastcall
c2da_current_destroy_00413d80(Table* self,void*) {
    self->helper.methods=final_methods();
    c2da_current_unload_004139e0(self,nullptr);
    default_string(self)->~StartupString();
    c2da_current_helper_destroy_00413ae0(&self->helper,nullptr);
    self->~Table();
}

extern "C" __declspec(noinline) Table* __fastcall
c2da_current_delete_00414a30(Table* self,void*,Word flags) {
    c2da_current_destroy_00413d80(self,nullptr);
    if(flags&1u)live_free(self);
    return self;
}

extern "C" __declspec(noinline) Helper* __fastcall
c2da_current_helper_delete_00413ca0(Helper* self,void*,Word flags) {
    c2da_current_helper_destroy_00413ae0(self,nullptr);
    self->~Helper();
    if(flags&1u)live_free(self);
    return self;
}

// Actual canonical String destruction for the imported array iterator. It is a
// physical ECX/unusedEDX RET0 callback, with no algorithm or extra lifetime copy.
extern "C" __declspec(noinline) void __fastcall
c2da_current_string_destroy_005e5c20(String* self,void*) {self->~StartupString();}
