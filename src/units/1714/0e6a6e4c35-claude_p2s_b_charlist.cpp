// Lane worker "p2s-b" (fork charlist): CSWSMessage::HandlePlayerToServerCharListMessage 005246F0 and
// CSWSMessage::HandlePlayerToServerCharacterDownload 00525350 with their missing closure. Transcribed from the original
// swkotor.exe (read-only); statement comments cite original instruction addresses.
//   005246F0 CSWSMessage::HandlePlayerToServerCharListMessage(CSWSPlayer*, BYTE)      RET 8   recovered_player_to_server_char_list_005246f0
//   00525350 CSWSMessage::HandlePlayerToServerCharacterDownload(CSWSPlayer*, BYTE)    RET 8   recovered_player_to_server_character_download_00525350
//   004AEDD0 CServerExoApp::GetServerMode() -> AX                                     RET 0   kotor_fastcall_004aedd0
//   004AEA00 CServerExoApp::AddCharListRequest(ulong)  (thunk -> 004B6CB0)             RET 4   kotor_fastcall_004aea00
//   004B6CB0 CServerExoAppInternal::AddCharListRequest(ulong) (jmp 006449D0 +100ECh)   RET 4   kotor_fastcall_004b6cb0
//   00576360 CSWSMessage::SendServerToPlayerCharList(CSWSPlayer*)                     RET 4   kotor_fastcall_00576360
//   00570C60 CSWSMessage::SendServerToPlayerUpdateCharResponse(CSWSPlayer*, BYTE, CResRef) RET 18h kotor_fastcall_00570c60
//   004C8E90 CSWSModule::PackPlayerCharacterListIntoMessage(CSWSPlayer*, CExoArrayList<NWPlayerCharacterList_st*>*) RET 8
//   004C6830 NWPlayerCharacterList_st::NWPlayerCharacterList_st()                     RET 0   kotor_fastcall_004c6830
//   004C5DF0 NWPlayerCharacterList_st::~NWPlayerCharacterList_st()                    RET 0   kotor_fastcall_004c5df0
//   004C4ED0 CExoArrayList<NWPlayerCharacterList_st*>::contains(NWPlayerCharacterList_st*) RET 4 kotor_fastcall_004c4ed0
//   00411BB0 CResGFF::ReadFieldSHORT(CResStruct*, char*, int&, short)  -> AX          RET 10h kotor_fastcall_00411bb0
//   0056B090 CSWSMessage::SendServerToPlayerCharacterDownloadReply(CSWSPlayer*)       RET 4   kotor_fastcall_0056b090
//   0056B140 CSWSMessage::SendServerToPlayerCharacterDownloadFail(CSWSPlayer*)        RET 4   kotor_fastcall_0056b140
//   00561870 CSWSPlayer::PackCreatureIntoMessage()                                    RET 0   kotor_fastcall_00561870
// Conventions: claude_dialogue_common.hpp (thiscall -> extern "C" __fastcall(self, edx, ...)); game heap only
// (operator new 006FA7E6 -> live_operator_new, _free 006FA390 -> live_free); .rdata constants as proven literals;
// scope guards stand in for the original EH states (normal-path destruction order equals the original order).
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

// ---------------------------------------------------------------------------------------------------------------
// Providers (c16 map, real bodies; none in the c16 trap census)
// ---------------------------------------------------------------------------------------------------------------
namespace options_video { struct String { char* text; unsigned length; }; }                 // CExoString passed by value
namespace client_intro {              // 004B2640 CExoArrayList<CExoString>::count(CExoString by value; callee destroys) [0371]
int __fastcall CountStrings_004b2640(void*, void*, options_video::String);
}
namespace kotor_menu20 { struct String { int as_int_005e5630(); }; }                        // CExoString::AsINT [0021]
namespace startup_rand {              // 005E6270 CExoString::SubString(CExoString* out, int start, int count) [0147]
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString*, void*,
                                                                   kotor_reconstruction::StartupString*, int, int);
}
namespace f200_ports {                // 005E6640 CExoBase::GetDirectoryList(list, CExoString const&, ushort, int, int) [0288]
void __fastcall Enumerate005e6640(void*, void*, void*, kotor_reconstruction::StartupString*, unsigned, unsigned, unsigned);
}
namespace directory_list { struct TimeList; void __fastcall AllocateTimes004c4f50(TimeList*, void*, int); } // 004C4F50 8-byte Allocate
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); } // 005E03C0
namespace menu_resources {            // [0027] 00408800 CExoResMan::AddResourceDirectory(CExoString const&)
struct Manager { unsigned directory_00408800(kotor_reconstruction::StartupString*); };
}
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; } // [007A39E8] CExoResMan*
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // [007A39E0] CExoBase*
namespace tlk_file { struct File; }
namespace file_internal {             // 005E68D0 CExoFile::CExoFile(CExoString const&, ushort, CExoString const&)
tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*, void*, kotor_reconstruction::StartupString*, unsigned,
                                                       kotor_reconstruction::StartupString*);
}

namespace kotor_q05 {                 // [0290 gff-views] 00410990 CResGFF::GetField(CResStruct*, ulong) RET 8
struct FieldRecord; struct StructHandle;
struct CResGFF { FieldRecord* GetField(StructHandle*, unsigned); };
}

struct ResRef16 { unsigned char bytes[16]; };                                                // CResRef by value
static_assert(sizeof(ResRef16) == 16, "CResRef");

