// mdl_parse_emitter.cpp - lane-m48-asciimdl (fork E): MdlNodeEmitter::InternalParseField 004658B0 of the GOG
// swkotor.exe (image base 0x400000), 004658B0..00468054 (10,149 bytes; listing d/004658B0.txt, local only).
// Reached only from MdlNode::ParseFieldDispatch 0043FA10 (type 5 -> jmp 004658B0) while parsing ASCII .mdl text;
// off the binary-MDL menu path.
//
// ABI: original __thiscall(this = emitter node, char* line) RET 4 -> D1 __fastcall(self, unused_edx, line)
// (asciimdl.hpp). EAX is not meaningful (every path ends with the call of 00465560; the dispatcher's caller ignores EAX).
//
// Transcription method: tools/E_gen_emitter.py walks EVERY instruction from 004658FC to 00468044 and consumes it with
// one of five recognised shapes (it aborts on any other instruction), emitting one statement per original keyword
// test; the statements in the body are that output (174 keyword tests, 174 distinct .rdata strings read from the
// pinned image; tools/E-keywords.json lists keyword, string address, instruction range and arguments). Shapes:
//   (K) push esi; call 00463530 firstword; push eax; push <kw>; call 0070ACAF _stricmp(kw, word); add esp,0Ch;
//       test eax,eax; jne <next>                         -> if (_stricmp(kw, firstword(line)) == 0)
//   (P) lea r,[edi|esp+off]; push r; lea r,[esi+n]; push r; call 00463680/004635E0/00463590; add esp,8
//                                                        -> parse_bool / parse_int / parse_cstr(line + n, target)
//   (S) lea r,[edi+off]; push r; lea r,[esi+n]; push 0073E2E4 "%f"; push r; call 006FAE08; add esp,0Ch
//                                                        -> sscanf(line + n, "%f", (float*)(node + off))
//   (C) push idx; push type; lea [edi+44h]; lea [edi+38h]; lea [esi+n]; push esi; call 00463530; add esp,4;
//       push eax; call 00464E40 / 00465090 / 00465320 (6 cdecl args, add esp,18h at the group join)
//       -> builder(firstword(line), line + n, node+38h controllers, node+44h controller data, type, idx);
//       the (up to) three builders of one controller name form an if / else-if chain (jmp <join> after each arm).
//   (A) 00465AE7..00465B8E flag application (one Enable* call per set local, original order).
// The first test's `push esi` is at 004658CB, before the local initialisation stores (004658CC..004658F8).
// The last three arms (tangentlength*, 00467F6B..00468041) end in compiler-duplicated copies of the common tail
// `push esi; mov ecx,edi; call 00465560; ret 4` (00467FA8..00467FB6, 00467FF6..00468004); they are transcribed as
// "base parser, then return", identical to reaching the common tail 00468044 (only the remaining tests are skipped,
// exactly as their jne targets do in the original).
// Argument evaluation: the original pushes the builder's other arguments before calling firstword for the keyword
// argument; firstword's only effect is its returned pointer, so C++ argument order is behaviour-equivalent.
//
// Locals (esp = entry-20h inside the tests): twelve flag locals zeroed at 004658CC..004658F8 and tested at
// 00465AE7..00465B8E. The `p2p` flag lives in BYTE 0 of the caller's argument slot of `line` ([esp+28h] at 004658CC
// = entry+4): the compiler reused the dead argument slot after loading it into ESI (004658BA). The same 4-byte slot is
// later the "%d" destination of `renderorder` (00465BAB..00465BC6: sscanf(line+0Bh, "%d", &slot); word [edi+108h] =
// low word of the slot). It is reproduced as a 32-bit local that starts as the argument pointer bits with byte 0
// cleared (004658CC mov byte [esp+28h],bl), so when sscanf converts nothing (EOF / no digits) the stored word is
// (pointer bits 8..15 << 8) | p2p-byte, as in the original (recorded: the value derives from the run-time argument
// pointer, which differs between the original process and this one; no constant is invented).
// p2p_sel / random are 32-bit locals (parse_int; cmp dword ==1 / test byte &1, &2); the other ten flags are bytes
// (parse_bool; cmp byte ,bl).
//
// No floating-point arithmetic (the three "%f" fields are written by sscanf directly into the node), no allocation,
// no EH frame, no throwing callee (D30-FINAL /EHsc rule satisfied). No dynamic initializers (constexpr strings only).
#include "d51e09c5f8-asciimdl.hpp"

using namespace aurora_model;

