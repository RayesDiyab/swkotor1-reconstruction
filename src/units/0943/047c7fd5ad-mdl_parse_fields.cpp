// mdl_parse_fields.cpp - lane-m48-asciimdl fork C: ASCII .mdl InternalParseField bodies of the GOG swkotor.exe
// (image base 0x400000): MdlNode 00465560, MdlNodeReference 00468070, MdlNodeLight 00469150, MdlNodeTriMesh 00469700,
// MdlNodeDanglyMesh 0046A160, MdlNodeAnimMesh 0046A240, MdlNodeSkin 0046A2F0, MdlNodeAABB 0046A3C0.
// Listings: lane-m48-asciimdl/d/<ADDR>.txt (local only; all Ghidra ranges). Off the menu path (ASCII models only).
// Shape of every body: `if (stricmp(keyword, firstword(line)) == 0) reader(line + strlen(keyword), &node[off])`, each
// test independent (no else) unless the listing jumps over the following tests (controller triplets below), then a
// tail call to the base-class parser. stricmp argument order is (keyword, firstword) as pushed (push eax; push kw).
// All are original __thiscall(node, line) RET 4 -> D1 __fastcall(self, unused_edx, line); EAX unused by every caller.
// Readers' output fields are passed as typed object pointers into the raw node storage (object-pointer casts only).
// EH: 00469700 states 0 (00445720 ctor block, 00469835) and 1 (local int list, 00469B6B) only free the block / list
// data if a callee throws; no callee here throws, so the unwind (frame 00716CA9) is cited, not emitted (D30-FINAL).
#include "d51e09c5f8-asciimdl.hpp"
#include "21ca817b3e-storage.h"   // historical_storage_007a8a88 / 007a8a7c / 007a8a70 (model46-m46-model-storage owner)

using namespace asciimdl;

namespace gui_viewport_list {
// D58: CExoArrayList<int>::Add 00444FE0 single owner (identical declaration to 3d/model/model_postprocess.cpp:20).
__declspec(noinline) void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*, void*, int);
}

extern "C" {
// font-info-parse (font-txi/font_txi.hpp:92: font_txi::U == std::uint32_t, font_txi::List == ArrayMutations); raw EAX unused here
aurora_model::U __cdecl read_vector_list_00421ff0(char*, aurora_model::List*);
}