extern "C" {
// CSWMessage / CSWSMessage (w2_message_native)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                              // MessageReadOverflow
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);                              // MessageReadUnderflow
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);                             // ReadBYTE
void* __fastcall kotor_fastcall_004d6180(void* msg, void*, void* result, U size);            // ReadCResRef
void __fastcall kotor_fastcall_004d68a0(void* msg, void*, U size, U a, U b);                 // CreateWriteMessage
void __fastcall kotor_fastcall_004d62c0(void* msg, void*, U value, U bits);                  // WriteBYTE
void __fastcall kotor_fastcall_004d6300(void* msg, void*, U value, U bits);                  // WriteWORD
void __fastcall kotor_fastcall_004d6340(void* msg, void*, U value, U bits);                  // WriteDWORD
void __fastcall kotor_fastcall_004d6380(void* msg, void*, ResRef16 ref, U size);             // WriteCResRef
void __fastcall kotor_fastcall_004d63e0(void* msg, void*, const void* data, U size);         // WriteVOIDPtr
void __fastcall kotor_fastcall_004d68e0(void* msg, void*, U value);                          // WriteBOOL
void __fastcall kotor_fastcall_004d6960(void* msg, void*, const void* s, U bits);            // WriteCExoString
void __fastcall kotor_fastcall_005785c0(void* msg, void*, void* loc, U gender);              // WriteCExoLocStringServer
U __fastcall kotor_fastcall_004d6440(void* msg, void*, void** data, U* size);                // GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* msg, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// server / player
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // GetSWSMessage
void* __fastcall server_get_module_004ae6b0(void* server, void*);                            // GetModule [0530]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject
void* __fastcall kotor_fastcall_005610b0(void* player, void*, void* out);                    // CSWSPlayer::GetPlayerName
U __fastcall kotor_fastcall_005624c0(void* player, void*);                                   // CSWSPlayer::SaveServerCharacter
void __fastcall kotor_fastcall_005d5170(void* net, void*, U player_id);                      // CNetLayer::GetPlayerInfo
void __fastcall w1x_unique_append_006449d0(void* list, void*, U value);                      // CExoArrayList<ulong>::AddUnique
// creature
void* __fastcall recovered_server_creature_construct_004f7a10(void* memory, void*, U id, U flag); // CSWSCreature::CSWSCreature
U __fastcall w2x_creature_stats_005afce0(void* stats, void*, void* gff, void* s, void* extra);    // CSWSCreatureStats::ReadStatsFromGff
void __fastcall recovered_save_stats_005b1b90(void* stats, void*, void* gff, void* s);            // CSWSCreatureStats::SaveStats
U __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* s);        // CSWSCreature::SaveCreature
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);        // CGameEffect::SetInteger
void __fastcall recovered_object_remove_effect_004d06c0(void* object, void*, U lo, U hi);        // CSWSObject::RemoveEffectById
void __fastcall recovered_stats_refresh_005addc0(void* stats, void*);                            // UpdateCombatInformation
// CExoString / CExoLocString / CResRef
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                        // CExoString::operator=(char*)
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                  // CExoString::Format
U __fastcall kotor_fastcall_005ea020(void* loc, void*, const void* other);                   // CExoLocString::operator==
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);       // CResRef(CExoString const&)
void __fastcall kotor_fastcall_00405920(void* list, void*);                                  // ~CExoArrayList<CExoString>
// resources / GFF / files
void* __fastcall c2da_lookup_004074d0(void* resman, void*, void* ref, U type);               // CExoResMan::GetResObject
U __fastcall c2da_current_resource_demand_00409b10(void* res, void*);                        // CRes::Demand
U __fastcall c2da_release_demand_00409b80(void* res, void*);                                 // CRes::Release
U __fastcall c2da_release_00409cf0(void* resman, void*, void* res);                          // CExoResMan::ReleaseResObject
U __fastcall kotor_fastcall_004088d0(void* resman, void*, void* dir);                        // CExoResMan::RemoveResourceDirectory
U __fastcall gui_load_resman_remove_file_00406b20(void* resman, void*, void* name, U type);  // CExoResMan::RemoveFile
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                         // CResGFF::CResGFF()
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version); // CResGFF::CreateGFFFile
U __fastcall recovered_gff_00413030(void* gff, void*, void* path, U type);                   // CResGFF::WriteGFFFile
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out);                           // CResGFF::GetTopLevelStruct
U __fastcall gff_label_00411630(void* gff, void*, void* s, const char* label);               // CResGFF::GetFieldByLabel [0291]
U __fastcall file_opened_005e6a10(void* file, void*);                                        // CExoFile::FileOpened
U __fastcall exofile_size_005e6950(void* file, void*);                                       // CExoFile::GetSize
U __fastcall exofile_read_005e6960(void* file, void*, void* out, U size, U count);           // CExoFile::Read
void __fastcall c2da_005e69e0(void* file, void*);                                            // CExoFile::~CExoFile

// Exports of this file
U __fastcall recovered_player_to_server_char_list_005246f0(void* message, void*, void* player, U minor);
U __fastcall recovered_player_to_server_character_download_00525350(void* message, void*, void* player, U minor);
U16 __fastcall kotor_fastcall_004aedd0(void* server, void*);
void __fastcall kotor_fastcall_004aea00(void* server, void*, U player_id);
void __fastcall kotor_fastcall_004b6cb0(void* internal, void*, U player_id);
U __fastcall kotor_fastcall_00576360(void* msg, void*, void* player);
U __fastcall kotor_fastcall_00570c60(void* msg, void*, void* player, U minor, ResRef16 ref);
void __fastcall kotor_fastcall_004c8e90(void* module, void*, void* player, void* list);
void* __fastcall kotor_fastcall_004c6830(void* entry, void*);
void __fastcall kotor_fastcall_004c5df0(void* entry, void*);
U __fastcall kotor_fastcall_004c4ed0(void* list, void*, void* entry);
U16 __fastcall kotor_fastcall_00411bb0(void* gff, void*, void* s, const char* label, U* ok, U fallback);
U __fastcall kotor_fastcall_0056b090(void* msg, void*, void* player);
U __fastcall kotor_fastcall_0056b140(void* msg, void*, void* player);
U __fastcall kotor_fastcall_00561870(void* player, void*);
}