namespace {
// .rdata keyword strings (read from the pinned image; suffix = original string address)
constexpr char kKw_0073f054[] = "random";
constexpr char kKw_00741df4[] = "fps";
constexpr char kKw_00741fb8[] = "tangentlengthbezierkey";
constexpr char kKw_00741fd0[] = "tangentlengthkey";
constexpr char kKw_00741fe4[] = "tangentlength";
constexpr char kKw_00741ff4[] = "tangentspreadbezierkey";
constexpr char kKw_0074200c[] = "tangentspreadkey";
constexpr char kKw_00742020[] = "tangentspread";
constexpr char kKw_00742030[] = "controlptdelaybezierkey";
constexpr char kKw_00742048[] = "controlptdelaykey";
constexpr char kKw_0074205c[] = "controlptdelay";
constexpr char kKw_0074206c[] = "controlptradiusbezierkey";
constexpr char kKw_00742088[] = "controlptradiuskey";
constexpr char kKw_0074209c[] = "controlptradius";
constexpr char kKw_007420ac[] = "numcontrolptsbezierkey";
constexpr char kKw_007420c4[] = "numcontrolptskey";
constexpr char kKw_007420d8[] = "numcontrolpts";
constexpr char kKw_007420e8[] = "targetsizebezierkey";
constexpr char kKw_007420fc[] = "targetsizekey";
constexpr char kKw_0074210c[] = "targetsize";
constexpr char kKw_00742118[] = "thresholdbezierkey";
constexpr char kKw_0074212c[] = "thresholdkey";
constexpr char kKw_0074213c[] = "threshold";
constexpr char kKw_00742148[] = "gravbezierkey";
constexpr char kKw_00742158[] = "gravkey";
constexpr char kKw_00742160[] = "grav";
constexpr char kKw_00742168[] = "dragbezierkey";
constexpr char kKw_00742178[] = "dragkey";
constexpr char kKw_00742180[] = "drag";
constexpr char kKw_00742188[] = "combinetimebezierkey";
constexpr char kKw_007421a0[] = "combinetimekey";
constexpr char kKw_007421b0[] = "combinetime";
constexpr char kKw_007421bc[] = "p2p_bezier3bezierkey";
constexpr char kKw_007421d4[] = "p2p_bezier3key";
constexpr char kKw_007421e4[] = "p2p_bezier3";
constexpr char kKw_007421f0[] = "p2p_bezier2bezierkey";
constexpr char kKw_00742208[] = "p2p_bezier2key";
constexpr char kKw_00742218[] = "p2p_bezier2";
constexpr char kKw_00742224[] = "lightningZigzagbezierkey";
constexpr char kKw_00742240[] = "lightningZigzagkey";
constexpr char kKw_00742254[] = "lightningZigzag";
constexpr char kKw_00742264[] = "lightningSubDivbezierkey";
constexpr char kKw_00742280[] = "lightningSubDivkey";
constexpr char kKw_00742294[] = "lightningSubDiv";
constexpr char kKw_007422a4[] = "lightningScalebezierkey";
constexpr char kKw_007422bc[] = "lightningScalekey";
constexpr char kKw_007422d0[] = "lightningScale";
constexpr char kKw_007422e0[] = "lightningRadiusbezierkey";
constexpr char kKw_007422fc[] = "lightningRadiuskey";
constexpr char kKw_00742310[] = "lightningRadius";
constexpr char kKw_00742320[] = "lightningDelaybezierkey";
constexpr char kKw_00742338[] = "lightningDelaykey";
constexpr char kKw_0074234c[] = "lightningDelay";
constexpr char kKw_0074235c[] = "bounce_cobezierkey";
constexpr char kKw_00742370[] = "bounce_cokey";
constexpr char kKw_00742380[] = "bounce_co";
constexpr char kKw_0074238c[] = "ysizebezierkey";
constexpr char kKw_0074239c[] = "ysizekey";
constexpr char kKw_007423a8[] = "ysize";
constexpr char kKw_007423b0[] = "xsizebezierkey";
constexpr char kKw_007423c0[] = "xsizekey";
constexpr char kKw_007423cc[] = "xsize";
constexpr char kKw_007423d4[] = "fpsbezierkey";
constexpr char kKw_007423e4[] = "fpskey";
constexpr char kKw_007423ec[] = "particleRotbezierkey";
constexpr char kKw_00742404[] = "particleRotkey";
constexpr char kKw_00742414[] = "particleRot";
constexpr char kKw_00742420[] = "spreadbezierkey";
constexpr char kKw_00742430[] = "spreadkey";
constexpr char kKw_0074243c[] = "spread";
constexpr char kKw_00742444[] = "massbezierkey";
constexpr char kKw_00742454[] = "masskey";
constexpr char kKw_0074245c[] = "mass";
constexpr char kKw_00742464[] = "lifeExpbezierkey";
constexpr char kKw_00742478[] = "lifeExpkey";
constexpr char kKw_00742484[] = "lifeExp";
constexpr char kKw_0074248c[] = "frameEndbezierkey";
constexpr char kKw_007424a0[] = "frameEndkey";
constexpr char kKw_007424ac[] = "frameEnd";
constexpr char kKw_007424b8[] = "frameStartbezierkey";
constexpr char kKw_007424cc[] = "frameStartkey";
constexpr char kKw_007424dc[] = "frameStart";
constexpr char kKw_007424e8[] = "sizeEnd_ybezierkey";
constexpr char kKw_007424fc[] = "sizeEnd_ykey";
constexpr char kKw_0074250c[] = "sizeEnd_y";
constexpr char kKw_00742518[] = "sizeMid_ybezierkey";
constexpr char kKw_0074252c[] = "sizeMid_ykey";
constexpr char kKw_0074253c[] = "sizeMid_y";
constexpr char kKw_00742548[] = "sizeStart_ybezierkey";
constexpr char kKw_00742560[] = "sizeStart_ykey";
constexpr char kKw_00742570[] = "sizeStart_y";
constexpr char kKw_0074257c[] = "sizeEndbezierkey";
constexpr char kKw_00742590[] = "sizeEndkey";
constexpr char kKw_0074259c[] = "sizeEnd";
constexpr char kKw_007425a4[] = "sizeMidbezierkey";
constexpr char kKw_007425b8[] = "sizeMidkey";
constexpr char kKw_007425c4[] = "sizeMid";
constexpr char kKw_007425cc[] = "sizeStartbezierkey";
constexpr char kKw_007425e0[] = "sizeStartkey";
constexpr char kKw_007425f0[] = "sizeStart";
constexpr char kKw_007425fc[] = "alphaEndbezierkey";
constexpr char kKw_00742610[] = "alphaEndkey";
constexpr char kKw_0074261c[] = "alphaEnd";
constexpr char kKw_00742628[] = "alphaMidbezierkey";
constexpr char kKw_0074263c[] = "alphaMidkey";
constexpr char kKw_00742648[] = "alphaMid";
constexpr char kKw_00742654[] = "alphaStartbezierkey";
constexpr char kKw_00742668[] = "alphaStartkey";
constexpr char kKw_00742678[] = "alphaStart";
constexpr char kKw_00742684[] = "percentEndbezierkey";
constexpr char kKw_00742698[] = "percentEndkey";
constexpr char kKw_007426a8[] = "percentEnd";
constexpr char kKw_007426b4[] = "percentMidbezierkey";
constexpr char kKw_007426c8[] = "percentMidkey";
constexpr char kKw_007426d8[] = "percentMid";
constexpr char kKw_007426e4[] = "percentStartbezierkey";
constexpr char kKw_007426fc[] = "percentStartkey";
constexpr char kKw_0074270c[] = "percentStart";
constexpr char kKw_0074271c[] = "colorEndbezierkey";
constexpr char kKw_00742730[] = "colorEndkey";
constexpr char kKw_0074273c[] = "colorEnd";
constexpr char kKw_00742748[] = "colorMidbezierkey";
constexpr char kKw_0074275c[] = "colorMidkey";
constexpr char kKw_00742768[] = "colorMid";
constexpr char kKw_00742774[] = "colorStartbezierkey";
constexpr char kKw_00742788[] = "colorStartkey";
constexpr char kKw_00742798[] = "colorStart";
constexpr char kKw_007427a4[] = "m_fRandomBirthRatebezierkey";
constexpr char kKw_007427c0[] = "m_fRandomBirthRatekey";
constexpr char kKw_007427d8[] = "m_fRandomBirthRate";
constexpr char kKw_007427ec[] = "birthratebezierkey";
constexpr char kKw_00742800[] = "birthratekey";
constexpr char kKw_00742810[] = "birthrate";
constexpr char kKw_0074281c[] = "randvelbezierkey";
constexpr char kKw_00742830[] = "randvelkey";
constexpr char kKw_0074283c[] = "randvel";
constexpr char kKw_00742844[] = "velocitybezierkey";
constexpr char kKw_00742858[] = "velocitykey";
constexpr char kKw_00742864[] = "velocity";
constexpr char kKw_00742870[] = "detonatebezierkey";
constexpr char kKw_00742884[] = "detonatekey";
constexpr char kKw_00742890[] = "detonate";
constexpr char kKw_0074289c[] = "blurlengthbezierkey";
constexpr char kKw_007428b0[] = "blurlengthkey";
constexpr char kKw_007428c0[] = "blurlength";
constexpr char kKw_007428cc[] = "controlptsmoothing";
constexpr char kKw_007428e0[] = "numBranches";
constexpr char kKw_007428ec[] = "m_sDepthTextureName";
constexpr char kKw_00742900[] = "m_bFrameBlending";
constexpr char kKw_00742914[] = "blastLength";
constexpr char kKw_00742920[] = "blastRadius";
constexpr char kKw_0074292c[] = "twosidedtex";
constexpr char kKw_00742938[] = "deadspace";
constexpr char kKw_00742944[] = "loop";
constexpr char kKw_0074294c[] = "ygrid";
constexpr char kKw_00742954[] = "xgrid";
constexpr char kKw_0074295c[] = "chunkName";
constexpr char kKw_00742968[] = "texture";
constexpr char kKw_00742970[] = "blend";
constexpr char kKw_00742978[] = "render";
constexpr char kKw_00742980[] = "update";
constexpr char kKw_00742988[] = "spawntype";
constexpr char kKw_00742994[] = "renderorder";
constexpr char kKw_007429a0[] = "depth_texture";
constexpr char kKw_007429b0[] = "inherit_part";
constexpr char kKw_007429c0[] = "splat";
constexpr char kKw_007429c8[] = "inherit_local";
constexpr char kKw_007429d8[] = "inheritvel";
constexpr char kKw_007429e4[] = "inherit";
constexpr char kKw_007429ec[] = "bounce";
constexpr char kKw_007429f4[] = "m_isTinted";
constexpr char kKw_00742a00[] = "affectedByWind";
constexpr char kKw_00742a10[] = "p2p_sel";
constexpr char kKw_00742a18[] = "p2p";
constexpr char kFmt_0073e2e4[] = "%f";   // .rdata 0073E2E4
constexpr char kFmt_0073d720[] = "%d";   // .rdata 0073D720

// Typed views of the observed node offsets handed to the readers (object-pointer conversions only).
volatile bool* node_bool(MdlNode* n, U off) { return reinterpret_cast<volatile bool*>(at(n, off)); }
volatile std::int32_t* node_int(MdlNode* n, U off) { return reinterpret_cast<volatile std::int32_t*>(at(n, off)); }
char* node_chars(MdlNode* n, U off) { return reinterpret_cast<char*>(at(n, off)); }
float* node_float(MdlNode* n, U off) { return reinterpret_cast<float*>(at(n, off)); }
List* list_at(MdlNode* n, U off) { return reinterpret_cast<List*>(at(n, off)); }
// parse_bool writes one byte: the byte local, or byte 0 of the 32-bit argument-slot local (x86 little-endian)
volatile bool* local_bool(volatile U8* b) { return reinterpret_cast<volatile bool*>(b); }
volatile bool* local_bool(volatile std::int32_t* slot) { return reinterpret_cast<volatile bool*>(slot); }

// 00465BAB..00465BC6: sscanf(line + 0Bh, "%d", &slot); mov dx,word ptr [esp+30h] (= the slot); mov word [edi+108h],dx
void sscanf_to_slot_then_store_word(MdlNode* self, char* line, volatile std::int32_t* slot) {
    int value = *slot;                                                       // slot contents before the call
    kotor_sscanf_006fae08(line + 0xB, kFmt_0073d720, &value);                // writes only on a successful conversion
    *slot = value;
    put<U16>(self, 0x108, static_cast<U16>(static_cast<U>(*slot) & 0xFFFFu));
}
} // namespace