namespace {
// .rdata strings (read from the pinned image with tools/pe.py)
constexpr char kF_0073e2e4[] = "%f";
constexpr char kF3_0073ee40[] = "%f %f %f";
constexpr char kS_0073d5d0[] = "%s";
constexpr char kNull_0073ee04[] = "NULL";
// MdlNode 00465560
constexpr char kPosition_00741fac[] = "position";
constexpr char kOrientation_00741fa0[] = "orientation";
constexpr char kPositionkey_00741f94[] = "positionkey";
constexpr char kPositionbezierkey_00741f80[] = "positionbezierkey";
constexpr char kOrientationkey_00741f70[] = "orientationkey";
constexpr char kOrientationbezierkey_00741f58[] = "orientationbezierkey";
constexpr char kWirecolor_00741f4c[] = "wirecolor";
constexpr char kScale_00741f44[] = "scale";
constexpr char kScalekey_00741f38[] = "scalekey";
constexpr char kScalebezierkey_00741f28[] = "scalebezierkey";
constexpr char kParent_00741f20[] = "parent";
// MdlNodeReference 00468070
constexpr char kRefModel_00742a2c[] = "refModel";
constexpr char kReattachable_00742a1c[] = "reattachable";
// MdlNodeLight 00469150
constexpr char kColor_00742c54[] = "color";
constexpr char kColorkey_00742c48[] = "colorkey";
constexpr char kColorbezierkey_00742c38[] = "colorbezierkey";
constexpr char kRadius_00742aa4[] = "radius";
constexpr char kRadiuskey_00742c2c[] = "radiuskey";
constexpr char kRadiusbezierkey_00742c1c[] = "radiusbezierkey";
constexpr char kShadowradius_00742c0c[] = "shadowradius";
constexpr char kShadowradiuskey_00742bfc[] = "shadowradiuskey";
constexpr char kShadowradiusbezierkey_00742be4[] = "shadowradiusbezierkey";
constexpr char kVerticaldisplacement_00742bcc[] = "verticaldisplacement";
constexpr char kVerticaldisplacementkey_00742bb4[] = "verticaldisplacementkey";
constexpr char kVerticaldisplacementbezierkey_00742b94[] = "verticaldisplacementbezierkey";
constexpr char kMultiplier_00742b88[] = "multiplier";
constexpr char kMultiplierkey_00742b78[] = "multiplierkey";
constexpr char kMultiplierbezierkey_00742b64[] = "multiplierbezierkey";
constexpr char kAmbientonly_00742b58[] = "ambientonly";
constexpr char kShadow_00742b50[] = "shadow";
constexpr char kIsdynamic_00742b44[] = "isdynamic";
constexpr char kNDynamicType_00742b34[] = "nDynamicType";
constexpr char kAffectdynamic_00742b24[] = "affectdynamic";
constexpr char kLightpriority_00742b14[] = "lightpriority";
constexpr char kGenerateflare_00742b04[] = "generateflare";
constexpr char kTexturenames_00742af4[] = "texturenames";
constexpr char kFlaresizes_00742ae8[] = "flaresizes";
constexpr char kFlarepositions_00742ad8[] = "flarepositions";
constexpr char kFlarecolorshifts_00742ac4[] = "flarecolorshifts";
constexpr char kFlareradius_00742ab8[] = "flareradius";
constexpr char kFadingLight_00742aac[] = "fadingLight";
// MdlNodeTriMesh 00469700
constexpr char kTexture0_00742e18[] = "texture0";
constexpr char kTexture1_00742e0c[] = "texture1";
constexpr char kBitmap_00742e04[] = "bitmap";
constexpr char kVerts_00742dfc[] = "verts";
constexpr char kTverts_00742df4[] = "tverts";
constexpr char kAnimateuv_00742de8[] = "animateuv";
constexpr char kUvdirectionx_00742dd8[] = "uvdirectionx";
constexpr char kUvdirectiony_00742dc8[] = "uvdirectiony";
constexpr char kUvjitter_00742dbc[] = "uvjitter";
constexpr char kUvjitterspeed_00742dac[] = "uvjitterspeed";
constexpr char kTverts1_00742da4[] = "tverts1";
constexpr char kTverts2_00742d9c[] = "tverts2";
constexpr char kTverts3_00742d94[] = "tverts3";
constexpr char kColors_00742d8c[] = "colors";
constexpr char kTexindices1_00742d80[] = "texindices1";
constexpr char kTexindices2_00742d74[] = "texindices2";
constexpr char kTexindices3_00742d68[] = "texindices3";
constexpr char kLmt_00742d60[] = ".lmt";
constexpr char kTverts1offset_00742d50[] = "tverts1offset";
constexpr char kFaces_00742d48[] = "faces";
constexpr char kDiffuse_00742d40[] = "diffuse";
constexpr char kAmbient_00741e90[] = "ambient";
constexpr char kTransparencyhint_00742d2c[] = "transparencyhint";
constexpr char kBeaming_00742d24[] = "beaming";
constexpr char kRender_00742978[] = "render";
constexpr char kRotatetexture_00742d14[] = "rotatetexture";
constexpr char kBmin_00742a9c[] = "bmin";
constexpr char kBmax_00742a94[] = "bmax";
constexpr char kVertexindicescount_00742d00[] = "vertexindicescount";
constexpr char kLeftoverfaces_00742cf0[] = "leftoverfaces";
constexpr char kVertexindices_00742ce0[] = "vertexindices";
constexpr char kLightmapped_00742cd4[] = "lightmapped";
constexpr char kBackgroundGeometry_00742cbc[] = "m_bIsBackgroundGeometry";
constexpr char kSelfillumcolor_00742cac[] = "selfillumcolor";
constexpr char kSelfillumcolorkey_00742c98[] = "selfillumcolorkey";
constexpr char kSelfillumcolorbezierkey_00742c80[] = "selfillumcolorbezierkey";
constexpr char kAlpha_00742c78[] = "alpha";
constexpr char kAlphakey_00742c6c[] = "alphakey";
constexpr char kAlphabezierkey_00742c5c[] = "alphabezierkey";
// MdlNodeDanglyMesh 0046A160
constexpr char kDisplacement_00742e44[] = "displacement";
constexpr char kPeriod_00742e3c[] = "period";
constexpr char kTightness_00742e30[] = "tightness";
constexpr char kConstraints_00742e24[] = "constraints";
// MdlNodeAnimMesh 0046A240
constexpr char kSampleperiod_00742e6c[] = "sampleperiod";
constexpr char kAnimverts_00742e60[] = "animverts";
constexpr char kAnimtverts_00742e54[] = "animtverts";
// MdlNodeSkin 0046A2F0
constexpr char kWeights_00742eb0[] = "weights";
constexpr char kQboneRefInv_00742ea0[] = "qbone_ref_inv";
constexpr char kTboneRefInv_00742e90[] = "tbone_ref_inv";
constexpr char kBoneconstantindices_00742e7c[] = "boneconstantindices";
// MdlNodeAABB 0046A3C0
constexpr char kAabb_00742a48[] = "aabb";

// push esi; call 00463530 (firstword); push eax; push kw; call 0070ACAF (stricmp); add esp,0Ch; test eax,eax
bool keyword(const char* kw, char* line) { return kotor_stricmp_0070acaf(kw, firstword_00463530(line)) == 0; }
float* f32(void* node, U off) { return reinterpret_cast<float*>(at(node, off)); }
volatile std::int32_t* i32(void* node, U off) { return reinterpret_cast<volatile std::int32_t*>(at(node, off)); }
volatile bool* b8(void* node, U off) { return reinterpret_cast<volatile bool*>(at(node, off)); }

// Controller triplet (e.g. 004655EB..0046567E): single / key / bezier tests chained with else (a matching test jumps
// past the remaining two, "jmp <add esp,18h>"). Each builder is called as
// (firstword(line), line + skip, &node[38h] controllers, &node[44h] data, type, index); the single form passes index -1.
void controller_triplet(MdlNode* n, char* line, I type, I index,
                        const char* k_single, U s_single, const char* k_key, U s_key, const char* k_bezier, U s_bezier) {
    if (keyword(k_single, line))
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + s_single, list_at(n, 0x38),
                                                   list_at(n, 0x44), type, -1);
    else if (keyword(k_key, line))
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + s_key, list_at(n, 0x38),
                                            list_at(n, 0x44), type, index);
    else if (keyword(k_bezier, line))
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + s_bezier, list_at(n, 0x38),
                                           list_at(n, 0x44), type, index);
}