namespace {
constexpr U kResBic = 0x7df;                  // BIC resource type
constexpr U kResIfo = 0x7de;                  // IFO resource type
constexpr U kEffectType3e = 0x3e;             // .rdata [00746774] dword 3Eh
constexpr U8 kObjectTypeCreatureMin = 4;      // .rdata [00746633] byte 04 (ja: type > 4)
using SS = kotor_reconstruction::StartupString;
inline SS* ss(void* p) { return static_cast<SS*>(p); }
inline void* resman() { return kotor_resource_demand::resource_manager_007a39e8; }
inline void add_dir(void* s) { (void)kotor_resource_demand::resource_manager_007a39e8->directory_00408800(ss(s)); }
inline void remove_dir(void* s) { (void)kotor_fastcall_004088d0(resman(), nullptr, s); }
inline void* base() { return kotor_menu_10_19::dependencies::base_007a39e0; }
inline void get_dir_list(void* list, void* dir, U type, U dirs, U order) {
    f200_ports::Enumerate005e6640(base(), nullptr, list, ss(dir), type, dirs, order);
}
inline void vdelete(void* object) { (void)vcall<void*>(object, 0, 1u); }          // scalar deleting dtor (vtable[0], 1)
inline U str_len(void* s) { return async_string_length_005e5790(s, nullptr); }
// A CExoString built by a callee into caller storage; destroyed when the scope ends.
struct Adopted {
    U words[2] = {0, 0};
    Adopted() = default;
    Adopted(const Adopted&) = delete;
    Adopted& operator=(const Adopted&) = delete;
    ~Adopted() { frame_LogStringDtor_005e5c20(words, nullptr); }
};
struct AdoptedLoc {
    U words[2] = {0, 0};
    AdoptedLoc() = default;
    AdoptedLoc(const AdoptedLoc&) = delete;
    AdoptedLoc& operator=(const AdoptedLoc&) = delete;
    ~AdoptedLoc() { exo_locstring_destroy_005ea000(words, nullptr); }
};
// CExoArrayList<CExoString> local {data, count, capacity}; inline eh_vector_destructor_iterator + free at scope end
struct StringList {
    U data = 0, count = 0, capacity = 0;
    StringList() = default;
    StringList(const StringList&) = delete;
    StringList& operator=(const StringList&) = delete;
    void clear_storage() {
        if (data != 0) {
            c2da_destroy_array_006fa852(ptr(data), 8, rdi(ptr(data - 4), 0), reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
            live_free(ptr(data - 4));
        }
    }
    ~StringList() { clear_storage(); }
};
// CExoArrayList<NWPlayerCharacterList_st*> local; storage freed at scope end
struct EntryList {
    U data = 0, count = 0, capacity = 0;
    EntryList() = default;
    EntryList(const EntryList&) = delete;
    EntryList& operator=(const EntryList&) = delete;
    ~EntryList() { live_free(ptr(data)); }
};
static_assert(sizeof(StringList) == 12 && sizeof(EntryList) == 12, "CExoArrayList");
struct GffList { unsigned char b[0x14]; };    // CResList

// new(0xA0) + CResGFF(type, ext, ref) / CResGFF()
void* new_gff_ref(const void* ref) {
    void* memory = live_operator_new(0xa0);
    if (memory == nullptr) return nullptr;
    return gui_load_gff_construct_00410630(memory, nullptr, kResBic, "BIC ", ref);
}
void* new_gff_empty() {
    void* memory = live_operator_new(0xa0);
    if (memory == nullptr) return nullptr;
    return recovered_gff_construct_004105a0(memory, nullptr);
}
void* new_creature() {                                                       // new(0AC8h) + CSWSCreature(7F000000h, 1)
    void* memory = live_operator_new(0xac8);
    if (memory == nullptr) return nullptr;
    return recovered_server_creature_construct_004f7a10(memory, nullptr, kInvalid, 1);
}
void* new_entry() {                                                          // new(40h) + NWPlayerCharacterList_st()
    void* memory = live_operator_new(0x40);
    if (memory == nullptr) return nullptr;
    return kotor_fastcall_004c6830(memory, nullptr);
}
ResRef16 resref_value(const void* p) { ResRef16 r; std::memcpy(&r, p, 16); return r; }
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// 004AEDD0 CServerExoApp::GetServerMode: [[this+4]+10008h] word in AX
extern "C" U16 __fastcall kotor_fastcall_004aedd0(void* server, void*) {
    return rd16(rp(server, 4), 0x10008);                                                      // 004AEDD0..004AEDDA
}
// 004B6CB0 CServerExoAppInternal::AddCharListRequest(id): CExoArrayList<ulong>::AddUnique on this+100ECh
extern "C" void __fastcall kotor_fastcall_004b6cb0(void* internal, void*, U player_id) {
    w1x_unique_append_006449d0(fa(internal, 0x100ec), nullptr, player_id);                   // 004B6CB0..004B6CB6
}
// 004AEA00 CServerExoApp::AddCharListRequest(id): ecx = [this+4]; jmp 004B6CB0
extern "C" void __fastcall kotor_fastcall_004aea00(void* server, void*, U player_id) {
    kotor_fastcall_004b6cb0(rp(server, 4), nullptr, player_id);                              // 004AEA00..004AEA03
}

// ---------------------------------------------------------------------------------------------------------------
// 00411BB0 CResGFF::ReadFieldSHORT(CResStruct*, char* label, int& ok, short fallback) RET 10h -> AX
extern "C" U16 __fastcall kotor_fastcall_00411bb0(void* gff, void*, void* s, const char* label, U* ok, U fallback) {
    if (s == nullptr || label == nullptr) {                                                   // 00411BB2..00411BC2
        *ok = 0;                                                                              // 00411C04..00411C09
        // 00411C0F: original quirk - AX = low word of [esp+14h], i.e. of the &ok argument (not the fallback)
        return static_cast<U16>(word(ok));
    }
    const U index = gff_label_00411630(gff, nullptr, s, label);                               // 00411BC4..00411BC6
    void* const field = reinterpret_cast<kotor_q05::CResGFF*>(gff)->GetField(static_cast<kotor_q05::StructHandle*>(s), index);                      // 00411BCB..00411BCF
    if (field != nullptr && rd(field, 0) == 3) {                                              // 00411BD4..00411BDB
        *ok = 1;                                                                              // 00411BDD..00411BE2
        return rd16(field, 8);                                                                // 00411BE8
    }
    *ok = 0;                                                                                  // 00411BF0..00411BFA
    return static_cast<U16>(fallback);                                                        // 00411BF4
}

// ---------------------------------------------------------------------------------------------------------------
// NWPlayerCharacterList_st (40h): +0 CExoLocString first, +8 CExoLocString last, +10h byte kind, +11h CResRef,
// +22h word portrait id, +24h CResRef portrait, +34h CExoArrayList<{int class; byte level}> (8-byte elements)
extern "C" void* __fastcall kotor_fastcall_004c6830(void* e, void*) {
    (void)exo_locstring_construct_005e9e10(e, nullptr);                                       // 004C684E
    (void)exo_locstring_construct_005e9e10(fa(e, 8), nullptr);                                // 004C685C
    resref_default(fa(e, 0x11));                                                              // 004C6869
    resref_default(fa(e, 0x24));                                                              // 004C6871
    wr(e, 0x38, 0); wr(e, 0x3c, 0); wr(e, 0x34, 0);                                           // 004C687A..004C6880
    wr8(e, 0x10, 0);                                                                          // 004C6883
    wr16(e, 0x22, 0xffff);                                                                    // 004C6886
    return e;                                                                                 // 004C688C
}
extern "C" void __fastcall kotor_fastcall_004c5df0(void* e, void*) {
    live_free(rp(e, 0x34));                                                                   // 004C5E0D..004C5E19
    wr(e, 0x34, 0);                                                                           // 004C5E26
    exo_locstring_destroy_005ea000(fa(e, 8), nullptr);                                        // 004C5E23..004C5E2D
    exo_locstring_destroy_005ea000(e, nullptr);                                               // 004C5E32..004C5E3C
}
// 004C4ED0 contains: some element with equal first and last names
extern "C" U __fastcall kotor_fastcall_004c4ed0(void* list, void*, void* e) {
    for (I i = 0; i < rdi(list, 4); ++i) {                                                    // 004C4ED5..004C4F0A (signed)
        void* const item = rp(rp(list, 0), static_cast<U>(i) * 4);                            // 004C4EE3..004C4EE5
        if (kotor_fastcall_005ea020(item, nullptr, e) != 0 &&                                 // 004C4EE8..004C4EF2
            kotor_fastcall_005ea020(fa(item, 8), nullptr, fa(e, 8)) != 0)                     // 004C4EF4..004C4F02
            return 1;                                                                         // 004C4F15..004C4F1E
    }
    return 0;                                                                                 // 004C4F0C..004C4F12
}

// 004C8E90 CSWSModule::PackPlayerCharacterListIntoMessage(CSWSPlayer* unused, list) RET 8
extern "C" void __fastcall kotor_fastcall_004c8e90(void* module, void*, void*, void* list) {
    U counter = 0;                                                                            // [esp+10h]
    ResRef name_ref; resref_default(&name_ref);                                               // 004C8EB5..004C8EC1 [esp+28h]
    ExoString number;                                                                         // 004C8EC6..004C8ECA [esp+20h]
    void* pos = rp(rp(module, 0x48), 0);                                                      // 004C8ECF..004C8EDA [esp+18h]
    while (pos != nullptr) {                                                                  // 004C8EDE / 004C901A
        void* const node = unresolved_list_at_pos_005e9b60(rp(module, 0x48), nullptr, pos);   // 004C8EF0..004C8EF4 ebx
        void* const e = new_entry();                                                          // 004C8EF9..004C8F1D edi
        (void)recovered_localized_assign_005e9e70(e, nullptr, fa(node, 8));                   // 004C8F1F..004C8F2A
        (void)recovered_localized_assign_005e9e70(fa(e, 8), nullptr, fa(node, 0x10));         // 004C8F2F..004C8F36
        wr8(e, 0x10, 0xe);                                                                    // 004C8F4B
        frame_LogFormat_005e5680(&number, "%i", static_cast<U>(static_cast<U16>(counter)));  // 004C8F3B..004C8F54 [0073E2EC]
        { ResRef tmp; (void)async_ref_assign_string_004061f0(&name_ref, nullptr, &tmp, &number); } // 004C8F57..004C8F65
        resref_assign(fa(e, 0x11), &name_ref);                                                // 004C8F6A..004C8F77
        if (kotor_fastcall_004c4ed0(list, nullptr, e) == 0) {                                 // 004C8F7C..004C8F86
            if (rd(list, 4) == rd(list, 8)) {                                                 // 004C8F88..004C8F8E
                const U capacity = rd(list, 8) != 0 ? rd(list, 8) * 2 : 0x10;                 // 004C8F90..004C8F98
                const U old = rd(list, 0);                                                    // 004C8F9D
                wr(list, 8, capacity);                                                        // 004C8FA7
                wp(list, 0, live_operator_new(capacity * 4));                                 // 004C8F9F..004C8FB2
                for (U i = 0; static_cast<I>(i) < rdi(list, 4); ++i)                          // 004C8FB7..004C8FCE
                    wr(rp(list, 0), i * 4, rd(ptr(old), i * 4));
                if (old != 0) live_free(ptr(old));                                            // 004C8FD0..004C8FD5
            }
            wp(rp(list, 0), rd(list, 4) * 4, e);                                              // 004C8FDD..004C8FE2
            wr(list, 4, rd(list, 4) + 1);                                                     // 004C8FE5
        } else {
            kotor_fastcall_004c5df0(e, nullptr);                                              // 004C8FEA..004C8FEC
            live_free(e);                                                                     // 004C8FF1..004C8FF2
        }
        (void)unresolved_list_next_005e9b90(rp(module, 0x48), nullptr, &pos);                // 004C8FFA..004C9006
        ++counter;                                                                            // 004C900B..004C9016
    }
}                                                                                             // 004C9020..004C9042

// ---------------------------------------------------------------------------------------------------------------
// 00576360 CSWSMessage::SendServerToPlayerCharList(CSWSPlayer*) RET 4
namespace {
// One SERVERVAULT character file: 00576540..005768A6 (first pass, kind 11h, AddUnique 006449D0) and
// 00576920..00576CCC (second pass, kind 02h, inlined AddUnique growing with 005E03C0).
void char_list_entry(StringList& files, U index, ExoString& name, EntryList& entries, U& level_word, bool first_pass) {
    str_assign(&name, ptr(files.data + index * 8));                                           // 00576540..0057654C
    {
        Adopted sub;                                                                          // [esp+68h]/[esp+70h]
        (void)startup_rand::SubString005e6270(ss(&name), nullptr, ss(sub.words), 0,
                                              static_cast<int>(str_len(&name) - 4));          // 00576551..00576568
        str_assign(&name, sub.words);                                                         // 0057656D..0057657A
    }                                                                                         // 0057657F..0057658B
    void* gff = nullptr;
    {
        void* const memory = live_operator_new(0xa0);                                        // 00576590..00576595
        if (memory != nullptr) {                                                              // 005765A3..005765AD
            ResRef ref; (void)async_ref_construct_string_00406d60(&ref, nullptr, &name);      // 005765AF..005765BB
            gff = gui_load_gff_construct_00410630(memory, nullptr, kResBic, "BIC ", &ref);    // 005765C0..005765D9
        }
    }
    if (rd(gff, 0x94) != 0) {                                                                 // 005765DF..005765ED
        U top = 0;                                                                            // [esp+0Ch] CResStruct
        U ok = 0;                                                                             // [esp+10h]
        (void)kotor_fastcall_00411240(gff, nullptr, &top);                                    // 005765F3..005765FA
        void* const e = new_entry();                                                          // 005765FF..00576624 edi
        {
            ExoLocString fallback;                                                            // 00576626..00576635
            AdoptedLoc out;
            void* const r = recovered_gff_localized_00411fd0(gff, nullptr, out.words, &top, "FirstName", &ok, &fallback); // 0057663A..0057665C
            (void)recovered_localized_assign_005e9e70(e, nullptr, r);                         // 00576661..0057666C
        }                                                                                     // 00576671..00576694
        {
            ExoLocString fallback;                                                            // 00576699..0057669D
            AdoptedLoc out;
            void* const r = recovered_gff_localized_00411fd0(gff, nullptr, out.words, &top, "LastName", &ok, &fallback); // 005766A2..005766C4
            (void)recovered_localized_assign_005e9e70(fa(e, 8), nullptr, r);                  // 005766C9..005766D5
        }                                                                                     // 005766DA..005766F7
        { ResRef tmp; (void)async_ref_assign_string_004061f0(fa(e, 0x11), nullptr, &tmp, &name); } // 005766FC..0057670C
        wr8(e, 0x10, first_pass ? 0x11 : 0x02);                                               // 00576727 / 00576B07
        wr16(e, 0x22, recovered_gff_word_00411b40(gff, nullptr, &top, "PortraitId", &ok, 0xffff)); // 00576711..0057673C
        {
            ResRef fallback; resref_from(&fallback, "");                                      // 00576730..00576740 [0073D71C]
            ResRef out;
            void* const r = gui_load_gff_resref_00411e10(gff, nullptr, &out, &top, "Portrait", &ok, &fallback); // 00576745..00576766
            resref_assign(fa(e, 0x24), r);                                                    // 0057676B..00576777
        }
        GffList classes;                                                                      // [esp+94h]
        if (gff_list_004118c0(gff, nullptr, &classes, &top, "ClassList") != 0) {              // 0057677C..00576797
            const U count = gff_list_count_00411940(gff, nullptr, &classes);                  // 0057679D..005767A7 [esp+3Ch]
            for (U j = 0; j < count; ++j) {                                                   // 005767AC..00576859 (jb)
                U element = 0;                                                                // [esp+50h]
                if (gff_list_element_00411990(gff, nullptr, &element, &classes, j) == 0) continue; // 005767C0..005767D7
                const I cls = gui_load_gff_int_00411c90(gff, nullptr, &element, "Class", &ok, -1); // 005767D9..005767EC [esp+44h]
                const U16 level = kotor_fastcall_00411bb0(gff, nullptr, &element, "ClassLevel", &ok, 0); // 005767F1..00576808
                level_word = (level_word & 0xffffff00u) | (level & 0xffu);                    // 00576813 (byte store into a dword slot)
                void* const classes_list = fa(e, 0x34);
                if (rd(classes_list, 4) == rd(classes_list, 8)) {                             // 0057680D..0057681C
                    const U capacity = rd(classes_list, 8) != 0 ? rd(classes_list, 8) * 2 : 0x10; // 0057681E..00576826
                    directory_list::AllocateTimes004c4f50(static_cast<directory_list::TimeList*>(classes_list), nullptr,
                                                          static_cast<int>(capacity));        // 0057682B..0057682E
                }
                const U slot = rd(classes_list, 0) + rd(classes_list, 4) * 8;                 // 00576833..00576836
                wr(ptr(slot), 0, static_cast<U>(cls));                                        // 0057683C
                wr(ptr(slot), 4, level_word);                                                 // 00576843
                wr(classes_list, 4, rd(classes_list, 4) + 1);                                 // 00576847
            }
        }
        if (kotor_fastcall_004c4ed0(&entries, nullptr, e) == 0) {                             // 0057685F..0057686B
            if (first_pass) {
                w1x_unique_append_006449d0(&entries, nullptr, word(e));                       // 0057686D..00576872
            } else {
                U found = 0;                                                                  // 00576C4D..00576C6D inlined AddUnique
                for (I k = 0; k < static_cast<I>(entries.count); ++k)
                    if (rd(ptr(entries.data), static_cast<U>(k) * 4) == word(e)) ++found;
                if (found == 0) {
                    if (entries.count == entries.capacity) {                                  // 00576C6F..00576C75
                        const U capacity = entries.capacity != 0 ? entries.capacity * 2 : 0x10; // 00576C77..00576C7F
                        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&entries),
                                                            nullptr, capacity);               // 00576C84..00576C89
                    }
                    wr(ptr(entries.data), entries.count * 4, word(e));                        // 00576C8E..00576C96
                    ++entries.count;                                                          // 00576C99
                }
            }
        } else {
            kotor_fastcall_004c5df0(e, nullptr);                                              // 00576879..0057687B
            live_free(e);                                                                     // 00576880..00576881
        }
    }
    vdelete(gff);                                                                             // 0057688F..00576895
}
}  // namespace

