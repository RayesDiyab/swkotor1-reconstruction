#pragma once
namespace async_load {struct Ref;struct String;}
extern "C" async_load::Ref* __fastcall async_ref_construct_string_00406d60(async_load::Ref*,void*,async_load::String*);
extern "C" void __fastcall recovered_vm_stack_clear_005d2140(void*,void*);
extern "C" void __fastcall recovered_vm_stack_shrink_005d21f0(void*,void*,int);
extern "C" unsigned __fastcall recovered_vm_script_delete_005d44f0(void*,void*,void*);
extern "C" unsigned __fastcall frame_LogIsEmpty_005e58a0(void*,void*);

// Legacy guest-word adapters retain raw transport, then call the exact nominal API.
inline void* async_ref_construct_string_00406d60(void*a,void*b,void*c){return async_ref_construct_string_00406d60(static_cast<async_load::Ref*>(a),b,static_cast<async_load::String*>(c));}