// The original follows sscanf("%s") with an inline byte copy whose source is the sscanf result taken as an address
// (0046975B..00469768, 0046979A..004697A8, 004697DD..004697EA); it runs only when sscanf("%s") returns 0, which it
// cannot (it returns 1 or EOF). Kept for fidelity (same helper shape as lane-m48-a anim_parse.cpp).
void copy_from_result(char* destination, int result) {
    const char* source = reinterpret_cast<const char*>(static_cast<std::intptr_t>(result));
    for (I i = 0;; ++i) { const char c = source[i]; destination[i] = c; if (c == '\0') break; }
}
void scan_name(char* text, char* destination) {
    const int result = kotor_sscanf_006fae08(text, kS_0073d5d0, destination);
    if (result == 0) copy_from_result(destination, result);
}
} // namespace

extern "C" {
// ================================================================ 00465560 MdlNode::InternalParseField (thiscall RET 4)
void __fastcall mdl_node_internal_parse_field_00465560(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 00465560..00465572
        if (keyword(kPosition_00741fac, line))                                                  // 00465578..0046558E
            kotor_sscanf_006fae08(line + 8, kF3_0073ee40, f32(self, 0x10), f32(self, 0x14), f32(self, 0x18)); // 00465590..004655AA
        if (keyword(kOrientation_00741fa0, line))                                               // 004655AD..004655C3
            mdl_parse_quaternion_00463a30(line + 0x0B, at(self, 0x1C));                         // 004655C5..004655D2
        controller_triplet(self, line, 8, 0x10, kPosition_00741fac, 8, kPositionkey_00741f94, 0x0B,
                           kPositionbezierkey_00741f80, 0x11);                                  // 004655D5..0046567E
        controller_triplet(self, line, 0x14, 0x1C, kOrientation_00741fa0, 0x0B, kOrientationkey_00741f70, 0x0E,
                           kOrientationbezierkey_00741f58, 0x14);                               // 00465681..0046572A
        float wire[3] = {0.0f, 0.0f, 0.0f};                                                     // 0046572E..0046573E [esp+8..10h]
        if (keyword(kWirecolor_00741f4c, line))                                                 // 00465746..0046575B
            kotor_sscanf_006fae08(line + 9, kF3_0073ee40, &wire[0], &wire[1], &wire[2]);        // 0046575D..0046577A (locals never read)
        controller_triplet(self, line, 0x24, -1, kScale_00741f44, 5, kScalekey_00741f38, 8,
                           kScalebezierkey_00741f28, 0x0E);                                     // 0046577D..00465826
    }
    if (kotor_strncmp_006faff0(kParent_00741f20, line, 6) == 0) {                               // 00465829..0046583B
        char name[0x40] = {};   // [esp+14h]; D2 (0046584B): original stack bytes are unwritten if sscanf hits EOF
        kotor_sscanf_006fae08(line + 6, kS_0073d5d0, name);                                     // 0046583D..0046584B
        const bool is_null = kotor_stricmp_0070acaf(name, kNull_0073ee04) == 0;                 // 00465850..00465862
        void* const tree = ptr<void>(get<U>(self, 0x08));                                       // 00465864 eax = [edi+8]
        if (is_null) {
            put<U>(self, 0x0C, 0);                                                              // 00465869
            put<U>(tree, 0x28, addr(self));                                                     // 00465870 tree root = node
            return;                                                                             // 00465878 ret 4
        }
        MdlNode* parent = nullptr;                                                              // eax = tree (0) when no tree
        if (tree != nullptr)                                                                    // 0046587B..0046587D
            parent = mdl_find_node_00464af0(ptr<MdlNode>(get<U>(tree, 0x28)), name);           // 0046587F..0046588D
        put<U>(self, 0x0C, addr(parent));                                                       // 00465892
        if (parent != nullptr)                                                                  // 00465890 / 00465895
            gui_viewport_list::add_00444fe0(list_at(parent, 0x2C), nullptr, static_cast<int>(addr(self))); // 00465897..0046589B
    }
}                                                                                               // 004658A5 ret 4