extern "C" U __fastcall kotor_fastcall_00576360(void* msg, void*, void* player) {
    ExoString name;                                                                           // 00576386..0057638E [esp+28h] state 0
    StringList files;                                                                         // 0057639C..005763A4 [esp+30h]
    U initial_size = 2;                                                                       // 005763B4 [esp+54h]
    ExoString player_name;                                                                    // 005763A8..005763BC [esp+60h] state 1
    ExoString vault;                                                                          // 005763C1..005763CD [esp+58h] state 2
    EntryList entries;                                                                        // 005763D2..005763DA [esp+1Ch] state 4
    U level_word = 0;                                                                         // [esp+48h] (original: stale stack bytes above the byte)
    kotor_fastcall_004d68a0(msg, nullptr, initial_size, 0xffffffffu, 1);                      // 005763DE..005763F1
    kotor_fastcall_005d5170(vcall<void*>(server(), 0x10), nullptr, rd(player, 4));            // 005763F6..0057640D (result unused)
    {
        Adopted tmp;                                                                          // [esp+44h]
        str_assign(&player_name, kotor_fastcall_005610b0(player, nullptr, tmp.words));        // 00576412..0057642B
    }                                                                                         // 00576430..0057643C
    { ExoString dir("SERVERVAULT:"); get_dir_list(&files, &dir, 0xffff, 1, 0); }              // 00576441..00576480 [00745514]
    options_video::String by_value;
    (void)WindowCopy005e5b00(&by_value, nullptr, &player_name);                               // 00576485..00576493
    if (client_intro::CountStrings_004b2640(&files, nullptr, by_value) != 0) {                // 00576498..005764A3
        if (files.data != 0) {                                                                // 005764A9..005764AF
            files.clear_storage();                                                            // 005764B1..005764C6
            files.data = 0; files.capacity = 0;                                               // 005764CE..005764D2
        }
        files.count = 0;                                                                      // 005764DA
        frame_LogFormat_005e5680(&vault, "SERVERVAULT:%s", WindowCStr005e5670(&player_name, nullptr)); // 005764D6..005764F3
        get_dir_list(&files, &vault, kResBic, 0, 0);                                          // 005764F6..0057650D
        if (files.count != 0) {                                                               // 00576512..00576516
            add_dir(&vault);                                                                  // 0057651C..00576527
            for (U i = 0; static_cast<I>(i) < static_cast<I>(files.count); ++i)               // 0057652C..005768A6 (jl)
                char_list_entry(files, i, name, entries, level_word, true);
            remove_dir(&vault);                                                               // 005768AC..005768B7
        }
    }
    { ExoString dir("SERVERVAULT:"); get_dir_list(&files, &dir, kResBic, 0, 0); }             // 005768BC..005768FA
    if (files.count != 0) {                                                                   // 005768FF..00576905
        for (U i = 0; static_cast<I>(i) < static_cast<I>(files.count); ++i)                   // 0057690B..00576CCC (jl)
            char_list_entry(files, i, name, entries, level_word, false);
    }
    if (rd(server_get_module_004ae6b0(server(), nullptr), 0x1c8) != 0)                        // 00576CD2..00576CE6
        kotor_fastcall_004c8e90(server_get_module_004ae6b0(server(), nullptr), nullptr, player, &entries); // 00576CE8..00576D01
    kotor_fastcall_004d6300(msg, nullptr, entries.count, 0x10);                               // 00576D06..00576D0F
    for (U k = 0; static_cast<I>(k) < static_cast<I>(entries.count); ++k) {                   // 00576D14..00576E4A (jl)
        void* const e = rp(ptr(entries.data), k * 4);                                         // 00576D22..00576D2A esi
        kotor_fastcall_005785c0(msg, nullptr, e, 0);                                          // 00576D2D..00576D31
        kotor_fastcall_005785c0(msg, nullptr, fa(e, 8), 0);                                   // 00576D36..00576D3D
        kotor_fastcall_004d6380(msg, nullptr, resref_value(fa(e, 0x11)), 0x10);               // 00576D42..00576D64
        kotor_fastcall_004d62c0(msg, nullptr, rd8(e, 0x10), 8);                               // 00576D69..00576D73
        kotor_fastcall_004d6300(msg, nullptr, rd16(e, 0x22), 0x10);                           // 00576D78..00576D83
        kotor_fastcall_004d6380(msg, nullptr, resref_value(fa(e, 0x24)), 0x10);               // 00576D88..00576DAA
        const U classes = rd(e, 0x38);                                                        // 00576DAF ebx
        kotor_fastcall_004d62c0(msg, nullptr, classes, 8);                                    // 00576DB2..00576DB7
        for (U j = 0; j < classes; ++j) {                                                     // 00576DBC..00576DEC (jb)
            kotor_fastcall_004d6340(msg, nullptr, rd(rp(e, 0x34), j * 8), 0x20);              // 00576DC2..00576DCF
            kotor_fastcall_004d62c0(msg, nullptr, rd8(rp(e, 0x34), j * 8 + 4), 8);            // 00576DD4..00576DE4
        }
        live_free(rp(e, 0x34));                                                               // 00576DEE..00576DFE (inlined dtor)
        wr(e, 0x34, 0);                                                                       // 00576E09
        exo_locstring_destroy_005ea000(fa(e, 8), nullptr);                                    // 00576E06..00576E18
        exo_locstring_destroy_005ea000(e, nullptr);                                           // 00576E1D..00576E27
        live_free(e);                                                                         // 00576E2C..00576E2D (data[k] left dangling)
    }
    void* data = nullptr;                                                                     // [esp+78h]
    U size = 0;                                                                               // [esp+54h]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;                   // 00576E50..00576E63 / 00576E69..00576EFF
    return kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x11, 2, data, size);         // 00576F02..00576FB6
}   // scope end: free(entries), ~vault, ~player_name, files storage, ~name (00576E69.. / 00576F20..)

