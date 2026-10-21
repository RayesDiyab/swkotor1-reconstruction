// lane-m46-startup: CClientExoApp typed method table (original .rdata 0074E6D8, 10 slots, D1/D21/D24) and the
// client/game-object leaves consumed by game-prepare client_callees.cpp (00677E50, 00605C40) and gui-load
// mainmenu.h (005F1420, 0060F090, 006792E0). Evidence: lane-m46-startup/dis/*.asm; slot words read from the
// pinned image (0074E6D8..0074E6FC): 005C5630, 005ED790, 005EE350, 005ED7A0, 005ED6E0, 005ED890, 005ED520,
// 0063E7F0, 005ED990, 005ED8A0. The next word (0074E700) is the CSWTlkTable table (lane-tlk), so the extent is 10.
//
// The M40/M41 CClientExoApp constructor (client-services construct_005ee2f0) and the shutdown body
// kotor_fastcall_005ee360 store the raw identity word 0074E6D8 into [this]. methods_of() therefore accepts that
// identity word or the typed table's own address (same convention as lane-tlk MethodsOf, D41: no Word->function
// pointer reinterpretation; anything else is a contract violation -> TerminateProcess(86) as in D41 routes).
// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit startup46-client-exo-app
// (startup46/client_exo_app.cpp). Only change: one kotor_host_diag_words call (D94 proposal) in the host
// typed-dispatch helper methods_of() before the unchanged TerminateProcess(86). No reconstructed body changed.
#include <cstdint>
#include <cstring>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "05a3c5e4fb-allocator_live.h"

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // window_shared_cells.h (M40)

extern "C" {
// Consumers' declarations (identical): game_prepare_producers.h:100-101, r4_lowers.h:98, mainmenu.h:76-78.
void __fastcall kotor_fastcall_00677e50(void* message, void*);
void __fastcall kotor_fastcall_00605c40(void* area, void*, int);
void* __fastcall kotor_fastcall_005ed580(void* client, void*, std::uint32_t id);   // GetGameObject (lane-input join)
void* __fastcall kotor_fastcall_005f1420(void* internal, void*);
void __fastcall kotor_fastcall_0060f090(void* creature, void*, std::uint32_t);
void __fastcall kotor_fastcall_006792e0(void* control, void*, std::uint32_t);
unsigned __fastcall frame_R4_005ed5b0(void*, void*, unsigned);                      // r4-lowers (GetCreatureByGameObjectID)
void __fastcall gui_load_slot_ret4_0060e760(void*, void*, std::uint32_t);          // gui-load-gui-objects (0060E760 `ret 4`)
// New names required by this unit (not in M45): CNetLayer::SendMessageToPlayer 005D4CB0 (thiscall RET 10h),
// CClientExoAppInternal::HandleMessage 005EE700 (RET 10h) and ::ShutDownToMainMenu 005FCBD0 (RET 0).
std::int32_t __fastcall net_layer_send_message_to_player_005d4cb0(void* net, void*, std::uint32_t player, const char* message,
                                                                 std::uint32_t length, std::uint32_t flags);
void __fastcall client_internal_handle_message_005ee700(void* internal, void*, std::uint32_t object_id, unsigned char* message,
                                                       std::uint32_t length, std::uint32_t ignore);
void __fastcall client_internal_shut_down_to_main_menu_005fcbd0(void* internal, void*);
// CClientExoApp members (slots below) and GetPlayerCreature 005ED540 (non-virtual; new name, used by 00605C40).
void* __fastcall client_get_player_creature_005ed540(void* client, void*);
}

namespace kotor_client_exo_app {
using Word = std::uint32_t;
inline Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
inline void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
inline void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }

struct Methods {
    void (__fastcall* slot00_void_return_0c)(void*, void*, Word, Word, Word);                               // 005C5630 `ret 0Ch`
    void (__fastcall* handle_message)(void*, void*, Word, unsigned char*, Word, Word);                         // 005ED790
    float (__fastcall* get_fps)(void*, void*);                                                                  // 005EE350 (ST0)
    Word (__fastcall* continue_message_processing)(void*, void*);                                               // 005ED7A0
    void* (__fastcall* get_net_layer)(void*, void*);                                                            // 005ED6E0
    void (__fastcall* shut_down_to_main_menu)(void*, void*);                                                    // 005ED890
    void* (__fastcall* get_connection_lib)(void*, void*);                                                       // 005ED520
    Word (__fastcall* slot1c_return_zero)(void*, void*);                                                        // 0063E7F0 `xor eax,eax; ret`
    Word (__stdcall* get_application_id)();                                                                     // 005ED990 (stdcall, ECX unused)
    void (__fastcall* set_application_ids_match)(void*, void*, Word);                                           // 005ED8A0 RET 4
};
static_assert(sizeof(Methods) == 0x28, "ten observed slots 0074E6D8..0074E6FC");