// ================================================================ 00468070 MdlNodeReference::InternalParseField (RET 4)
void __fastcall mdl_reference_internal_parse_field_00468070(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 00468070..0046807F
        if (keyword(kRefModel_00742a2c, line))                                                  // 00468081..00468097
            parse_cstr_00463590(line + 8, reinterpret_cast<char*>(at(self, 0x50)));             // 00468099..004680A6
        if (keyword(kReattachable_00742a1c, line))                                              // 004680A9..004680BF
            parse_int_004635e0(line + 0x0C, i32(self, 0x70));                                   // 004680C1..004680CE
    }
    mdl_node_internal_parse_field_00465560(self, nullptr, line);                                // 004680D1..004680D4
}

// ================================================================ 00469150 MdlNodeLight::InternalParseField (RET 4)
void __fastcall mdl_light_internal_parse_field_00469150(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 00469150..0046915F
        controller_triplet(self, line, 0x4C, -1, kColor_00742c54, 5, kColorkey_00742c48, 8,
                           kColorbezierkey_00742c38, 0x0E);                                     // 00469165..0046920E
        controller_triplet(self, line, 0x58, -1, kRadius_00742aa4, 6, kRadiuskey_00742c2c, 9,
                           kRadiusbezierkey_00742c1c, 0x0F);                                    // 00469211..004692BA
        controller_triplet(self, line, 0x60, -1, kShadowradius_00742c0c, 0x0C, kShadowradiuskey_00742bfc, 0x0F,
                           kShadowradiusbezierkey_00742be4, 0x15);                              // 004692BD..00469366
        controller_triplet(self, line, 0x64, -1, kVerticaldisplacement_00742bcc, 0x14,
                           kVerticaldisplacementkey_00742bb4, 0x17, kVerticaldisplacementbezierkey_00742b94, 0x1D); // 00469369..00469412
        controller_triplet(self, line, 0x8C, -1, kMultiplier_00742b88, 0x0A, kMultiplierkey_00742b78, 0x0D,
                           kMultiplierbezierkey_00742b64, 0x13);                                // 00469415..004694C7
        if (keyword(kAmbientonly_00742b58, line)) parse_int_004635e0(line + 0x0B, i32(self, 0x94));      // 004694CA..004694F2
        if (keyword(kShadow_00742b50, line)) parse_int_004635e0(line + 6, i32(self, 0xA0));              // 004694F5..0046951D
        if (keyword(kIsdynamic_00742b44, line)) parse_int_004635e0(line + 9, i32(self, 0x98));           // 00469520..00469548
        if (keyword(kNDynamicType_00742b34, line)) parse_int_004635e0(line + 0x0C, i32(self, 0x98));     // 0046954B..00469573
        if (keyword(kAffectdynamic_00742b24, line)) parse_int_004635e0(line + 0x0D, i32(self, 0x9C));    // 00469576..0046959E
        if (keyword(kLightpriority_00742b14, line)) parse_int_004635e0(line + 0x0D, i32(self, 0x90));    // 004695A1..004695C9
        if (keyword(kGenerateflare_00742b04, line)) parse_int_004635e0(line + 0x0D, i32(self, 0xA4));    // 004695CC..004695F4
        if (keyword(kTexturenames_00742af4, line)) mdl_parse_cstr_list_00464880(line + 0x0C, list_at(self, 0x84)); // 004695F7..0046961F
        if (keyword(kFlaresizes_00742ae8, line)) mdl_parse_float_list_004628c0(line + 0x0A, list_at(self, 0x60));  // 00469622..00469647
        if (keyword(kFlarepositions_00742ad8, line)) mdl_parse_float_list_004628c0(line + 0x0E, list_at(self, 0x6C)); // 0046964A..0046966F
        if (keyword(kFlarecolorshifts_00742ac4, line)) read_vector_list_00421ff0(line + 0x10, list_at(self, 0x78)); // 00469672..00469697
        if (keyword(kFlareradius_00742ab8, line)) kotor_sscanf_006fae08(line + 0x0B, kF_0073e2e4, f32(self, 0x50)); // 0046969A..004696C4
        if (keyword(kFadingLight_00742aac, line)) parse_int_004635e0(line + 0x0B, i32(self, 0xA8));      // 004696C7..004696EF
    }
    mdl_node_internal_parse_field_00465560(self, nullptr, line);                                // 004696F2..004696F5
}