// ---------------------------------------------------------------------------------------------------------------
// 00570C60 CSWSMessage::SendServerToPlayerUpdateCharResponse(CSWSPlayer*, BYTE minor, CResRef ref) RET 18h
namespace {
enum class Load { Failed, Loaded };
// ReadStatsFromGff into a fresh CSWSCreature; on failure the creature is deleted. (00570E24..00570E7B and twins)
Load read_creature(void* gff, void* s, void*& stats_slot) {
    void* const creature = new_creature();                                                    // 00570E24..00570E4F ebx
    stats_slot = fa(creature, 0xa74);                                                         // 00570E63 edi = &creature->stats
    if (w2x_creature_stats_005afce0(rp(creature, 0xa74), nullptr, gff, s, fa(creature, 0xa50)) != 0) { // 00570E51..00570E77
        vdelete(creature);                                                                    // 00570E7D..00570E81
        return Load::Failed;
    }
    return Load::Loaded;                                                                      // (creature is never deleted: leak)
}
}  // namespace

extern "C" U __fastcall kotor_fastcall_00570c60(void* msg, void*, void* player, U minor, ResRef16 ref) {
    StringList files;                                                                         // 00570C8E..00570C96 [esp+30h]
    ExoString path;                                                                           // 00570C9A..00570CA5 [esp+10h] state 1
    ExoString index_text;                                                                     // 00570CAA..00570CB6 [esp+28h] state 2
    ExoString player_name;                                                                    // 00570CBB..00570CC7 [esp+18h] state 3
    void* stats_slot = nullptr;                                                               // edi
    switch (static_cast<U8>(minor)) {                                                         // 00570CCC..00570CE9
    case 1: {                                                                                 // 0057100A
        void* const gff = new_gff_ref(&ref);                                                  // 0057100A..00571040 esi
        if (rd(gff, 0x94) == 0) { vdelete(gff); return 0; }                                   // 00571042..00571052 / 005714FC
        U top = 0;                                                                            // [esp+3Ch]
        (void)kotor_fastcall_00411240(gff, nullptr, &top);                                    // 00571058..0057105D
        if (read_creature(gff, &top, stats_slot) == Load::Failed) { vdelete(gff); return 0; } // 00571062..005710C9
        vdelete(gff);                                                                         // 005710CE..005710D2
        break;                                                                                // -> 00570EB6
    }
    case 4: {                                                                                 // 00570EE5
        c2da_ref_to_string_00405f70(&ref, nullptr, &index_text);                              // 00570EE5..00570EED
        ResRef module_ref; resref_from(&module_ref, "Module");                                // 00570EF2..00570EFE [007442A8]
        void* const res = c2da_lookup_004074d0(resman(), nullptr, &module_ref, kResIfo);      // 00570F03..00570F16
        if (res == nullptr) return 0;                                                         // 00570F1B..00570F1F
        (void)c2da_current_resource_demand_00409b10(res, nullptr);                            // 00570F25..00570F27
        U top = 0;
        (void)kotor_fastcall_00411240(res, nullptr, &top);                                    // 00570F2C..00570F33
        GffList players;                                                                      // [esp+8Ch]
        U element = 0;                                                                        // [esp+58h]
        bool loaded = false;
        if (gff_list_004118c0(res, nullptr, &players, &top, "Mod_PlayerList") != 0 &&         // 00570F38..00570F53
            gff_list_element_00411990(res, nullptr, &element, &players,
                static_cast<U>(reinterpret_cast<kotor_menu20::String*>(&index_text)->as_int_005e5630())) != 0) { // 00570F59..00570F79
            loaded = read_creature(res, &element, stats_slot) == Load::Loaded;               // 00570F7B..00570FD8
        }
        (void)c2da_release_demand_00409b80(res, nullptr);                                     // 00570FDA / 00570FF2
        (void)c2da_release_00409cf0(resman(), nullptr, res);                                  // 00570FE1..00570FE8 / 00570FF9..00571000
        if (!loaded) return 0;                                                                // 00570FED -> 00571502
        break;                                                                                // 00571005 -> 00570EB6
    }
    case 5: {                                                                                 // 00570CEF
        {
            Adopted tmp;                                                                      // [esp+74h]
            str_assign(&player_name, kotor_fastcall_005610b0(player, nullptr, tmp.words));    // 00570CEF..00570D09
        }                                                                                     // 00570D0E..00570D1A
        { ExoString dir("SERVERVAULT:"); get_dir_list(&files, &dir, 0xffff, 1, 0); }          // 00570D1F..00570D5E [00745514]
        options_video::String by_value;
        (void)WindowCopy005e5b00(&by_value, nullptr, &player_name);                           // 00570D63..00570D71
        if (client_intro::CountStrings_004b2640(&files, nullptr, by_value) == 0) return 0;    // 00570D76..00570D85
        frame_LogFormat_005e5680(&path, "SERVERVAULT:%s", WindowCStr005e5670(&player_name, nullptr)); // 00570D8B..00570DA6 [00745340]
        add_dir(&path);                                                                       // 00570DA0..00570DAE
        void* const gff = new_gff_ref(&ref);                                                  // 00570DB3..00570DE9 esi
        if (rd(gff, 0x94) == 0) {                                                             // 00570DEB..00570DF9
            vdelete(gff);                                                                     // 00570DFB..00570E01
            remove_dir(&path);                                                                // 00570E03..00570E0E
            return 0;                                                                         // 00570E13
        }
        U top = 0;
        (void)kotor_fastcall_00411240(gff, nullptr, &top);                                    // 00570E18..00570E1F
        if (read_creature(gff, &top, stats_slot) == Load::Failed) {                           // 00570E24..00570E7B
            vdelete(gff);                                                                     // 00570E83..00570E89
            remove_dir(&path);                                                                // 00570E8B..00570E96
            return 0;                                                                         // 00570E9B
        }
        vdelete(gff);                                                                         // 00570EA0..00570EA4
        remove_dir(&path);                                                                    // 00570EA6..00570EB1
        break;                                                                                // -> 00570EB6
    }
    default:
        return 0;                                                                             // 005710D9..00571140 (inlined list dtor)
    }
    // 00570EB6: write the loaded stats into a fresh BIC and ship the file bytes
    void* const gff = new_gff_empty();                                                        // 00570EB6..00571145 esi
    void* const root = live_operator_new(4);                                                  // 00571147..00571159 ebx [esp+24h] (unchecked)
    U created;
    {
        ExoString version("V2.0");                                                            // 0057115B..00571168 [00745338]
        ExoString type("BIC ");                                                               // 0057116D..0057117E [00745384]
        created = recovered_gff_00411260(gff, nullptr, root, &type, &version);                // 00571183..00571198
    }                                                                                         // 0057119D..005711BE
    if (created == 0) {                                                                       // 005711C3..005711C9
        live_free(root);                                                                      // 005714ED..005714EE
        if (gff != nullptr) vdelete(gff);                                                     // 005714F6..00571500
        return 0;
    }
    U result = 0;
    {
        ExoString temp_name;                                                                  // 005711CF..005711D3 [esp+4Ch] state 0Eh
        ExoString unused;                                                                     // 005711D8..005711E4 [esp+64h] state 0Fh
        recovered_save_stats_005b1b90(rp(stats_slot, 0), nullptr, gff, root);                 // 005711E9..005711F7
        (void)GuiAssign005e5140(&temp_name, nullptr, "TEMP:temp_charfile");                   // 005711FC..00571205 [00749070]
        (void)recovered_gff_00413030(gff, nullptr, &temp_name, kResBic);                      // 0057120A..00571216
        { ExoString temp("TEMP:"); add_dir(&temp); }                                          // 0057121B..0057124D [00745310]
        void* file = nullptr;                                                                 // [esp+20h]
        {
            void* const memory = live_operator_new(4);                                        // 00571252..0057125E
            if (memory != nullptr) {                                                          // 00571262..0057126C
                ExoString mode("rb");                                                         // 0057126E..00571277 [0073D5B0] (flag [esp+54h])
                file = file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(memory), nullptr,
                                                                 ss(&temp_name), kResBic, ss(&mode)); // 0057127C..005712A4
            }                                                                                 // 005712B6..005712CC
        }
        if (file_opened_005e6a10(file, nullptr) == 0) {                                       // 005712D1..005712DA
            if (file != nullptr) { c2da_005e69e0(file, nullptr); live_free(file); }           // 005714A0..005714B1
            live_free(root);                                                                  // 005714B4..005714B5 (ebx = root here)
            if (gff != nullptr) vdelete(gff);                                                 // 005714BD..005714C7
        } else {
            const U size = exofile_size_005e6950(file, nullptr);                              // 005712E0..005712E7 edi
            void* const bytes = live_operator_new(size);                                      // 005712E9..005712F7 ebx
            (void)exofile_read_005e6960(file, nullptr, bytes, 1, size);                       // 005712F6..005712FC
            if (file != nullptr) { c2da_005e69e0(file, nullptr); live_free(file); }           // 00571301..00571318
            kotor_fastcall_004d68a0(msg, nullptr, 0x15, 0xffffffffu, 1);                      // 0057131B..00571325
            kotor_fastcall_004d62c0(msg, nullptr, minor, 8);                                  // 0057132A..00571334
            kotor_fastcall_004d6380(msg, nullptr, ref, 0x10);                                 // 00571339..0057135B
            kotor_fastcall_004d6340(msg, nullptr, size, 0x20);                                // 00571360..00571367
            kotor_fastcall_004d63e0(msg, nullptr, bytes, size);                               // 0057136C..00571374
            { ExoString temp("TEMP:"); remove_dir(&temp); }                                   // 00571379..005713AB
            (void)gui_load_resman_remove_file_00406b20(resman(), nullptr, &temp_name, kResBic); // 005713B0..005713C0
            live_free(bytes);                                                                 // 005713C5..005713C6
            result = 1;                                                                       // continue at 005713CE
        }
    }                                                                                         // 005713CE..005713EB / 005714C9..005714E6
    if (result == 0) return 0;                                                                // 005714EB -> 00571502
    live_free(root);                                                                          // 005713F0..005713F5 ([esp+24h])
    if (gff != nullptr) vdelete(gff);                                                         // 005713FD..00571407
    void* data = nullptr;                                                                     // [esp+60h]
    U size = 0;                                                                               // [esp+5Ch]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;                   // 00571409..0057141C
    return kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x11, 4, data, size);         // 00571422..00571439
}   // scope end: ~player_name, ~index_text, ~path, ~CExoArrayList<CExoString> (00571442..00571482 / 00571502..00571544)

