// model_tree.cpp - lane-3d-model rev2: the part-tree predicates handed over by D35 (SpawnRoom path):
// TreeHasSingleTexture 00443B30, TreeHasTransparency 00443D70, TreeHasGeometry 004440D0, TreeSetGeometryFlags
// 004443E0. Each original body is the recursive function with its first levels unrolled by the compiler; the
// unrolled levels are expressed here as a depth-limited local recursion and the original out-of-line recursive
// call is kept at the same depth (children of level 4 / 8 / 9 / 10 respectively).
// Part fields: vtbl+2C AsTriMesh, +30 children data, +34 children count, +40 owner (scene object), +2C (byte)
// geometry flag; trimesh +44 texture, +84 alpha (float); owner +168 the single texture seen so far.
#include "a7e200e217-model_instance.hpp"

using namespace aurora_model;

extern "C" {
U8 __cdecl aurora_tree_has_single_texture_00443b30(Part* part);
U8 __cdecl aurora_tree_has_transparency_00443d70(Part* part);
U8 __cdecl aurora_tree_has_geometry_004440d0(Part* part);
U8 __cdecl aurora_tree_set_geometry_flags_004443e0(Part* part);
U* __fastcall part_list_at_00437c40(List* self, void* unused_edx, I index);   // model_postprocess.cpp
}

namespace {
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
Part* child_at(Part* p, I i) { return ptr<Part>(ptr<U>(get<U>(p, 0x30))[i]); }
I child_count(Part* p) { return get<I>(p, 0x34); }
void* as_trimesh(Part* p) { return ptr<void>(part_vt(p)->as_trimesh(p, nullptr)); }

// ---- TreeHasSingleTexture: owner == 0 -> false; a trimesh texture is recorded in owner+168 on first sight,
// later ones must have the same first dword (texture name handle) as the recorded one.
enum class Step { Fail, Next };
Step single_texture_step(Part* p) {
    void* const mesh = as_trimesh(p);                                               // 00443B38..00443B3C
    unsigned char* const owner = get<unsigned char*>(p, 0x40);                      // 00443B3F
    if (owner == nullptr) return Step::Fail;                                        // 00443B42..00443B44
    if (mesh != nullptr) {                                                          // 00443B46..00443B48
        const U tex = get<U>(mesh, 0x44);                                           // 00443B52
        const U seen = get<U>(owner, 0x168);                                        // 00443B4A
        if (seen == 0) put<U>(owner, 0x168, tex);                                   // 00443B50..00443B57
        else if (get<U>(ptr<void>(seen), 0) != get<U>(ptr<void>(tex), 0)) return Step::Fail; // 00443BA6..00443BBB
    }
    return Step::Next;
}
bool single_texture_levels(Part* p, I depth) {
    if (single_texture_step(p) == Step::Fail) return false;
    for (I i = 0; i < child_count(p); ++i) {                                        // counts re-read every pass ([+34])
        if (depth < 4) { if (!single_texture_levels(child_at(p, i), depth + 1)) return false; }
        else if (aurora_tree_has_single_texture_00443b30(child_at(p, i)) == 0) return false; // 00443CF0..00443D01
    }
    return true;
}

// ---- TreeHasTransparency: owner == 0 -> subtree skipped (false); trimesh alpha < 1.0 -> true.
enum class Seen { Skip, Hit, Descend };
Seen transparency_step(Part* p) {
    void* const mesh = as_trimesh(p);                                               // 00443D78..00443D7C
    if (get<U>(p, 0x40) == 0) return Seen::Skip;                                    // 00443D7F..00443D84
    if (mesh != nullptr && get<float>(mesh, 0x84) < 1.0f) return Seen::Hit;         // 00443D8D..00443DA2 (fcomp 0073D764; jp: >=, unordered)
    return Seen::Descend;
}
bool transparency_levels(Part* p, I depth) {
    const Seen s = transparency_step(p);
    if (s == Seen::Skip) return false;
    if (s == Seen::Hit) return true;
    for (I i = 0; i < child_count(p); ++i) {
        if (depth < 8) { if (transparency_levels(child_at(p, i), depth + 1)) return true; }
        else if (aurora_tree_has_transparency_00443d70(child_at(p, i)) != 0) return true; // 00443FE8..00443FF9
    }
    return false;
}

// ---- TreeHasGeometry: owner == 0 -> subtree skipped (false); any trimesh -> true.
bool geometry_levels(Part* p, I depth) {
    void* const mesh = as_trimesh(p);                                               // 004440D8..004440DC
    if (get<U>(p, 0x40) == 0) return false;                                         // 004440DF..004440E4
    if (mesh != nullptr) return true;                                               // 004440ED..004440F1
    for (I i = 0; i < child_count(p); ++i) {
        if (depth < 9) { if (geometry_levels(child_at(p, i), depth + 1)) return true; }
        else if (aurora_tree_has_geometry_004440d0(child_at(p, i)) != 0) return true; // 004442E5..004442F6
    }
    return false;
}

// ---- TreeSetGeometryFlags: flag(p) = any child flagged || p is a trimesh; returned as the byte at +2C.
void set_geometry_levels(Part* p, I depth) {
    put<U8>(p, 0x2C, 0);                                                            // 004443ED / 00444419 / ...
    for (I i = 0; i < child_count(p); ++i) {
        if (depth < 10) {
            Part* const child = child_at(p, i);
            set_geometry_levels(child, depth + 1);
            if (get<U8>(child, 0x2C) != 0) put<U8>(p, 0x2C, 1);                     // e.g. 004445F2..004445F9
        } else {
            Part* const child = ptr<Part>(*part_list_at_00437c40(reinterpret_cast<List*>(at(p, 0x30)), nullptr, i)); // 004445C0..004445C8
            if (aurora_tree_set_geometry_flags_004443e0(child) != 0) put<U8>(p, 0x2C, 1); // 004445CA..004445D7
        }
    }
    if (as_trimesh(p) != nullptr) put<U8>(p, 0x2C, 1);                              // e.g. 004445E3..004445EE
}
}

// 00443B30 TreeHasSingleTexture(part), cdecl -> AL.
U8 __cdecl aurora_tree_has_single_texture_00443b30(Part* part) {
    return single_texture_levels(part, 0) ? 1 : 0;                                  // 00443B30..00443D6E
}
// 00443D70 TreeHasTransparency(part), cdecl -> AL.
U8 __cdecl aurora_tree_has_transparency_00443d70(Part* part) {
    return transparency_levels(part, 0) ? 1 : 0;                                    // 00443D70..004440CA
}
// 004440D0 TreeHasGeometry(part), cdecl -> AL.
U8 __cdecl aurora_tree_has_geometry_004440d0(Part* part) {
    return geometry_levels(part, 0) ? 1 : 0;                                        // 004440D0..004443DF
}
// 004443E0 TreeSetGeometryFlags(part), cdecl -> AL = part[+2C].
U8 __cdecl aurora_tree_set_geometry_flags_004443e0(Part* part) {
    set_geometry_levels(part, 0);                                                   // 004443E0..0044482A
    return get<U8>(part, 0x2C);                                                     // 0044482A
}
