// lane-m46-startup: CExoResMan removal leaves referenced by routes-lane shutdown_bodies.cpp
// (kotor_original_calls.h: kotor_fastcall_004088d0/004088c0/00408830). Evidence: dis/004088c0.asm,
// dis/004088d0.asm, dis/00408830.asm (+ tail 004088A3..004088B5).
// 00408830 is the image-file (kind 4, list +1Ch) specialisation of RemoveKeyTable 00407830; its body uses the
// same genuine list/key owners as the M45 next-remove-current 00407830 reconstruction.
#include "1ffddd3d85-remove_current.h"
#include "d08b97bcca-allocator_live.h"
#include <new>

extern "C" {
// routes-lane kotor_original_calls.h:155-157 (identical declarations).
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);
void __fastcall kotor_fastcall_004088c0(void*, void*, void*);
void __fastcall kotor_fastcall_00408830(void*, void*, void*);
}

// 004088C0..004088CC RemoveFixedKeyTableFile(name): push 1; push name; call 00407830 (ECX = this); RET 4.
extern "C" void __fastcall kotor_fastcall_004088c0(void* manager, void*, void* name) {
    (void)async_remove_key_00407830(static_cast<async_load::Manager*>(manager), nullptr, static_cast<async_load::String*>(name), 1);
}
// 004088D0..004088DC RemoveResourceDirectory(name): same with kind 2.
extern "C" void __fastcall kotor_fastcall_004088d0(void* manager, void*, void* name) {
    (void)async_remove_key_00407830(static_cast<async_load::Manager*>(manager), nullptr, static_cast<async_load::String*>(name), 2);
}

// 00408830..004088B5 RemoveResourceImageFile(name), thiscall RET 4, EAX 0/1.
//  list = [this+1Ch] (reloaded at each use); pos = [list] (head); 0 -> return 0.
//  key = GetAtPos(list, pos) 005E9B60; while pos: if key && String::== (key+20h, name) 005E5200 -> found;
//  key = GetNext(list, &pos) 005E9B90.  found: 0040D2E0(key); Remove(list, pos) 005E9C10; 0040D0D0(key);
//  free(key) 006FA390; return 1.
extern "C" void __fastcall kotor_fastcall_00408830(void* manager, void*, void* name_word) {
    using base_ini_reconstruction::List;
    using base_ini_reconstruction::ListWrapper;
    async_load::Manager* const self = static_cast<async_load::Manager*>(manager);
    async_load::String* const name = static_cast<async_load::String*>(name_word);
    auto* wrapper = std::launder(reinterpret_cast<ListWrapper*>(self->bytes + 0x1c));               // 00408836
    auto list = [&]() -> List* { return static_cast<volatile ListWrapper*>(wrapper)->internal; };
    void* position = static_cast<volatile List*>(list())->head;                                     // 00408839..0040883D ([esp+0Ch])
    if (!position) return;                                                                          // 00408841 (EAX 0)
    void* key = reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(list(), nullptr, position)); // 00408843..00408849
    while (position) {                                                                              // 0040884B..00408851 / 00408877..0040887D
        if (key) {                                                                                  // 00408857..00408859
            async_load::String* const key_name = std::launder(reinterpret_cast<async_load::String*>(static_cast<unsigned char*>(key) + 0x20));
            if (add_key_lower::Equal005e5200(key_name, nullptr, name)) {                            // 0040885B..00408866
                async_unload_key_0040d2e0(key, nullptr);                                            // 00408888..0040888A
                add_key_lower::Remove005e9c10(list(), nullptr, position);                           // 0040888F..00408897
                add_key_lower::Destroy0040d0d0(key, nullptr);                                       // 0040889C..0040889E
                live_free(key);                                                                     // 004088A3..004088A4
                return;                                                                             // EAX 1 (consumer declares void)
            }
        }
        key = reinterpret_cast<void*>(unresolved_list_next_005e9b90(list(), nullptr, &position));   // 00408868..00408875
    }
}