// ---------------------------------------------------------------------------------------------------------------
// 00561870 CSWSPlayer::PackCreatureIntoMessage() RET 0
extern "C" U __fastcall kotor_fastcall_00561870(void* player, void*) {
    U result = 1;                                                                             // 005618A2..005618A7 [esp+14h]
    void* const msg = recovered_server_sws_message_004aedb0(server(), nullptr);              // 00561897..005618B5 [esp+20h]
    void* const gff = new_gff_empty();                                                        // 005618B0..005618DC ebx [esp+18h]
    void* const root = live_operator_new(4);                                                  // 005618E0..005618FD ebp [esp+24h] (unchecked)
    U created;
    {
        ExoString version("V2.0");                                                            // 005618F4..00561901 [00745338]
        ExoString type("BIC ");                                                               // 00561906..00561913 [00745384]
        created = recovered_gff_00411260(gff, nullptr, root, &type, &version);                // 00561918..0056192A
    }                                                                                         // 0056192F..0056194B
    if (created == 0) {                                                                       // 00561950..00561952
        result = 0;                                                                           // 00561C0A
    } else {
        ExoString temp_name;                                                                  // 00561958..0056195C [esp+30h] state 3
        ExoString unused;                                                                     // 00561961..0056196D [esp+48h] state 4
        void* object = kotor_fastcall_004ae750(server(), nullptr, rd(player, 0x28));          // 00561972..00561983
        if (!(object != nullptr && rd8(object, 8) > kObjectTypeCreatureMin)) object = nullptr; // 00561988..00561999 (ja)
        void* const creature = vcall<void*>(object, 0x30);                                    // 0056199B..0056199F (null object faults)
        if (creature == nullptr) {                                                            // 005619A4..005619A6
            result = 0;                                                                       // 00561BE1
        } else {
            if (rd(creature, 0xa48) == 1) {                                                   // 005619AC..005619B3
                const I count = rdi(creature, 0x128);                                         // 005619B5
                if (count > 0) {                                                              // 005619BB..005619BD (jle)
                    for (I i = 0; i < count; ++i) {                                           // 005619C5..005619E4 (jl)
                        void* const effect = rp(rp(creature, 0x124), static_cast<U>(i) * 4);
                        if (rd16(effect, 8) != kEffectType3e) continue;                       // 005619D0..005619DC
                        recovered_effect_integer_005036a0(effect, nullptr, 2, 1);             // 005619E8..005619EF
                        void* const again = rp(rp(creature, 0x124), static_cast<U>(i) * 4);   // 005619F4..005619FA
                        recovered_object_remove_effect_004d06c0(creature, nullptr, rd(again, 0), rd(again, 4)); // 005619FD..00561A06
                        break;
                    }
                }
                recovered_stats_refresh_005addc0(rp(creature, 0xa74), nullptr);              // 00561A0B..00561A11
            }
            kotor_fastcall_004d68a0(msg, nullptr, 0x1000, 0xffffffffu, 1);                    // 00561A1A..00561A27
            (void)recovered_save_creature_00500610(creature, nullptr, gff, root);             // 00561A2C..00561A30
            (void)GuiAssign005e5140(&temp_name, nullptr, "TEMP:temp_charfile");               // 00561A35..00561A3E [00749070]
            (void)recovered_gff_00413030(gff, nullptr, &temp_name, kResBic);                  // 00561A43..00561A4F
            { ExoString temp("TEMP:"); add_dir(&temp); }                                      // 00561A54..00561A80 [00745310]
            void* file = nullptr;                                                             // ebp
            {
                void* const memory = live_operator_new(4);                                    // 00561A85..00561A91
                if (memory != nullptr) {                                                      // 00561A95..00561A9C
                    ExoString mode("rb");                                                     // 00561A9E..00561AA7 [0073D5B0] (flag [esp+1Ch])
                    file = file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(memory), nullptr,
                                                                     ss(&temp_name), kResBic, ss(&mode)); // 00561AAC..00561ACF
                }                                                                             // 00561AD5..00561AE8
            }
            if (file_opened_005e6a10(file, nullptr) != 0) {                                   // 00561AED..00561AF6
                const U size = exofile_size_005e6950(file, nullptr);                          // 00561AFC..00561B03 edi
                void* const bytes = live_operator_new(size);                                  // 00561B05..00561B11 ebx
                (void)exofile_read_005e6960(file, nullptr, bytes, 1, size);                   // 00561B0E..00561B16
                if (file != nullptr) { c2da_005e69e0(file, nullptr); live_free(file); }       // 00561B1B..00561B27
                kotor_fastcall_005785c0(msg, nullptr, vcall<void*>(creature, 0x8c), 0);       // 00561B2F..00561B42 first name
                kotor_fastcall_005785c0(msg, nullptr, vcall<void*>(creature, 0x90), 0);       // 00561B47..00561B56 last name
                kotor_fastcall_004d6340(msg, nullptr, size, 0x20);                            // 00561B5B..00561B60
                kotor_fastcall_004d63e0(msg, nullptr, bytes, size);                           // 00561B65..00561B69
                { ExoString temp("TEMP:"); remove_dir(&temp); }                               // 00561B6E..00561B9A
                (void)gui_load_resman_remove_file_00406b20(resman(), nullptr, &temp_name, kResBic); // 00561B9F..00561BAF
                live_free(bytes);                                                             // 00561BB4..00561BB5
            } else {
                result = 0;                                                                   // 00561BC3..00561BC5
                if (file != nullptr) { c2da_005e69e0(file, nullptr); live_free(file); }       // 00561BCD..00561BD7
            }
        }
    }                                                                                         // 00561BE5..00561BFF
    live_free(root);                                                                          // 00561C12..00561C13
    if (gff != nullptr) vdelete(gff);                                                         // 00561C1B..00561C25
    return result;                                                                            // 00561C27..00561C3D
}

