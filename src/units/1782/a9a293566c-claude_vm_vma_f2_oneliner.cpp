// Lane vmA, sub-batch F2 (part 2): SpeakOneLinerConversation and its one-liner dialogue cascade, reconstructed from
// the original swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are original instructions.
//   0054 3F70 ExecuteCommandSpeakOneLinerConversation -> CSWSObject::RunDialogOneLiner 004CB220
//     -> CSWSDialog::GetStartEntryOneLiner 0059EE00
//     -> CSWSMessage::SendServerToPlayerChatMultiLangMessage 00571570 -> ..._Helper 0056D3C0
// .rdata facts used as literals: [0073D71C] ""; [00745E54] "DLG "; [0073ECD0] byte 0 (default gender);
// [00747444] float 1000.0 (447A0000h); [0074666C] byte 0 "found"; [00746644] 7F000000h OBJECT_INVALID.
#include "520143efd3-claude_vm_vma_common.hpp"

namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }  // CExoString::!=(char*) [0374]
namespace kotor_reconstruction { struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); }; } // CExoString::==(char*) [0077]

extern "C" {
// ---- existing providers (c31 map; object in brackets; RET checked against the original) ----
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);              // CExoString(const char*) [0459]
void* __fastcall kotor_fastcall_00405ed0(void* r, void*);                                     // CResRef() [0263]
void* __fastcall ref_assign_copy_00406120(void* r, void*, void* out, const void* src);       // CResRef::operator=(CResRef&) by value RET 8 [0278]
void* __fastcall async_ref_assign_string_004061f0(void* r, void*, void* out, const void* s); // CResRef::operator=(CExoString&) RET 8 [0200]
int __fastcall c2da_equal_text_004060d0(void* r, void*, const char* text);                   // CResRef::operator==(char*) RET 4 [0087]
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref); // CResGFF(type, ext, ref) RET 0Ch [0289]
void* __fastcall exo_locstring_construct_005e9e10(void* l, void*);                           // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* l, void*);                              // ~CExoLocString [0265]
void* __fastcall recovered_localized_assign_005e9e70(void* l, void*, const void* src);       // CExoLocString::operator= RET 4 [0886]
void* __fastcall kotor_fastcall_005a0e80(void* dialog, void*);                               // CSWSDialog::CSWSDialog [claude_dialogue_start]
void __fastcall kotor_fastcall_005a4880(void* dialog, void*);                                // ~CSWSDialog [claude_dialogue_start]
U __fastcall kotor_fastcall_005a2ae0(void* dialog, void*, void* gff, U load_all);            // CSWSDialog::LoadDialog RET 8 [claude_dialogue_start]
int __fastcall kotor_fastcall_0059ec90(void* dialog, void*, void* object, const void* entry); // CSWSDialog::CheckScript RET 8 [claude_dialogue_start]
int __fastcall kotor_fastcall_0059ed70(void* dialog, void*, void* object, const void* script); // CSWSDialog::RunScript RET 8 [claude_dialogue_start]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                 // CServerExoApp::GetPlayerList [w1_native_b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);              // CExoLinkedListInternal::GetAtPos [0168]
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                   // CExoLinkedListInternal::GetNext [0168]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);             // CSWMessage::CreateWriteMessage RET 0Ch [w2_message_native]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                         // CSWSMessage::WriteOBJECTIDServer RET 4 [w2_message_native]
void __fastcall kotor_fastcall_005785c0(void* message, void*, void* loc, U gender);          // CSWSMessage::WriteCExoLocStringServer RET 8 [w2_message_native]
void __fastcall kotor_fastcall_004d6380(void* message, void*, U r0, U r1, U r2, U r3, U bits); // CSWMessage::WriteCResRef(by value, bits) RET 14h [w2_message_native]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);             // CSWMessage::WriteDWORD RET 8 [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);            // CSWMessage::GetWriteMessage RET 8 [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage RET 14h [w2_message_native]
// ---- reconstructed in this file ----
U __fastcall kotor_fastcall_004cb220(void* object, void*, void* name, U target);
U __fastcall kotor_fastcall_0059ee00(void* dialog, void*, void* object, void* text, void* sound, void* script);
U __fastcall kotor_fastcall_00571570(void* message, void*, U type, U speaker, void* text, U target, U gender,
    const U* ids, U id_count, U restrict_to_ids, void* sound, U object, U value);
