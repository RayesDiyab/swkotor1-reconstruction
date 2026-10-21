// claude_closeB_load_b.cpp (lane worker closeB, load fork): saved-game load gates of the 0886 unit, part B, transcribed
// from the original swkotor.exe (read-only):
//   00411D70 CResGFF::ReadFieldDWORD64(CResStruct*, char*, int* ok, uint64 default)    RET 14h  kotor_fastcall_00411d70
//   005043A0 CGameEffect::LoadGameEffect(CResGFF*, CResStruct*)                        RET 8    kotor_fastcall_005043a0
//   004D1BE0 CSWSObject::LoadEffectList(CResGFF*, CResStruct*)                         RET 8    kotor_fastcall_004d1be0
//            (+ closeB_object_load_effect_list_tail_004d1c36: the loop from 004D1C36 on, for the 0886 ObjectCommon port)
//   004D0480 CSWSObject::LoadListenData(CResGFF*, CResStruct*)                         RET 8    kotor_fastcall_004d0480
//   004EB020 CSWSCreaturePartyFollowInfo::Load(CResGFF*, CResStruct*)                  RET 8    kotor_fastcall_004eb020
//   004FB180 CSWSCreature::LoadFollowInfo(CResGFF*, CResStruct*)                       RET 8    kotor_fastcall_004fb180
//   00500350 CSWSCreature::LoadCreature(CResGFF*, CResStruct*, int from_save)          RET 0Ch, EAX 0/1 kotor_fastcall_00500350
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only. Reader status
// out-parameters are argument slots in the original (here locals); handle buffers are 10h-byte locals.
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
using U = std::uint32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
using U64 = std::uint64_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline U rd8(U a) { return *static_cast<U8*>(ptr(a)); }
inline U rd16(U a) { U16 v; std::memcpy(&v, ptr(a), 2); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<U8*>(ptr(a)) = static_cast<U8>(v); }
inline void wr16(U a, U v) { const U16 x = static_cast<U16>(v); std::memcpy(ptr(a), &x, 2); }
inline void wrf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
inline float bits_float(U v) { float f; std::memcpy(&f, &v, 4); return f; }
template <class R, class... A> R vcall(U object, U slot, A... a) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
struct Vector { float x, y, z; };
constexpr U k_effect_type_0746714 = 0x26;   // [00746714]
constexpr U k_effect_type_0746708 = 0x23;   // [00746708]
constexpr U k_duration_innate_07473bc = 3;  // word [007473BC]
}

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); } // 005B3190 CExoString() [0603]
namespace kotor_q05 { struct StructHandle; struct FieldRecord; struct CResGFF {
    FieldRecord* __thiscall GetField(StructHandle*, unsigned); unsigned __thiscall GetDataField(FieldRecord*, unsigned*); }; }