// 0056B090 CSWSMessage::SendServerToPlayerCharacterDownloadReply(CSWSPlayer*) RET 4
extern "C" U __fastcall kotor_fastcall_0056b090(void* msg, void*, void* player) {
    U restore = 0;                                                                            // 0056B09D edi
    if (kotor_fastcall_00561080(player, nullptr) == nullptr) return 0;                        // 0056B09B..0056B0A6
    void* const creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30);     // 0056B0A8..0056B0B3
    if (creature == nullptr) return 0;                                                        // 0056B0B6..0056B0BA
    if (rd(creature, 0x9dc) != 0) { wr(creature, 0xf8, 0); restore = 1; }                     // 0056B0BC..0056B0CC
    if (kotor_fastcall_00561870(player, nullptr) == 0) {                                      // 0056B0D1..0056B0DA
        if (restore != 0) wr(creature, 0xf8, 1);                                              // 0056B0DC..0056B0E0
        return 0;                                                                             // 0056B0EA..0056B0F1
    }
    if (restore != 0) wr(creature, 0xf8, 1);                                                  // 0056B0F4..0056B0F8
    void* data = nullptr;                                                                     // [esp+10h]
    U size = 0;                                                                               // [esp+18h] (argument slot)
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;                   // 0056B102..0056B115
    return kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x2b, 2, data, size);         // 0056B117..0056B135
}