// ---- slot bodies -------------------------------------------------------------------------------------------
// 005C5630 `ret 0Ch` (shared COMDAT-folded empty virtual; D24 typed own body for this slot type).
// [M45 compose] 005C5630 (ret 0Ch) body removed: earliest-staged owner 3d-render scene3d gui_scene_void_return_0c_005c5630 (M45) (definition removed; declaration kept)
void __fastcall client_slot00_005c5630(void*, void*, Word, Word, Word);
// 005ED790: mov ecx,[ecx+4]; jmp 005EE700.
void __fastcall client_handle_message_005ed790(void* self, void*, Word id, unsigned char* message, Word length, Word ignore) {
    client_internal_handle_message_005ee700(pointer_at(self, 4), nullptr, id, message, length, ignore);
}
// 005EE350: fld dword [007A22B4]; ret. 007A22B4 is .data 1.0f with no writer in the image (sole reference is this
// load, full-image scan) - one owner here, original cold value.
const float fps_007a22b4 = 1.0f;
float __fastcall client_get_fps_005ee350(void*, void*) { return fps_007a22b4; }
// 005ED7A0: mov ecx,[ecx+4]; jmp 0063E7A0 (return_true: mov eax,1; ret).
Word __fastcall client_continue_message_processing_005ed7a0(void*, void*) { return 1; }
// 005ED6E0: return [[this+4]+10h].
void* __fastcall client_get_net_layer_005ed6e0(void* self, void*) { return pointer_at(pointer_at(self, 4), 0x10); }
// 005ED890: mov ecx,[ecx+4]; jmp 005FCBD0.
void __fastcall client_shut_down_to_main_menu_005ed890(void* self, void*) { client_internal_shut_down_to_main_menu_005fcbd0(pointer_at(self, 4), nullptr); }
// 005ED520: return [[this+4]+34h].
void* __fastcall client_get_connection_lib_005ed520(void* self, void*) { return pointer_at(pointer_at(self, 4), 0x34); }
// 0063E7F0 return_zero (D24 typed body of this slot's type).
Word __fastcall client_slot1c_return_zero_0063e7f0(void*, void*) { return 0; }
// 005ED990 GetApplicationId: return [[007A39FC]+18h].
Word __stdcall client_get_application_id_005ed990() { return word_at(kotor_menu_window_cells::manager_007a39fc, 0x18); }
// 005ED8A0: [[this+4]+208h] = value; RET 4.
void __fastcall client_set_application_ids_match_005ed8a0(void* self, void*, Word value) { put_word(pointer_at(self, 4), 0x208, value); }

extern "C" const Methods kotor_vtable_cclientexoapp_0074e6d8 = {
    &client_slot00_005c5630, &client_handle_message_005ed790, &client_get_fps_005ee350,
    &client_continue_message_processing_005ed7a0, &client_get_net_layer_005ed6e0, &client_shut_down_to_main_menu_005ed890,
    &client_get_connection_lib_005ed520, &client_slot1c_return_zero_0063e7f0, &client_get_application_id_005ed990,
    &client_set_application_ids_match_005ed8a0};

// Typed dispatch for a live CClientExoApp receiver ([this] = 0074E6D8 identity word or the typed table address).
const Methods* methods_of(const void* client) {
    const Word word = word_at(client, 0);
    if (word == 0x0074e6d8u || word == static_cast<Word>(reinterpret_cast<std::uintptr_t>(&kotor_vtable_cclientexoapp_0074e6d8)))
        return &kotor_vtable_cclientexoapp_0074e6d8;
    ;
    TerminateProcess(GetCurrentProcess(), 86);   // D41 contract violation code (unknown receiver table)
    return nullptr;
}

// Game-object virtual slots (CSWCObject/CGameObject tables are not published as typed structs in M45): D21 naked
// veneers reloading [ecx] and tail-jumping the slot; exact thiscall stack contract; no C++ casts.
__declspec(naked) void* __fastcall vcall_0c(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0Ch] } }
__declspec(naked) void* __fastcall vcall_2c(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 2Ch] } }
__declspec(naked) void __fastcall vcall_a4(void*, void*, const float*, Word, Word, Word) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0A4h] } }
__declspec(naked) void __fastcall vcall_a8(void*, void*, Word) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0A8h] } }
}  // namespace kotor_client_exo_app