extern "C" {
void* __cdecl live_operator_new(unsigned);                                                 // 006FA7E6
void __cdecl live_free(void*);                                                             // 006FA390
U __fastcall gff_label_00411630(void* gff, void*, void* st, const char* label);           // 00411630 [0291]
U __fastcall gui_load_gff_struct_00411a10(void* gff, void*, void* out, void* parent, const char* label); // 00411A10 [0292]
U __fastcall gff_list_004118c0(void* gff, void*, void* out, void* parent, const char* label); // 004118C0 [0291]
U __fastcall gff_list_count_00411940(void* gff, void*, void* list);                      // 00411940 [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index); // 00411990 [0291]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* st, const char* label, U* ok, unsigned char def); // [0292]
unsigned short __fastcall recovered_gff_word_00411b40(void* gff, void*, void* st, const char* label, U* ok, unsigned short def); // (0886)
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* st, const char* label, U* ok, U def); // [0292]
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* st, const char* label, U* ok, int def); // [0292]
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* st, const char* label, U* ok, float def); // [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* st, const char* label, U* ok, void* def); // [0292]
Vector* __fastcall gui_load_gff_vector_004122a0(void* gff, void*, Vector* out, void* st, const char* label, U* ok, const Vector* def); // 004122A0 [0292]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);          // 005E5A90 [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                             // 005E5C20 [0459]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);               // 005E5C50 [0188]
void __fastcall recovered_effect_creator_00503a00(void* effect, void*, U id);             // 00503A00 SetCreator (0886)
void __fastcall recovered_effect_resize_00503630(void* effect, void*, U count);           // 00503630 SetNumIntegers [1091]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);        // 00503E40 CGameEffect(int) (0886)
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // 004D14F0 ApplyEffect (0886)
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                   // 00503590 ~CGameEffect [w1 b7]
void __fastcall recovered_object_set_listen_expression_004cf8f0(void* object, void*, void* text, U id); // 004CF8F0 [vm_actions_t1]
void* __fastcall recovered_follow_info_construct_004f79e0(void* info, void*);             // 004F79E0 [w2_ifo_native]
U __fastcall w2x_creature_stats_005afce0(void* stats, void*, void* gff, void* st, void* out); // 005AFCE0 ReadStatsFromGff (0886)
void __fastcall recovered_set_detect_mode_0050ee30(void* creature, void*, U mode);        // 0050EE30 [w2_ifo_native]
void __fastcall recovered_set_stealth_mode_0050ee50(void* creature, void*, U mode, U flag); // 0050EE50 [w2_ifo_native]
void __fastcall recovered_creature_scripts_004ebf20(void* creature, void*, void* gff, void* st); // 004EBF20 [0990]
void __fastcall recovered_creature_inventory_004ffda0(void* creature, void*, void* gff, void* st, U a, U b, U c); // 004FFDA0 [0995]
void __fastcall w2x_known_spells_005aeb30(void* stats, void*, void* gff, void* st);       // 005AEB30 (0886)
void __fastcall w2x_template_finish_004f1c40(void* creature, void*);                      // 004F1C40 PostProcess (0886)
void __fastcall w2x_object_state_004d1cf0(void* object, void*, void* gff, void* st);      // 004D1CF0 LoadObjectState (0886)

U64 __fastcall kotor_fastcall_00411d70(void* gff, void*, void* st, const char* label, U* ok, U64 def);
void __fastcall kotor_fastcall_005043a0(void* effect, void*, void* gff, void* st);
void __fastcall closeB_object_load_effect_list_tail_004d1c36(void* object, void*, void* gff, void* list);
void __fastcall kotor_fastcall_004d1be0(void* object, void*, void* gff, void* root);
void __fastcall kotor_fastcall_004d0480(void* object, void*, void* gff, void* root);
void __fastcall kotor_fastcall_004eb020(void* info, void*, void* gff, void* st);
void __fastcall kotor_fastcall_004fb180(void* creature, void*, void* gff, void* root);
U __fastcall kotor_fastcall_00500350(void* creature, void*, void* gff, void* root, U from_save);
}

// ===================================================================================== 00411D70
U64 __fastcall kotor_fastcall_00411d70(void* gff, void*, void* st, const char* label, U* ok, U64 def) {
    if (st == nullptr || label == nullptr) { *ok = 0; return def; }                     // 00411D76..00411D82, 00411DF9..00411E0D
    const U index = gff_label_00411630(gff, nullptr, st, label);                         // 00411D84..00411D86
    kotor_q05::FieldRecord* const field = static_cast<kotor_q05::CResGFF*>(gff)->GetField(
        static_cast<kotor_q05::StructHandle*>(st), index);                               // 00411D8B..00411D8F
    if (field == nullptr || rd(word(field)) != 6) { *ok = 0; return def; }               // 00411D94..00411D9B, 00411DE2..00411DF6
    U size = 0;                                                                          // [esp+0C] (struct argument slot reused)
    const U data = static_cast<kotor_q05::CResGFF*>(gff)->GetDataField(field, &size);    // 00411D9D..00411DA5
    if (data == 0 || size < 8) { *ok = 0; return def; }                                  // 00411DAC..00411DB5, 00411DCB..00411DDF
    *ok = 1;                                                                             // 00411DBB
    U64 v; std::memcpy(&v, ptr(data), 8); return v;                                      // 00411DC1..00411DC3
}