// 0056B140 CSWSMessage::SendServerToPlayerCharacterDownloadFail(CSWSPlayer*) RET 4
extern "C" U __fastcall kotor_fastcall_0056b140(void* msg, void*, void* player) {
    ExoString name;                                                                           // 0056B15C..0056B160 [esp+10h]
    if (kotor_fastcall_005624c0(player, nullptr) == 0) return 0;                              // 0056B165..0056B17A
    c2da_ref_to_string_00405f70(fa(player, 0x65), nullptr, &name);                            // 0056B17C..0056B184
    kotor_fastcall_004d68a0(msg, nullptr, str_len(&name), 0xffffffffu, 1);                    // 0056B189..0056B199
    kotor_fastcall_004d6960(msg, nullptr, &name, 0x20);                                       // 0056B19E..0056B1A7
    kotor_fastcall_004d68e0(msg, nullptr, rd8(player, 0x64) == 4 ? 1u : 0u);                  // 0056B1AC..0056B1B9
    void* data = nullptr;                                                                     // [esp+0Ch]
    U size = 0;                                                                               // [esp+8]
    if (kotor_fastcall_004d6440(msg, nullptr, &data, &size) == 0) return 0;                   // 0056B1BE..0056B1D1
    return kotor_fastcall_00567e60(msg, nullptr, rd(player, 4), 0x2b, 3, data, size);         // 0056B1F9..0056B237
}

// ---------------------------------------------------------------------------------------------------------------
// 005246F0 CSWSMessage::HandlePlayerToServerCharListMessage(CSWSPlayer*, BYTE minor) RET 8
extern "C" U __fastcall recovered_player_to_server_char_list_005246f0(void* message, void*, void* player, U minor) {
    ResRef ref; resref_default(&ref);                                                         // 005246F6..005246FA [esp+4]
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                          // 005246FF..0052470A
    const U8 sub = static_cast<U8>(minor);                                                    // 0052470C movzx
    if (sub == 1) {                                                                           // 00524711..00524712
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                      // 00524791..0052479C
        if (kotor_fastcall_004aedd0(server(), nullptr) == 1 ||                                // 005247A2..005247B4
            kotor_fastcall_004aedd0(server(), nullptr) == 2)                                  // 005247B6..005247C8
            return kotor_fastcall_00576360(message, nullptr, player);                         // 005247EC..005247FC
        kotor_fastcall_004aea00(server(), nullptr, rd(player, 4));                            // 005247CA..005247DB
        return 1;                                                                             // 005247E0..005247E9
    }
    if (sub != 3) return 0;                                                                   // 00524714..00524719
    const U8 kind = kotor_fastcall_004d6720(message, nullptr, 8);                             // 00524722..00524726
    minor = (minor & 0xffffff00u) | kind;                                                     // 0052472B (byte store into the minor slot)
    {
        ResRef read;                                                                          // [esp+14h]
        void* const r = kotor_fastcall_004d6180(message, nullptr, &read, 0x10);               // 0052472F..00524738
        resref_assign(&ref, r);                                                               // 0052473D..00524747
    }
    if (kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;                          // 0052474C..00524757
    return kotor_fastcall_00570c60(message, nullptr, player, minor, resref_value(&ref));      // 00524759..0052478E
}

// 00525350 CSWSMessage::HandlePlayerToServerCharacterDownload(CSWSPlayer*, BYTE minor) RET 8
extern "C" U __fastcall recovered_player_to_server_character_download_00525350(void* message, void*, void* player, U minor) {
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;                          // 00525351..0052535C
    if (static_cast<U8>(minor) != 1) return 0;                                                // 00525364..0052536A
    if (rd8(player, 0x64) == 1)                                                               // 0052536C..00525377
        return kotor_fastcall_0056b090(message, nullptr, player);                             // 00525382..00525388
    return kotor_fastcall_0056b140(message, nullptr, player);                                 // 00525379..0052537F
}
