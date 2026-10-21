// Lane worker "s2p-cc" (fork "rolls"): HandleServerToPlayerCCMessage 00656D20 minors 7, 8, 9, 10, 17.
// Transcribed from the original swkotor.exe (read-only); see claude_s2p_cc.hpp for the frame/exit model.
//   minor  7 (0065A659) saving throw  <- CSWSCreature::BroadcastSavingThrowData
//   minor  8 (0065AF21) spell cast    <- CSWSObject::BroadcastSpellData
//   minor  9 (0065B4A0) skill check   <- CSWSCreature::BroadcastSkillData
//   minor 10 (0065A213) resist spell  <- CSWVirtualMachineCommands::ExecuteCommandResistSpell
//   minor 17 (0065A422) resist spell (variant) <- ExecuteCommandResistSpell
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// CExoString::Format(dst, fmt, ...) with the original's already-evaluated C-string arguments.
inline void format2(void* dst, const char* fmt, const char* a1, const char* a2) {
    kotor_reconstruction::StartupFormat005e5680(ss(dst), fmt, a1, a2);
}
inline void format1(void* dst, const char* fmt, const char* a1) { kotor_reconstruction::StartupFormat005e5680(ss(dst), fmt, a1); }
inline void format3i(void* dst, const char* fmt, I a1, I a2, I a3) {
    kotor_reconstruction::StartupFormat005e5680(ss(dst), fmt, a1, a2, a3);
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 7 (0065A659..0065AC79): saving throw line, TLK 0x57E; optional " <mod>" parts TLK 0xA593 / 0xA594.
// -----------------------------------------------------------------------------------------------------------------
U case_07(Frame& f) {
    const U8 save_type = rd_byte(f, 8);                              // 0065A659..0065A666 [esp+2B]
    (void)rd_byte(f, 8);                                             // 0065A662..0065A66A (value discarded)
    const U8 roll = rd_byte(f, 5);                                   // 0065A66F..0065A67C [esp+46]
    const U8 bonus_a = rd_byte(f, 8);                                // 0065A678..0065A689 [esp+23]
    const U8 bonus_b = rd_byte(f, 8);                                // 0065A685..0065A698 bl / [esp+47]
    const U8 dc = rd_byte(f, 7);                                     // 0065A694..0065A6A3 [esp+33]
    const U id = rd_obj(f);                                          // 0065A6A1..0065A6B0 edi
    if (overflow(f, 1) != 0) return kFail;                           // 0065A6AC..0065A6B9
    ExoString mods;                                                  // 0065A6BF [esp+18C]
    if (bonus_b != 0) {                                              // 0065A6CB..0065A6D5
        token_int(0, static_cast<I>(static_cast<signed char>(bonus_b)));   // 0065A6DB..0065A717 (movsx)
        Tmp part;                                                    // [esp+B34]
        (void)part.set(fetch(part.p(), 0xa593));                     // 0065A71C..0065A735
        assign_concat(&mods, &mods, part.p());                       // 0065A73A..0065A77B mods = mods + part (EAX)
    }                                                                // 0065A780..0065A78F ~part
    if (bonus_a != 0) {                                              // 0065A794..0065A79A
        token_int(0, static_cast<I>(static_cast<signed char>(bonus_a)));   // 0065A7A0..0065A7DC (movsx)
        Tmp part;                                                    // [esp+B44]
        (void)part.set(fetch(part.p(), 0xa594));                     // 0065A7E1..0065A7FA
        assign_concat(&mods, &mods, part.p());                       // 0065A7FF..0065A840
    }                                                                // 0065A845..0065A854
    ExoString unused_ba4;                                            // 0065A859 [esp+BA4]
    ExoString type_text;                                             // 0065A865 [esp+1BC]
    ExoString unused_11e4;                                           // 0065A879 [esp+11E4]
    ExoString result_text;                                           // 0065A88D [esp+27C]
    ExoString name;                                                  // 0065A8A1 [esp+1F4]
    ExoString unused_b94;                                            // 0065A8B5 [esp+B94]
    object_name_or_gui(id, &name);                                   // 0065A8C9..0065A927
    token(0, &name);                                                 // 0065A92C..0065A93C
    switch (save_type) {                                             // 0065A941..0065A94D movzx; dec chain
    case 1: assign_gui(&type_text, 0x55f); break;                    // 0065A9C5..0065AA02
    case 2: assign_gui(&type_text, 0x55e); break;                    // 0065A98C..0065AA02
    case 3: assign_gui(&type_text, 0x560); break;                    // 0065A953..0065AA02
    default: break;
    }
    token(1, &type_text);                                            // 0065AA07..0065AA17
    const I roll_i = static_cast<I>(roll);                                       // 0065AA1C movzx esi
    const I total = static_cast<I>(static_cast<signed char>(bonus_a)) +
                    (roll_i + static_cast<I>(static_cast<signed char>(bonus_b))); // 0065AA21..0065AA2F edi
    if (total >= static_cast<I>(dc)) assign_gui(&result_text, 0x570);  // 0065AA32..0065AA72 (jl signed)
    else assign_gui(&result_text, 0x571);                            // 0065AA74..0065AAB1
    token(2, &result_text);                                          // 0065AAB6..0065AAC6
    token_int(3, total);                                             // 0065AACB..0065AAFC
    token_int(4, roll_i);                                            // 0065AB01..0065AB32
    token(5, &mods);                                                 // 0065AB37..0065AB47
    token_int(6, static_cast<I>(dc));                                // 0065AB4C..0065AB89 (movzx)
    assign_fetch(&f.a, 0x57e);                                       // 0065AB8E..0065ABCC
    append_a(f);                                                     // 0065ABD1..0065ABED
    return kDone;                                                    // 0065ABF2..0065AC79 -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 8 (0065AF21..0065B49B): spell cast line (spell TLK at CSWSpell+144h, or TLK 0x7E24 "<caster> casts <spell>"),
// optional " %d (%d +/- %d " + simple(0xA416) ... simple(0xA418) ")" breakdown, appended with flag 1.
// Quirk: a read overflow still exits through 00660382 (EAX = 1) without appending.
// -----------------------------------------------------------------------------------------------------------------
U case_08(Frame& f) {
    ExoString unused_374;                                            // 0065AF21 [esp+374]
    ExoString spell_name;                                            // 0065AF2D [esp+204]
    const U caster = rd_obj(f);                                      // 0065AF41..0065AF52 [esp+3C]
    const U target = rd_obj(f);                                      // 0065AF50..0065AF5F [esp+34]
    const U spell_id = rd_dword(f, 0x20);                            // 0065AF5B..0065AF6A edi
    const U has_name = rd_bool(f);                                   // 0065AF68..0065AF78 ebx
    ExoString custom_name;                                           // 0065AF71 [esp+21C]
    if (has_name != 0) {                                             // 0065AF7F..0065AF89
        ResRef sound;                                                // [esp+14A0]
        void* sound_ptr = kotor_fastcall_00405ed0(&sound, nullptr);  // 0065AF8B..0065AF94
        Tmp loc;                                                     // [esp+FE4]
        str_assign(&custom_name, loc.set(read_locstring(f, loc.p(), sound_ptr, nullptr)));  // 0065AF99..0065AFCD
    }
    const U total = rd_dword(f, 0x20);                               // 0065AFD2..0065AFDF [esp+24]
    const U base = rd_dword(f, 0x20);                                // 0065AFDB..0065AFEC [esp+1C]
    (void)rd_dword(f, 0x20);                                         // 0065AFE8..0065AFF0 (value discarded)
    if (overflow(f, 1) == 0) {                                       // 0065AFF5..0065B000 (jne 0065B464)
        void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr,
                                                                static_cast<I>(spell_id));   // 0065B006..0065B013
        bool lines = spell != nullptr;                               // 0065B018..0065B01C
        if (lines) {
            if (rd(spell, 0x144) != 0) {                             // 0065B022..0065B02A
                object_name_or_gui(caster, &f.b);                    // 0065B030..0065B083
                token(0, &f.b);                                      // 0065B088..0065B095
                assign_fetch(&f.a, rd(spell, 0x144));                // 0065B09A..0065B0D3 -> 0065B1EB ~temp
            } else {
                if (has_name == 0) assign_gui(&spell_name, rd(spell, 8));    // 0065B0D8..0065B11E
                else str_assign(&spell_name, &custom_name);          // 0065B120..0065B12F
                object_name_or_gui(caster, &f.b);                    // 0065B134..0065B188
                token(0, &f.b);                                      // 0065B18D..0065B19A
                token(1, &spell_name);                               // 0065B19F..0065B1AF
                assign_fetch(&f.a, 0x7e24);                          // 0065B1B4..0065B1F3
            }
            const I t = static_cast<I>(base);                        // 0065B1F8 edi
            const I b = static_cast<I>(total);                       // 0065B1FC ebx
            I d = t - b;                                             // 0065B200..0065B202 esi
            if (t != 0) {                                            // 0065B204..0065B206
                ExoString numbers;                                   // 0065B20C [esp+20C]
                if (d >= 0) format3i(&numbers, " %d (%d + %d ", t, b, d);     // 0065B218..0065B234 (jl signed)
                else { d = (d ^ (d >> 31)) - (d >> 31); format3i(&numbers, " %d (%d - %d ", t, b, d); }  // 0065B236..0065B24D cdq abs
                ExoString close(")");                                // 0065B255..0065B261 [esp+4F4]
                ExoString space(" ");                                // 0065B266..0065B27A [esp+364]
                Tmp s18;                                             // [esp+1014]
                void* s18p = s18.set(simple_string(s18.p(), 0xa418));   // 0065B27F..0065B29F esi
                Tmp s16;                                             // [esp+C24]
                void* s16p = s16.set(simple_string(s16.p(), 0xa416));   // 0065B2A1..0065B2C1 ebx
                Tmp c1;                                              // [esp+1204]
                void* r1 = c1.set(concat(c1.p(), &f.a, &space));     // 0065B2C3..0065B2DF
                Tmp c2;                                              // [esp+C14]
                void* r2 = c2.set(concat(c2.p(), r1, s16p));         // 0065B2E4..0065B2F7
                Tmp c3;                                              // [esp+1004]
                void* r3 = c3.set(concat(c3.p(), r2, &numbers));     // 0065B2FC..0065B316
                Tmp c4;                                              // [esp+C04]
                void* r4 = c4.set(concat(c4.p(), r3, s18p));         // 0065B31B..0065B32F
                Tmp c5;                                              // [esp+1390]
                void* r5 = c5.set(concat(c5.p(), r4, &close));       // 0065B334..0065B34E
                str_assign(&f.a, r5);                                // 0065B353..0065B360
            }                                                        // 0065B365..0065B427 (reverse destruction)
        }
        (void)object_name(target, &f.b);                             // 0065B42C..0065B43E (result unused)
        append_a(f, 0x80, 1);                                        // 0065B443..0065B45F
    }
    return kDone;                                                    // 0065B464..0065B49B -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 9 (0065B4A0..0065BC54): skill check line TLK 0x580 (tokens 0..8), then the target's floating feedback
// ("%s *%s*" / "*%s*" via CSWCObject::SetFeedbackInfo) for selected skill ids 0x149 / 0x5F9 / 0x5FB.
// -----------------------------------------------------------------------------------------------------------------
U case_09(Frame& f) {
    ExoString sign;                                                  // 0065B4A0 [esp+1CC]
    ExoString unused_664;                                            // 0065B4AC [esp+664]
    ExoString skill_text;                                            // 0065B4C0 [esp+118]
    ExoString extra_text;                                            // 0065B4D4 [esp+24C]
    ExoString result_text;                                           // 0065B4E8 [esp+E0]
    const U actor = rd_obj(f);                                       // 0065B501..0065B511 [esp+24]
    const U target = rd_obj(f);                                      // 0065B50F..0065B51E [esp+4C]
    const U skill = rd_dword(f, 0x20);                               // 0065B51A..0065B52B [esp+38]
    const I roll = static_cast<I>(rd_byte(f, 5));                    // 0065B527..0065B53B [esp+3C] movzx
    I modifier = static_cast<I>(static_cast<signed char>(rd_byte(f, 8)));  // 0065B537..0065B548 edi movsx
    const I dc = static_cast<I>(rd_byte(f, 7));                      // 0065B544..0065B553 [esp+34] movzx
    (void)rd_bool(f);                                                // 0065B557..0065B559 (value discarded)
    const U mode = rd_byte(f, 8);                                    // 0065B55E..0065B56E [esp+48] movzx
    const U extra = rd_dword(f, 0x20);                               // 0065B56A..0065B57B [esp+1C]
    if (overflow(f, 1) != 0) return kFail;                           // 0065B577..0065B5EC (locals destroyed)
    assign_gui(&skill_text, skill);                                  // 0065B5F1..0065B631
    assign_gui(&extra_text, extra);                                  // 0065B636..0065B677
    if (mode != 5) {                                                 // 0065B67C..0065B683
        if (mode <= 4) {                                             // 0065B689..0065B692 table 00662FA0
            if (mode == 1 || mode == 4) assign_gui(&result_text, 0x570);   // 0065B699..0065B715
            else assign_gui(&result_text, 0x571);                    // 0065B6D5..0065B715 (0, 2, 3)
        }
        const I total = modifier + roll;                             // 0065B71A..0065B720 esi
        if (modifier < 0) {                                          // 0065B71A..0065B72A (jge)
            assign_text(&sign, "-");                                 // 0065B72C..0065B731
            modifier = -modifier;                                    // 0065B736
        } else {
            assign_text(&sign, "+");                                 // 0065B73A..0065B73F
        }
        object_name_or_gui(actor, &f.b);                             // 0065B744..0065B79A
        token(0, &f.b);                                              // 0065B79F..0065B7AC
        token(1, &result_text);                                      // 0065B7B1..0065B7C1
        token(2, &skill_text);                                       // 0065B7C6..0065B7D6
        token_int(3, total);                                         // 0065B7DB..0065B80F
        token_int(4, roll);                                          // 0065B814..0065B84C
        token(5, &sign);                                             // 0065B851..0065B861
        token(6, &extra_text);                                       // 0065B866..0065B876
        token_int(7, modifier);                                      // 0065B87B..0065B8AF
        token_int(8, dc);                                            // 0065B8B4..0065B8EC
        assign_fetch(&f.a, 0x580);                                   // 0065B8F1..0065B932
        append_a(f);                                                 // 0065B937..0065B952
    }                                                                // 0065B957 esi = mode (reload)
    void* object = game_object(target);                              // 0065B95B..0065B968
    if (object == nullptr) return kDone;                             // 0065B96D..0065B96F
    void* feedback = vcall<void*>(object, 0xc);                      // 0065B975..0065B97C edi
    if (feedback == nullptr) return kDone;                           // 0065B97E..0065B980
    const bool skill_a = skill == 0x149 || skill == 0x5f9 || skill == 0x5fb;   // sub 149h / 4B0h / 2 chains
    switch (mode) {                                                  // 0065B986..0065B98F table 00662FB4
    case 0:                                                          // 0065B996
    case 1:                                                          // 0065BA1B
        if (mode == 0 ? skill_a : (skill == 0x5f9 || skill == 0x5fb)) {
            const char* r = cstr(&result_text);                      // 0065B9B1..0065B9B8
            const char* s = cstr(&skill_text);                       // 0065B9BE..0065B9C5
            format2(&f.a, "%s *%s*", s, r);                          // 0065B9CB..0065B9DA
            if (length(&f.a) > 0x14) format1(&f.a, "*%s*", cstr(&result_text));  // 0065B9DD..0065BA07
            feedback_set_info_0063d2d0(feedback, nullptr, &f.a);     // 0065BA0A..0065BA11
        }
        break;
    case 3:                                                          // 0065BA99
    case 5:                                                          // 0065BB4A
        if (mode == 3 ? skill_a : skill == 0x149) {
            Tmp text;                                                // [esp+2B4] / [esp+2AC]
            (void)text.set(gui_string(text.p(), mode == 3 ? 0x5ef : 0x55a));   // 0065BAB4..0065BAC9 / 0065BB58..0065BB6D
            const char* r = cstr(text.p());                          // 0065BACE..0065BAE0
            const char* s = cstr(&skill_text);                       // 0065BAE6..0065BAED
            format2(&f.a, "%s *%s*", s, r);                          // 0065BAF3..0065BB02
            if (length(&f.a) > 0x14) format1(&f.a, "*%s*", cstr(text.p()));    // 0065BB05..0065BB2F
            feedback_set_info_0063d2d0(feedback, nullptr, &f.a);     // 0065BB32..0065BB39
        }                                                            // 0065BB3E..0065BBF0 ~text
        break;
    default: break;                                                  // 2, 4, > 5 -> 0065BBF5
    }
    return kDone;                                                    // 0065BBF5..0065BC54 -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 10 (0065A213..0065A41D): resist-spell line. kind 1 -> TLK 0x96C6 alone; otherwise token 1 = GUI 0x571
// (kind 0) or 0x578 and TLK 0x96C5.
// -----------------------------------------------------------------------------------------------------------------
U case_10(Frame& f) {
    ExoString text;                                                  // 0065A213 [esp+1A4]
    const U id = rd_obj(f);                                          // 0065A221..0065A233 edi
    const U kind = rd_byte(f, 8);                                    // 0065A22F..0065A23D [esp+1C] movzx
    if (overflow(f, 1) != 0) return kFail;                           // 0065A241..0065A262
    object_name_or_gui(id, &f.b);                                    // 0065A267..0065A2B6
    token(0, &f.b);                                                  // 0065A2BB..0065A2C8
    if (kind == 1) {                                                 // 0065A311..0065A314
        assign_fetch(&f.a, 0x96c6);                                  // 0065A316..0065A354
    } else {
        assign_gui(&text, kind == 0 ? 0x571 : 0x578);                // 0065A2D5..0065A30C / 0065A38E..0065A3CC
        token(1, &text);                                             // 0065A3D1..0065A3E1
        assign_fetch(&f.a, 0x96c5);                                  // 0065A3E6..0065A41D -> 0065A34D
    }
    append_a(f);                                                     // 0065A359..0065A375
    return kDone;                                                    // 0065A37A..0065A389 -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 17 (0065A422..0065A654): spell-resistance roll line TLK 0xA423 (tokens 0..4).
// -----------------------------------------------------------------------------------------------------------------
U case_17(Frame& f) {
    const U first = rd_obj(f);                                       // 0065A422..0065A42B edi
    const U second = rd_obj(f);                                      // 0065A429..0065A436 [esp+1C]
    const I v1 = static_cast<I>(rd_byte(f, 8));                      // 0065A432..0065A443 ebx movzx
    const I v2 = static_cast<I>(rd_byte(f, 8));                      // 0065A43F..0065A44E [esp+24] movzx
    const I v3 = static_cast<I>(rd_byte(f, 8));                      // 0065A452..0065A462 [esp+34] movzx
    if (overflow(f, 1) != 0) return kFail;                           // 0065A45E..0065A46D
    object_name_or_gui(second, &f.b);                                // 0065A473..0065A4C6
    token(0, &f.b);                                                  // 0065A4CB..0065A4D8
    token_int(1, v1);                                                // 0065A4DD..0065A50F
    token_int(2, v2);                                                // 0065A514..0065A54A
    object_name_or_gui(first, &f.b);                                 // 0065A54F..0065A59E
    token(3, &f.b);                                                  // 0065A5A3..0065A5B0
    token_int(4, v3);                                                // 0065A5B5..0065A5EB
    assign_fetch(&f.a, 0xa423);                                      // 0065A5F0..0065A62F
    append_a(f);                                                     // 0065A634..0065A64F
    return kDone;                                                    // 0065A654 -> 00660387
}
}  // namespace s2pcc