// ===================================================================================== 005043A0
void __fastcall kotor_fastcall_005043a0(void* effect, void*, void* gff, void* st) {
    const U e = word(effect);
    alignas(4) unsigned char temp[8];                                                    // [esp+18] CExoString
    kotor_movie::string_element_construct_005b3190(temp, nullptr);                       // 005043BE..005043C2 CExoString()
    U ok = 0;
    const U64 id = kotor_fastcall_00411d70(gff, nullptr, st, "Id", &ok, 0);              // 005043CF..005043E8
    wr(e, static_cast<U>(id)); wr(e + 4, static_cast<U>(id >> 32));                      // 005043FC..005043FE
    wr16(e + 8, recovered_gff_word_00411b40(gff, nullptr, st, "Type", &ok, 0));          // 005043ED..00504418
    wr16(e + 0xa, recovered_gff_word_00411b40(gff, nullptr, st, "SubType", &ok, 0));     // 00504406..00504423
    wrf(e + 0xc, gui_load_gff_float_00411d00(gff, nullptr, st, "Duration", &ok, 0.0f));  // 00504421..0050443B
    wr(e + 0x88, gui_load_gff_byte_00411a60(gff, nullptr, st, "SkipOnLoad", &ok, 0));   // 00504439..00504462
    const U day = gui_load_gff_dword_00411c20(gff, nullptr, st, "ExpireDay", &ok, 0);   // 00504450..0050446F
    const U time = gui_load_gff_dword_00411c20(gff, nullptr, st, "ExpireTime", &ok, 0); // 0050446D..0050447E
    wr(e + 0x10, day); wr(e + 0x14, time);                                               // 00504492..00504495
    recovered_effect_creator_00503a00(effect, nullptr,
        gui_load_gff_dword_00411c20(gff, nullptr, st, "CreatorId", &ok, 0));             // 00504483..005044A0
    wr(e + 0x1c, gui_load_gff_dword_00411c20(gff, nullptr, st, "SpellId", &ok, 0));      // 005044A5..005044BB
    wr(e + 0x20, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "IsExposed", &ok, 0))); // 005044B9..005044DF
    recovered_effect_resize_00503630(effect, nullptr,
        static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "NumIntegers", &ok, 0))); // 005044D0..005044EA
    alignas(4) unsigned char list[0x14] = {}, element[0x10] = {};
    const U type = rd16(e + 8);                                                          // 005044EF
    if ((type == k_effect_type_0746714 || type == k_effect_type_0746708) &&
        static_cast<int>(rd(e + 0x30)) < 0x15) {                                         // 005044F3..00504517
        // old-format integer list: first NumIntegers-6 values, zeros up to index 15, the rest after (to 21)
        const U split = rd(e + 0x30) - 6;                                                // 00504527..0050452D [esp+54]
        if (gff_list_004118c0(gff, nullptr, list, st, "IntList") == 1) {                 // 0050451D..00504539
            U i = 0, j = 0;                                                              // [esp+58] / ebp
            do {
                if (i < split) {                                                         // 00504547..0050454B (jae)
                    // quirk kept: when the list is shorter than needed the original spins here without advancing
                    if (j >= gff_list_count_00411940(gff, nullptr, list)) continue;      // 0050454D..0050455B -> 005045EE
                    gff_list_element_00411990(gff, nullptr, element, list, j);           // 00504561..0050456E
                    wr(rd(e + 0x34) + j * 4, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, element, "Value", &ok, 0))); // 00504573..0050458E
                    ++j; ++i;                                                            // 005045E4..005045EA
                } else if (i < 0xf) {                                                    // 00504593..00504596
                    wr(rd(e + 0x34) + j * 4, 0);                                         // 00504598..0050459B
                    ++i;                                                                 // 005045E9 (j not advanced)
                } else {
                    if (j >= gff_list_count_00411940(gff, nullptr, list)) continue;      // 005045A4..005045B2 -> 005045EE
                    gff_list_element_00411990(gff, nullptr, element, list, j);           // 005045B4..005045C1
                    wr(rd(e + 0x34) + j * 4, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, element, "Value", &ok, 0))); // 005045C6..005045E1
                    ++j; ++i;
                }
            } while (i < 0x15);                                                          // 005045EE..005045F5
        }
        recovered_effect_resize_00503630(effect, nullptr, 0x15);                         // 005045FB..005045FF
    } else if (static_cast<int>(rd(e + 0x30)) > 0 &&                                     // 00504606..0050460B
               gff_list_004118c0(gff, nullptr, list, st, "IntList") == 1 &&              // 0050460D..00504622
               gff_list_count_00411940(gff, nullptr, list) != 0) {                       // 00504624..00504634
        U j = 0;
        do {
            gff_list_element_00411990(gff, nullptr, element, list, j);                   // 00504636..00504643
            wr(rd(e + 0x34) + j * 4, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, element, "Value", &ok, 0))); // 00504648..00504663 (no bound: quirk)
            ++j;
        } while (j < gff_list_count_00411940(gff, nullptr, list));                       // 00504666..00504675
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "FloatList") == 1) {                   // 00504677..0050468C
        for (U j = 0; j < 4; ++j) {                                                      // 0050468E..005046D2 (count not checked)
            gff_list_element_00411990(gff, nullptr, element, list, j);
            wrf(e + 0x38 + j * 4, gui_load_gff_float_00411d00(gff, nullptr, element, "Value", &ok, 0.0f));
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "StringList") == 1) {                  // 005046D4..005046E9
        for (U j = 0; j < 6; ++j) {                                                      // 005046EF..00504785
            gff_list_element_00411990(gff, nullptr, element, list, j);                   // 005046F8..00504705
            alignas(4) unsigned char def[8], out[8];
            frame_LogStringCtor_005e5a90(def, nullptr, "");                              // 0050470A..00504713 ([0073D71C])
            void* const text = gui_load_gff_string_00411ec0(gff, nullptr, out, element, "Value", &ok, def); // 00504718..00504738
            async_assign_string_005e5c50(temp, nullptr, text);                           // 0050473D..00504747
            frame_LogStringDtor_005e5c20(out, nullptr);                                  // 0050474C..00504755
            frame_LogStringDtor_005e5c20(def, nullptr);                                  // 0050475A..00504763
            async_assign_string_005e5c50(ptr(e + 0x48 + j * 8), nullptr, temp);          // 00504768..00504771
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "ObjectList") == 1) {                  // 0050478B..005047A0
        for (U j = 0; j < 4; ++j) {                                                      // 005047A2..005047DA
            gff_list_element_00411990(gff, nullptr, element, list, j);
            wr(e + 0x78 + j * 4, gui_load_gff_dword_00411c20(gff, nullptr, element, "Value", &ok, 0));
        }
    }
    frame_LogStringDtor_005e5c20(temp, nullptr);                                         // 005047DC..005047E8
}