U __fastcall kotor_fastcall_0056d3c0(void* message, void*, U player, U type, U speaker, void* text, U target,
    U gender, U listed, void* sound, U object, U value);
}

using namespace vma;

namespace {
struct ResRef { unsigned char bytes[16]; };           // CResRef (trivially destructible)
static_assert(sizeof(ResRef) == 16, "CResRef is 16 bytes");
struct LocString { unsigned char bytes[8]; };         // CExoLocString
constexpr U k_dialog_frame_size = 0x12c;              // [esp+78h .. esp+1A4h) of 004CB220
constexpr U8 k_default_gender_0073ecd0 = 0;           // .rdata 0073ECD0 (byte)
constexpr float k_chat_range_00747444 = 1000.0f;      // .rdata 00747444 (447A0000h)
inline void* gff_vdtor(void* gff) { return vcall<void*>(gff, 0, 1u); }   // vtbl+0 scalar deleting destructor(1)
}  // namespace

extern "C" {

// ================================================================== 00543F70 SpeakOneLinerConversation(dialog, tokenTarget) [routine 417]
// Quirk (original): a pop failure returns -2000 (0xFFFFF830), not -2001.
int __fastcall kotor_fastcall_00543f70(void* self, void*, int, unsigned argc) {
    ExoString name;
    frame_LogStringCtor_005e5a90(&name, nullptr, "");                                  // 00543F8D..96 [esp+14]
    U target = k_invalid;                                                              // 00543FA0 [esp+C]
    try {
        void* array = object_array();                                                  // 00543F9B..B3 (ebx)
        const I n = static_cast<I>(argc);                                              // 00543FB8 edi
        if ((n >= 1 && !pop_string(&name)) ||                                          // 00543FBC..D5 (jl)
            (n >= 2 && !pop_object(&target))) {                                        // 00543FD7..EE
            exo_destroy(&name); return k_push_failed;                                  // 00543FF0..17
        }
        void* object = nullptr;
        if (at<U>(self, 4) == 1 &&                                                     // 0054401A..1E
            static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, self_id(self), &object)) == 0 && // 00544020..36
            as_object(object) != nullptr)                                              // 00544038..43
            (void)kotor_fastcall_004cb220(as_object(object), nullptr, &name, target); // 00544045..5A
    } catch (...) { exo_destroy(&name); throw; }
    exo_destroy(&name);                                                                // 0054405F..6B
    return 0;                                                                          // 00544076
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 004CB220 CSWSObject::RunDialogOneLiner(CExoString* dialog, target id): thiscall, RET 8, EAX 0/1.
// Loads the DLG (the object's own conversation when the name is ""), takes the first start entry that is a
// one-liner, broadcasts its text/sound as chat type 8 to nearby players and runs the entry script.
// Quirks (original): a failed operator new(A0h) leaves a null CResGFF that is dereferenced; the gender dword passed
// on carries uninitialised upper bytes in the original (only the byte is meaningful; zero-extended here).
U __fastcall kotor_fastcall_004cb220(void* object, void*, void* name, U target) {
    ResRef dialog_ref; kotor_fastcall_00405ed0(&dialog_ref, nullptr);                  // 004CB241..45 [esp+24]
    alignas(8) unsigned char dialog[k_dialog_frame_size];
    kotor_fastcall_005a0e80(dialog, nullptr);                                          // 004CB24A..4E [esp+78]
    LocString text;
    try {
        exo_locstring_construct_005e9e10(&text, nullptr);                              // 004CB255..60 [esp+10]
    } catch (...) { kotor_fastcall_005a4880(dialog, nullptr); throw; }
    U result = 0;
    try {
        ResRef sound; kotor_fastcall_00405ed0(&sound, nullptr);                        // 004CB265..71 [esp+48]
        ResRef script; kotor_fastcall_00405ed0(&script, nullptr);                      // 004CB276..7A [esp+38]
        U8 gender = k_default_gender_0073ecd0;                                         // 004CB285..8D [esp+18]
        void* array = object_array();                                                  // 004CB27F..9F [esp+20]
        void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);  // 004CB296..B6 (ebp)
        ResRef temp;                                                                   // [esp+58]
        if (reinterpret_cast<kotor_reconstruction::StartupString*>(name)->BorrowedEquals005e5290("")) { // 004CB2A8..BF
            ResRef own;                                                                // [esp+68]
            void* ref = vcall<void*>(object, 0x80, static_cast<void*>(&own));          // 004CB2C1..CA vtbl+80h (conversation)
            ref_assign_copy_00406120(&dialog_ref, nullptr, &temp, ref);                // 004CB2D0..DA
        } else {
            async_ref_assign_string_004061f0(&dialog_ref, nullptr, &temp, name);       // 004CB2E1..EB
        }
        if (c2da_equal_text_004060d0(&dialog_ref, nullptr, "") == 0) {                 // 004CB2F0..00
            void* memory = live_operator_new(0xa0);                                    // 004CB302..0C
            void* gff = nullptr;
            if (memory) {                                                              // 004CB313..1D
                try {
                    gff = gui_load_gff_construct_00410630(memory, nullptr, 0x7ed, "DLG ", &dialog_ref); // 004CB31F..30
                } catch (...) { live_free(memory); throw; }                            // EH state 2
            }
            if (at<U>(gff, 0x94) == 0) {                                               // 004CB33B..49
                gff_vdtor(gff);                                                        // 004CB34B..51
            } else {
                const U loaded = kotor_fastcall_005a2ae0(dialog, nullptr, gff, 0);     // 004CB394..9D
                gff_vdtor(gff);                                                        // 004CB3A4..B2
                if (loaded &&
                    kotor_fastcall_0059ee00(dialog, nullptr, object, &text, &sound, &script)) { // 004CB3B4..D2
                    void* target_object = nullptr;
                    if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, target, &target_object)) == 0 && // 004CB3D8..F4
                        as_object(target_object) != nullptr)                           // 004CB3F6..01
                        gender = static_cast<U8>(vcall<U>(as_object(target_object), 0x88)); // 004CB403..16 vtbl+88h (AL)
                    (void)kotor_fastcall_00571570(message, nullptr, 8, at<U>(object, 4), &text, target, gender,
                        nullptr, 0, 0, &sound, k_invalid, 0);                          // 004CB41A..3B
                    (void)kotor_fastcall_0059ed70(dialog, nullptr, object, &script);   // 004CB440..4D
                    result = 1;                                                        // 004CB476
                }
            }
        }
    } catch (...) { exo_locstring_destroy_005ea000(&text, nullptr); kotor_fastcall_005a4880(dialog, nullptr); throw; }
    exo_locstring_destroy_005ea000(&text, nullptr);                                    // 004CB353..5E / 004CB452..5D
    kotor_fastcall_005a4880(dialog, nullptr);                                          // 004CB363..72 / 004CB462..71
    return result;                                                                     // 004CB377 / 004CB476
}