using namespace kotor_client_exo_app;

// 005ED540 CClientExoApp::GetPlayerCreature: mov ecx,[ecx+4]; jmp 005F1420.
extern "C" void* __fastcall client_get_player_creature_005ed540(void* client, void*) { return kotor_fastcall_005f1420(pointer_at(client, 4), nullptr); }

// 005F1420..005F1455 CClientExoAppInternal::GetPlayerCreature: id=[+20]; if [+3BC]==id return [+3C0];
// c = GetCreatureByGameObjectID([[007A39FC]+4], id) 005ED5B0; [+3C0]=c; [+3BC]=[+20] (reloaded); return c.
extern "C" void* __fastcall kotor_fastcall_005f1420(void* internal, void*) {
    const Word id = word_at(internal, 0x20);                                                        // 005F1423
    if (word_at(internal, 0x3bc) == id) return pointer_at(internal, 0x3c0);                         // 005F1426..005F1435
    const unsigned c = frame_R4_005ed5b0(pointer_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, id);   // 005F1436..005F1440
    put_word(internal, 0x3c0, c);                                                                   // 005F1448
    put_word(internal, 0x3bc, word_at(internal, 0x20));                                             // 005F1445/005F144E
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(c));
}

// 0060F090..0060F0C3 CSWCCreature::SwitchMode(mode): 0 -> [+3A8]=0; 1 -> 1; 2 -> 2; other values: no store. RET 4.
extern "C" void __fastcall kotor_fastcall_0060f090(void* creature, void*, std::uint32_t mode) {
    if (mode == 0) put_word(creature, 0x3a8, 0);                                                    // 0060F097/0060F0B9
    else if (mode == 1) put_word(creature, 0x3a8, 1);                                               // 0060F09A/0060F0AC
    else if (mode == 2) put_word(creature, 0x3a8, 2);                                               // 0060F09D/0060F09F
}

// 006792E0..0067932F CSWPlayerControl::SetEnabled(e): [+0C]=e; id=[+4]; if id != 7F000000h { c =
// GetCreatureByGameObjectID([[007A39FC]+4], id); if c: SwitchMode(c, [+0C] ? [0074F224]=1 : [0074F220]=0) (tail) }. RET 4.
extern "C" void __fastcall kotor_fastcall_006792e0(void* control, void*, std::uint32_t enabled) {
    put_word(control, 0x0c, enabled);                                                               // 006792E7
    const Word id = word_at(control, 4);                                                            // 006792EA
    if (id == 0x7f000000u) return;                                                                  // 006792ED..006792F2
    const unsigned c = frame_R4_005ed5b0(pointer_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, id);   // 006792F4..006792FE
    if (c == 0) return;                                                                             // 00679305..00679307
    static const Word mode_on_0074f224 = 1, mode_off_0074f220 = 0;                                  // .rdata words (pinned image)
    kotor_fastcall_0060f090(reinterpret_cast<void*>(static_cast<std::uintptr_t>(c)), nullptr,
                            word_at(control, 0x0c) ? mode_on_0074f224 : mode_off_0074f220);         // 00679309..0067932B
}

// 00677E50..00677E9E CSWCMessage::SendPlayerToServerInput_AbortDriveControl (no ECX use; RET 0; EAX = send result):
//  m = new(3) (006FA7E6); null -> return; m = {70h, 06h, 21h}; net = [[007A39FC]+4]->GetNetLayer() (slot +10);
//  r = 005D4CB0(net, [0074C528] = FFFFFFFDh, m, 3, 0); free(m); return r.
extern "C" void __fastcall kotor_fastcall_00677e50(void*, void*) {
    unsigned char* const m = static_cast<unsigned char*>(live_operator_new(3));                    // 00677E51..00677E58
    if (!m) return;                                                                                 // 00677E5D..00677E62
    m[0] = 0x70; m[1] = 0x06; m[2] = 0x21;                                                          // 00677E64..00677E6B
    void* const client = pointer_at(kotor_menu_window_cells::manager_007a39fc, 4);                  // 00677E6F..00677E74
    static const Word server_player_0074c528 = 0xfffffffdu;                                         // .rdata 0074C528
    void* const net = methods_of(client)->get_net_layer(client, nullptr);                            // 00677E7D..00677E85
    (void)net_layer_send_message_to_player_005d4cb0(net, nullptr, server_player_0074c528,
                                                   reinterpret_cast<const char*>(m), 3, 0);         // 00677E7F..00677E8A
    live_free(m);                                                                                   // 00677E8F..00677E92
}