// ===================================================================================== 004D1BE0
void __fastcall closeB_object_load_effect_list_tail_004d1c36(void* object, void*, void* gff, void* list) {
    alignas(4) unsigned char element[0x10] = {};
    U i = 0;                                                                             // ebx
    do {
        gff_list_element_00411990(gff, nullptr, element, list, i);                       // 004D1C37..004D1C44 (result unchecked)
        void* const memory = live_operator_new(0x8c);                                    // 004D1C49..004D1C53
        void* const effect = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 0) : nullptr; // 004D1C56..004D1C73
        kotor_fastcall_005043a0(effect, nullptr, gff, element);                          // 004D1C75..004D1C85 (NULL unchecked)
        const U e = word(effect);
        if (rd(e + 0x88) != 1 && (rd8(e + 0xa) & 7) != k_duration_innate_07473bc) {      // 004D1C8A..004D1CA2
            recovered_effect_apply_004d14f0(object, nullptr, effect, 1, 0);              // 004D1CA4..004D1CAD
        } else {
            recovered_effect_destroy_00503590(effect, nullptr);                          // 004D1CB4..004D1CB6
            live_free(effect);                                                           // 004D1CBB..004D1CBC
        }
        ++i;                                                                             // 004D1CCB
    } while (i < gff_list_count_00411940(gff, nullptr, list));                           // 004D1CC4..004D1CD3 (jb)
}
void __fastcall kotor_fastcall_004d1be0(void* object, void*, void* gff, void* root) {
    alignas(4) unsigned char list[0x14] = {};
    if (gff_list_004118c0(gff, nullptr, list, root, "EffectList") == 0) return;          // 004D1BFD..004D1C19
    if (gff_list_count_00411940(gff, nullptr, list) == 0) return;                        // 004D1C20..004D1C30
    closeB_object_load_effect_list_tail_004d1c36(object, nullptr, gff, list);            // 004D1C36..
}