// ================================================================ 00469700 MdlNodeTriMesh::InternalParseField (RET 4)
// Node fields: +A8 texture0 name, +C8 texture1 name, +134 MdlNodeTriMesh_PostProcess* (0x6C block, 00445720),
// +138 animate-uv flag, +13C..+148 uv floats, +58 per-face list (0x20 stride), +8C/+98 diffuse/ambient,
// +184..+189 bool bytes, +A4 transparency hint, +64/+70 bmin/bmax, +E8/+F4/+100 vertex-index lists.
// PostProcess block lists: +00 verts, +0C tverts, +18 tverts1, +24 tverts2, +30 tverts3, +3C colors, +48 faces.
void __fastcall mdl_trimesh_internal_parse_field_00469700(MdlNode* self, void*, char* line) {
    if (keyword(kTexture0_00742e18, line))                                                      // 00469727..00469741
        scan_name(line + 8, reinterpret_cast<char*>(at(self, 0xA8)));                           // 00469743..00469768
    if (keyword(kTexture1_00742e0c, line))                                                      // 0046976A..00469780
        scan_name(line + 8, reinterpret_cast<char*>(at(self, 0xC8)));                           // 00469782..004697A8
    char bitmap[0x40];                                                                          // [esp+40h] (frame B+40..B+7F)
    bitmap[0] = '\0';                                                                           // 004697AB mov byte [esp+44h],0
    if (keyword(kBitmap_00742e04, line))                                                        // 004697AA..004697C5
        scan_name(line + 6, bitmap);                                                            // 004697C7..004697EA
    if (bitmap[0] != '\0') {                                                                    // 004697EC..004697F2
        char* const texture0 = reinterpret_cast<char*>(at(self, 0xA8));                         // 004697F4..0046980C strcpy
        for (I i = 0;; ++i) { const char c = bitmap[i]; texture0[i] = c; if (c == '\0') break; }
    }
    if (mdl_parse_flag_007b927c == 0) {                                                         // 0046980E..00469815
        if (get<U>(self, 0x134) == 0) {                                                         // 0046981B..00469823
            void* const block = live_operator_new(0x6C);                                        // 00469825..0046982C
            void* const post = block != nullptr ? mdl_trimesh_postprocess_construct_00445720(block, nullptr) : nullptr; // 00469833..0046984B
            put<U>(self, 0x134, addr(post));                                                    // 00469858
        }
        void* const post = ptr<void>(get<U>(self, 0x134));                                      // 0046985E edi
        List* const verts = list_at(post, 0x00);
        List* const tverts = list_at(post, 0x0C);                                               // 00469864 [B+0C]
        List* const tverts1 = list_at(post, 0x18);                                              // 0046987D ebx
        List* const tverts2 = list_at(post, 0x24);                                              // 00469867 [B+3C]
        List* const tverts3 = list_at(post, 0x30);                                              // 00469873 [B+18]
        List* const colors = list_at(post, 0x3C);                                               // 00469876 [B+14]
        List* const faces = list_at(post, 0x48);                                                // 00469879 [B+10]
        if (keyword(kVerts_00742dfc, line)) read_vector_list_00421ff0(line + 5, verts);         // 0046987C..004698AD
        if (keyword(kTverts_00742df4, line)) read_vector_list_00421ff0(line + 6, tverts);       // 004698B0..004698D6
        I animate = 0;                                                                          // 004698DA [B+1C] = 0
        if (keyword(kAnimateuv_00742de8, line)) parse_int_004635e0(line + 9, &animate);         // 004698E2..00469907
        if (animate != 0) put<U>(self, 0x138, 1);                                               // 0046990A..00469912
        if (get<U>(self, 0x138) != 0) {                                                         // 0046991C..00469924
            if (keyword(kUvdirectionx_00742dd8, line)) kotor_sscanf_006fae08(line + 0x0C, kF_0073e2e4, f32(self, 0x13C)); // 0046992A..00469957
            if (keyword(kUvdirectiony_00742dc8, line)) kotor_sscanf_006fae08(line + 0x0C, kF_0073e2e4, f32(self, 0x140)); // 0046995A..00469987
            if (keyword(kUvjitter_00742dbc, line)) kotor_sscanf_006fae08(line + 8, kF_0073e2e4, f32(self, 0x144));        // 0046998A..004699B7
            if (keyword(kUvjitterspeed_00742dac, line)) kotor_sscanf_006fae08(line + 0x0D, kF_0073e2e4, f32(self, 0x148)); // 004699BA..004699E7
        }
        if (keyword(kTverts1_00742da4, line)) read_vector_list_00421ff0(line + 7, tverts1);     // 004699EA..00469A0C
        if (keyword(kTverts2_00742d9c, line)) read_vector_list_00421ff0(line + 7, tverts2);     // 00469A0F..00469A35
        if (keyword(kTverts3_00742d94, line)) read_vector_list_00421ff0(line + 7, tverts3);     // 00469A38..00469A5E
        if (keyword(kColors_00742d8c, line)) read_vector_list_00421ff0(line + 6, colors);       // 00469A61..00469A87
        // texindices1..3 read into the static scratch lists 007A8A88 / 007A8A7C / 007A8A70 (push imm32).
        if (keyword(kTexindices1_00742d80, line))                                               // 00469A8A..00469AB0
            read_vector_list_00421ff0(line + 0x0B, reinterpret_cast<List*>(&historical_storage_007a8a88));
        if (keyword(kTexindices2_00742d74, line))                                               // 00469AB3..00469AD9
            read_vector_list_00421ff0(line + 0x0B, reinterpret_cast<List*>(&historical_storage_007a8a7c));
        if (keyword(kTexindices3_00742d68, line))                                               // 00469ADC..00469B02
            read_vector_list_00421ff0(line + 0x0B, reinterpret_cast<List*>(&historical_storage_007a8a70));
        // Lightmap texture-coordinate offsets from "<texture1>.lmt" (00469B05..00469CB2).
        char* const texture1 = reinterpret_cast<char*>(at(self, 0xC8));                         // 00469B0D edi; [B+14]
        if (texture1[0] != '\0' && tverts1->size > 0) {                                         // 00469B05..00469B22
            char copy[0x40];                                                                    // [esp+84h] = B+80; written, never read
            for (I i = 0;; ++i) { const char c = texture1[i]; copy[i] = c; if (c == '\0') break; } // 00469B28..00469B3F
            void* const resource = aurres_get_0044c740(texture1, kLmt_00742d60, nullptr, 1);    // 00469B41..00469B50
            if (resource != nullptr) {                                                          // 00469B55..00469B57
                List offsets;                                                                   // [B+20] data, [B+24] size, [B+28] capacity
                offsets.data = 0; offsets.size = 0; offsets.capacity = 0;                       // 00469B5F..00469B67
                for (char* l = resource_next_0044bfa0(); l != nullptr; l = resource_next_0044bfa0()) { // 00469B76..00469BE9
                    while (*l == ' ' || *l == '\t') ++l;                                        // 00469B85..00469B90
                    if (keyword(kTverts1offset_00742d50, l))                                    // 00469B92..00469BA8
                        mdl_parse_int_list_00464630(l + 0x0D, &offsets);                        // 00469BAA..00469BB8
                    if (keyword(kTexture1_00742e0c, l))                                         // 00469BBB..00469BD1
                        parse_cstr_00463590(l + 8, texture1);                                   // 00469BD3..00469BDD
                }
                aurres_release_0044c8f0(resource, 0);                                           // 00469BEB..00469BEE
                if (static_cast<I>(static_cast<U>(tverts1->size) * 4u) == offsets.size && tverts1->size > 0) { // 00469BF3..00469C10
                    const U base = offsets.data;                                                // 00469C16 edi = data (+8)
                    for (I i = 0; i < tverts1->size; ++i) {                                     // 00469C20..00469C90 (size re-read)
                        const U rec = base + static_cast<U>(i) * 16u;                           // 4 ints per vertex
                        const double scale = static_cast<double>(get<I>(ptr<void>(rec), 0));    // 00469C20 fild [edi-8]
                        const float scale_stored = static_cast<float>(scale);                   // 00469C28 fst dword [esp+10h]
                        void* v = ptr<void>(tverts1->data + static_cast<U>(i) * 12u);           // 00469C23..00469C25
                        const double z = scale * static_cast<double>(get<float>(v, 8));         // 00469C2C fmul [eax+8] (wide)
                        const double y = static_cast<double>(scale_stored) * static_cast<double>(get<float>(v, 4)); // 00469C2F..00469C33
                        const double x = static_cast<double>(scale_stored) * static_cast<double>(get<float>(v, 0)); // 00469C36..00469C3A
                        put<float>(v, 0, static_cast<float>(x));                                // 00469C3C..00469C44
                        put<float>(v, 4, static_cast<float>(y));                                // 00469C46..00469C4E
                        put<float>(v, 8, static_cast<float>(z));                                // 00469C51..00469C59
                        v = ptr<void>(tverts1->data + static_cast<U>(i) * 12u);                 // 00469C5F..00469C61
                        put<float>(v, 0, static_cast<float>(static_cast<double>(get<I>(ptr<void>(rec), 4)) +
                                                            static_cast<double>(get<float>(v, 0))));        // 00469C5C..00469C67
                        v = ptr<void>(tverts1->data + static_cast<U>(i) * 12u + 4u);            // 00469C69..00469C6D
                        put<float>(v, 0, static_cast<float>(static_cast<double>(get<I>(ptr<void>(rec), 8)) +
                                                            static_cast<double>(get<float>(v, 0))));        // 00469C6B..00469C73
                        vector_divide_assign_004a97d0(reinterpret_cast<float*>(ptr<void>(tverts1->data + static_cast<U>(i) * 12u)),
                                                      nullptr, static_cast<float>(get<I>(ptr<void>(rec), 12))); // 00469C75..00469C7F
                    }
                }
                if (offsets.data != 0) live_free(ptr<void>(offsets.data));                      // 00469C92..00469CA6
            }
        }
        if (faces->size == 0 && keyword(kFaces_00742d48, line))                                 // 00469CB6..00469CD9
            mdl_parse_maxface_list_00468a00(line + 5, faces);                                   // 00469CDB..00469CE5
        if (faces->size > 0) {                                                                  // 00469CE8..00469CED
            mdl_maxface_list_set_size_00464350(list_at(self, 0x58), nullptr, faces->size);       // 00469CEF..00469CF3
            for (I i = 0; i < faces->size; ++i)                                                 // 00469CF8..00469D3D (size re-read)
                put<U>(ptr<void>(get<U>(self, 0x58)), static_cast<U>(i) * 0x20u + 0x10u,
                       get<U>(ptr<void>(faces->data), static_cast<U>(i) * 0x14u + 0x0Cu));      // 00469D10..00469D21
        }
        if (keyword(kDiffuse_00742d40, line))                                                   // 00469D3F..00469D56
            kotor_sscanf_006fae08(line + 7, kF3_0073ee40, f32(self, 0x8C), f32(self, 0x90), f32(self, 0x94)); // 00469D58..00469D75
        if (keyword(kAmbient_00741e90, line))                                                   // 00469D78..00469D8E
            kotor_sscanf_006fae08(line + 7, kF3_0073ee40, f32(self, 0x98), f32(self, 0x9C), f32(self, 0xA0)); // 00469D90..00469DAD
        if (keyword(kShadow_00742b50, line)) parse_bool_00463680(line + 6, b8(self, 0x187));     // 00469DB0..00469DD8
        if (keyword(kTransparencyhint_00742d2c, line)) parse_int_004635e0(line + 0x10, i32(self, 0xA4)); // 00469DDB..00469E03
        if (keyword(kBeaming_00742d24, line)) parse_bool_00463680(line + 7, b8(self, 0x188));    // 00469E06..00469E2E
        if (keyword(kRender_00742978, line)) parse_bool_00463680(line + 6, b8(self, 0x189));     // 00469E31..00469E59
        if (keyword(kRotatetexture_00742d14, line)) parse_bool_00463680(line + 0x0D, b8(self, 0x185)); // 00469E5C..00469E84
        if (keyword(kBmin_00742a9c, line))                                                      // 00469E87..00469E9D
            kotor_sscanf_006fae08(line + 4, kF3_0073ee40, f32(self, 0x64), f32(self, 0x68), f32(self, 0x6C)); // 00469E9F..00469EB9
        if (keyword(kBmax_00742a94, line))                                                      // 00469EBC..00469ED2
            kotor_sscanf_006fae08(line + 4, kF3_0073ee40, f32(self, 0x70), f32(self, 0x74), f32(self, 0x78)); // 00469ED4..00469EEE
        if (keyword(kVertexindicescount_00742d00, line)) mdl_parse_int_list_00464630(line + 0x12, list_at(self, 0x100)); // 00469EF1..00469F19
        if (keyword(kLeftoverfaces_00742cf0, line)) mdl_parse_uint_list_00468b90(line + 0x0D, list_at(self, 0xF4));     // 00469F1C..00469F44
        if (keyword(kVertexindices_00742ce0, line)) mdl_parse_uint_ptr_list_00464a40(line + 0x0D, list_at(self, 0xE8));  // 00469F47..00469F6F
        if (keyword(kLightmapped_00742cd4, line)) parse_bool_00463680(line + 0x0B, b8(self, 0x184));   // 00469F72..00469F9A
        if (keyword(kBackgroundGeometry_00742cbc, line)) parse_bool_00463680(line + 0x17, b8(self, 0x186)); // 00469F9D..00469FC5
        controller_triplet(self, line, 0x64, -1, kSelfillumcolor_00742cac, 0x0E, kSelfillumcolorkey_00742c98, 0x11,
                           kSelfillumcolorbezierkey_00742c80, 0x17);                            // 00469FC8..0046A071
        controller_triplet(self, line, 0x84, -1, kAlpha_00742c78, 5, kAlphakey_00742c6c, 8,
                           kAlphabezierkey_00742c5c, 0x0E);                                     // 0046A074..0046A126
    }
    mdl_node_internal_parse_field_00465560(self, nullptr, line);                                // 0046A129..0046A12C
}                                                                                               // 0046A148 ret 4