extern "C" void __fastcall mdl_emitter_internal_parse_field_004658b0(MdlNode* self, void*, char* line) {
    if (mdl_parse_flag_007b927c == 0) {                                     // 004658B0 al=[007B927C]; 004658C0..004658C5 jne 00468044
    // 004658CC..004658F8: flag locals (ebx = 0); slot_arg = argument slot of `line` with byte 0 cleared.
    volatile std::int32_t slot_arg = static_cast<std::int32_t>(addr(line) & ~0xFFu);   // 004658CC mov byte [esp+28h],bl
    volatile std::int32_t v_m4 = 0;                                         // 004658D0 [esp+20h] p2p_sel
    volatile U8 b_m11 = 0;                                                  // 004658D4 [esp+13h] affectedByWind
    volatile U8 b_m10 = 0;                                                  // 004658D8 [esp+14h] m_isTinted
    volatile U8 b_m0f = 0;                                                  // 004658DC [esp+15h] bounce
    volatile std::int32_t v_m8 = 0;                                         // 004658E0 [esp+1Ch] random
    volatile U8 b_m0e = 0;                                                  // 004658E4 [esp+16h] inherit
    volatile U8 b_m0d = 0;                                                  // 004658E8 [esp+17h] inheritvel
    volatile U8 b_m0c = 0;                                                  // 004658EC [esp+18h] inherit_local
    volatile U8 b_m0b = 0;                                                  // 004658F0 [esp+19h] splat
    volatile U8 b_m0a = 0;                                                  // 004658F4 [esp+1Ah] inherit_part
    volatile U8 b_m09 = 0;                                                  // 004658F8 [esp+1Bh] depth_texture
    // ---- generated by tools/E_gen_emitter.py (one statement per original keyword test, original order)
    if (kotor_stricmp_0070acaf(kKw_00742a18, firstword_00463530(line)) == 0)   // 004658CB..00465921 "p2p"
        parse_bool_00463680(line + 0x3, local_bool(&slot_arg));
    if (kotor_stricmp_0070acaf(kKw_00742a10, firstword_00463530(line)) == 0)   // 00465924..0046594A "p2p_sel"
        parse_int_004635e0(line + 0x7, &v_m4);
    if (kotor_stricmp_0070acaf(kKw_00742a00, firstword_00463530(line)) == 0)   // 0046594D..00465973 "affectedByWind"
        parse_bool_00463680(line + 0xE, local_bool(&b_m11));
    if (kotor_stricmp_0070acaf(kKw_007429f4, firstword_00463530(line)) == 0)   // 00465976..0046599C "m_isTinted"
        parse_bool_00463680(line + 0xA, local_bool(&b_m10));
    if (kotor_stricmp_0070acaf(kKw_007429ec, firstword_00463530(line)) == 0)   // 0046599F..004659C5 "bounce"
        parse_bool_00463680(line + 0x6, local_bool(&b_m0f));
    if (kotor_stricmp_0070acaf(kKw_0073f054, firstword_00463530(line)) == 0)   // 004659C8..004659EE "random"
        parse_int_004635e0(line + 0x6, &v_m8);
    if (kotor_stricmp_0070acaf(kKw_007429e4, firstword_00463530(line)) == 0)   // 004659F1..00465A17 "inherit"
        parse_bool_00463680(line + 0x7, local_bool(&b_m0e));
    if (kotor_stricmp_0070acaf(kKw_007429d8, firstword_00463530(line)) == 0)   // 00465A1A..00465A40 "inheritvel"
        parse_bool_00463680(line + 0xA, local_bool(&b_m0d));
    if (kotor_stricmp_0070acaf(kKw_007429c8, firstword_00463530(line)) == 0)   // 00465A43..00465A69 "inherit_local"
        parse_bool_00463680(line + 0xD, local_bool(&b_m0c));
    if (kotor_stricmp_0070acaf(kKw_007429c0, firstword_00463530(line)) == 0)   // 00465A6C..00465A92 "splat"
        parse_bool_00463680(line + 0x5, local_bool(&b_m0b));
    if (kotor_stricmp_0070acaf(kKw_007429b0, firstword_00463530(line)) == 0)   // 00465A95..00465ABB "inherit_part"
        parse_bool_00463680(line + 0xC, local_bool(&b_m0a));
    if (kotor_stricmp_0070acaf(kKw_007429a0, firstword_00463530(line)) == 0)   // 00465ABE..00465AE4 "depth_texture"
        parse_bool_00463680(line + 0xD, local_bool(&b_m09));
    // 00465AE7..00465B8E: apply the flags parsed from this line (tests on the locals)
    if (static_cast<U8>(slot_arg) != 0) mdl_emitter_enable_point_to_point_0048d790(self, nullptr);   // 00465AE7..00465AEF
    if (v_m4 == 1) mdl_emitter_enable_point_to_point_sel_0048d7a0(self, nullptr);   // 00465AF4..00465AFD
    if (b_m11 != 0) mdl_emitter_enable_affected_by_wind_0048d7b0(self, nullptr);   // 00465B02..00465B0A
    if (b_m10 != 0) mdl_emitter_enable_tinted_0048d7c0(self, nullptr);   // 00465B0F..00465B17
    if (b_m0f != 0) mdl_emitter_enable_bouncing_0048d7d0(self, nullptr);   // 00465B1C..00465B24
    if ((static_cast<U8>(v_m8) & 1u) != 0) mdl_emitter_enable_random_0048d7e0(self, nullptr);   // 00465B29..00465B32
    if ((static_cast<U8>(v_m8) & 2u) != 0) mdl_emitter_enable_random_order_0048d7f0(self, nullptr);   // 00465B37..00465B40
    if (b_m0e != 0) mdl_emitter_enable_inheriting_0048d800(self, nullptr);   // 00465B45..00465B4D
    if (b_m0d != 0) mdl_emitter_enable_inheriting_velocity_0048d810(self, nullptr);   // 00465B52..00465B5A
    if (b_m0c != 0) mdl_emitter_enable_inheriting_local_0048d820(self, nullptr);   // 00465B5F..00465B67
    if (b_m0b != 0) mdl_emitter_enable_splat_0048d830(self, nullptr);   // 00465B6C..00465B74
    if (b_m0a != 0) mdl_emitter_enable_inheriting_part_0048d840(self, nullptr);   // 00465B79..00465B81
    if (b_m09 != 0) mdl_emitter_enable_depth_texture_0048d850(self, nullptr);   // 00465B86..00465B8E
    if (kotor_stricmp_0070acaf(kKw_00742994, firstword_00463530(line)) == 0)   // 00465B93..00465BC6 "renderorder"
        sscanf_to_slot_then_store_word(self, line, &slot_arg);
    if (kotor_stricmp_0070acaf(kKw_00742988, firstword_00463530(line)) == 0)   // 00465BCD..00465BF2 "spawntype"
        parse_int_004635e0(line + 0x9, node_int(self, 0x6C));
    if (kotor_stricmp_0070acaf(kKw_00742980, firstword_00463530(line)) == 0)   // 00465BF5..00465C1A "update"
        parse_cstr_00463590(line + 0x6, node_chars(self, 0x70));
    if (kotor_stricmp_0070acaf(kKw_00742978, firstword_00463530(line)) == 0)   // 00465C1D..00465C45 "render"
        parse_cstr_00463590(line + 0x6, node_chars(self, 0x90));
    if (kotor_stricmp_0070acaf(kKw_00742970, firstword_00463530(line)) == 0)   // 00465C48..00465C70 "blend"
        parse_cstr_00463590(line + 0x5, node_chars(self, 0xB0));
    if (kotor_stricmp_0070acaf(kKw_00742968, firstword_00463530(line)) == 0)   // 00465C73..00465C9B "texture"
        parse_cstr_00463590(line + 0x7, node_chars(self, 0xD0));
    if (kotor_stricmp_0070acaf(kKw_0074295c, firstword_00463530(line)) == 0)   // 00465C9E..00465CC6 "chunkName"
        parse_cstr_00463590(line + 0x9, node_chars(self, 0xF0));
    if (kotor_stricmp_0070acaf(kKw_00742954, firstword_00463530(line)) == 0)   // 00465CC9..00465CEE "xgrid"
        parse_int_004635e0(line + 0x5, node_int(self, 0x64));
    if (kotor_stricmp_0070acaf(kKw_0074294c, firstword_00463530(line)) == 0)   // 00465CF1..00465D16 "ygrid"
        parse_int_004635e0(line + 0x5, node_int(self, 0x68));
    if (kotor_stricmp_0070acaf(kKw_00742944, firstword_00463530(line)) == 0)   // 00465D19..00465D41 "loop"
        parse_int_004635e0(line + 0x4, node_int(self, 0x104));
    if (kotor_stricmp_0070acaf(kKw_00742938, firstword_00463530(line)) == 0)   // 00465D44..00465D6E "deadspace"
        kotor_sscanf_006fae08(line + 0x9, kFmt_0073e2e4, node_float(self, 0x50));
    if (kotor_stricmp_0070acaf(kKw_0074292c, firstword_00463530(line)) == 0)   // 00465D71..00465D99 "twosidedtex"
        parse_int_004635e0(line + 0xB, node_int(self, 0x100));
    if (kotor_stricmp_0070acaf(kKw_00742920, firstword_00463530(line)) == 0)   // 00465D9C..00465DC6 "blastRadius"
        kotor_sscanf_006fae08(line + 0xB, kFmt_0073e2e4, node_float(self, 0x54));
    if (kotor_stricmp_0070acaf(kKw_00742914, firstword_00463530(line)) == 0)   // 00465DC9..00465DF3 "blastLength"
        kotor_sscanf_006fae08(line + 0xB, kFmt_0073e2e4, node_float(self, 0x58));
    if (kotor_stricmp_0070acaf(kKw_00742900, firstword_00463530(line)) == 0)   // 00465DF6..00465E1E "m_bFrameBlending"
        parse_bool_00463680(line + 0x10, node_bool(self, 0x10A));
    if (kotor_stricmp_0070acaf(kKw_007428ec, firstword_00463530(line)) == 0)   // 00465E21..00465E49 "m_sDepthTextureName"
        parse_cstr_00463590(line + 0x13, node_chars(self, 0x10B));
    if (kotor_stricmp_0070acaf(kKw_007428e0, firstword_00463530(line)) == 0)   // 00465E4C..00465E71 "numBranches"
        parse_int_004635e0(line + 0xB, node_int(self, 0x5C));
    if (kotor_stricmp_0070acaf(kKw_007428cc, firstword_00463530(line)) == 0)   // 00465E74..00465E99 "controlptsmoothing"
        parse_int_004635e0(line + 0x12, node_int(self, 0x60));
    // controller group 00465E9C..00465F4E
    if (kotor_stricmp_0070acaf(kKw_007428c0, firstword_00463530(line)) == 0) {   // 00465E9C "blurlength" (call 00465ED1)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0xB4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007428b0, firstword_00463530(line)) == 0) {   // 00465ED8 "blurlengthkey" (call 00465F0D)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0xB4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074289c, firstword_00463530(line)) == 0) {   // 00465F14 "blurlengthbezierkey" (call 00465F49)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0xB4, -1);
    }
    // controller group 00465F51..00466003
    if (kotor_stricmp_0070acaf(kKw_00742890, firstword_00463530(line)) == 0) {   // 00465F51 "detonate" (call 00465F86)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0x1F6, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742884, firstword_00463530(line)) == 0) {   // 00465F8D "detonatekey" (call 00465FC2)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x1F6, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742870, firstword_00463530(line)) == 0) {   // 00465FC9 "detonatebezierkey" (call 00465FFE)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x1F6, -1);
    }
    // controller group 00466006..004660B8
    if (kotor_stricmp_0070acaf(kKw_00742864, firstword_00463530(line)) == 0) {   // 00466006 "velocity" (call 0046603B)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0xA8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742858, firstword_00463530(line)) == 0) {   // 00466042 "velocitykey" (call 00466077)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0xA8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742844, firstword_00463530(line)) == 0) {   // 0046607E "velocitybezierkey" (call 004660B3)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0xA8, -1);
    }
    // controller group 004660BB..0046616D
    if (kotor_stricmp_0070acaf(kKw_0074283c, firstword_00463530(line)) == 0) {   // 004660BB "randvel" (call 004660F0)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x8C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742830, firstword_00463530(line)) == 0) {   // 004660F7 "randvelkey" (call 0046612C)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x8C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074281c, firstword_00463530(line)) == 0) {   // 00466133 "randvelbezierkey" (call 00466168)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x8C, -1);
    }
    // controller group 00466170..00466219
    if (kotor_stricmp_0070acaf(kKw_00742810, firstword_00463530(line)) == 0) {   // 00466170 "birthrate" (call 004661A2)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0x58, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742800, firstword_00463530(line)) == 0) {   // 004661A9 "birthratekey" (call 004661DB)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0x58, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007427ec, firstword_00463530(line)) == 0) {   // 004661E2 "birthratebezierkey" (call 00466214)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0x58, -1);
    }
    // controller group 0046621C..004662CE
    if (kotor_stricmp_0070acaf(kKw_007427d8, firstword_00463530(line)) == 0) {   // 0046621C "m_fRandomBirthRate" (call 00466251)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xF0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007427c0, firstword_00463530(line)) == 0) {   // 00466258 "m_fRandomBirthRatekey" (call 0046628D)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x15, list_at(self, 0x38), list_at(self, 0x44), 0xF0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007427a4, firstword_00463530(line)) == 0) {   // 00466294 "m_fRandomBirthRatebezierkey" (call 004662C9)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x1B, list_at(self, 0x38), list_at(self, 0x44), 0xF0, -1);
    }
    // controller group 004662D1..00466383
    if (kotor_stricmp_0070acaf(kKw_00742798, firstword_00463530(line)) == 0) {   // 004662D1 "colorStart" (call 00466306)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x188, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742788, firstword_00463530(line)) == 0) {   // 0046630D "colorStartkey" (call 00466342)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x188, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742774, firstword_00463530(line)) == 0) {   // 00466349 "colorStartbezierkey" (call 0046637E)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0x188, -1);
    }
    // controller group 00466386..00466438
    if (kotor_stricmp_0070acaf(kKw_00742768, firstword_00463530(line)) == 0) {   // 00466386 "colorMid" (call 004663BB)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0x11C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074275c, firstword_00463530(line)) == 0) {   // 004663C2 "colorMidkey" (call 004663F7)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x11C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742748, firstword_00463530(line)) == 0) {   // 004663FE "colorMidbezierkey" (call 00466433)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x11C, -1);
    }
    // controller group 0046643B..004664ED
    if (kotor_stricmp_0070acaf(kKw_0074273c, firstword_00463530(line)) == 0) {   // 0046643B "colorEnd" (call 00466470)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0x17C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742730, firstword_00463530(line)) == 0) {   // 00466477 "colorEndkey" (call 004664AC)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x17C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074271c, firstword_00463530(line)) == 0) {   // 004664B3 "colorEndbezierkey" (call 004664E8)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x17C, -1);
    }
    // controller group 004664F0..004665A2
    if (kotor_stricmp_0070acaf(kKw_0074270c, firstword_00463530(line)) == 0) {   // 004664F0 "percentStart" (call 00466525)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0xDC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007426fc, firstword_00463530(line)) == 0) {   // 0046652C "percentStartkey" (call 00466561)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0xDC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007426e4, firstword_00463530(line)) == 0) {   // 00466568 "percentStartbezierkey" (call 0046659D)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x15, list_at(self, 0x38), list_at(self, 0x44), 0xDC, -1);
    }
    // controller group 004665A5..00466657
    if (kotor_stricmp_0070acaf(kKw_007426d8, firstword_00463530(line)) == 0) {   // 004665A5 "percentMid" (call 004665DA)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0xE0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007426c8, firstword_00463530(line)) == 0) {   // 004665E1 "percentMidkey" (call 00466616)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0xE0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007426b4, firstword_00463530(line)) == 0) {   // 0046661D "percentMidbezierkey" (call 00466652)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0xE0, -1);
    }
    // controller group 0046665A..0046670C
    if (kotor_stricmp_0070acaf(kKw_007426a8, firstword_00463530(line)) == 0) {   // 0046665A "percentEnd" (call 0046668F)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0xE4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742698, firstword_00463530(line)) == 0) {   // 00466696 "percentEndkey" (call 004666CB)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0xE4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742684, firstword_00463530(line)) == 0) {   // 004666D2 "percentEndbezierkey" (call 00466707)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0xE4, -1);
    }
    // controller group 0046670F..004667B8
    if (kotor_stricmp_0070acaf(kKw_00742678, firstword_00463530(line)) == 0) {   // 0046670F "alphaStart" (call 00466741)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x54, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742668, firstword_00463530(line)) == 0) {   // 00466748 "alphaStartkey" (call 0046677A)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x54, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742654, firstword_00463530(line)) == 0) {   // 00466781 "alphaStartbezierkey" (call 004667B3)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0x54, -1);
    }
    // controller group 004667BB..0046686D
    if (kotor_stricmp_0070acaf(kKw_00742648, firstword_00463530(line)) == 0) {   // 004667BB "alphaMid" (call 004667F0)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0xD8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074263c, firstword_00463530(line)) == 0) {   // 004667F7 "alphaMidkey" (call 0046682C)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0xD8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742628, firstword_00463530(line)) == 0) {   // 00466833 "alphaMidbezierkey" (call 00466868)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0xD8, -1);
    }
    // controller group 00466870..00466919
    if (kotor_stricmp_0070acaf(kKw_0074261c, firstword_00463530(line)) == 0) {   // 00466870 "alphaEnd" (call 004668A2)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0x50, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742610, firstword_00463530(line)) == 0) {   // 004668A9 "alphaEndkey" (call 004668DB)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x50, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007425fc, firstword_00463530(line)) == 0) {   // 004668E2 "alphaEndbezierkey" (call 00466914)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x50, -1);
    }
    // controller group 0046691C..004669CE
    if (kotor_stricmp_0070acaf(kKw_007425f0, firstword_00463530(line)) == 0) {   // 0046691C "sizeStart" (call 00466951)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0x90, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007425e0, firstword_00463530(line)) == 0) {   // 00466958 "sizeStartkey" (call 0046698D)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0x90, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007425cc, firstword_00463530(line)) == 0) {   // 00466994 "sizeStartbezierkey" (call 004669C9)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0x90, -1);
    }
    // controller group 004669D1..00466A83
    if (kotor_stricmp_0070acaf(kKw_007425c4, firstword_00463530(line)) == 0) {   // 004669D1 "sizeMid" (call 00466A06)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0xE8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007425b8, firstword_00463530(line)) == 0) {   // 00466A0D "sizeMidkey" (call 00466A42)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0xE8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007425a4, firstword_00463530(line)) == 0) {   // 00466A49 "sizeMidbezierkey" (call 00466A7E)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0xE8, -1);
    }
    // controller group 00466A86..00466B38
    if (kotor_stricmp_0070acaf(kKw_0074259c, firstword_00463530(line)) == 0) {   // 00466A86 "sizeEnd" (call 00466ABB)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x94, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742590, firstword_00463530(line)) == 0) {   // 00466AC2 "sizeEndkey" (call 00466AF7)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x94, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074257c, firstword_00463530(line)) == 0) {   // 00466AFE "sizeEndbezierkey" (call 00466B33)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x94, -1);
    }
    // controller group 00466B3B..00466BED
    if (kotor_stricmp_0070acaf(kKw_00742570, firstword_00463530(line)) == 0) {   // 00466B3B "sizeStart_y" (call 00466B70)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x98, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742560, firstword_00463530(line)) == 0) {   // 00466B77 "sizeStart_ykey" (call 00466BAC)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x98, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742548, firstword_00463530(line)) == 0) {   // 00466BB3 "sizeStart_ybezierkey" (call 00466BE8)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x14, list_at(self, 0x38), list_at(self, 0x44), 0x98, -1);
    }
    // controller group 00466BF0..00466CA2
    if (kotor_stricmp_0070acaf(kKw_0074253c, firstword_00463530(line)) == 0) {   // 00466BF0 "sizeMid_y" (call 00466C25)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0xEC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074252c, firstword_00463530(line)) == 0) {   // 00466C2C "sizeMid_ykey" (call 00466C61)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0xEC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742518, firstword_00463530(line)) == 0) {   // 00466C68 "sizeMid_ybezierkey" (call 00466C9D)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xEC, -1);
    }
    // controller group 00466CA5..00466D57
    if (kotor_stricmp_0070acaf(kKw_0074250c, firstword_00463530(line)) == 0) {   // 00466CA5 "sizeEnd_y" (call 00466CDA)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0x9C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007424fc, firstword_00463530(line)) == 0) {   // 00466CE1 "sizeEnd_ykey" (call 00466D16)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0x9C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007424e8, firstword_00463530(line)) == 0) {   // 00466D1D "sizeEnd_ybezierkey" (call 00466D52)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0x9C, -1);
    }
    // controller group 00466D5A..00466E03
    if (kotor_stricmp_0070acaf(kKw_007424dc, firstword_00463530(line)) == 0) {   // 00466D5A "frameStart" (call 00466D8C)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x70, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007424cc, firstword_00463530(line)) == 0) {   // 00466D93 "frameStartkey" (call 00466DC5)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x70, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007424b8, firstword_00463530(line)) == 0) {   // 00466DCC "frameStartbezierkey" (call 00466DFE)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0x70, -1);
    }
    // controller group 00466E06..00466EAF
    if (kotor_stricmp_0070acaf(kKw_007424ac, firstword_00463530(line)) == 0) {   // 00466E06 "frameEnd" (call 00466E38)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0x6C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007424a0, firstword_00463530(line)) == 0) {   // 00466E3F "frameEndkey" (call 00466E71)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x6C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074248c, firstword_00463530(line)) == 0) {   // 00466E78 "frameEndbezierkey" (call 00466EAA)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x6C, -1);
    }
    // controller group 00466EB2..00466F5B
    if (kotor_stricmp_0070acaf(kKw_00742484, firstword_00463530(line)) == 0) {   // 00466EB2 "lifeExp" (call 00466EE4)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x78, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742478, firstword_00463530(line)) == 0) {   // 00466EEB "lifeExpkey" (call 00466F1D)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0x78, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742464, firstword_00463530(line)) == 0) {   // 00466F24 "lifeExpbezierkey" (call 00466F56)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x78, -1);
    }
    // controller group 00466F5E..00467007
    if (kotor_stricmp_0070acaf(kKw_0074245c, firstword_00463530(line)) == 0) {   // 00466F5E "mass" (call 00466F90)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x4, list_at(self, 0x38), list_at(self, 0x44), 0x7C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742454, firstword_00463530(line)) == 0) {   // 00466F97 "masskey" (call 00466FC9)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x7C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742444, firstword_00463530(line)) == 0) {   // 00466FD0 "massbezierkey" (call 00467002)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x7C, -1);
    }
    // controller group 0046700A..004670BC
    if (kotor_stricmp_0070acaf(kKw_0074243c, firstword_00463530(line)) == 0) {   // 0046700A "spread" (call 0046703F)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x6, list_at(self, 0x38), list_at(self, 0x44), 0xA0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742430, firstword_00463530(line)) == 0) {   // 00467046 "spreadkey" (call 0046707B)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0xA0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742420, firstword_00463530(line)) == 0) {   // 00467082 "spreadbezierkey" (call 004670B7)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0xA0, -1);
    }
    // controller group 004670BF..00467171
    if (kotor_stricmp_0070acaf(kKw_00742414, firstword_00463530(line)) == 0) {   // 004670BF "particleRot" (call 004670F4)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x88, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742404, firstword_00463530(line)) == 0) {   // 004670FB "particleRotkey" (call 00467130)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x88, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007423ec, firstword_00463530(line)) == 0) {   // 00467137 "particleRotbezierkey" (call 0046716C)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x14, list_at(self, 0x38), list_at(self, 0x44), 0x88, -1);
    }
    // controller group 00467174..0046721D
    if (kotor_stricmp_0070acaf(kKw_00741df4, firstword_00463530(line)) == 0) {   // 00467174 "fps" (call 004671A6)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x3, list_at(self, 0x38), list_at(self, 0x44), 0x68, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007423e4, firstword_00463530(line)) == 0) {   // 004671AD "fpskey" (call 004671DF)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x6, list_at(self, 0x38), list_at(self, 0x44), 0x68, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007423d4, firstword_00463530(line)) == 0) {   // 004671E6 "fpsbezierkey" (call 00467218)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0x68, -1);
    }
    // controller group 00467220..004672D2
    if (kotor_stricmp_0070acaf(kKw_007423cc, firstword_00463530(line)) == 0) {   // 00467220 "xsize" (call 00467255)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x5, list_at(self, 0x38), list_at(self, 0x44), 0xAC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007423c0, firstword_00463530(line)) == 0) {   // 0046725C "xsizekey" (call 00467291)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0xAC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007423b0, firstword_00463530(line)) == 0) {   // 00467298 "xsizebezierkey" (call 004672CD)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0xAC, -1);
    }
    // controller group 004672D5..00467387
    if (kotor_stricmp_0070acaf(kKw_007423a8, firstword_00463530(line)) == 0) {   // 004672D5 "ysize" (call 0046730A)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x5, list_at(self, 0x38), list_at(self, 0x44), 0xB0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074239c, firstword_00463530(line)) == 0) {   // 00467311 "ysizekey" (call 00467346)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x8, list_at(self, 0x38), list_at(self, 0x44), 0xB0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074238c, firstword_00463530(line)) == 0) {   // 0046734D "ysizebezierkey" (call 00467382)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0xB0, -1);
    }
    // controller group 0046738A..00467433
    if (kotor_stricmp_0070acaf(kKw_00742380, firstword_00463530(line)) == 0) {   // 0046738A "bounce_co" (call 004673BC)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0x5C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742370, firstword_00463530(line)) == 0) {   // 004673C3 "bounce_cokey" (call 004673F5)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0x5C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074235c, firstword_00463530(line)) == 0) {   // 004673FC "bounce_cobezierkey" (call 0046742E)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0x5C, -1);
    }
    // controller group 00467436..004674E8
    if (kotor_stricmp_0070acaf(kKw_0074234c, firstword_00463530(line)) == 0) {   // 00467436 "lightningDelay" (call 0046746B)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0xB8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742338, firstword_00463530(line)) == 0) {   // 00467472 "lightningDelaykey" (call 004674A7)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0xB8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742320, firstword_00463530(line)) == 0) {   // 004674AE "lightningDelaybezierkey" (call 004674E3)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x17, list_at(self, 0x38), list_at(self, 0x44), 0xB8, -1);
    }
    // controller group 004674EB..0046759D
    if (kotor_stricmp_0070acaf(kKw_00742310, firstword_00463530(line)) == 0) {   // 004674EB "lightningRadius" (call 00467520)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0xBC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007422fc, firstword_00463530(line)) == 0) {   // 00467527 "lightningRadiuskey" (call 0046755C)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xBC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007422e0, firstword_00463530(line)) == 0) {   // 00467563 "lightningRadiusbezierkey" (call 00467598)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x18, list_at(self, 0x38), list_at(self, 0x44), 0xBC, -1);
    }
    // controller group 004675A0..00467652
    if (kotor_stricmp_0070acaf(kKw_007422d0, firstword_00463530(line)) == 0) {   // 004675A0 "lightningScale" (call 004675D5)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0xC0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007422bc, firstword_00463530(line)) == 0) {   // 004675DC "lightningScalekey" (call 00467611)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0xC0, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007422a4, firstword_00463530(line)) == 0) {   // 00467618 "lightningScalebezierkey" (call 0046764D)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x17, list_at(self, 0x38), list_at(self, 0x44), 0xC0, -1);
    }
    // controller group 00467655..00467707
    if (kotor_stricmp_0070acaf(kKw_00742294, firstword_00463530(line)) == 0) {   // 00467655 "lightningSubDiv" (call 0046768A)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0xC4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742280, firstword_00463530(line)) == 0) {   // 00467691 "lightningSubDivkey" (call 004676C6)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xC4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742264, firstword_00463530(line)) == 0) {   // 004676CD "lightningSubDivbezierkey" (call 00467702)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x18, list_at(self, 0x38), list_at(self, 0x44), 0xC4, -1);
    }
    // controller group 0046770A..004677BC
    if (kotor_stricmp_0070acaf(kKw_00742254, firstword_00463530(line)) == 0) {   // 0046770A "lightningZigzag" (call 0046773F)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0xC8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742240, firstword_00463530(line)) == 0) {   // 00467746 "lightningZigzagkey" (call 0046777B)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xC8, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742224, firstword_00463530(line)) == 0) {   // 00467782 "lightningZigzagbezierkey" (call 004677B7)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x18, list_at(self, 0x38), list_at(self, 0x44), 0xC8, -1);
    }
    // controller group 004677BF..00467871
    if (kotor_stricmp_0070acaf(kKw_00742218, firstword_00463530(line)) == 0) {   // 004677BF "p2p_bezier2" (call 004677F4)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x80, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742208, firstword_00463530(line)) == 0) {   // 004677FB "p2p_bezier2key" (call 00467830)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x80, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007421f0, firstword_00463530(line)) == 0) {   // 00467837 "p2p_bezier2bezierkey" (call 0046786C)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x14, list_at(self, 0x38), list_at(self, 0x44), 0x80, -1);
    }
    // controller group 00467874..00467926
    if (kotor_stricmp_0070acaf(kKw_007421e4, firstword_00463530(line)) == 0) {   // 00467874 "p2p_bezier3" (call 004678A9)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x84, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007421d4, firstword_00463530(line)) == 0) {   // 004678B0 "p2p_bezier3key" (call 004678E5)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x84, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007421bc, firstword_00463530(line)) == 0) {   // 004678EC "p2p_bezier3bezierkey" (call 00467921)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x14, list_at(self, 0x38), list_at(self, 0x44), 0x84, -1);
    }
    // controller group 00467929..004679D2
    if (kotor_stricmp_0070acaf(kKw_007421b0, firstword_00463530(line)) == 0) {   // 00467929 "combinetime" (call 0046795B)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xB, list_at(self, 0x38), list_at(self, 0x44), 0x60, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007421a0, firstword_00463530(line)) == 0) {   // 00467962 "combinetimekey" (call 00467994)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x60, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742188, firstword_00463530(line)) == 0) {   // 0046799B "combinetimebezierkey" (call 004679CD)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x14, list_at(self, 0x38), list_at(self, 0x44), 0x60, -1);
    }
    // controller group 004679D5..00467A7E
    if (kotor_stricmp_0070acaf(kKw_00742180, firstword_00463530(line)) == 0) {   // 004679D5 "drag" (call 00467A07)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x4, list_at(self, 0x38), list_at(self, 0x44), 0x64, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742178, firstword_00463530(line)) == 0) {   // 00467A0E "dragkey" (call 00467A40)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x64, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742168, firstword_00463530(line)) == 0) {   // 00467A47 "dragbezierkey" (call 00467A79)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x64, -1);
    }
    // controller group 00467A81..00467B2A
    if (kotor_stricmp_0070acaf(kKw_00742160, firstword_00463530(line)) == 0) {   // 00467A81 "grav" (call 00467AB3)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x4, list_at(self, 0x38), list_at(self, 0x44), 0x74, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742158, firstword_00463530(line)) == 0) {   // 00467ABA "gravkey" (call 00467AEC)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x7, list_at(self, 0x38), list_at(self, 0x44), 0x74, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742148, firstword_00463530(line)) == 0) {   // 00467AF3 "gravbezierkey" (call 00467B25)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x74, -1);
    }
    // controller group 00467B2D..00467BDF
    if (kotor_stricmp_0070acaf(kKw_0074213c, firstword_00463530(line)) == 0) {   // 00467B2D "threshold" (call 00467B62)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0x9, list_at(self, 0x38), list_at(self, 0x44), 0xA4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074212c, firstword_00463530(line)) == 0) {   // 00467B69 "thresholdkey" (call 00467B9E)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xC, list_at(self, 0x38), list_at(self, 0x44), 0xA4, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742118, firstword_00463530(line)) == 0) {   // 00467BA5 "thresholdbezierkey" (call 00467BDA)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0xA4, -1);
    }
    // controller group 00467BE2..00467C94
    if (kotor_stricmp_0070acaf(kKw_0074210c, firstword_00463530(line)) == 0) {   // 00467BE2 "targetsize" (call 00467C17)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xA, list_at(self, 0x38), list_at(self, 0x44), 0xFC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007420fc, firstword_00463530(line)) == 0) {   // 00467C1E "targetsizekey" (call 00467C53)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0xFC, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007420e8, firstword_00463530(line)) == 0) {   // 00467C5A "targetsizebezierkey" (call 00467C8F)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x13, list_at(self, 0x38), list_at(self, 0x44), 0xFC, -1);
    }
    // controller group 00467C97..00467D49
    if (kotor_stricmp_0070acaf(kKw_007420d8, firstword_00463530(line)) == 0) {   // 00467C97 "numcontrolpts" (call 00467CCC)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x100, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007420c4, firstword_00463530(line)) == 0) {   // 00467CD3 "numcontrolptskey" (call 00467D08)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x100, -1);
    } else if (kotor_stricmp_0070acaf(kKw_007420ac, firstword_00463530(line)) == 0) {   // 00467D0F "numcontrolptsbezierkey" (call 00467D44)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x16, list_at(self, 0x38), list_at(self, 0x44), 0x100, -1);
    }
    // controller group 00467D4C..00467DFE
    if (kotor_stricmp_0070acaf(kKw_0074209c, firstword_00463530(line)) == 0) {   // 00467D4C "controlptradius" (call 00467D81)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xF, list_at(self, 0x38), list_at(self, 0x44), 0x104, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742088, firstword_00463530(line)) == 0) {   // 00467D88 "controlptradiuskey" (call 00467DBD)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x12, list_at(self, 0x38), list_at(self, 0x44), 0x104, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074206c, firstword_00463530(line)) == 0) {   // 00467DC4 "controlptradiusbezierkey" (call 00467DF9)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x18, list_at(self, 0x38), list_at(self, 0x44), 0x104, -1);
    }
    // controller group 00467E01..00467EB3
    if (kotor_stricmp_0070acaf(kKw_0074205c, firstword_00463530(line)) == 0) {   // 00467E01 "controlptdelay" (call 00467E36)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xE, list_at(self, 0x38), list_at(self, 0x44), 0x108, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742048, firstword_00463530(line)) == 0) {   // 00467E3D "controlptdelaykey" (call 00467E72)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x11, list_at(self, 0x38), list_at(self, 0x44), 0x108, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00742030, firstword_00463530(line)) == 0) {   // 00467E79 "controlptdelaybezierkey" (call 00467EAE)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x17, list_at(self, 0x38), list_at(self, 0x44), 0x108, -1);
    }
    // controller group 00467EB6..00467F68
    if (kotor_stricmp_0070acaf(kKw_00742020, firstword_00463530(line)) == 0) {   // 00467EB6 "tangentspread" (call 00467EEB)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x10C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_0074200c, firstword_00463530(line)) == 0) {   // 00467EF2 "tangentspreadkey" (call 00467F27)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x10C, -1);
    } else if (kotor_stricmp_0070acaf(kKw_00741ff4, firstword_00463530(line)) == 0) {   // 00467F2E "tangentspreadbezierkey" (call 00467F63)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x16, list_at(self, 0x38), list_at(self, 0x44), 0x10C, -1);
    }
    // controller group 00467F6B..00467FA5
    if (kotor_stricmp_0070acaf(kKw_00741fe4, firstword_00463530(line)) == 0) {   // 00467F6B "tangentlength" (call 00467FA0)
        mdl_add_generic_controller_single_00464e40(firstword_00463530(line), line + 0xD, list_at(self, 0x38), list_at(self, 0x44), 0x110, -1);
        mdl_node_internal_parse_field_00465560(self, nullptr, line);   // ..00467FB6: base parser, then return
        return;
    }
    // controller group 00467FB9..00467FF3
    if (kotor_stricmp_0070acaf(kKw_00741fd0, firstword_00463530(line)) == 0) {   // 00467FB9 "tangentlengthkey" (call 00467FEE)
        mdl_add_generic_controller_00465090(firstword_00463530(line), line + 0x10, list_at(self, 0x38), list_at(self, 0x44), 0x110, -1);
        mdl_node_internal_parse_field_00465560(self, nullptr, line);   // ..00468004: base parser, then return
        return;
    }
    // controller group 00468007..00468041
    if (kotor_stricmp_0070acaf(kKw_00741fb8, firstword_00463530(line)) == 0) {   // 00468007 "tangentlengthbezierkey" (call 0046803C)
        mdl_add_bezier_controller_00465320(firstword_00463530(line), line + 0x16, list_at(self, 0x38), list_at(self, 0x44), 0x110, -1);
    }
    }
    mdl_node_internal_parse_field_00465560(self, nullptr, line);            // 00468044..00468052 push esi; mov ecx,edi; call 00465560; ret 4
}