// ===================================================================================== 004D0480
void __fastcall kotor_fastcall_004d0480(void* object, void*, void* gff, void* root) {
    alignas(4) unsigned char temp[8], list[0x14] = {}, element[0x10] = {};
    kotor_movie::string_element_construct_005b3190(temp, nullptr);                       // 004D04A5..004D04A9 CExoString()
    U ok = 0;
    if (gff_list_004118c0(gff, nullptr, list, root, "ExpressionList") != 0 &&            // 004D04B4..004D04CE
        gff_list_count_00411940(gff, nullptr, list) != 0) {                              // 004D04D4..004D04E2
        U i = 0;
        do {
            gff_list_element_00411990(gff, nullptr, element, list, i);                   // 004D04E8..004D04F5
            const int id = gui_load_gff_int_00411c90(gff, nullptr, element, "ExpressionId", &ok, 0); // 004D04FA..004D051B
            alignas(4) unsigned char def[8], out[8];
            frame_LogStringCtor_005e5a90(def, nullptr, "");                              // 004D0512..004D051D
            void* const text = gui_load_gff_string_00411ec0(gff, nullptr, out, element, "ExpressionString", &ok, def); // 004D0522..004D0542
            async_assign_string_005e5c50(temp, nullptr, text);                           // 004D0547..004D0551
            frame_LogStringDtor_005e5c20(out, nullptr);                                  // 004D0556..004D055F
            frame_LogStringDtor_005e5c20(def, nullptr);                                  // 004D0564..004D056D
            recovered_object_set_listen_expression_004cf8f0(object, nullptr, temp, static_cast<U>(id)); // 004D0572..004D057C
            ++i;
        } while (i < gff_list_count_00411940(gff, nullptr, list));                       // 004D0581..004D0590 (jb)
    }
    frame_LogStringDtor_005e5c20(temp, nullptr);                                         // 004D0596..004D05A2
}