// 0059EE00 CSWSDialog::GetStartEntryOneLiner(object, CExoLocString* text, CResRef* sound, CResRef* script):
// thiscall, RET 10h, EAX 0/1.  Starts: [+14] count (unsigned), [+18] array (14h each, +10 entry index);
// entries: [+8] array (B0h each): +0 speaker, +8 text, +10 script, +34 sound, +A8 reply count.
U __fastcall kotor_fastcall_0059ee00(void* dialog, void*, void* object, void* text, void* sound, void* script) {
    for (U i = 0; i < at<U>(dialog, 0x14); ++i) {                                      // 0059EE08..76 (jbe / jb)
        unsigned char* start = at<unsigned char*>(dialog, 0x18) + i * 0x14;
        unsigned char* entry = at<unsigned char*>(dialog, 8) + at<U>(start, 0x10) * 0xb0; // 0059EE20..35
        if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(entry), nullptr, "")) continue; // 0059EE30..3E
        if (!kotor_fastcall_0059ec90(dialog, nullptr, object, at<unsigned char*>(dialog, 0x18) + i * 0x14)) continue; // 0059EE40..50
        entry = at<unsigned char*>(dialog, 8) + at<U>(at<unsigned char*>(dialog, 0x18) + i * 0x14, 0x10) * 0xb0; // 0059EE52..62
        if (at<U>(entry, 0xa8) != 0) continue;                                         // 0059EE62..6B
        entry = at<unsigned char*>(dialog, 8) + at<U>(at<unsigned char*>(dialog, 0x18) + i * 0x14, 0x10) * 0xb0; // 0059EE84..9A
        recovered_localized_assign_005e9e70(text, nullptr, entry + 8);                 // 0059EE9E..A3
        ResRef temp;
        entry = at<unsigned char*>(dialog, 8) + at<U>(at<unsigned char*>(dialog, 0x18) + i * 0x14, 0x10) * 0xb0; // 0059EEA8..B8
        ref_assign_copy_00406120(sound, nullptr, &temp, entry + 0x34);                 // 0059EEBC..C6
        entry = at<unsigned char*>(dialog, 8) + at<U>(at<unsigned char*>(dialog, 0x18) + i * 0x14, 0x10) * 0xb0; // 0059EECB..DB
        ref_assign_copy_00406120(script, nullptr, &temp, entry + 0x10);                // 0059EEDF..E9
        return 1;                                                                      // 0059EEF1
    }
    return 0;                                                                          // 0059EE7B
}

