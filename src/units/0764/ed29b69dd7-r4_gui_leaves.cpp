// lane-m46-startup: R4 frame-lower / GUI leaves (consumers: r4-lowers r4_lowers.h:100-115, game-prepare
// load_screen.h:28). Evidence: lane-m46-startup/dis/*.asm. Raw-offset style of the consuming lanes.
#include <cstdint>
#include <cstring>
#include "459924a746-array_mutations.h"     // task-14 ArrayMutations (aurres-array-mutations root)

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

extern "C" {
// Consumers' declarations (identical to r4_lowers.h).
void __fastcall kotor_fastcall_0040bd20(void* gui, void*, void* panel);
void __fastcall kotor_fastcall_0062eb10(void* in_game, void*);
std::uint32_t __fastcall kotor_fastcall_006a6d80(void* dialog, void*, std::uint32_t index);
void __fastcall kotor_fastcall_00628710(void* in_game, void*, std::uint32_t index, std::uint32_t id, std::uint32_t animation);
void __fastcall kotor_fastcall_00506a60(void* server_area, void*, void* client_area);
void __fastcall kotor_fastcall_006c7ec0(void* transition, void*, std::uint32_t id, void* position);   // still missing (same unresolved set)
// Existing owners (declarations identical to their stage headers).
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                       // r4-lowers (CClientExoApp::GetModule)
void* __fastcall frame_GetServerArea_00605640(void*, void*);                         // r4-lowers
unsigned __fastcall frame_ContainsPanel_0040bd70(void* gui, void*, void* panel);     // r4-lowers-r4-gui-lowers
void __fastcall frame_RemovePanel_0040c830(void* gui, void*, void* panel);           // r4-lowers-r4-gui-lowers
void __fastcall frame_RemoveArray_006857a0(void*, void*, int);                       // r4-lowers-r4-gui-lowers (CExoArrayList::DelIndex)
void __fastcall kotor_fastcall_0040acc0(void* manager, void*) noexcept(false);       // gui-load mainmenu (DetermineNeedToDraw)
std::int32_t __fastcall gui_load_int_list_index_of_006c2a40(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t);   // gui-load listbox.h:285
void __fastcall aurora_list_insert_at_004167a0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t, std::int32_t); // 3d-model model_instance.hpp:69 (List = ArrayMutations)
}
namespace kotor_original { void __fastcall kotor_fastcall_00417fb0(void* bar, void* unused_edx, std::int32_t value); }   // load_screen.h:28

namespace {
using Word = std::uint32_t;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }
kotor_reconstruction::ArrayMutations* list_at(void* o, Word off) { return reinterpret_cast<kotor_reconstruction::ArrayMutations*>(static_cast<unsigned char*>(o) + off); }
Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
// CSWGuiControl table slot +04 (thiscall, one stack word, RET 4) - D21 naked tail veneer (no typed table for
// CSWGuiProgressBar 0073E7A0 is published in M45).
__declspec(naked) void __fastcall vcall_04_one(void*, void*, Word) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 4] } }
}

// 0040BD20..0040BD69 CSWGuiManager::SendPanelToBack(panel), RET 4:
//  if IndexOf(&+94h, panel) != -1 return; i = IndexOf(&+88h, panel); if i != -1 { DelIndex(&+88h, i) 006857A0;
//  InsertAt(&+88h, panel, 0) 004167A0 }; DetermineNeedToDraw(this) 0040ACC0.
extern "C" void __fastcall kotor_fastcall_0040bd20(void* gui, void*, void* panel) {
    if (gui_load_int_list_index_of_006c2a40(list_at(gui, 0x94), nullptr, word_of(panel)) != -1) return;   // 0040BD28..0040BD37
    const std::int32_t i = gui_load_int_list_index_of_006c2a40(list_at(gui, 0x88), nullptr, word_of(panel)); // 0040BD3A..0040BD43
    if (i != -1) {                                                                                          // 0040BD48..0040BD4B
        frame_RemoveArray_006857a0(list_at(gui, 0x88), nullptr, i);                                         // 0040BD4D..0040BD50
        aurora_list_insert_at_004167a0(list_at(gui, 0x88), nullptr, word_of(panel), 0);                     // 0040BD55..0040BD5A
    }
    kotor_fastcall_0040acc0(gui, nullptr);                                                                  // 0040BD5F..0040BD61
}