// ===================================================================================== 004EB020 / 004FB180
void __fastcall kotor_fastcall_004eb020(void* info, void*, void* gff, void* st) {
    const U s = word(info);
    U ok = 0;
    wr(s, gui_load_gff_dword_00411c20(gff, nullptr, st, "FollowObject", &ok, rd(s)));    // 004EB031..004EB05C
    for (const U offset : {4u, 0x10u, 0x1cu}) {                                          // 004EB046..004EB0CF
        const char* const label = offset == 4u ? "FollowLocation" : offset == 0x10u ? "LastLeaderPos" : "LastFollowerPos";
        Vector out;
        const Vector* const v = gui_load_gff_vector_004122a0(gff, nullptr, &out, st, label, &ok,
                                                             static_cast<const Vector*>(ptr(s + offset))); // default = current
        std::memcpy(ptr(s + offset), v, 12);
    }
    wrf(s + 0x28, gui_load_gff_float_00411d00(gff, nullptr, st, "MaxSpeed", &ok, bits_float(rd(s + 0x28)))); // 004EB0C3..004EB0E8
    wr(s + 0x2c, gui_load_gff_byte_00411a60(gff, nullptr, st, "StickToPos", &ok, static_cast<unsigned char>(rd(s + 0x2c)))); // 004EB0EB..004EB115
    wr(s + 0x30, gui_load_gff_dword_00411c20(gff, nullptr, st, "Result", &ok, rd(s + 0x30)));         // 004EB104..004EB11D
    wr(s + 0x34, gui_load_gff_dword_00411c20(gff, nullptr, st, "TimeElapsed", &ok, rd(s + 0x34)));    // 004EB120..004EB136
    wr(s + 0x38, gui_load_gff_byte_00411a60(gff, nullptr, st, "InSafetyRange", &ok, static_cast<unsigned char>(rd(s + 0x38)))); // 004EB139..004EB153
}
void __fastcall kotor_fastcall_004fb180(void* creature, void*, void* gff, void* root) {
    const U s = word(creature);
    alignas(4) unsigned char st[0x10] = {};
    if (gui_load_gff_struct_00411a10(gff, nullptr, st, root, "FollowInfo") == 0) return; // 004FB186..004FB1A0
    if (rd(s + 0x4c0) != 0) return;                                                      // 004FB1A2..004FB1AA
    void* const memory = live_operator_new(0x3c);                                        // 004FB1AC..004FB1B3
    void* const info = memory != nullptr ? recovered_follow_info_construct_004f79e0(memory, nullptr) : nullptr; // 004FB1B6..004FB1C3
    wr(s + 0x4c0, word(info));                                                           // 004FB1CD
    kotor_fastcall_004eb020(info, nullptr, gff, st);                                     // 004FB1C5..004FB1D3 (NULL unchecked)
}