// 00571570 CSWSMessage::SendServerToPlayerChatMultiLangMessage(type, speaker, text, target, gender, ids, count,
// restrict, sound, object, value): thiscall, RET 2Ch.  Only type 8: for every player in the speaker's area within
// 1000 m (squared length <= 1E6; NaN skips) -- or, with `restrict` set, only the listed player ids -- send the
// helper message; stops at the first failed send.  Returns the last send status (0 when the speaker is unknown).
// Quirk (original): the GetAtPos result is not null-checked before CSWSPlayer::GetGameObject.
U __fastcall kotor_fastcall_00571570(void* message, void*, U type, U speaker, void* text, U target, U gender,
                                     const U* ids, U id_count, U restrict_to_ids, void* sound, U object, U value) {
    Vector speaker_pos = {0.0f, 0.0f, 0.0f};                                           // 0057157C..8C [esp+10..18]
    if (static_cast<U8>(type) != 8) return 0;                                          // 00571573..9B
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);      // 0057159E..AC (edi)
    if (!players) return 0;                                                            // 005715AE..BA (eax = 0)
    void* speaker_object = nullptr;
    void* speaker_area = nullptr;
    U status;
    if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, speaker, &speaker_object)) == 0) { // 005715BD..E4
        speaker_area = recovered_object_area_004cb120(speaker_object, nullptr);       // 005715E6..FC
        copy_vector(&speaker_pos, static_cast<unsigned char*>(speaker_object) + 0x90); // 005715F1..0B
        status = 1;                                                                    // 0057160F
    } else {
        status = 0;                                                                    // 00571616
    }
    void* position = at<void*>(at<void*>(players, 0), 0);                              // 00571618..22
    while (position && status == 1) {                                                  // 00571622 / 00571630..33
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position); // 00571639..41
        if (kotor_fastcall_00561080(player, nullptr)) {                                // 00571643..4C
            U listed = 0;                                                              // 0057165A edi
            U restrict_flag = restrict_to_ids;                                         // 00571656 ebx
            for (U k = 0; k < id_count; ++k) {                                         // 0057165C..77 (jbe / jb)
                if (ids[k] == at<U>(player, 4)) { listed = 1; restrict_flag = 0; break; } // 00571665..7E
            }
            void* go = kotor_fastcall_00561080(player, nullptr);                       // 00571680..82
            Vector d;
            d.x = static_cast<float>(static_cast<double>(at<float>(go, 0x90)) - speaker_pos.x); // 00571687..98
            d.y = static_cast<float>(static_cast<double>(at<float>(go, 0x94)) - speaker_pos.y); // 0057169C..A3
            d.z = static_cast<float>(static_cast<double>(at<float>(go, 0x98)) - speaker_pos.z); // 005716A7..AE
            if (restrict_flag == 0 &&                                                  // 00571696 / 005716B2
                recovered_object_area_004cb120(kotor_fastcall_00561080(player, nullptr), nullptr) == speaker_area) { // 005716B4..C6
                // 005716C8..E2: z*z + y*y + x*x (53-bit x87; float products exact in double)
                const double sum = (static_cast<double>(d.z) * d.z + static_cast<double>(d.y) * d.y) +
                                   static_cast<double>(d.x) * d.x;
                const double range = static_cast<double>(k_chat_range_00747444) * k_chat_range_00747444; // 005716E4..EA
                if (!(range < sum) && !(sum != sum))                                   // 005716F0..F7 fcompp, test ah,1 (C0: less or unordered)
                    status = kotor_fastcall_0056d3c0(message, nullptr, at<U>(player, 4), 8, speaker, text, target,
                                                     gender, listed, sound, object, value); // 005716F9..2C
            }
        }
        (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position); // 0057172E..39
    }                                                                                  // 0057173E..44
    return status;                                                                     // 0057174C
}