// 0062EB10..0062EBDE CGuiInGame::HideAreaTransition:
//  m = GetModule(app) (twice, 0062EB1E/0062EB30); if m && (a=[m+48]) && (s=GetServerArea(a)) && [s+2C4] {
//    if ContainsPanel([+38], [+94]) SendPanelToBack([+38], [+94]); return }
//  if [+108] && [+38] && [+94] { SetTransitionObject([+94], 7F000000h, &{0,0,0}) 006C7EC0;
//    if ContainsPanel([+38], [+94]) RemovePanel([+38], [+94]) }.
extern "C" void __fastcall kotor_fastcall_0062eb10(void* in_game, void*) {
    void* const app = pointer_at(kotor_menu_window_cells::manager_007a39fc, 4);                      // 0062EB10..0062EB1B
    if (kotor_fastcall_005ed530(app, nullptr)) {                                                     // 0062EB1E..0062EB25
        void* const module = kotor_fastcall_005ed530(pointer_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);   // 0062EB27..0062EB30
        if (void* const area = pointer_at(module, 0x48)) {                                          // 0062EB35..0062EB3A
            void* const server = frame_GetServerArea_00605640(area, nullptr);                       // 0062EB3C
            if (server && word_at(server, 0x2c4) != 0) {                                            // 0062EB41..0062EB4D
                if (frame_ContainsPanel_0040bd70(pointer_at(in_game, 0x38), nullptr, pointer_at(in_game, 0x94)))   // 0062EB4F..0062EB60
                    kotor_fastcall_0040bd20(pointer_at(in_game, 0x38), nullptr, pointer_at(in_game, 0x94));         // 0062EB62..0062EB6C
                return;
            }
        }
    }
    if (word_at(in_game, 0x108) == 0 || word_at(in_game, 0x38) == 0 || word_at(in_game, 0x94) == 0) return;   // 0062EB76..0062EB8F
    float zero[3] = {0.0f, 0.0f, 0.0f};                                                              // 0062EB9B..0062EBAB
    kotor_fastcall_006c7ec0(pointer_at(in_game, 0x94), nullptr, 0x7f000000u, zero);                  // 0062EB91..0062EBB3
    if (frame_ContainsPanel_0040bd70(pointer_at(in_game, 0x38), nullptr, pointer_at(in_game, 0x94)))  // 0062EBB8..0062EBC9
        frame_RemovePanel_0040c830(pointer_at(in_game, 0x38), nullptr, pointer_at(in_game, 0x94));    // 0062EBCB..0062EBD5
}

// 006A6D80..006A6D9A CSWGuiDialog::GetReplyIndex(i): i < [+6C] (signed) && i >= 0 ? [+1970h+i*4] : -1. RET 4.
extern "C" std::uint32_t __fastcall kotor_fastcall_006a6d80(void* dialog, void*, std::uint32_t index) {
    const std::int32_t i = static_cast<std::int32_t>(index);
    if (i >= static_cast<std::int32_t>(word_at(dialog, 0x6c)) || i < 0) return 0xffffffffu;           // 006A6D84..006A6D8B
    return word_at(dialog, 0x1970 + static_cast<Word>(i) * 4);                                       // 006A6D8D
}

// 00628710..00628748 CGuiInGame::FUN_00628710(index, id, animation16), RET 0Ch:
//  if index < [+198] (unsigned) && (a=[+1A0]) && [+1A4] { a[index]=id; ([+1A4] reloaded)[index] = (u16)animation }.
extern "C" void __fastcall kotor_fastcall_00628710(void* in_game, void*, std::uint32_t index, std::uint32_t id, std::uint32_t animation) {
    if (index >= word_at(in_game, 0x198)) return;                                                    // 00628714..0062871A (jae)
    unsigned char* const ids = static_cast<unsigned char*>(pointer_at(in_game, 0x1a0));              // 0062871C
    if (!ids || word_at(in_game, 0x1a4) == 0) return;                                                // 00628722..0062872F
    put_word(ids, index * 4, id);                                                                    // 00628731..00628735
    const std::uint16_t a = static_cast<std::uint16_t>(animation);                                   // 0062873E
    std::memcpy(static_cast<unsigned char*>(pointer_at(in_game, 0x1a4)) + index * 2, &a, 2);         // 00628738..00628743
}

// 00506A60 CSWSArea::AttachmentFromClientArea(client_area): [+2D0] = client_area; RET 4.
extern "C" void __fastcall kotor_fastcall_00506a60(void* server_area, void*, void* client_area) {
    put_word(server_area, 0x2d0, word_of(client_area));
}

// 00417FB0..00417FD9 CSWGuiProgressBar::SetCurValue(v), RET 4: v<0 -> 0; v>[+5C] -> [+5C]; if [+60] != v
// { [+60]=v; tail [vtable+4](this, this+4) }.
void __fastcall kotor_original::kotor_fastcall_00417fb0(void* bar, void*, std::int32_t value) {
    std::int32_t v = value;
    if (v < 0) v = 0;                                                                                // 00417FB4..00417FBA
    else if (v > static_cast<std::int32_t>(word_at(bar, 0x5c))) v = static_cast<std::int32_t>(word_at(bar, 0x5c));   // 00417FBC..00417FC3
    if (static_cast<std::int32_t>(word_at(bar, 0x60)) == v) return;                                  // 00417FC5..00417FC8
    put_word(bar, 0x60, static_cast<Word>(v));                                                       // 00417FCD
    vcall_04_one(bar, nullptr, word_of(static_cast<unsigned char*>(bar) + 4));                       // 00417FCA..00417FD6
}