// ===================================================================================== 00500350
U __fastcall kotor_fastcall_00500350(void* creature, void*, void* gff, void* root, U from_save) {
    const U s = word(creature);
    U ok = 0;                                                                            // original: the root argument slot
    wr(s + 0x8c, gui_load_gff_dword_00411c20(gff, nullptr, root, "AreaId", &ok, 0));    // 0050035B..00500380
    if (w2x_creature_stats_005afce0(ptr(rd(s + 0xa74)), nullptr, gff, root, ptr(s + 0xa50)) != 0) // 00500371..0050038D
        return 0;                                                                        // 0050038F..00500394
    (void)gui_load_gff_byte_00411a60(gff, nullptr, root, "DetectMode", &ok, 0);          // 00500397..005003A6 (value unused: quirk kept)
    if ((rd8(s + 0xa00) & 2) == 0) {                                                     // 005003AB..005003B2
        const U flags = rd(s + 0x9fc);                                                   // 005003B4
        const U was_off = (~(flags >> 1)) & 1;                                           // 005003BA..005003C0
        wr(s + 0x9fc, flags | 2);                                                        // 005003C3..005003C8
        if (was_off != 0) recovered_set_detect_mode_0050ee30(creature, nullptr, 1);      // 005003C6..005003D4
    }
    const U stealth = gui_load_gff_byte_00411a60(gff, nullptr, root, "StealthMode", &ok, 0); // 005003D9..005003ED
    if ((rd8(s + 0xa00) & 1) == 0) {                                                     // 005003F0..005003F7
        U flags = rd(s + 0x9fc), changed;                                                // 005003FC..00500402
        if (stealth == 1) { changed = (~flags) & stealth; flags |= stealth; }            // 00500404..0050040C
        else { changed = flags & 1; flags &= 0xfffffffeu; }                              // 0050040E..00500411
        wr(s + 0x9fc, flags);                                                            // 00500416
        if (changed != 0) {                                                              // 00500414..0050041C
            if (stealth == 1) {
                if ((flags & 0xc) == 0) recovered_set_stealth_mode_0050ee50(creature, nullptr, stealth, 0); // 00500423..0050042E
                else wr(s + 0x9fc, rd(s + 0x9fc) & 0xfffffffeu);                         // 00500444
            } else if (stealth == 0) {
                recovered_set_stealth_mode_0050ee50(creature, nullptr, 0, 0);            // 00500435..0050043D
            } else {
                wr(s + 0x9fc, rd(s + 0x9fc) & 0xfffffffeu);                              // 00500444
            }
        }
    }
    wr(s + 0x4f8, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, root, "CreatureSize", &ok, 3))); // 0050044B..00500461
    wr(s + 0xec, gui_load_gff_byte_00411a60(gff, nullptr, root, "IsDestroyable", &ok, 1));   // 0050045F..00500488
    wr(s + 0xf0, gui_load_gff_byte_00411a60(gff, nullptr, root, "IsRaiseable", &ok, 1));     // 00500479..005004A8
    wr(s + 0xf4, gui_load_gff_byte_00411a60(gff, nullptr, root, "DeadSelectable", &ok, 1));  // 00500496..005004C5
    U anim_state = gui_load_gff_byte_00411a60(gff, nullptr, root, "AmbientAnimState", &ok, 0); // 005004B6..005004CB
    if (anim_state == 4 || anim_state == 3) {                                            // 005004D0..005004D6
        const U appearance = rd(s + 0xd4);                                               // 005004D8
        if (appearance == 0x279b || appearance == 0x2785 || appearance == 0x28a0) anim_state = 0xe; // 005004DE..005004F6
    }
    wr8(s + 0x4c4, anim_state);                                                          // 0050050D
    const int animation = gui_load_gff_int_00411c90(gff, nullptr, root, "Animation", &ok, 0x2710); // 005004F8..00500513
    vcall<void>(s, 0x7c, static_cast<U>(animation));                                     // 00500518..0050051B
    const U fired = gui_load_gff_byte_00411a60(gff, nullptr, root, "CreatnScrptFird", &ok, 0); // 0050051E..0050052D
    if (ok == 1) wr(s + 0x34c, fired);                                                   // 00500532..00500546
    else if (from_save == 1) wr(s + 0x34c, 1);                                           // 0050054E..00500552
    recovered_creature_scripts_004ebf20(creature, nullptr, gff, root);                   // 00500558..0050055C
    recovered_creature_inventory_004ffda0(creature, nullptr, gff, root, 1, from_save == 0 ? 1u : 0u, 0); // 00500561..00500571
    w2x_known_spells_005aeb30(ptr(rd(s + 0xa74)), nullptr, gff, root);                   // 00500576..0050057E
    const U disguised = gui_load_gff_byte_00411a60(gff, nullptr, root, "PM_IsDisguised", &ok, 0); // 00500583..00500597
    wr(s + 0xa48, disguised);                                                            // 0050059C
    if (disguised != 0)                                                                  // 0050059A..005005A3
        wr16(s + 0xa4c, recovered_gff_word_00411b40(gff, nullptr, root, "PM_Appearance", &ok, 0)); // 005005A5..005005B9
    wr(s + 0x19c, gui_load_gff_byte_00411a60(gff, nullptr, root, "Listening", &ok, 0));  // 005005C0..005005D8
    kotor_fastcall_004d0480(creature, nullptr, gff, root);                               // 005005D7..005005E1
    w2x_template_finish_004f1c40(creature, nullptr);                                     // 005005E6..005005E8
    kotor_fastcall_004fb180(creature, nullptr, gff, root);                               // 005005ED..005005F1
    w2x_object_state_004d1cf0(creature, nullptr, gff, root);                             // 005005F6..005005FA
    return 1;                                                                            // 00500601
}