// 00605C40..00605DB8 (+jump table 00605E38) CSWCArea::HiliteAreaObjects(on), thiscall RET 4.
//  for i < [+210]: o = GetGameObject([[007A39FC]+4], [[+20C]+i*4]) 005ED580; s = o ? o->vt[0C]() : 0;
//  if s && s[+90] (byte) && (t = s[+8] byte) in 5..10 per the table:
//   5 (00605CB7): c = s->vt[2C](); on ? (c == GetPlayerCreature(...) 005ED540 -> colour (1,1,1);
//        c[+2DC]==1 -> (.3,1,.3); ==0 -> (.4,.4,1); else (1,.1,.1); s->vt[A4](&colour,2,0,1))
//        : (c->0060E760(0) `ret 4`; s->vt[A8](2))
//   6 (00605DBB), 9 (00605DFF), 10 (00605DDD): on ? s->vt[A4](&(.3,1,1),2,0,1) : s->vt[A8](2)
//   7, 8: nothing.
extern "C" void __fastcall kotor_fastcall_00605c40(void* area, void*, int on) {
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(word_at(area, 0x210)); ++i) {             // 00605C45..00605C54, 00605D9E..00605DAB
        void* const client = pointer_at(kotor_menu_window_cells::manager_007a39fc, 4);              // 00605C67..00605C70
        void* const object = kotor_fastcall_005ed580(client, nullptr, word_at(pointer_at(area, 0x20c), static_cast<Word>(i) * 4));   // 00605C61..00605C74
        if (!object) continue;                                                                      // 00605C79..00605C7B
        void* const s = vcall_0c(object, nullptr);                                                  // 00605C81..00605C85
        if (!s) continue;                                                                           // 00605C88..00605C8C
        if (static_cast<const unsigned char*>(s)[0x90] == 0) continue;                              // 00605C92..00605C9A
        const unsigned type = static_cast<const unsigned char*>(s)[8];                              // 00605CA0
        if (type < 5 || type > 10) continue;                                                        // 00605CA4..00605CAA
        float colour[3];
        switch (type) {                                                                             // 00605CB0 jmp [eax*4+00605E38]
        case 5: {
            void* const c = vcall_2c(s, nullptr);                                                   // 00605CB7..00605CBB
            if (!on) {                                                                              // 00605CBE..00605CC2
                gui_load_slot_ret4_0060e760(c, nullptr, 0);                                         // 00605D89..00605D8D
                vcall_a8(s, nullptr, 2);                                                            // 00605D92..00605D98
                break;
            }
            void* const player = client_get_player_creature_005ed540(pointer_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);   // 00605CC8..00605CD1
            if (c == player) { colour[0] = 1.0f; colour[1] = 1.0f; colour[2] = 1.0f; }             // 00605CE4..00605CF9
            else {
                const unsigned char k = static_cast<const unsigned char*>(c)[0x2dc];               // 00605D0C
                if (k == 1) { colour[0] = 0.3f; colour[1] = 1.0f; colour[2] = 0.3f; }               // 00605D16..00605D2B (3E99999A,3F800000,3E99999A)
                else if (k == 0) { colour[0] = 0.4f; colour[1] = 0.4f; colour[2] = 1.0f; }          // 00605D3F..00605D54 (3ECCCCCD,3ECCCCCD,3F800000)
                else { colour[0] = 1.0f; colour[1] = 0.1f; colour[2] = 0.1f; }                      // 00605D64..00605D79 (3F800000,3DCCCCCD,3DCCCCCD)
            }
            vcall_a4(s, nullptr, colour, 2, 0, 1);                                                  // 00605CDA..00605D81 (push 1; push 0; push 2; push &colour)
            break;
        }
        case 6: case 9: case 10:
            if (!on) { vcall_a8(s, nullptr, 2); break; }                                            // 00605DBB/00605DDD/00605DFF -> 00605D92
            colour[0] = 0.3f; colour[1] = 1.0f; colour[2] = 1.0f;                                   // (3E99999A, 3F800000, 3F800000)
            vcall_a4(s, nullptr, colour, 2, 0, 1);                                                  // 00605E1F..00605E2A
            break;
        default:                                                                                    // 7, 8 -> 00605D9E
            break;
        }
    }
}