// ================================================================ 0046A160 MdlNodeDanglyMesh::InternalParseField (RET 4)
void __fastcall mdl_dangly_internal_parse_field_0046a160(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 0046A160..0046A16F
        if (keyword(kDisplacement_00742e44, line)) kotor_sscanf_006fae08(line + 0x0C, kF_0073e2e4, f32(self, 0x1A8)); // 0046A175..0046A1A2
        if (keyword(kPeriod_00742e3c, line)) kotor_sscanf_006fae08(line + 6, kF_0073e2e4, f32(self, 0x1B0));          // 0046A1A5..0046A1D2
        if (keyword(kTightness_00742e30, line)) kotor_sscanf_006fae08(line + 9, kF_0073e2e4, f32(self, 0x1AC));       // 0046A1D5..0046A202
        if (keyword(kConstraints_00742e24, line)) mdl_parse_float_list_004628c0(line + 0x0B, list_at(self, 0x19C));   // 0046A205..0046A22D
    }
    mdl_trimesh_internal_parse_field_00469700(self, nullptr, line);                             // 0046A230..0046A233
}

// ================================================================ 0046A240 MdlNodeAnimMesh::InternalParseField (RET 4)
void __fastcall mdl_anim_mesh_internal_parse_field_0046a240(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 0046A240..0046A24F
        if (keyword(kSampleperiod_00742e6c, line)) kotor_sscanf_006fae08(line + 0x0C, kF_0073e2e4, f32(self, 0x19C)); // 0046A255..0046A282
        if (keyword(kAnimverts_00742e60, line)) read_vector_list_00421ff0(line + 9, list_at(self, 0x1A0));            // 0046A285..0046A2AD
        if (keyword(kAnimtverts_00742e54, line)) read_vector_list_00421ff0(line + 0x0A, list_at(self, 0x1AC));        // 0046A2B0..0046A2D8
    }
    mdl_trimesh_internal_parse_field_00469700(self, nullptr, line);                             // 0046A2DB..0046A2DE
}

