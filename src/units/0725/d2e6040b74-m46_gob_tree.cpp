// lane-m46-3dcore fork B: Gob vtable 00741268 slot bodies - textures/materials, part-tree edits, attach, lifetime.
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1); instruction addresses cited per behaviour. Conventions as in
// m46_gob_small.cpp. Allocator family: 006FA7E6 -> live_operator_new, 006FA390 -> live_free (M41),
// CExoArrayList::Allocate 005E03C0 -> menu_list_growth::allocate_005e03c0 (M41). Original SEH unwind states
// (FuncInfo 00715E38 / 00715E60 / 00715E7B / 00715E98 / 00715EFB / 00715F18 / 00715C3C / 00716403) only free the
// local list on unwind; no callee on these paths throws (original operator new returns NULL), so no cleanup funclet
// is emitted (D30-FINAL unwind rule).
#include <cstdio>
#include <cstring>
#include "fb4ea0259f-m46_gob_externs.h"

using namespace m46_gob;
using aurora_model::Gob;

// ---------------------------------------------------------------- cells referenced only by the Gob slot bodies
// Image-wide reference search (see m46_gob_externs.h): no other function reads or writes these two cells.
extern "C" {
volatile float gob_sphere_scale_0078e338 = 0x1.59999ap+0f;     // .data 0078E338 = 3FACCCCD (1.35f); read 0043F0AA
std::FILE* volatile aurora_texture_log_007a8a3c = nullptr;      // BSS 007A8A3C; reads 0044673B / 0044691E only
}

namespace {
constexpr char kTxi_0073f09c[] = ".txi";                         // .rdata 0073F09C
constexpr char kBlending_007410fc[] = "blending";                // .rdata 007410FC
constexpr char kAdditive_007410f0[] = "additive";                // .rdata 007410F0 (repe cmpsb, ecx = 9)
constexpr char kPunchthrough_007410e0[] = "punchthrough";        // .rdata 007410E0 (repe cmpsb, ecx = 0Dh)
constexpr char kNull_0073ee04[] = "NULL";                        // .rdata 0073EE04 (dword + byte copy, 004486CC..004486DB)
constexpr char kUpdateScenes_00740ad8[] = "UpdateScenes";        // .rdata 00740AD8
constexpr char kEmitterTarget_007413fc[] = "_EmitterTarget";     // .rdata 007413FC
constexpr char kDying_00741418[] = "Dying";                      // .rdata 00741418
constexpr char kEmpty_0073d71c[] = "";                           // .rdata 0073D71C
constexpr char kFormatString_00740eac[] = "%s\n";                // .rdata 00740EAC
constexpr char kFormatIndex_00740ea8[] = "%i ";                  // .rdata 00740EA8
constexpr char kNewline_0073d5cc[] = "\n";                       // .rdata 0073D5CC

inline U* elements(const PointerList& list) { return ptr<U>(list.data); }
inline void free_list(PointerList& list) { if (list.data != 0) live_free(ptr<void>(list.data)); }   // test/push/006FA390
// Inline CExoArrayList<T*>::Add with the 005E03C0 growth call (e.g. 00449BE5..00449C0A): grow to 2x or 8 when full.
inline void list_add(PointerList* list, U value) {
    if (list->size == list->capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, list->capacity != 0 ? static_cast<U>(list->capacity) * 2u : 8u);
    ptr<U>(list->data)[list->size] = value;
    list->size = list->size + 1;
}
// Inline CExoArrayList<T*>::Remove(value) as compiled at 0044B47E..0044B4B5 / 0044B4D2..0044B510: size is decremented
// unconditionally, the reverse search stops at the match (or -1), the tail is shifted down one slot.
inline void list_remove_reverse(PointerList* list, U value) {
    const I last = list->size - 1;
    I index = last;
    while (index >= 0 && ptr<U>(list->data)[index] != value) --index;
    list->size = last;
    for (; index < list->size; ++index) ptr<U>(list->data)[index] = ptr<U>(list->data)[index + 1];
}
// Inline grow-by-new copy used by the work lists of 00446EE0 / 0044B670 (operator new 006FA7E6 + copy + free).
inline void work_push(PointerList& list, U value) {
    if (list.size == list.capacity) {                                   // 004470CE..004470D4 / 0044B718..0044B722
        const I capacity = list.capacity != 0 ? list.capacity * 2 : 8;  // 004470D6..004470DE
        U* old = ptr<U>(list.data);
        list.capacity = capacity;                                       // 004470ED
        U* fresh = static_cast<U*>(live_operator_new(static_cast<U>(capacity) * 4u));   // 004470E3..004470F1
        for (I i = 0; i < list.size; ++i) fresh[i] = old[i];            // 00447103..00447119
        if (old != nullptr) live_free(old);                             // 0044711F..00447124
        list.data = word(fresh);
    }
    ptr<U>(list.data)[list.size] = value;                               // 00447134
    list.size = list.size + 1;                                          // 0044713A
}
inline const TextureSlots* texture_vt(void* texture) { return table<TextureSlots>(texture); }

// Debug echo of the ReplaceTexture index map (only when the log FILE* 007A8A3C is set; no writer exists in the image).
// fprintf 006FAF01 -> std::fprintf: same format strings, same argument words.
void log_texture_map(const char* name, I count, const U16* indices) {
    std::FILE* log = aurora_texture_log_007a8a3c;
    if (log == nullptr) return;
    std::fprintf(log, kFormatString_00740eac, name);
    for (I i = 0; i < count; ++i) std::fprintf(aurora_texture_log_007a8a3c, kFormatIndex_00740ea8, static_cast<int>(indices[i]));
    std::fprintf(aurora_texture_log_007a8a3c, kNewline_0073d5cc);
}

// TXI "blending" scan shared by both branches of Gob::EnableRenderBumpedOut (0044821E..004482B5 / 00448326..004483B8).
void scan_blending_line(char* line, U& mode_a, U& mode_b, char* word_buffer) {
    while (*line == ' ' || *line == '\t') ++line;                       // 00448226..00448231
    if (_stricmp(kBlending_007410fc, firstword_00463530(line)) == 0)    // 00448233..00448249 (0070ACAF __stricmp)
        parse_cstr_00463590(line + 8, word_buffer);                     // 0044824B..00448254
    if (word_buffer[0] == 0) return;                                    // 0044825C..00448262
    if (std::memcmp(word_buffer, kAdditive_007410f0, 9) == 0) {         // 00448264..00448276
        mode_a = 0; mode_b = 2;                                         // 00448278..0044827C
    } else if (std::memcmp(word_buffer, kPunchthrough_007410e0, 13) == 0) {   // 00448286..00448298
        mode_a = 2; mode_b = 3;                                         // 0044829A..004482A2
    }
}
// Material list at +28 / +34 reset to a fresh 2-slot array (0044849D..004484E2 / 004484F5..0044852D): capacity 2,
// new data[2], copy the old `size` entries, free the old block, size = 2.
void reset_two(PointerList* list) {
    U* old = ptr<U>(list->data);                                        // 00448497
    list->capacity = 2;                                                 // 0044849F
    U* fresh = static_cast<U*>(live_operator_new(8));                   // 004484A6
    list->data = word(fresh);                                           // 004484AE
    for (I i = 0; i < list->size; ++i) fresh[i] = old[i];               // 004484B3..004484CE
    if (old != nullptr) live_free(old);                                 // 004484D0..004484D5
    list->size = 2;                                                     // 004484E2
}
}