// 0056D3C0 CSWSMessage::SendServerToPlayerChatMultiLang_Helper(player, type, speaker, text, target, gender, listed,
// sound, object, value): thiscall, RET 28h.  Type 8 only: message 9, minor 0Bh (+1 when listed); the minor dword
// keeps the upper bytes of the `type` argument slot (the original overwrites only its low byte).
U __fastcall kotor_fastcall_0056d3c0(void* message, void*, U player, U type, U speaker, void* text, U target,
                                     U gender, U listed, void* sound, U object, U value) {
    U size = 0;                                                                        // [esp] local
    if (static_cast<U8>(type) != 8) return 0;                                          // 0056D3C1..CA -> 0056D466
    kotor_fastcall_004d68a0(message, nullptr, 0x28, player, 1);                        // 0056D3D0..E1
    size = 0x28;                                                                       // 0056D3D9
    kotor_fastcall_00578570(message, nullptr, speaker);                                // 0056D3E6..ED
    kotor_fastcall_00578570(message, nullptr, target);                                 // 0056D3F2..F9
    kotor_fastcall_005785c0(message, nullptr, text, gender);                           // 0056D3FE..0A
    const U* r = static_cast<const U*>(sound);
    kotor_fastcall_004d6380(message, nullptr, r[0], r[1], r[2], r[3], 0x10);           // 0056D40F..32 (by value)
    kotor_fastcall_00578570(message, nullptr, object);                                 // 0056D437..3E
    kotor_fastcall_004d6340(message, nullptr, value, 0x20);                            // 0056D443..4C
    void* data = nullptr;                                                              // (the original reuses the player slot)
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;        // 0056D451..66
    const U minor = (type & 0xffffff00u) | static_cast<U8>(0xb + (listed != 0 ? 1 : 0)); // 0056D46E..86
    return kotor_fastcall_00567e60(message, nullptr, player, 9, minor, data, size);    // 0056D486..91
}

}  // extern "C"