// ================================================================ 0046A2F0 MdlNodeSkin::InternalParseField (RET 4)
void __fastcall mdl_skin_internal_parse_field_0046a2f0(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                                         // 0046A2F0..0046A2FF
        if (keyword(kWeights_00742eb0, line)) mdl_parse_skin_weight_list_00468d70(line + 7, list_at(self, 0x19C));    // 0046A305..0046A32D
        if (keyword(kQboneRefInv_00742ea0, line)) mdl_parse_quaternion_list_00468f60(line + 0x0D, list_at(self, 0x1B8)); // 0046A330..0046A358
        if (keyword(kTboneRefInv_00742e90, line)) read_vector_list_00421ff0(line + 0x0D, list_at(self, 0x1C4));       // 0046A35B..0046A383
        if (keyword(kBoneconstantindices_00742e7c, line)) mdl_parse_int_list_00464630(line + 0x13, list_at(self, 0x1D0)); // 0046A386..0046A3AE
    }
    mdl_trimesh_internal_parse_field_00469700(self, nullptr, line);                             // 0046A3B1..0046A3B4
}

// ================================================================ 0046A3C0 MdlNodeAABB::InternalParseField (RET 4)
void __fastcall mdl_aabb_internal_parse_field_0046a3c0(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0 && keyword(kAabb_00742a48, line))                          // 0046A3C0..0046A3E7
        mdl_parse_aabb_00463c20(line + 4, at(self, 0x19C));                                     // 0046A3E9..0046A3F9
    mdl_trimesh_internal_parse_field_00469700(self, nullptr, line);                             // 0046A3FC..0046A3FF
}
}