extern "C" {
// ---------------------------------------------------------------- textures / materials
// Gob::ReplaceTexture(const char* old_name, const char* new_name) (RET 8).
U __fastcall gob_replace_texture_00446520(Gob* self, void*, U old_name, U new_name) {   // +34
    const char* from = ptr<const char>(old_name);
    PointerList materials{};
    if (from[0] == 0) {                                                 // 0044653A..00446546
        find_all_materials_00445ec0(field<void*>(self, 0x6C), &materials);   // 00446556..00446563
        for (I i = 0; i < materials.size; ++i)                          // 0044656F..00446595
            material_replace_texture_0047b890(ptr<void>(elements(materials)[i]), nullptr, nullptr, ptr<const char>(new_name));   // 00446580..00446589
        free_list(materials);                                           // 00446597..004465A8
        return 0;                                                       // 004465B0
    }
    find_all_materials_00445ec0(field<void*>(self, 0x6C), &materials);  // 004465C5..004465D6
    for (I i = 0; i < materials.size; ++i) {                            // 004465E2..0044661F
        void* material = ptr<void>(elements(materials)[i]);
        material_replace_texture_0047b890(material, nullptr, from, ptr<const char>(new_name));   // 004465F0..004465F9
        if (field<U>(self, 0x1A8) != 0)                                 // 004465FE..00446606
            texture_join::set_environment_00421480(field<font::Texture*>(ptr<void>(elements(materials)[i]), 0), nullptr, 1);   // 00446608..00446613
    }
    field<U8>(self, 0x167) = 1;                                         // 00446629
    free_list(materials);                                               // 00446623..0044663B
    return 1;                                                           // 00446643
}
// Gob::RestoreTexture() (plain ret): every tri-mesh part's material gets its node's original texture name back.
U __fastcall gob_restore_texture_00446660(Gob* self, void*) {          // +3C
    PointerList parts{};                                                // 0044667A..00446684
    find_all_tri_mesh_parts_004461d0(field<void*>(self, 0x6C), &parts); // 00446688..00446695
    for (I i = 0; i < parts.size; ++i) {                                // 004466A1..004466E1
        void* part = ptr<void>(elements(parts)[i]);                     // 004466B0..004466B4
        const U node = field<U>(part, 4);                               // 004466B7
        // 004466BA..004466CA: name = ((node flags & 21h) == 21h ? node : 0) + 0A8h (the original computes the
        // address with no null test; a node without both flags yields the raw word 0A8h).
        const U mask = (field<U8>(ptr<void>(node), 0) & 0x21u) == 0x21u ? 0xFFFFFFFFu : 0u;
        const U name = (node & mask) + 0xA8u;
        material_replace_texture_0047b890(field<void*>(part, 0x44), nullptr, nullptr, ptr<const char>(name));   // 004466D0..004466D5
    }
    free_list(parts);                                                   // 004466E3..004466F6
    return 1;                                                           // 00446702
}
// Gob::ReplaceTexture(const char* old_name, const char* new_name, int count, const U16* indices, int unused) (RET 14h).
U __fastcall gob_replace_texture_00446720(Gob* self, void*, U old_name, U new_name, U count, U indices, U) {   // +30
    const I n = static_cast<I>(count);
    const U16* map = ptr<const U16>(indices);
    log_texture_map(ptr<const char>(new_name), n, map);                 // 0044673B..004467AF
    U widened[256];                                                     // [esp+20h], 400h bytes (sub esp,410h)
    for (I i = 0; i < n; ++i) widened[i] = map[i];                      // 004467B2..004467CC (unbounded as in the original)
    const char* from = ptr<const char>(old_name);
    const char* selected = (from != nullptr && from[0] != 0) ? from : nullptr;   // 004467CE..004467E2
    PointerList parts{};
    find_all_tri_mesh_parts_004461d0(field<void*>(self, 0x6C), &parts); // 004467E4..00446804
    for (I i = 0; i < parts.size; ++i) {                                // 00446810..004468BF
        void* part = ptr<void>(elements(parts)[i]);                     // 00446820..00446824
        void* node = field<void*>(part, 4);                             // 00446827
        const U owner = field<U>(node, 8);                              // 0044682A
        const U8 type = field<U8>(ptr<void>(owner), 0x4C) & 0x7Fu;      // 0044682D..00446830
        const U is_mesh = type == 2 ? 0xFFFFFFFFu : 0u;                 // 00446833..0044683A
        U value;
        if ((owner & is_mesh) != 0) {                                   // 0044683C..00446841
            value = ptr<U>(field<U>(ptr<void>(owner & is_mesh), 0xB8))[field<U>(node, 4)];   // 00446851..0044685A
        } else {
            // 0044685F..0044687C: (type == 5 ? owner : 0) + 84h dereferenced without a null test (original).
            const U is_skin = (field<U8>(ptr<void>(owner), 0x4C) & 0x7Fu) == 5 ? 0xFFFFFFFFu : 0u;
            const U holder = field<U>(ptr<void>(owner & is_skin), 0x84);
            value = ptr<U>(field<U>(ptr<void>(holder), 0xB8))[field<U>(node, 4)];
        }
        // 0044687F..00446892: material->ReplaceTexture(selected, new_name, value, widened, count).
        material_replace_texture_0047b7e0(field<void*>(part, 0x44), nullptr, selected, ptr<const char>(new_name),
                                          value, widened, n);
        if (field<U>(self, 0x1A8) != 0)                                 // 00446897..004468A3
            texture_join::set_environment_00421480(field<font::Texture*>(field<void*>(ptr<void>(elements(parts)[i]), 0x44), 0),
                                                   nullptr, 1);         // 004468A5..004468B3
    }
    field<U8>(self, 0x167) = 1;                                         // 004468D4
    free_list(parts);                                                   // 004468C7..004468EA
    return 1;                                                           // 004468F9
}
// Gob::ReplaceTextureSubtree(const char* part_name, const char* new_name, int count, const U16* indices) (RET 10h).
U __fastcall gob_replace_texture_subtree_00446910(Gob* self, void*, U part_name, U new_name, U count, U indices) {   // +38
    const I n = static_cast<I>(count);
    const U16* map = ptr<const U16>(indices);
    log_texture_map(ptr<const char>(new_name), n, map);                 // 0044691E..00446992
    U widened[256];                                                     // [esp+1Ch], 400h bytes (sub esp,40Ch)
    for (I i = 0; i < n; ++i) widened[i] = map[i];                      // 00446997..004469AB
    PointerList materials{};                                            // 004469AD..004469B5
    aurora_model::Part* subtree = find_part_0043fe00(field<aurora_model::Part*>(self, 0x6C), ptr<const char>(part_name));   // 004469B9..004469CC
    if (subtree == nullptr) {                                           // 004469D4..004469D6
        free_list(materials);                                           // 004469D8..004469EC
        return 0;                                                       // 004469F4
    }
    find_all_materials_00445ec0(subtree, &materials);                   // 004469FB..00446A01
    for (I i = 0; i < materials.size; ++i) {                            // 00446A0D..00446A5E
        // 00446A20..00446A38: material->ReplaceTexture(0, new_name, part_name, widened, count).
        material_replace_texture_0047b7e0(ptr<void>(elements(materials)[i]), nullptr, nullptr, ptr<const char>(new_name),
                                          part_name, widened, n);
        if (field<U>(self, 0x1A8) != 0)                                 // 00446A3D..00446A45
            texture_join::set_environment_00421480(field<font::Texture*>(ptr<void>(elements(materials)[i]), 0), nullptr, 1);   // 00446A47..00446A52
    }
    field<U8>(self, 0x167) = 1;                                         // 00446A66
    free_list(materials);                                               // 00446A60..00446A7B
    return 1;                                                           // 00446A83
}
// Gob::SetTextureAttributes(name, attributes) (RET 8).
U __fastcall gob_set_texture_attributes_00446e10(Gob* self, void*, U name, U attributes) {   // +44
    PointerList textures{};                                             // 00446E29..00446E33
    get_all_named_textures_00446ab0(field<void*>(self, 0x6C), &textures, name);   // 00446E37..00446E49
    if (textures.size <= 0) {                                           // 00446E55..00446E57
        free_list(textures);                                            // 00446EB1..00446EC2
        return 0;                                                       // 00446ECE
    }
    for (I i = 0; i < textures.size; ++i)                               // 00446E5A..00446E7D
        aur_texture_set_attributes_00422ce0(ptr<void>(elements(textures)[i]), attributes);   // 00446E65..00446E6E
    free_list(textures);                                                // 00446E80..00446E92
    return 1;                                                           // 00446E9A
}
// Gob::SetEnvironmentMap(const char* name) (RET 4).
U __fastcall gob_set_environment_map_00448690(Gob* self, void*, U name) {   // +48
    char buffer[64];                                                    // [esp+14h] (sub esp,4Ch frame)
    const char* source = ptr<const char>(name);
    if (source != nullptr && source[0] != 0) std::strcpy(buffer, source);   // 004486AC..004486CA (inline copy, unbounded)
    else std::memcpy(buffer, kNull_0073ee04, 5);                        // 004486CC..004486DB
    void* texture = aur_texture_get_reference_00423a60(buffer, 0);      // 004486DF..004486E6
    if (texture_vt(texture)->is_invalid(texture, nullptr) != 0) {      // 004486ED..004486FA (+10, AL)
        texture_release_00420d70(texture);                              // 004486FC..00448702
        return 0;                                                       // 00448706
    }
    void* old = field<void*>(self, 0x1A8);                              // 0044871C
    safe_pointer_assign_00447fa0(kotor_scene3d::at(self, 0x1A8), nullptr, texture);   // 00448722..0044872B
    texture_release_00420d70(old);                                      // 00448730..00448731
    if (field<U>(self, 0x1A8) != 0) {                                   // 00448736..0044873F
        PointerList materials{};
        find_all_materials_00445ec0(field<void*>(self, 0x6C), &materials);   // 0044874D..0044875A
        for (I i = 0; i < materials.size; ++i)                          // 00448766..00448787
            texture_join::set_environment_00421480(field<font::Texture*>(ptr<void>(elements(materials)[i]), 0), nullptr, 1);   // 00448770..0044877B
        free_list(materials);                                           // 00448789..0044879A
    }
    return 1;                                                           // 004487A9
}
// Gob::EnableRenderBumpedOut(const char* texture_name, U value) (RET 8).
U __fastcall gob_enable_render_bumped_out_00448130(Gob* self, void*, U texture_name, U value) {   // +150
    static_assert(sizeof(char) == 1, "byte buffers");
    field<U8>(self, 0x15) = 1;                                          // 0044815E
    PointerList parts{};                                                // 00448161..00448169
    // 0044816D..00448180: this->EnumParts(PartHasMaterial 0043EE40, &parts) (vtbl+110, raw words).
    (void)gob_vt(self)->s110_gob_enum_parts(self, nullptr,
        &part_has_material_0043ee40, word(&parts));
    const char* name = ptr<const char>(texture_name);                   // 00448186
    void* texture = aur_texture_get_reference_00423a60(name, 0);        // 0044818D..00448198
    U mode_a = 0;                                                       // [esp+1Ch] = 0 (004481AA)
    U mode_b = 1;                                                       // [esp+14h] = 1 (004481AE)
    char word_buffer[32];                                               // [esp+30h]
    word_buffer[0] = 0;                                                 // 00448193
    if (texture_vt(texture)->is_txi_loaded(texture, nullptr) == 0) {   // 004481B2..004481B7 (+34)
        volatile aurora_model::I size = 0;                              // [esp+18h] (0044818F)
        void* resource = aurres_get_0044c740(name, kTxi_0073f09c, &size, 1);   // 004481BD..004481C9
        if (resource == nullptr) {                                      // 004481D3
            (void)texture_vt(texture)->set_txi_loaded(texture, nullptr, 1);   // 004481D7..004481DC (+A4)
            free_list(parts);                                           // 0044857A..0044858E (Gob+11C not written)
            return 0;                                                   // EAX incidental
        }
        I length = size;                                                // 004481E7
        if (length < 0) length = -1 - length;                           // 004481EB..004481F4 (or eax,-1; sub eax,ecx)
        if (length > 0) {                                               // 004481F8..004481FA
            void* bytes = aurres_data_bytes_0044bd10(0, resource);      // 00448203..00448206
            (void)texture_vt(texture)->set_txi_data(texture, nullptr, bytes, length);   // 0044820E..00448211 (+A0)
            for (char* line = resource_next_0044bfa0(); line != nullptr; line = resource_next_0044bfa0())   // 00448217..004482B7
                scan_blending_line(line, mode_a, mode_b, word_buffer);
        }
        (void)aurres_release_0044c8f0(resource, size);                  // 004482BD..004482C3
        (void)texture_vt(texture)->set_txi_loaded(texture, nullptr, 1); // 004482C8..004482D0 (+A4)
    } else {
        const char* data = static_cast<const char*>(texture_vt(texture)->get_txi_data(texture, nullptr));   // 004482DB..004482DF (+94)
        I remaining = static_cast<I>(texture_vt(texture)->get_txi_size(texture, nullptr));   // 004482E5..004482EB (+90)
        if (data != nullptr && remaining > 0) {                         // 004482F1..004482FD
            char line[0x2000];                                          // [esp+50h] (__chkstk 2040h frame)
            do {
                I copied = 0;                                           // 00448303
                do {                                                    // 00448305..00448317
                    line[copied] = *data;
                    ++copied; ++data;
                } while (copied < remaining && line[copied - 1] != '\n');
                remaining -= copied;                                    // 00448319
                if (remaining < 0) break;                               // 0044831B js
                line[copied] = 0;                                       // 00448321
                scan_blending_line(line, mode_a, mode_b, word_buffer);  // 00448326..004483B4
            } while (remaining > 0);                                    // 004483B8..004483BA
        }
    }
    for (I i = 0; i < parts.size; ++i) {                                // 004483C0..00448563
        void* material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 004483D1..004483D8
        void* held = field<void*>(material, 0x0C);                      // 004483DB
        void* slot = kotor_scene3d::at(material, 0x0C);                 // 004483E0
        if (held != nullptr) {                                          // 004483DE..004483E3
            exo_array_list_remove_00445080(static_cast<PointerList*>(kotor_scene3d::at(held, 4)), nullptr, slot);   // 004483EB..004483EF
            field<U>(slot, 0) = 0;                                      // 004483F5
            texture_release_00420d70(held);                             // 004483F4..004483FB
        }
        material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 00448403..0044840A
        slot = kotor_scene3d::at(material, 0x0C);                       // 00448410
        void* current = field<void*>(material, 0x0C);                   // 0044840D
        if (current != nullptr)                                         // 00448413..0044844F (inline Remove(slot))
            list_remove_reverse(static_cast<PointerList*>(kotor_scene3d::at(current, 4)), word(slot));
        field<void*>(slot, 0) = texture;                                // 00448457..0044845B
        list_add(static_cast<PointerList*>(kotor_scene3d::at(texture, 4)), word(slot));   // 004483CE / 0044845D..00448482
        material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 00448485..00448494
        reset_two(static_cast<PointerList*>(kotor_scene3d::at(material, 0x28)));   // 00448497..004484E2
        material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 004484E5..004484EC
        reset_two(static_cast<PointerList*>(kotor_scene3d::at(material, 0x34)));   // 004484EF..0044852D
        material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 00448534..0044853B
        ptr<U>(field<U>(material, 0x28))[1] = mode_a;                   // 0044853E..00448545
        ptr<U>(field<U>(material, 0x34))[1] = mode_b;                   // 00448548..00448559
    }
    field<U>(self, 0x11C) = value;                                      // 00448569..00448574
    free_list(parts);                                                   // 0044857A..0044858E
    return 0;                                                           // EAX incidental (void)
}
// Gob::DisableRenderBumpedOut() (plain ret).
U __fastcall gob_disable_render_bumped_out_004485c0(Gob* self, void*) {   // +154
    if (field<U8>(self, 0x15) == 0) return 0;                           // 004485DC..004485E7 (EAX incidental)
    field<U8>(self, 0x15) = 0;                                          // 004485EE
    PointerList parts{};                                                // 004485F1..004485F9
    (void)gob_vt(self)->s110_gob_enum_parts(self, nullptr,
        &part_has_material_0043ee40, word(&parts));   // 004485FD..0044860F
    for (I i = 0; i < parts.size; ++i) {                                // 00448615..00448651
        void* material = field<void*>(ptr<void>(elements(parts)[i]), 0x44);   // 00448620..00448627
        void* held = field<void*>(material, 0x0C);                      // 0044862A
        void* slot = kotor_scene3d::at(material, 0x0C);                 // 0044862D
        if (held != nullptr)                                            // 00448630..00448634
            exo_array_list_remove_00445080(static_cast<PointerList*>(kotor_scene3d::at(held, 4)), nullptr, slot);   // 00448636..0044863A
        field<U>(slot, 0) = 0;                                          // 00448640
        texture_release_00420d70(held);                                 // 0044863F..00448642 (also for null)
    }
    field<U>(self, 0x11C) = 0;                                          // 0044865E
    free_list(parts);                                                   // 00448658..00448670
    return 0;                                                           // EAX incidental (void)
}

// ---------------------------------------------------------------- scene graph / part tree
// Gob::AttachToPart(Gob* object, Part* part, int mode) (RET 0Ch) -> behaviour.
U __fastcall gob_attach_to_part_004439a0(Gob* self, void*, U object, U part, U mode) {   // +54
    void* p = ptr<void>(part);
    // 004439BB..004439CE: part->GetWorld(&Gob+78, &Gob+84) (Part vtbl+0C).
    (void)table<PartTable>(p)->s00c(p, nullptr, word(kotor_scene3d::at(self, 0x78)), word(kotor_scene3d::at(self, 0x84)));
    void* behavior;
    switch (mode) {                                                     // 004439D1..004439E2 jump table 00443B1C
    case 0: {                                                           // 004439E9 CAurBehaviorAttach (3Ch)
        void* memory = live_operator_new(0x3C);
        behavior = memory != nullptr ? behavior_attach_0049fb10(memory, nullptr, self, ptr<void>(object), p) : nullptr;
        break;
    }
    case 1: {                                                           // 00443A20 ...AttachTranslate (4Ch)
        void* memory = live_operator_new(0x4C);
        behavior = memory != nullptr ? behavior_attach_translate_0049fdc0(memory, nullptr, self, ptr<void>(object), p) : nullptr;
        break;
    }
    case 2: {                                                           // 00443A50 ...AttachRotate (48h)
        void* memory = live_operator_new(0x48);
        behavior = memory != nullptr ? behavior_attach_rotate_0049fe00(memory, nullptr, self, ptr<void>(object), p) : nullptr;
        break;
    }
    case 3: {                                                           // 00443A80 ...AttachNoOrientation (3Ch)
        void* memory = live_operator_new(0x3C);
        behavior = memory != nullptr ? behavior_attach_no_orientation_0049fe40(memory, nullptr, self, ptr<void>(object), p) : nullptr;
        break;
    }
    default:                                                            // 004439DC ja 00443ABE: ebx = [esp+24h] (the part word)
        behavior = p;
        break;
    }
    (void)gob_vt(self)->s058_gob_set_behavior(self, nullptr, behavior); // 00443AC2..00443AC7 (vtbl+58)
    void* scene = field<void*>(ptr<void>(object), 0x68);                // 00443ACA
    void* current = field<void*>(self, 0x68);                           // 00443ACD
    if (current != scene) {                                             // 00443AD0..00443AD2
        if (current != nullptr) (void)scene_vt(current)->s0b4_scene_child_remove(current, nullptr, self);   // 00443AD4..00443ADB
        field<void*>(self, 0x68) = scene;                               // 00443AE3
        if (scene != nullptr) (void)scene_vt(scene)->s0b0_scene_child_add(scene, nullptr, self);   // 00443AE1..00443AED
        (void)gob_vt(self)->do_event(self, nullptr, kUpdateScenes_00740ad8, 0);   // 00443AF3..00443AFE
    }
    return word(behavior);                                              // 00443B0B mov eax,ebx
}
// Gob::ReattachReference(Gob* object, const char* part) (RET 8): breadth-first walk re-attaching reference gobs.
U __fastcall gob_reattach_reference_00446ee0(Gob* self, void*, U object, U part_name) {   // +A4
    PointerList work{};                                                 // 00446EFA..00446F0C
    menu_list_growth::allocate_005e03c0(&work, nullptr, 8);             // 00446F13..00446F1D
    ptr<U>(work.data)[work.size] = field<U>(self, 0x6C);                // 00446F10 / 00446F2A
    work.size = work.size + 1;                                          // 00446F2D..00446F2E
    while (work.size != 0) {                                            // 0044714C..0044714E
        void* part = ptr<void>(ptr<U>(work.data)[0]);                   // 00446F40..00446F44
        void* reference = ptr<void>(table<PartTable>(part)->s038(part, nullptr));   // 00446F42..00446F48 (+38)
        if (reference != nullptr) {
            const U node = field<U>(reference, 4);                      // 00446F55
            if ((field<U8>(ptr<void>(node), 0) & 0x11u) == 0x11u &&    // 00446F58..00446F60
                field<U>(ptr<void>(node), 0x70) != 0 &&                 // 00446F66..00446F6B
                field<U>(reference, 0x4C) != 0) {                       // 00446F71..00446F76
                void* target = field<void*>(reference, 0x4C);           // the referenced Gob
                if (object != 0) {                                      // 00446F7C..00446F87
                    (void)gob_vt(target)->s050_gob_attach_to_object(target, nullptr, ptr<void>(object),
                                                                     ptr<const char>(part_name), 0);   // 00446F89..00446F9B
                } else {
                    const char* name = ptr<const char>(table<PartTable>(reference)->s004(reference, nullptr));   // 00446FA0..00446FA6 Part::Name
                    target = field<void*>(reference, 0x4C);             // 00446FA9
                    (void)gob_vt(target)->s050_gob_attach_to_object(target, nullptr, self, name, 0);   // 00446FAC..00446FB2
                }
                target = field<void*>(reference, 0x4C);                 // 00446FB9
                U position_bits[3];
                const U at_word = gob_vt(target)->s064_gob_get_position(as_gob(target), nullptr, word(position_bits));   // 00446FBC..00446FC5
                const float* p = ptr<const float>(at_word);
                Vector to;                                              // second Vector argument (pushed first)
                to.x = S(W(p[0]) - W(field<float>(self, 0x78)));        // 00446FE1..00446FEB
                to.y = S(W(p[1]) - W(field<float>(self, 0x7C)));        // 00446FD7..00446FF5
                to.z = S(W(p[2]) - W(field<float>(self, 0x80)));        // 00446FC8..00447000
                const Quaternion& q = field<Quaternion>(self, 0x84);    // lea esi,[ebp+84h]
                Vector from;                                            // the gob's z axis from its orientation
                from.z = S(W(1.0f) - (W(q.x) * W(q.x) + W(q.y) * W(q.y)) * 2.0);   // 00447008..00447027 (fsubr [0073D764])
                from.y = S(((W(q.z) * W(q.y)) - (W(q.w) * W(q.x))) * 2.0);           // 00447029..00447036
                from.x = S(((W(q.w) * W(q.y)) + (W(q.z) * W(q.x))) * 2.0);           // 00447038..00447045
                Quaternion arc, combined;
                const Quaternion* r = rotation_arc_004ab630(&arc, from, to);   // 00447068..0044706C
                const Quaternion* m = quaternion_multiply_004a9ac0(&combined, r, &field<Quaternion>(self, 0x84));   // 00447074..0044707A
                field<Quaternion>(self, 0x84) = *m;                     // 0044707F..00447095 (four DWORD copies)
            }
        }
        // 00447098..004470AD: pop the front element.
        work.size = work.size - 1;
        for (I i = 0; i < work.size; ++i) ptr<U>(work.data)[i] = ptr<U>(work.data)[i + 1];
        // 004470AF..00447146: append the processed part's children (+30 data, +34 count).
        for (I c = 0; c < field<I>(part, 0x34); ++c) work_push(work, ptr<U>(field<U>(part, 0x30))[c]);
    }
    if (work.data != 0) live_free(ptr<void>(work.data));                // 00447154..00447161
    return 0;                                                           // EAX incidental (void)
}
// Gob::Append(const char* model_name, const char* part_name) (RET 8).
U __fastcall gob_append_00449b10(Gob* self, void*, U model_name, U part_name) {   // +AC
    aurora_model::Part* part = find_part_0043fe00(field<aurora_model::Part*>(self, 0x6C), ptr<const char>(part_name));   // 00449B13..00449B1F
    if (part == nullptr) return 0;                                      // 00449B27..00449B33 (EAX = 0)
    aurora_model::MaxTree* model = aurora_find_model_00464110(ptr<const char>(model_name));   // 00449B36..00449B3C
    if (model == nullptr) return 0;                                     // 00449B46..00449B4F (EAX = 0)
    field<I>(model, 0x54) = field<I>(model, 0x54) + 1;                  // 00449B52..00449B5D (reference count)
    aurora_model::Part* appended = aurora_create_instance_tree_00449200(field<aurora_model::MdlNode*>(model, 0x28), self);   // 00449B55..00449B60
    (void)table<PartTable>(appended)->s06c(appended, nullptr, word(appended));   // 00449B67..00449B6F Part::Animate(self)
    field<U>(appended, 0x08) = 0; field<U>(appended, 0x0C) = 0; field<U>(appended, 0x10) = 0;   // 00449B72..00449B9E
    field<U>(appended, 0x14) = 0x3F800000u; field<U>(appended, 0x18) = 0;   // 00449BA1..00449BE2
    field<U>(appended, 0x1C) = 0; field<U>(appended, 0x20) = 0;
    list_add(static_cast<PointerList*>(kotor_scene3d::at(self, 0xEC)), word(model));      // 00449BDC..00449C0A
    list_add(static_cast<PointerList*>(kotor_scene3d::at(self, 0xF8)), word(appended));   // 00449C0D..00449C44
    list_add(static_cast<PointerList*>(kotor_scene3d::at(part, 0x30)), word(appended));   // 00449C47..00449C7B
    field<aurora_model::Part*>(appended, 0x3C) = part;                  // 00449C7F (parent)
    aurora_part_init_material_00442d70(self, appended, kEmpty_0073d71c);   // 00449C74..00449C82
    field<U8>(self, 0x167) = 1;                                         // 00449C89
    aurora_create_reference_objects_004448c0(self, appended);           // 00449C87..00449C90
    (void)gob_vt(self)->do_event(self, nullptr, kEmitterTarget_007413fc, 0);   // 00449C95..00449CA3
    return 1;                                                           // 00449CAC
}
// Gob::Amputate(const char* part_name) (RET 4): removes the named sub-tree and the models appended under it.
U __fastcall gob_amputate_0044b670(Gob* self, void*, U part_name) {    // +B0
    aurora_model::Part* part = find_part_0043fe00(field<aurora_model::Part*>(self, 0x6C), ptr<const char>(part_name));   // 0044B688..0044B697
    if (part == nullptr) return 0;                                      // 0044B6A3..0044B6BD
    PointerList work{};                                                 // 0044B6C2..0044B6CA
    menu_list_growth::allocate_005e03c0(&work, nullptr, 8);             // 0044B6CE..0044B6D8
    ptr<U>(work.data)[work.size] = word(part);                          // 0044B6DD..0044B6E5
    work.size = work.size + 1;                                          // 0044B6E9
    while (work.size != 0) {                                            // 0044B86A..0044B86C
        void* current = ptr<void>(ptr<U>(work.data)[work.size - 1]);    // 0044B6F8 (pop the back)
        work.size = work.size - 1;                                      // 0044B6FF
        for (I c = 0; c < field<I>(current, 0x34); ++c)                 // 0044B6FC..0044B78A (children +30/+34)
            work_push(work, ptr<U>(field<U>(current, 0x30))[c]);
        PointerList* appended = static_cast<PointerList*>(kotor_scene3d::at(self, 0xF8));   // Gob+F8 / +FC
        I matches = 0;
        for (I k = 0; k < appended->size; ++k) if (ptr<U>(appended->data)[k] == word(current)) ++matches;   // 0044B790..0044B7AF
        if (matches == 0) continue;                                     // 0044B7B1..0044B7B3
        I index = -1;                                                   // 0044B7B9..0044B7DC (first match or -1)
        for (I k = 0; k < appended->size; ++k) if (ptr<U>(appended->data)[k] == word(current)) { index = k; break; }
        void* model = ptr<void>(ptr<U>(field<U>(self, 0xEC))[index]);   // 0044B7DF..0044B7E5
        if (aurora_model_no_delete_007a8a04 == 0) {                     // 0044B7E8..0044B7F0
            field<I>(model, 0x54) = field<I>(model, 0x54) - 1;          // 0044B7F2 dec [eax+54h]
            if (field<I>(model, 0x54) == 0) aurora_maxtree_delete_0044aec0(static_cast<aurora_model::MaxTree*>(model));   // 0044B7F5..0044B7F8
        }
        appended->size = appended->size - 1;                            // 0044B800..0044B80D
        for (I k = index; k < appended->size; ++k) ptr<U>(appended->data)[k] = ptr<U>(appended->data)[k + 1];   // 0044B813..0044B837
        PointerList* models = static_cast<PointerList*>(kotor_scene3d::at(self, 0xEC));
        models->size = models->size - 1;                                // 0044B839..0044B846
        for (I k = index; k < models->size; ++k) ptr<U>(models->data)[k] = ptr<U>(models->data)[k + 1];   // 0044B84C..0044B868
    }
    // 0044B876..0044B8B4: remove the part from its parent's child list (+3C parent, +30/+34 list), inline Remove.
    list_remove_reverse(static_cast<PointerList*>(kotor_scene3d::at(field<void*>(part, 0x3C), 0x30)), word(part));
    field<U>(part, 0x3C) = 0;                                           // 0044B8B6
    (void)table<PartTable>(part)->s000(part, nullptr, 1);              // 0044B8BD..0044B8C3 deleting destructor
    field<U8>(self, 0x167) = 1;                                         // 0044B8CB
    if (work.data != 0) live_free(ptr<void>(work.data));                // 0044B8C5..0044B8DD
    return 1;                                                           // 0044B8EC
}

// ---------------------------------------------------------------- lifetime
// Gob::~Gob body (plain ret).
void __fastcall gob_destruct_0044b2f0(Gob* self, void*) {
    field<const aurora_model::GobVTable*>(self, 0) = &aurora_gob_vtable_00741268;   // 0044B310 mov [esi],741268h
    aurora_gob_count_007a8a18 = aurora_gob_count_007a8a18 - 1;          // 0044B316..0044B32D
    (void)gob_do_event_00486560(self, nullptr, kDying_00741418, 0);     // 0044B31E..0044B333 (direct call)
    (void)gob_play_animation_00485bd0(self, nullptr, 0, 0x3F800000u, 0, 0);   // 0044B338..0044B342 (direct call)
    void* scene = field<void*>(self, 0x68);                             // 0044B347
    if (scene != nullptr) {                                             // 0044B34A
        (void)scene_vt(scene)->s0b4_scene_child_remove(scene, nullptr, self);   // 0044B34E..0044B351
        field<void*>(self, 0x68) = nullptr;                             // 0044B361
        (void)gob_vt(self)->do_event(self, nullptr, kUpdateScenes_00740ad8, 0);   // 0044B357..0044B364
    }
    void* behavior = field<void*>(self, 0x188);                         // 0044B36A
    field<void*>(self, 0x188) = nullptr;                                // 0044B372
    if (behavior != nullptr) (void)table<BehaviorTable>(behavior)->scalar_delete(behavior, nullptr, 1);   // 0044B37A..0044B37E
    void* root = field<void*>(self, 0x6C);                              // 0044B380
    if (root != nullptr) {
        (void)table<PartTable>(root)->s000(root, nullptr, 1);          // 0044B387..0044B38B
        field<void*>(self, 0x6C) = nullptr;                             // 0044B38D
    }
    for (I i = 0; i < field<I>(self, 0xF0); ++i) {                      // 0044B390..0044B3C8 (count re-read)
        void* model = ptr<void>(ptr<U>(field<U>(self, 0xEC))[i]);       // 0044B3A6..0044B3AC
        if (aurora_model_no_delete_007a8a04 != 0) continue;             // 0044B3A0 / 0044B3AF
        field<I>(model, 0x54) = field<I>(model, 0x54) - 1;              // 0044B3B1
        if (field<I>(model, 0x54) == 0) aurora_maxtree_delete_0044aec0(static_cast<aurora_model::MaxTree*>(model));   // 0044B3B4..0044B3B7
    }
    field<I>(self, 0xF0) = 0;                                           // 0044B3CF
    field<I>(self, 0xFC) = 0;                                           // 0044B3D5
    static constexpr U kModelSlots[3] = {0x58u, 0x5Cu, 0x60u};
    for (U offset : kModelSlots) {                            // 0044B3CA..0044B42A (same sequence three times)
        void* model = field<void*>(self, offset);
        if (model == nullptr || aurora_model_no_delete_007a8a04 != 0) continue;
        field<I>(model, 0x54) = field<I>(model, 0x54) - 1;
        if (field<I>(model, 0x54) == 0) aurora_maxtree_delete_0044aec0(static_cast<aurora_model::MaxTree*>(model));
    }
    for (I i = 0; i < field<I>(self, 0x194); ++i) {                     // 0044B42D..0044B464
        live_free(ptr<void>(ptr<U>(field<U>(self, 0x190))[i]));         // 0044B440..0044B44A
        ptr<U>(field<U>(self, 0x190))[i] = 0;                           // 0044B44F..0044B458
    }
    field<I>(self, 0x194) = 0;                                          // 0044B466
    void* environment = field<void*>(self, 0x1A8);                      // 0044B466 ebp
    void* slot = kotor_scene3d::at(self, 0x1A8);                        // 0044B472 lea ebx,[esi+1A8h]
    if (environment != nullptr)                                         // 0044B47A..0044B4B5 inline Remove(slot)
        list_remove_reverse(static_cast<PointerList*>(kotor_scene3d::at(environment, 4)), word(slot));
    field<U>(slot, 0) = 0;                                              // 0044B4B8
    texture_release_00420d70(environment);                              // 0044B4B7..0044B4BE
    list_remove_reverse(&aurora_gobs_007a8a58, word(self));             // 0044B4C3..0044B510 (Gob list 007A8A58)
    scene_remove_from_scene_manager_00455540(self);                     // 0044B512..0044B513
    static constexpr U kOwnedSlots[2] = {0x174u, 0x178u};
    for (U offset : kOwnedSlots) {                                 // 0044B518..0044B541
        void* owned = field<void*>(self, offset);
        if (owned == nullptr) continue;
        (void)table<DeletingSlot>(owned)->scalar_delete(owned, nullptr, 1);
        field<void*>(self, offset) = nullptr;
    }
    if (field<U>(self, 0x1BC) != 0) live_free(field<void*>(self, 0x1BC));   // 0044B547..0044B557 (EH state 8)
    void* current = field<void*>(slot, 0);                              // 0044B55F (reloaded; zero here)
    if (current != nullptr)                                             // 0044B561..0044B5A2 inline Remove(slot)
        list_remove_reverse(static_cast<PointerList*>(kotor_scene3d::at(current, 4)), word(slot));
    if (field<U>(self, 0x19C) != 0) live_free(field<void*>(self, 0x19C));   // 0044B5A4..0044B5B4
    if (field<U>(self, 0x190) != 0) live_free(field<void*>(self, 0x190));   // 0044B5BC..0044B5CC
    field<const aurora_vt::VTable_00740a94*>(self, 0x110) = &aurora_vtable_path_info_00740a94;   // 0044B5D4
    if (field<U>(self, 0xF8) != 0) live_free(field<void*>(self, 0xF8)); // 0044B5DE..0044B5EE
    if (field<U>(self, 0xEC) != 0) live_free(field<void*>(self, 0xEC)); // 0044B5F6..0044B606
    if (field<U>(self, 0xB0) != 0) live_free(field<void*>(self, 0xB0)); // 0044B60E..0044B61E
    if (field<U>(self, 0x20) != 0) live_free(field<void*>(self, 0x20)); // 0044B626..0044B633
    field<const void*>(self, 0) = &aurora_object_vtable_00741108; // 0044B646 mov [esi],741108h (CAurObject)
    safe_pointer_list_destruct_004487c0(kotor_scene3d::at(self, 4), nullptr);   // 0044B63B..0044B64C
}
// Gob scalar deleting destructor (vtbl+00, RET 4).
U __fastcall gob_destruct_gob_0044b9e0(Gob* self, void*, U flags) {    // +00
    gob_destruct_0044b2f0(self, nullptr);                               // 0044B9E3
    if (flags & 1u) live_free(self);                                    // 0044B9E8..0044B9F0
    return word(self);                                                  // 0044B9F8 mov eax,esi
}
} // extern "C"
