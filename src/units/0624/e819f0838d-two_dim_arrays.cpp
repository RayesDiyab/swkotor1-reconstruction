// lane-rules: CTwoDimArrays::Load2DArrays 005C4C20, its 74 table loaders (75 calls; 005C3830
// is called twice), LoadIPRPCostTables 005C4730 / LoadIPRPParamTables 005C49C0, and the
// shutdown pair UnLoad2DArrays 005C5070 (+005C4940/005C4B90) / ~CTwoDimArrays 005BFCC0.
// swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000. D1 form, C++ linkage (EH frames).
// The per-loader bodies below were emitted by tools/gen_two_dim_arrays.py from dumpbin
// evidence (work/colcache.json: every loader verified against one of three shapes; 52 are
// byte-identical modulo name/offset, see work/subl.py); the citations are per instruction.
// 2DA names / column names are original .rdata strings (D13 generated, never committed);
// column-index cache cells 007A20C0..007A2270 are original .data cells owned by this lane.
#include "45d2335566-rules_common.h"
#include "55aae85039-two_dim_arrays.h"

namespace kotor_rules {
namespace {
// Shape A (e.g. 005C1B40..005C1BDA): table = new C2DA(CResRef(name), 0) [EH state 0 frees
// the 54h block if 00413CC0 unwinds]; field = table (stored even when the allocation
// failed: xor eax,eax / mov [edi+off],eax); null -> 0; Load2DArray 004143B0 == 0 -> 0
// (the loaded-but-failed table stays in the field); else 1.
int load_table(void* self, unsigned off, const char* name) {
    Table* const t = new_c2da(name);
    put_ptr(self, off, t);
    if (t == nullptr) return 0;
    return c2da_current_load_004143b0(t, nullptr) != 0 ? 1 : 0;
}
// Shape B (005C44F0/005C45B0/005C4670): as A, but a failed Load2DArray re-reads the field,
// deletes it through [vtbl+0](1) when non-null and zeroes the field before returning 0.
int load_or_discard(void* self, unsigned off, const char* name) {
    Table* const t = new_c2da(name);
    put_ptr(self, off, t);
    if (t == nullptr) return 0;
    if (c2da_current_load_004143b0(t, nullptr) != 0) return 1;
    if (Table* const again = ptr_at<Table>(self, off)) delete_c2da(again);
    wr32(self, off, 0);
    return 0;
}
// Shape C step: CExoString column(name) [005E5A90]; cell = C2DA::GetColumnIndex(field
// re-read, &column) [00413100, RET4]; ~CExoString [005E5C20] after the store.
void cache_column(void* self, unsigned off, const char* column, std::uint32_t* cell) {
    String name(column);
    *cell = static_cast<std::uint32_t>(c2da_current_column_index_00413100(ptr_at<Table>(self, off), nullptr, &name));
}
// IPRP element: new C2DA(CResRef(name.CStr()), 0): push 0 / 005E5670 / sub esp,10h /
// 00406D80 / 00413CC0, inside EH state 4 (54h block freed on unwind).
Table* new_c2da_from(String* name) {
    return static_cast<Table*>(allocate_construct(0x54, [name](void* p) {
        Ref r;
        r.construct_00406d80(WindowCStr005e5670(name, nullptr));
        return static_cast<void*>(c2da_current_construct_00413cc0(p, nullptr, r, 0));
    }));
}
inline Table* element(const void* self, unsigned array_off, int i) noexcept {
    return ptr_at<Table>(ptr_at<void>(self, array_off), static_cast<unsigned>(i) * 4u);
}
inline void set_element(void* self, unsigned array_off, int i, const void* v) noexcept {
    put_ptr(ptr_at<void>(self, array_off), static_cast<unsigned>(i) * 4u, v);
}
}  // namespace

// ---------------------------------------------------------------- table loaders
// 005C0270 CTwoDimArrays::Load2DArrays_Appearance (005C0270..005C08E3), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c0270(void* self, void*) {
    if (!load_table(self, 0xC, kotor_rules_str_00746efc)) return 0;  // 005C0294 alloc / 005C02BE 00406D80 / 005C02C5 00413CC0 / 005C02D7 store / 005C02E2 004143B0
    cache_column(self, 0xC, kotor_rules_str_0074b710, &kotor_rules_cell32_007a20c0);  // 005C02EF 005E5A90 / 005C030D 00413100 / 005C0316 store / 005C031F 005E5C20
    cache_column(self, 0xC, kotor_rules_str_00748738, &kotor_rules_cell32_007a20c4);  // 005C0324 005E5A90 / 005C0342 00413100 / 005C034B store / 005C0354 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6fc, &kotor_rules_cell32_007a20c8);  // 005C0359 005E5A90 / 005C0377 00413100 / 005C0380 store / 005C0389 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6f0, &kotor_rules_cell32_007a20cc);  // 005C038E 005E5A90 / 005C03AC 00413100 / 005C03B5 store / 005C03BE 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6e4, &kotor_rules_cell32_007a20d0);  // 005C03C3 005E5A90 / 005C03E1 00413100 / 005C03EA store / 005C03F3 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6d0, &kotor_rules_cell32_007a20d4);  // 005C03F8 005E5A90 / 005C0416 00413100 / 005C041F store / 005C0428 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6c4, &kotor_rules_cell32_007a20d8);  // 005C042D 005E5A90 / 005C044B 00413100 / 005C0454 store / 005C045D 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6b4, &kotor_rules_cell32_007a20e4);  // 005C0462 005E5A90 / 005C0480 00413100 / 005C0489 store / 005C0492 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b6a4, &kotor_rules_cell32_007a20e0);  // 005C0497 005E5A90 / 005C04B5 00413100 / 005C04BE store / 005C04C7 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b694, &kotor_rules_cell32_007a20dc);  // 005C04CC 005E5A90 / 005C04EA 00413100 / 005C04F3 store / 005C04FC 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b68c, &kotor_rules_cell32_007a20e8);  // 005C0501 005E5A90 / 005C051F 00413100 / 005C0528 store / 005C0531 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b67c, &kotor_rules_cell32_007a20ec);  // 005C0536 005E5A90 / 005C0554 00413100 / 005C055D store / 005C0566 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b670, &kotor_rules_cell32_007a20f4);  // 005C056B 005E5A90 / 005C0589 00413100 / 005C0592 store / 005C059B 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b664, &kotor_rules_cell32_007a20f8);  // 005C05A0 005E5A90 / 005C05BE 00413100 / 005C05C7 store / 005C05D0 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b658, &kotor_rules_cell32_007a20fc);  // 005C05D5 005E5A90 / 005C05F3 00413100 / 005C05FC store / 005C0605 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b64c, &kotor_rules_cell32_007a2100);  // 005C060A 005E5A90 / 005C0628 00413100 / 005C0631 store / 005C063A 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0073defc, &kotor_rules_cell32_007a2104);  // 005C063F 005E5A90 / 005C065D 00413100 / 005C0666 store / 005C066F 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b640, &kotor_rules_cell32_007a210c);  // 005C0674 005E5A90 / 005C0692 00413100 / 005C069B store / 005C06A4 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b638, &kotor_rules_cell32_007a2108);  // 005C06A9 005E5A90 / 005C06C7 00413100 / 005C06D0 store / 005C06D9 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b62c, &kotor_rules_cell32_007a20f0);  // 005C06DE 005E5A90 / 005C06FC 00413100 / 005C0705 store / 005C070E 005E5C20
    cache_column(self, 0xC, kotor_rules_str_00747c2c, &kotor_rules_cell32_007a2110);  // 005C0713 005E5A90 / 005C0731 00413100 / 005C073A store / 005C0743 005E5C20
    cache_column(self, 0xC, kotor_rules_str_007454e4, &kotor_rules_cell32_007a2114);  // 005C0748 005E5A90 / 005C0766 00413100 / 005C076F store / 005C0778 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b620, &kotor_rules_cell32_007a2118);  // 005C077D 005E5A90 / 005C079B 00413100 / 005C07A4 store / 005C07AD 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b2bc, &kotor_rules_cell32_007a211c);  // 005C07B2 005E5A90 / 005C07D0 00413100 / 005C07D9 store / 005C07E2 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b618, &kotor_rules_cell32_007a2120);  // 005C07E7 005E5A90 / 005C0805 00413100 / 005C080A store / 005C0817 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b608, &kotor_rules_cell32_007a2124);  // 005C081C 005E5A90 / 005C083A 00413100 / 005C0843 store / 005C084C 005E5C20
    cache_column(self, 0xC, kotor_rules_str_00749420, &kotor_rules_cell32_007a2128);  // 005C0851 005E5A90 / 005C086F 00413100 / 005C0878 store / 005C0881 005E5C20
    cache_column(self, 0xC, kotor_rules_str_0074b5fc, &kotor_rules_cell32_007a212c);  // 005C0886 005E5A90 / 005C08A4 00413100 / 005C08AD store / 005C08B6 005E5C20
    return 1;  // 005C08BB
}
// 005C0AE0 CTwoDimArrays::Load2DArrays_Gender (005C0AE0..005C0BAE), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c0ae0(void* self, void*) {
    if (!load_table(self, 0x10, kotor_rules_str_0074b76c)) return 0;  // 005C0AFE alloc / 005C0B28 00406D80 / 005C0B2F 00413CC0 / 005C0B41 store / 005C0B48 004143B0
    cache_column(self, 0x10, kotor_rules_str_0074b76c, &kotor_rules_cell32_007a2148);  // 005C0B51 005E5A90 / 005C0B6F 00413100 / 005C0B78 store / 005C0B81 005E5C20
    return 1;  // 005C0B86
}
// 005C0BB0 CTwoDimArrays::Load2DArrays_SurfaceMaterial (005C0BB0..005C0C7E), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c0bb0(void* self, void*) {
    if (!load_table(self, 0x14, kotor_rules_str_0074b774)) return 0;  // 005C0BCE alloc / 005C0BF8 00406D80 / 005C0BFF 00413CC0 / 005C0C11 store / 005C0C18 004143B0
    cache_column(self, 0x14, kotor_rules_str_00745eec, &kotor_rules_cell32_007a21b0);  // 005C0C21 005E5A90 / 005C0C3F 00413100 / 005C0C48 store / 005C0C51 005E5C20
    return 1;  // 005C0C56
}
// 005C0C80 CTwoDimArrays::Load2DArrays_VisualEffect (005C0C80..005C1002), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c0c80(void* self, void*) {
    if (!load_table(self, 0x88, kotor_rules_str_0074b848)) return 0;  // 005C0CA4 alloc / 005C0CCE 00406D80 / 005C0CD5 00413CC0 / 005C0CE7 store / 005C0CF5 004143B0
    cache_column(self, 0x88, kotor_rules_str_0074b834, &kotor_rules_cell32_007a21b4);  // 005C0D02 005E5A90 / 005C0D23 00413100 / 005C0D2C store / 005C0D35 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b824, &kotor_rules_cell32_007a21b8);  // 005C0D3A 005E5A90 / 005C0D5B 00413100 / 005C0D64 store / 005C0D6D 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b814, &kotor_rules_cell32_007a21c8);  // 005C0D72 005E5A90 / 005C0D93 00413100 / 005C0D9C store / 005C0DA5 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b804, &kotor_rules_cell32_007a21c4);  // 005C0DAA 005E5A90 / 005C0DCB 00413100 / 005C0DD4 store / 005C0DDD 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7f4, &kotor_rules_cell32_007a21bc);  // 005C0DE2 005E5A90 / 005C0E03 00413100 / 005C0E0C store / 005C0E15 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7e4, &kotor_rules_cell32_007a21c0);  // 005C0E1A 005E5A90 / 005C0E3B 00413100 / 005C0E44 store / 005C0E4D 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7d8, &kotor_rules_cell32_007a21cc);  // 005C0E52 005E5A90 / 005C0E73 00413100 / 005C0E7C store / 005C0E85 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7c4, &kotor_rules_cell32_007a21d0);  // 005C0E8A 005E5A90 / 005C0EAB 00413100 / 005C0EB4 store / 005C0EBD 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7b8, &kotor_rules_cell32_007a21dc);  // 005C0EC2 005E5A90 / 005C0EE3 00413100 / 005C0EEC store / 005C0EF5 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b7a8, &kotor_rules_cell32_007a21d8);  // 005C0EFA 005E5A90 / 005C0F1B 00413100 / 005C0F24 store / 005C0F2D 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b79c, &kotor_rules_cell32_007a21d4);  // 005C0F32 005E5A90 / 005C0F53 00413100 / 005C0F5C store / 005C0F65 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b78c, &kotor_rules_cell32_007a21e4);  // 005C0F6A 005E5A90 / 005C0F8B 00413100 / 005C0F94 store / 005C0F9D 005E5C20
    cache_column(self, 0x88, kotor_rules_str_0074b780, &kotor_rules_cell32_007a21e0);  // 005C0FA2 005E5A90 / 005C0FC3 00413100 / 005C0FCC store / 005C0FD5 005E5C20
    return 1;  // 005C0FDA
}
// 005C1010 CTwoDimArrays::Load2DArrays_PersistentVisualEffect (005C1010..005C157A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1010(void* self, void*) {
    if (!load_table(self, 0x1C, kotor_rules_str_0074b934)) return 0;  // 005C1034 alloc / 005C105E 00406D80 / 005C1065 00413CC0 / 005C1077 store / 005C1082 004143B0
    cache_column(self, 0x1C, kotor_rules_str_0074b928, &kotor_rules_cell32_007a21e8);  // 005C108F 005E5A90 / 005C10AD 00413100 / 005C10B6 store / 005C10BF 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b7c4, &kotor_rules_cell32_007a21ec);  // 005C10C4 005E5A90 / 005C10E2 00413100 / 005C10EB store / 005C10F4 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_00749fb0, &kotor_rules_cell32_007a21f0);  // 005C10F9 005E5A90 / 005C1117 00413100 / 005C1120 store / 005C1129 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_007476a8, &kotor_rules_cell32_007a21f4);  // 005C112E 005E5A90 / 005C114C 00413100 / 005C1155 store / 005C115E 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0073df04, &kotor_rules_cell32_007a21f8);  // 005C1163 005E5A90 / 005C1181 00413100 / 005C118A store / 005C1193 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_00749fa4, &kotor_rules_cell32_007a21fc);  // 005C1198 005E5A90 / 005C11B6 00413100 / 005C11BF store / 005C11C8 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b91c, &kotor_rules_cell32_007a2200);  // 005C11CD 005E5A90 / 005C11EB 00413100 / 005C11F4 store / 005C11FD 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b914, &kotor_rules_cell32_007a2204);  // 005C1202 005E5A90 / 005C1220 00413100 / 005C1229 store / 005C1232 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b908, &kotor_rules_cell32_007a2208);  // 005C1237 005E5A90 / 005C1255 00413100 / 005C125E store / 005C1267 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b900, &kotor_rules_cell32_007a220c);  // 005C126C 005E5A90 / 005C128A 00413100 / 005C1293 store / 005C129C 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8f4, &kotor_rules_cell32_007a2210);  // 005C12A1 005E5A90 / 005C12BF 00413100 / 005C12C8 store / 005C12D1 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8ec, &kotor_rules_cell32_007a2214);  // 005C12D6 005E5A90 / 005C12F4 00413100 / 005C12FD store / 005C1306 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8e0, &kotor_rules_cell32_007a2218);  // 005C130B 005E5A90 / 005C1329 00413100 / 005C1332 store / 005C133B 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8d4, &kotor_rules_cell32_007a221c);  // 005C1340 005E5A90 / 005C135E 00413100 / 005C1367 store / 005C1370 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8c8, &kotor_rules_cell32_007a2220);  // 005C1375 005E5A90 / 005C1393 00413100 / 005C139C store / 005C13A5 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8bc, &kotor_rules_cell32_007a2224);  // 005C13AA 005E5A90 / 005C13C8 00413100 / 005C13D1 store / 005C13DA 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8b0, &kotor_rules_cell32_007a2228);  // 005C13DF 005E5A90 / 005C13FD 00413100 / 005C1406 store / 005C140F 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b8a4, &kotor_rules_cell32_007a222c);  // 005C1414 005E5A90 / 005C1432 00413100 / 005C143B store / 005C1444 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b898, &kotor_rules_cell32_007a2230);  // 005C1449 005E5A90 / 005C1467 00413100 / 005C1470 store / 005C1479 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b88c, &kotor_rules_cell32_007a2234);  // 005C147E 005E5A90 / 005C149C 00413100 / 005C14A5 store / 005C14AE 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b880, &kotor_rules_cell32_007a2238);  // 005C14B3 005E5A90 / 005C14D1 00413100 / 005C14DA store / 005C14E3 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b870, &kotor_rules_cell32_007a223c);  // 005C14E8 005E5A90 / 005C1506 00413100 / 005C150F store / 005C1518 005E5C20
    cache_column(self, 0x1C, kotor_rules_str_0074b858, &kotor_rules_cell32_007a2240);  // 005C151D 005E5A90 / 005C153B 00413100 / 005C1544 store / 005C154D 005E5C20
    return 1;  // 005C1552
}
// 005C1580 CTwoDimArrays::Load2DArrays_CreatureSpeed (005C1580..005C168B), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1580(void* self, void*) {
    if (!load_table(self, 0x20, kotor_rules_str_0074b958)) return 0;  // 005C159E alloc / 005C15C8 00406D80 / 005C15CF 00413CC0 / 005C15E1 store / 005C15EC 004143B0
    cache_column(self, 0x20, kotor_rules_str_0074b950, &kotor_rules_cell32_007a2244);  // 005C15F9 005E5A90 / 005C1617 00413100 / 005C1620 store / 005C1629 005E5C20
    cache_column(self, 0x20, kotor_rules_str_0074b944, &kotor_rules_cell32_007a2248);  // 005C162E 005E5A90 / 005C164C 00413100 / 005C1655 store / 005C165E 005E5C20
    return 1;  // 005C1663
}
// 005C1690 CTwoDimArrays::Load2DArrays_DoorTypes (005C1690..005C175E), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1690(void* self, void*) {
    if (!load_table(self, 0x24, kotor_rules_str_0074b968)) return 0;  // 005C16AE alloc / 005C16D8 00406D80 / 005C16DF 00413CC0 / 005C16F1 store / 005C16F8 004143B0
    cache_column(self, 0x24, kotor_rules_str_00749420, &kotor_rules_cell32_007a224c);  // 005C1701 005E5A90 / 005C171F 00413100 / 005C1728 store / 005C1731 005E5C20
    return 1;  // 005C1736
}
// 005C1760 CTwoDimArrays::Load2DArrays_GenericDoors (005C1760..005C182E), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1760(void* self, void*) {
    if (!load_table(self, 0x28, kotor_rules_str_0074b974)) return 0;  // 005C177E alloc / 005C17A8 00406D80 / 005C17AF 00413CC0 / 005C17C1 store / 005C17C8 004143B0
    cache_column(self, 0x28, kotor_rules_str_00749420, &kotor_rules_cell32_007a2250);  // 005C17D1 005E5A90 / 005C17EF 00413100 / 005C17F8 store / 005C1801 005E5C20
    return 1;  // 005C1806
}
// 005C1830 CTwoDimArrays::Load2DArrays_Placeables (005C1830..005C1970), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1830(void* self, void*) {
    if (!load_table(self, 0x2C, kotor_rules_str_0074b99c)) return 0;  // 005C184E alloc / 005C1878 00406D80 / 005C187F 00413CC0 / 005C1891 store / 005C189C 004143B0
    cache_column(self, 0x2C, kotor_rules_str_00749420, &kotor_rules_cell32_007a2254);  // 005C18A9 005E5A90 / 005C18C7 00413100 / 005C18D0 store / 005C18D9 005E5C20
    cache_column(self, 0x2C, kotor_rules_str_0074868c, &kotor_rules_cell32_007a2258);  // 005C18DE 005E5A90 / 005C18FC 00413100 / 005C1905 store / 005C190E 005E5C20
    cache_column(self, 0x2C, kotor_rules_str_0074b984, &kotor_rules_cell32_007a225c);  // 005C1913 005E5A90 / 005C1931 00413100 / 005C193A store / 005C1943 005E5C20
    return 1;  // 005C1948
}
// 005C1B40 CTwoDimArrays::Load2DArrays_AreaTransition (005C1B40..005C1BDA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1b40(void* self, void*) {
    return load_table(self, 0x18, kotor_rules_str_0074b9d0);  // 005C1B5E alloc / 005C1B88 00406D80 / 005C1B8F 00413CC0 / 005C1BA9 004143B0 / 005C1BB2 ret 1
}
// 005C1BE0 CTwoDimArrays::Load2DArrays_IPRPMelee (005C1BE0..005C1C7A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1be0(void* self, void*) {
    return load_table(self, 0x30, kotor_rules_str_0074b9dc);  // 005C1BFE alloc / 005C1C28 00406D80 / 005C1C2F 00413CC0 / 005C1C49 004143B0 / 005C1C52 ret 1
}
// 005C1C80 CTwoDimArrays::Load2DArrays_ItemPropDef (005C1C80..005C1D1A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1c80(void* self, void*) {
    return load_table(self, 0x34, kotor_rules_str_0074b9ec);  // 005C1C9E alloc / 005C1CC8 00406D80 / 005C1CCF 00413CC0 / 005C1CE9 004143B0 / 005C1CF2 ret 1
}
// 005C1D20 CTwoDimArrays::Load2DArrays_Portrait (005C1D20..005C1DBA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1d20(void* self, void*) {
    return load_table(self, 0x38, kotor_rules_str_0074b9f8);  // 005C1D3E alloc / 005C1D68 00406D80 / 005C1D6F 00413CC0 / 005C1D89 004143B0 / 005C1D92 ret 1
}
// 005C1DC0 CTwoDimArrays::Load2DArrays_IPRPDamage (005C1DC0..005C1E5D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1dc0(void* self, void*) {
    return load_table(self, 0xB0, kotor_rules_str_0074ba04);  // 005C1DDE alloc / 005C1E08 00406D80 / 005C1E0F 00413CC0 / 005C1E2C 004143B0 / 005C1E35 ret 1
}
// 005C1E60 CTwoDimArrays::Load2DArrays_Traps (005C1E60..005C1EFD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1e60(void* self, void*) {
    return load_table(self, 0x8C, kotor_rules_str_0074ba14);  // 005C1E7E alloc / 005C1EA8 00406D80 / 005C1EAF 00413CC0 / 005C1ECC 004143B0 / 005C1ED5 ret 1
}
// 005C1F00 CTwoDimArrays::Load2DArrays_LightColor (005C1F00..005C1F9A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1f00(void* self, void*) {
    return load_table(self, 0x48, kotor_rules_str_007494bc);  // 005C1F1E alloc / 005C1F48 00406D80 / 005C1F4F 00413CC0 / 005C1F69 004143B0 / 005C1F72 ret 1
}
// 005C1FA0 CTwoDimArrays::Load2DArrays_Cursor (005C1FA0..005C203A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1fa0(void* self, void*) {
    return load_table(self, 0x4C, kotor_rules_str_0074ba1c);  // 005C1FBE alloc / 005C1FE8 00406D80 / 005C1FEF 00413CC0 / 005C2009 004143B0 / 005C2012 ret 1
}
// 005C2040 CTwoDimArrays::Load2DArrays_AmbientMusic (005C2040..005C20DA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2040(void* self, void*) {
    return load_table(self, 0x50, kotor_rules_str_0074ba24);  // 005C205E alloc / 005C2088 00406D80 / 005C208F 00413CC0 / 005C20A9 004143B0 / 005C20B2 ret 1
}
// 005C20E0 CTwoDimArrays::Load2DArrays_AmbientSound (005C20E0..005C217A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c20e0(void* self, void*) {
    return load_table(self, 0x54, kotor_rules_str_0074ba34);  // 005C20FE alloc / 005C2128 00406D80 / 005C212F 00413CC0 / 005C2149 004143B0 / 005C2152 ret 1
}
// 005C2180 CTwoDimArrays::Load2DArrays_FootstepSounds (005C2180..005C228B), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2180(void* self, void*) {
    if (!load_table(self, 0x58, kotor_rules_str_0074ba58)) return 0;  // 005C219E alloc / 005C21C8 00406D80 / 005C21CF 00413CC0 / 005C21E1 store / 005C21EC 004143B0
    cache_column(self, 0x58, kotor_rules_str_0074ba4c, &kotor_rules_cell32_007a21a0);  // 005C21F9 005E5A90 / 005C2217 00413100 / 005C2220 store / 005C2229 005E5C20
    cache_column(self, 0x58, kotor_rules_str_0074ba44, &kotor_rules_cell32_007a219c);  // 005C222E 005E5A90 / 005C224C 00413100 / 005C2255 store / 005C225E 005E5C20
    return 1;  // 005C2263
}
// 005C08F0 CTwoDimArrays::Load2DArrays_AppearanceSounds (005C08F0..005C0AD5), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c08f0(void* self, void*) {
    if (!load_table(self, 0x5C, kotor_rules_str_0074b758)) return 0;  // 005C0914 alloc / 005C093E 00406D80 / 005C0945 00413CC0 / 005C0957 store / 005C0962 004143B0
    cache_column(self, 0x5C, kotor_rules_str_0074b02c, &kotor_rules_cell32_007a2144);  // 005C096F 005E5A90 / 005C098D 00413100 / 005C0996 store / 005C099F 005E5C20
    cache_column(self, 0x5C, kotor_rules_str_0074b74c, &kotor_rules_cell32_007a2130);  // 005C09A4 005E5A90 / 005C09C2 00413100 / 005C09CB store / 005C09D4 005E5C20
    cache_column(self, 0x5C, kotor_rules_str_0074b740, &kotor_rules_cell32_007a2138);  // 005C09D9 005E5A90 / 005C09F7 00413100 / 005C0A00 store / 005C0A09 005E5C20
    cache_column(self, 0x5C, kotor_rules_str_0074b734, &kotor_rules_cell32_007a2134);  // 005C0A0E 005E5A90 / 005C0A2C 00413100 / 005C0A35 store / 005C0A3E 005E5C20
    cache_column(self, 0x5C, kotor_rules_str_0074b728, &kotor_rules_cell32_007a213c);  // 005C0A43 005E5A90 / 005C0A61 00413100 / 005C0A6A store / 005C0A73 005E5C20
    cache_column(self, 0x5C, kotor_rules_str_0074b720, &kotor_rules_cell32_007a2140);  // 005C0A78 005E5A90 / 005C0A96 00413100 / 005C0A9F store / 005C0AA8 005E5C20
    return 1;  // 005C0AAD
}
// 005C2290 CTwoDimArrays::Load2DArrays_WeaponSounds (005C2290..005C235E), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2290(void* self, void*) {
    if (!load_table(self, 0x60, kotor_rules_str_0074ba68)) return 0;  // 005C22AE alloc / 005C22D8 00406D80 / 005C22DF 00413CC0 / 005C22F1 store / 005C22F8 004143B0
    cache_column(self, 0x60, kotor_rules_str_0074ba4c, &kotor_rules_cell32_007a21a4);  // 005C2301 005E5A90 / 005C231F 00413100 / 005C2328 store / 005C2331 005E5C20
    return 1;  // 005C2336
}
// 005C2360 CTwoDimArrays::Load2DArrays_AmmunitionTypes (005C2360..005C23FA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2360(void* self, void*) {
    return load_table(self, 0x64, kotor_rules_str_0074ba78);  // 005C237E alloc / 005C23A8 00406D80 / 005C23AF 00413CC0 / 005C23C9 004143B0 / 005C23D2 ret 1
}
// 005C2400 CTwoDimArrays::Load2DArrays_Keymap (005C2400..005C249A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2400(void* self, void*) {
    return load_table(self, 0x68, kotor_rules_str_0074ba88);  // 005C241E alloc / 005C2448 00406D80 / 005C244F 00413CC0 / 005C2469 004143B0 / 005C2472 ret 1
}
// 005C24A0 CTwoDimArrays::Load2DArrays_BindableKey (005C24A0..005C253A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c24a0(void* self, void*) {
    return load_table(self, 0x6C, kotor_rules_str_0074ba90);  // 005C24BE alloc / 005C24E8 00406D80 / 005C24EF 00413CC0 / 005C2509 004143B0 / 005C2512 ret 1
}
// 005C1980 CTwoDimArrays::Load2DArrays_PlaceableSounds (005C1980..005C1B30), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c1980(void* self, void*) {
    if (!load_table(self, 0x70, kotor_rules_str_0074b9bc)) return 0;  // 005C19A4 alloc / 005C19CE 00406D80 / 005C19D5 00413CC0 / 005C19E7 store / 005C19F2 004143B0
    cache_column(self, 0x70, kotor_rules_str_0074b02c, &kotor_rules_cell32_007a2260);  // 005C19FF 005E5A90 / 005C1A1D 00413100 / 005C1A26 store / 005C1A2F 005E5C20
    cache_column(self, 0x70, kotor_rules_str_007496f0, &kotor_rules_cell32_007a2264);  // 005C1A34 005E5A90 / 005C1A52 00413100 / 005C1A5B store / 005C1A64 005E5C20
    cache_column(self, 0x70, kotor_rules_str_00749418, &kotor_rules_cell32_007a2268);  // 005C1A69 005E5A90 / 005C1A87 00413100 / 005C1A90 store / 005C1A99 005E5C20
    cache_column(self, 0x70, kotor_rules_str_0074b9b4, &kotor_rules_cell32_007a226c);  // 005C1A9E 005E5A90 / 005C1ABC 00413100 / 005C1AC5 store / 005C1ACE 005E5C20
    cache_column(self, 0x70, kotor_rules_str_0074b9a8, &kotor_rules_cell32_007a2270);  // 005C1AD3 005E5A90 / 005C1AF1 00413100 / 005C1AFA store / 005C1B03 005E5C20
    return 1;  // 005C1B08
}
// 005C2540 CTwoDimArrays::Load2DArrays_CameraStyle (005C2540..005C25DA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2540(void* self, void*) {
    return load_table(self, 0x74, kotor_rules_str_00747a14);  // 005C255E alloc / 005C2588 00406D80 / 005C258F 00413CC0 / 005C25A9 004143B0 / 005C25B2 ret 1
}
// 005C25E0 CTwoDimArrays::Load2DArrays_Tutorial (005C25E0..005C267D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c25e0(void* self, void*) {
    return load_table(self, 0x118, kotor_rules_str_0074baa0);  // 005C25FE alloc / 005C2628 00406D80 / 005C262F 00413CC0 / 005C264C 004143B0 / 005C2655 ret 1
}
// 005C2680 CTwoDimArrays::Load2DArrays_DifficultyOptions (005C2680..005C271A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2680(void* self, void*) {
    return load_table(self, 0x7C, kotor_rules_str_0074baac);  // 005C269E alloc / 005C26C8 00406D80 / 005C26CF 00413CC0 / 005C26E9 004143B0 / 005C26F2 ret 1
}
// 005C2720 CTwoDimArrays::Load2DArrays_Gamma (005C2720..005C27BD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2720(void* self, void*) {
    return load_table(self, 0x80, kotor_rules_str_0073efcc);  // 005C273E alloc / 005C2768 00406D80 / 005C276F 00413CC0 / 005C278C 004143B0 / 005C2795 ret 1
}
// 005C27C0 CTwoDimArrays::Load2DArrays_StateScripts (005C27C0..005C285D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c27c0(void* self, void*) {
    return load_table(self, 0x84, kotor_rules_str_0074babc);  // 005C27DE alloc / 005C2808 00406D80 / 005C280F 00413CC0 / 005C282C 004143B0 / 005C2835 ret 1
}
// 005C2860 CTwoDimArrays::Load2DArrays_Poison (005C2860..005C28FD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2860(void* self, void*) {
    return load_table(self, 0x90, kotor_rules_str_0074bacc);  // 005C287E alloc / 005C28A8 00406D80 / 005C28AF 00413CC0 / 005C28CC 004143B0 / 005C28D5 ret 1
}
// 005C2900 CTwoDimArrays::Load2DArrays_Disease (005C2900..005C299D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2900(void* self, void*) {
    return load_table(self, 0x94, kotor_rules_str_0074bad4);  // 005C291E alloc / 005C2948 00406D80 / 005C294F 00413CC0 / 005C296C 004143B0 / 005C2975 ret 1
}
// 005C29A0 CTwoDimArrays::Load2DArrays_RepAdjustments (005C29A0..005C2A3D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c29a0(void* self, void*) {
    return load_table(self, 0x98, kotor_rules_str_0074badc);  // 005C29BE alloc / 005C29E8 00406D80 / 005C29EF 00413CC0 / 005C2A0C 004143B0 / 005C2A15 ret 1
}
// 005C2A40 CTwoDimArrays::Load2DArrays_FractionalCR (005C2A40..005C2ADD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2a40(void* self, void*) {
    return load_table(self, 0x9C, kotor_rules_str_0074bae8);  // 005C2A5E alloc / 005C2A88 00406D80 / 005C2A8F 00413CC0 / 005C2AAC 004143B0 / 005C2AB5 ret 1
}
// 005C2AE0 CTwoDimArrays::Load2DArrays_EncounterDifficulty (005C2AE0..005C2B7D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2ae0(void* self, void*) {
    return load_table(self, 0xA8, kotor_rules_str_0074baf8);  // 005C2AFE alloc / 005C2B28 00406D80 / 005C2B2F 00413CC0 / 005C2B4C 004143B0 / 005C2B55 ret 1
}
// 005C2B80 CTwoDimArrays::Load2DArrays_ExcitedDuration (005C2B80..005C2C1D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2b80(void* self, void*) {
    return load_table(self, 0xA0, kotor_rules_str_0074bb08);  // 005C2B9E alloc / 005C2BC8 00406D80 / 005C2BCF 00413CC0 / 005C2BEC 004143B0 / 005C2BF5 ret 1
}
// 005C2C20 CTwoDimArrays::Load2DArrays_Regeneration (005C2C20..005C2D34), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2c20(void* self, void*) {
    if (!load_table(self, 0xA4, kotor_rules_str_0074bb30)) return 0;  // 005C2C3E alloc / 005C2C68 00406D80 / 005C2C6F 00413CC0 / 005C2C81 store / 005C2C8F 004143B0
    cache_column(self, 0xA4, kotor_rules_str_0074bb24, &kotor_rules_cell32_007a21ac);  // 005C2C9C 005E5A90 / 005C2CBD 00413100 / 005C2CC6 store / 005C2CCF 005E5C20
    cache_column(self, 0xA4, kotor_rules_str_0074bb18, &kotor_rules_cell32_007a21a8);  // 005C2CD4 005E5A90 / 005C2CF5 00413100 / 005C2CFE store / 005C2D07 005E5C20
    return 1;  // 005C2D0C
}
// 005C2D40 CTwoDimArrays::Load2DArrays_XpBase (005C2D40..005C2DDD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2d40(void* self, void*) {
    return load_table(self, 0xC0, kotor_rules_str_0074bb40);  // 005C2D5E alloc / 005C2D88 00406D80 / 005C2D8F 00413CC0 / 005C2DAC 004143B0 / 005C2DB5 ret 1
}
// 005C2DE0 CTwoDimArrays::Load2DArrays_IPRPMonsterDamage (005C2DE0..005C2E7D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2de0(void* self, void*) {
    return load_table(self, 0xAC, kotor_rules_str_0074bb48);  // 005C2DFE alloc / 005C2E28 00406D80 / 005C2E2F 00413CC0 / 005C2E4C 004143B0 / 005C2E55 ret 1
}
// 005C2E80 CTwoDimArrays::Load2DArrays_IPRPBonusCost (005C2E80..005C2F1D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2e80(void* self, void*) {
    return load_table(self, 0xB4, kotor_rules_str_0074bb58);  // 005C2E9E alloc / 005C2EC8 00406D80 / 005C2ECF 00413CC0 / 005C2EEC 004143B0 / 005C2EF5 ret 1
}
// 005C2F20 CTwoDimArrays::Load2DArrays_IPRPSRCost (005C2F20..005C2FBD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2f20(void* self, void*) {
    return load_table(self, 0xB8, kotor_rules_str_0074bb68);  // 005C2F3E alloc / 005C2F68 00406D80 / 005C2F6F 00413CC0 / 005C2F8C 004143B0 / 005C2F95 ret 1
}
// 005C2FC0 CTwoDimArrays::Load2DArrays_IPRPNeg5Cost (005C2FC0..005C305D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c2fc0(void* self, void*) {
    return load_table(self, 0xBC, kotor_rules_str_0074bb74);  // 005C2FDE alloc / 005C3008 00406D80 / 005C300F 00413CC0 / 005C302C 004143B0 / 005C3035 ret 1
}
// 005C3060 CTwoDimArrays::Load2DArrays_Ranges (005C3060..005C30FD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3060(void* self, void*) {
    return load_table(self, 0xC4, kotor_rules_str_00748cb4);  // 005C307E alloc / 005C30A8 00406D80 / 005C30AF 00413CC0 / 005C30CC 004143B0 / 005C30D5 ret 1
}
// 005C3100 CTwoDimArrays::Load2DArrays_OnHit (005C3100..005C319D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3100(void* self, void*) {
    return load_table(self, 0xC8, kotor_rules_str_0074bb84);  // 005C311E alloc / 005C3148 00406D80 / 005C314F 00413CC0 / 005C316C 004143B0 / 005C3175 ret 1
}
// 005C31A0 CTwoDimArrays::Load2DArrays_OnHitDuration (005C31A0..005C323D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c31a0(void* self, void*) {
    return load_table(self, 0xCC, kotor_rules_str_0074bb90);  // 005C31BE alloc / 005C31E8 00406D80 / 005C31EF 00413CC0 / 005C320C 004143B0 / 005C3215 ret 1
}
// 005C3240 CTwoDimArrays::Load2DArrays_DamageHitVisual (005C3240..005C32DD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3240(void* self, void*) {
    return load_table(self, 0xD0, kotor_rules_str_0074bba0);  // 005C325E alloc / 005C3288 00406D80 / 005C328F 00413CC0 / 005C32AC 004143B0 / 005C32B5 ret 1
}
// 005C32E0 CTwoDimArrays::Load2DArrays_Animations (005C32E0..005C366D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c32e0(void* self, void*) {
    if (!load_table(self, 0x3C, kotor_rules_str_0074bc1c)) return 0;  // 005C3304 alloc / 005C332E 00406D80 / 005C3335 00413CC0 / 005C3347 store / 005C3352 004143B0
    cache_column(self, 0x3C, kotor_rules_str_0074bc14, &kotor_rules_cell32_007a217c);  // 005C335F 005E5A90 / 005C337D 00413100 / 005C3386 store / 005C338F 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bc0c, &kotor_rules_cell32_007a2168);  // 005C3394 005E5A90 / 005C33B2 00413100 / 005C33BB store / 005C33C4 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_007481f0, &kotor_rules_cell32_007a2170);  // 005C33C9 005E5A90 / 005C33E7 00413100 / 005C33F0 store / 005C33F9 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bc04, &kotor_rules_cell32_007a2178);  // 005C33FE 005E5A90 / 005C341C 00413100 / 005C3425 store / 005C342E 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbf8, &kotor_rules_cell32_007a214c);  // 005C3433 005E5A90 / 005C3451 00413100 / 005C345A store / 005C3463 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbe4, &kotor_rules_cell32_007a2180);  // 005C3468 005E5A90 / 005C3486 00413100 / 005C348F store / 005C3498 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_00747798, &kotor_rules_cell32_007a2154);  // 005C349D 005E5A90 / 005C34BB 00413100 / 005C34C4 store / 005C34CD 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_00745eec, &kotor_rules_cell32_007a2174);  // 005C34D2 005E5A90 / 005C34F0 00413100 / 005C34F9 store / 005C3502 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbdc, &kotor_rules_cell32_007a2150);  // 005C3507 005E5A90 / 005C3525 00413100 / 005C352E store / 005C3537 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbd4, &kotor_rules_cell32_007a216c);  // 005C353C 005E5A90 / 005C355A 00413100 / 005C3563 store / 005C356C 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbcc, &kotor_rules_cell32_007a215c);  // 005C3571 005E5A90 / 005C358F 00413100 / 005C3598 store / 005C35A1 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbc4, &kotor_rules_cell32_007a2164);  // 005C35A6 005E5A90 / 005C35C4 00413100 / 005C35CD store / 005C35D6 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbb8, &kotor_rules_cell32_007a2158);  // 005C35DB 005E5A90 / 005C35F9 00413100 / 005C3602 store / 005C360B 005E5C20
    cache_column(self, 0x3C, kotor_rules_str_0074bbb0, &kotor_rules_cell32_007a2160);  // 005C3610 005E5A90 / 005C362E 00413100 / 005C3637 store / 005C3640 005E5C20
    return 1;  // 005C3645
}
// 005C3670 CTwoDimArrays::Load2DArrays_BodyBag (005C3670..005C370D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3670(void* self, void*) {
    return load_table(self, 0xD4, kotor_rules_str_0074bc28);  // 005C368E alloc / 005C36B8 00406D80 / 005C36BF 00413CC0 / 005C36DC 004143B0 / 005C36E5 ret 1
}
// 005C3710 CTwoDimArrays::Load2DArrays_ForceAdjust (005C3710..005C3824), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3710(void* self, void*) {
    if (!load_table(self, 0xD8, kotor_rules_str_0074bc48)) return 0;  // 005C372E alloc / 005C3758 00406D80 / 005C375F 00413CC0 / 005C3771 store / 005C377F 004143B0
    cache_column(self, 0xD8, kotor_rules_str_0074bc3c, &kotor_rules_cell32_007a2188);  // 005C378C 005E5A90 / 005C37AD 00413100 / 005C37B6 store / 005C37BF 005E5C20
    cache_column(self, 0xD8, kotor_rules_str_0074bc30, &kotor_rules_cell32_007a2184);  // 005C37C4 005E5A90 / 005C37E5 00413100 / 005C37EE store / 005C37F7 005E5C20
    return 1;  // 005C37FC
}
// 005C3830 CTwoDimArrays::Load2DArrays_GameEffects (005C3830..005C38CD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3830(void* self, void*) {
    return load_table(self, 0xDC, kotor_rules_str_0074bc54);  // 005C384E alloc / 005C3878 00406D80 / 005C387F 00413CC0 / 005C389C 004143B0 / 005C38A5 ret 1
}
// 005C38D0 CTwoDimArrays::Load2DArrays_Repute (005C38D0..005C396D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c38d0(void* self, void*) {
    return load_table(self, 0xE0, kotor_rules_str_0074bc60);  // 005C38EE alloc / 005C3918 00406D80 / 005C391F 00413CC0 / 005C393C 004143B0 / 005C3945 ret 1
}
// 005C3970 CTwoDimArrays::Load2DArrays_WeaponDischarge (005C3970..005C3A0D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3970(void* self, void*) {
    return load_table(self, 0xE4, kotor_rules_str_0074bc68);  // 005C398E alloc / 005C39B8 00406D80 / 005C39BF 00413CC0 / 005C39DC 004143B0 / 005C39E5 ret 1
}
// 005C3A10 CTwoDimArrays::Load2DArrays_DroidDischarge (005C3A10..005C3AAD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3a10(void* self, void*) {
    return load_table(self, 0xE8, kotor_rules_str_0074bc78);  // 005C3A2E alloc / 005C3A58 00406D80 / 005C3A5F 00413CC0 / 005C3A7C 004143B0 / 005C3A85 ret 1
}
// 005C3AB0 CTwoDimArrays::Load2DArrays_PlotXP (005C3AB0..005C3B4D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3ab0(void* self, void*) {
    return load_table(self, 0xEC, kotor_rules_str_00748f64);  // 005C3ACE alloc / 005C3AF8 00406D80 / 005C3AFF 00413CC0 / 005C3B1C 004143B0 / 005C3B25 ret 1
}
// 005C3B50 CTwoDimArrays::Load2DArrays_CombatAnimations (005C3B50..005C3BEA), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3b50(void* self, void*) {
    return load_table(self, 0x78, kotor_rules_str_0074bc88);  // 005C3B6E alloc / 005C3B98 00406D80 / 005C3B9F 00413CC0 / 005C3BB9 004143B0 / 005C3BC2 ret 1
}
// 005C3BF0 CTwoDimArrays::Load2DArrays_DialogAnimations (005C3BF0..005C3D65), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3bf0(void* self, void*) {
    if (!load_table(self, 0x40, kotor_rules_str_0074bc9c)) return 0;  // 005C3C0E alloc / 005C3C38 00406D80 / 005C3C3F 00413CC0 / 005C3C51 store / 005C3C5C 004143B0
    cache_column(self, 0x40, kotor_rules_str_007481f0, &kotor_rules_cell32_007a2190);  // 005C3C69 005E5A90 / 005C3C87 00413100 / 005C3C90 store / 005C3C99 005E5C20
    cache_column(self, 0x40, kotor_rules_str_0074bbf8, &kotor_rules_cell32_007a218c);  // 005C3C9E 005E5A90 / 005C3CBC 00413100 / 005C3CC5 store / 005C3CCE 005E5C20
    cache_column(self, 0x40, kotor_rules_str_00747798, &kotor_rules_cell32_007a2194);  // 005C3CD3 005E5A90 / 005C3CF1 00413100 / 005C3CFA store / 005C3D03 005E5C20
    cache_column(self, 0x40, kotor_rules_str_0074bbdc, &kotor_rules_cell32_007a2198);  // 005C3D08 005E5A90 / 005C3D26 00413100 / 005C3D2F store / 005C3D38 005E5C20
    return 1;  // 005C3D3D
}
// 005C3D70 CTwoDimArrays::Load2DArrays_NPC (005C3D70..005C3E0D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3d70(void* self, void*) {
    return load_table(self, 0xF0, kotor_rules_str_0074bcb0);  // 005C3D8E alloc / 005C3DB8 00406D80 / 005C3DBF 00413CC0 / 005C3DDC 004143B0 / 005C3DE5 ret 1
}
// 005C3E10 CTwoDimArrays::Load2DArrays_Spells (005C3E10..005C3EAD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3e10(void* self, void*) {
    return load_table(self, 0xF4, kotor_rules_str_0074bcb4);  // 005C3E2E alloc / 005C3E58 00406D80 / 005C3E5F 00413CC0 / 005C3E7C 004143B0 / 005C3E85 ret 1
}
// 005C3EB0 CTwoDimArrays::Load2DArrays_Feat (005C3EB0..005C3F4D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3eb0(void* self, void*) {
    return load_table(self, 0xF8, kotor_rules_str_00748ca0);  // 005C3ECE alloc / 005C3EF8 00406D80 / 005C3EFF 00413CC0 / 005C3F1C 004143B0 / 005C3F25 ret 1
}
// 005C3F50 CTwoDimArrays::Load2DArrays_Formations (005C3F50..005C3FED), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3f50(void* self, void*) {
    return load_table(self, 0xFC, kotor_rules_str_0074bcbc);  // 005C3F6E alloc / 005C3F98 00406D80 / 005C3F9F 00413CC0 / 005C3FBC 004143B0 / 005C3FC5 ret 1
}
// 005C3FF0 CTwoDimArrays::Load2DArrays_GrenadeSound (005C3FF0..005C408D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c3ff0(void* self, void*) {
    return load_table(self, 0x100, kotor_rules_str_0074bcc8);  // 005C400E alloc / 005C4038 00406D80 / 005C403F 00413CC0 / 005C405C 004143B0 / 005C4065 ret 1
}
// 005C4090 CTwoDimArrays::Load2DArrays_Planetary (005C4090..005C412D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4090(void* self, void*) {
    return load_table(self, 0x104, kotor_rules_str_0074bcd4);  // 005C40AE alloc / 005C40D8 00406D80 / 005C40DF 00413CC0 / 005C40FC 004143B0 / 005C4105 ret 1
}
// 005C4130 CTwoDimArrays::Load2DArrays_FeedBackText (005C4130..005C41CD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4130(void* self, void*) {
    return load_table(self, 0x108, kotor_rules_str_0074bce0);  // 005C414E alloc / 005C4178 00406D80 / 005C417F 00413CC0 / 005C419C 004143B0 / 005C41A5 ret 1
}
// 005C41D0 CTwoDimArrays::Load2DArrays_ForceShields (005C41D0..005C426D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c41d0(void* self, void*) {
    return load_table(self, 0x10C, kotor_rules_str_0074bcf0);  // 005C41EE alloc / 005C4218 00406D80 / 005C421F 00413CC0 / 005C423C 004143B0 / 005C4245 ret 1
}
// 005C4270 CTwoDimArrays::Load2DArrays_Heads (005C4270..005C430A), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4270(void* self, void*) {
    return load_table(self, 0x44, kotor_rules_str_0074bd00);  // 005C428E alloc / 005C42B8 00406D80 / 005C42BF 00413CC0 / 005C42D9 004143B0 / 005C42E2 ret 1
}
// 005C4310 CTwoDimArrays::Load2DArrays_ItemValues (005C4310..005C43AD), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4310(void* self, void*) {
    return load_table(self, 0x114, kotor_rules_str_0074bd08);  // 005C432E alloc / 005C4358 00406D80 / 005C435F 00413CC0 / 005C437C 004143B0 / 005C4385 ret 1
}
// 005C43B0 CTwoDimArrays::Load2DArrays_VideoEffects (005C43B0..005C444D), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c43b0(void* self, void*) {
    return load_table(self, 0x110, kotor_rules_str_0074bd14);  // 005C43CE alloc / 005C43F8 00406D80 / 005C43FF 00413CC0 / 005C441C 004143B0 / 005C4425 ret 1
}
// 005C4450 CTwoDimArrays::Load2DArrays_Movies (005C4450..005C44ED), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4450(void* self, void*) {
    return load_table(self, 0x11C, kotor_rules_str_0074bd24);  // 005C446E alloc / 005C4498 00406D80 / 005C449F 00413CC0 / 005C44BC 004143B0 / 005C44C5 ret 1
}
// 005C44F0 CTwoDimArrays::Load2DArrays_EffectIcon (005C44F0..005C45A7), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c44f0(void* self, void*) {
    return load_or_discard(self, 0x120, kotor_rules_str_0074bd2c);  // 005C450E alloc / 005C4538 00406D80 / 005C453F 00413CC0 / store / 005C455C 004143B0 / 005C4573 [vtbl+0](1) / 005C4575 field=0
}
// 005C45B0 CTwoDimArrays::Load2DArrays_RemoveFXOnDeath (005C45B0..005C4667), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c45b0(void* self, void*) {
    return load_or_discard(self, 0x124, kotor_rules_str_0074bd38);  // 005C45CE alloc / 005C45F8 00406D80 / 005C45FF 00413CC0 / store / 005C461C 004143B0 / 005C4633 [vtbl+0](1) / 005C4635 field=0
}
// 005C4670 CTwoDimArrays::Load2DArrays_TexPacks (005C4670..005C4727), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4670(void* self, void*) {
    return load_or_discard(self, 0x128, kotor_rules_str_0074bd48);  // 005C468E alloc / 005C46B8 00406D80 / 005C46BF 00413CC0 / store / 005C46DC 004143B0 / 005C46F3 [vtbl+0](1) / 005C46F5 field=0
}

// ---------------------------------------------------------------- IPRP tables
// 005C4730 CTwoDimArrays::LoadIPRPCostTables (005C4730..005C493F), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c4730(void* self, void*) {
    Table* const index = new_c2da(kotor_rules_str_0074bd60);              // 005C4752..005C478C (EH state 0)
    if (index == nullptr) return 0;                                       // 005C4792/005C479C
    if (c2da_current_load_004143b0(index, nullptr) == 0) return 0;        // 005C47A2..005C47AB (index table not deleted: original)
    wr8(self, 0x08, static_cast<unsigned char>(rd32(index, 0x24)));       // 005C47B1/005C47B8 byte <- C2DA rows
    String name;                                                          // 005C47BB 005B3190, EH state 1
    put_ptr(self, 0x00, live_operator_new(static_cast<Word>(rd8(self, 0x08)) * 4u)); // 005C47C0..005C47D5 (no null test)
    for (int i = 0; i < static_cast<int>(rd8(self, 0x08)); ++i) {         // 005C47DD..005C47E1 / 005C48D2..005C48D9 (count re-read)
        set_element(self, 0x00, i, nullptr);                              // 005C47F0/005C47F2
        std::int32_t present = 1;                                         // 005C4800 [esp+14h]=1
        if (kotor_rules_cell8_007a39dc == 1) {                            // 005C47F9/005C4808 (image value 0: not taken)
            int_entry(index, i, kotor_rules_str_0074bd54, &present);      // 005C480A..005C4838 (EH state 2)
            if (present == 0) continue;                                   // 005C483D..005C4843 -> 005C48D2
        }
        (void)string_entry(index, i, kotor_rules_str_00745eec, &name);    // 005C4849..005C4877 00413270 (result ignored; EH state 3)
        set_element(self, 0x00, i, new_c2da_from(&name));                 // 005C487C..005C48BC (EH state 4)
        // 005C48BF..005C48C9: Load2DArray on the re-read element, also when the 54h
        // allocation failed (ECX=0, as the original).
        if (c2da_current_load_004143b0(element(self, 0x00, i), nullptr) == 0) {
            delete_c2da(index);                                           // 005C4911..005C4918
            return 0;                                                     // 005C4926 ~name, 005C4930
        }
    }
    delete_c2da(index);                                                   // 005C48DF..005C48E6 [vtbl+0](1)
    return 1;                                                             // 005C48F4 ~name, 005C48F9
}

// 005C49C0 CTwoDimArrays::LoadIPRPParamTables (005C49C0..005C4B80), RET, EAX 0/1.
int __fastcall kotor_fastcall_005c49c0(void* self, void*) {
    Table* const index = new_c2da(kotor_rules_str_0074bd7c);              // 005C49E2..005C4A1C (EH state 0)
    if (index == nullptr) return 0;                                       // 005C4A22/005C4A2C
    if (c2da_current_load_004143b0(index, nullptr) == 0) return 0;        // 005C4A32..005C4A3B (index table not deleted: original)
    wr8(self, 0x09, static_cast<unsigned char>(rd32(index, 0x24)));       // 005C4A41/005C4A48
    String name;                                                          // 005C4A4B 005B3190, EH state 1
    put_ptr(self, 0x04, live_operator_new(static_cast<Word>(rd8(self, 0x09)) * 4u)); // 005C4A50..005C4A65 (no null test)
    for (int i = 0; i < static_cast<int>(rd8(self, 0x09)); ++i) {         // 005C4A6E..005C4A72 / 005C4B15..005C4B1C
        set_element(self, 0x04, i, nullptr);                              // 005C4A80/005C4A83
        (void)string_entry(index, i, kotor_rules_str_0074bd70, &name);    // 005C4A8A..005C4AB8 00413270 (EH state 2)
        set_element(self, 0x04, i, new_c2da_from(&name));                 // 005C4ABD..005C4AFE (EH state 3)
        if (c2da_current_load_004143b0(element(self, 0x04, i), nullptr) == 0) { // 005C4B01..005C4B13
            delete_c2da(index);                                           // 005C4B53..005C4B59
            return 0;                                                     // 005C4B67 ~name
        }
    }
    delete_c2da(index);                                                   // 005C4B22..005C4B28
    return 1;                                                             // 005C4B36 ~name, 005C4B3B
}

// 005C4940 CTwoDimArrays::UnLoadIPRPCostTables (005C4940..005C4999), RET, EAX 1.
int __fastcall kotor_fastcall_005c4940(void* self, void*) {
    if (rd32(self, 0x00) == 0) return 1;                                  // 005C4943/005C4946 -> 005C4993
    for (int i = 0; i < static_cast<int>(rd8(self, 0x08)); ++i) {         // 005C494C..005C4950 / 005C4978..005C497F
        if (Table* const t = element(self, 0x00, i)) {                    // 005C4952..005C4959
            c2da_current_unload_004139e0(t, nullptr);                     // 005C495B
            if (Table* const again = element(self, 0x00, i)) delete_c2da(again); // 005C4960..005C496D
            set_element(self, 0x00, i, nullptr);                          // 005C496F/005C4971
        }
    }
    live_free(ptr_at<void>(self, 0x00));                                  // 005C4981..005C4984 006FA390
    wr32(self, 0x00, 0);                                                  // 005C498C
    return 1;                                                             // 005C4993
}

// 005C4B90 CTwoDimArrays::UnLoadIPRPParamTables (005C4B90..005C4BF0), RET, EAX 1.
int __fastcall kotor_fastcall_005c4b90(void* self, void*) {
    if (rd32(self, 0x04) == 0) return 1;                                  // 005C4B93..005C4B98 -> 005C4BEA
    for (int i = 0; i < static_cast<int>(rd8(self, 0x09)); ++i) {         // 005C4B9E..005C4BA2 / 005C4BCD..005C4BD4
        if (Table* const t = element(self, 0x04, i)) {                    // 005C4BA4..005C4BAC
            c2da_current_unload_004139e0(t, nullptr);                     // 005C4BAE
            if (Table* const again = element(self, 0x04, i)) delete_c2da(again); // 005C4BB3..005C4BC1
            set_element(self, 0x04, i, nullptr);                          // 005C4BC3/005C4BC6
        }
    }
    live_free(ptr_at<void>(self, 0x04));                                  // 005C4BD6..005C4BDA
    wr32(self, 0x04, 0);                                                  // 005C4BE2
    return 1;                                                             // 005C4BEA
}

// ---------------------------------------------------------------- Load2DArrays
// 005C4C20 CTwoDimArrays::Load2DArrays (005C4C20..005C506A), RET, EAX 0/1. The two IPRP
// loaders' results are ignored (005C4C23/005C4C2A: no test); every later loader returning
// 0 ends the chain with 0 (je 005C5060). Gates on byte 007A39DC (image value 0, no writer):
// ==2 skips 005C1FA0..005C2720 (005C4D1F), ==1 returns 1 before 005C27C0 (005C4DFE).
int __fastcall kotor_fastcall_005c4c20(void* self, void*) {
    kotor_fastcall_005c4730(self, nullptr);                               // 005C4C23
    kotor_fastcall_005c49c0(self, nullptr);                               // 005C4C2A
    using Loader = int (__fastcall*)(void*, void*);
    static const Loader first[] = {                                       // 005C4C31..005C4D19
        kotor_fastcall_005c0270, kotor_fastcall_005c0ae0, kotor_fastcall_005c0bb0, kotor_fastcall_005c0c80,
        kotor_fastcall_005c1010, kotor_fastcall_005c1580, kotor_fastcall_005c1690, kotor_fastcall_005c1760,
        kotor_fastcall_005c1830, kotor_fastcall_005c1b40, kotor_fastcall_005c1be0, kotor_fastcall_005c1c80,
        kotor_fastcall_005c1d20, kotor_fastcall_005c1dc0, kotor_fastcall_005c1e60, kotor_fastcall_005c1f00};
    static const Loader client_only[] = {                                 // 005C4D2C..005C4DF8 (skipped when [007A39DC]==2)
        kotor_fastcall_005c1fa0, kotor_fastcall_005c2040, kotor_fastcall_005c20e0, kotor_fastcall_005c2180,
        kotor_fastcall_005c08f0, kotor_fastcall_005c2290, kotor_fastcall_005c2360, kotor_fastcall_005c2400,
        kotor_fastcall_005c24a0, kotor_fastcall_005c1980, kotor_fastcall_005c2540, kotor_fastcall_005c25e0,
        kotor_fastcall_005c2680, kotor_fastcall_005c2720};
    static const Loader rest[] = {                                        // 005C4E0B..005C505E (skipped when [007A39DC]==1)
        kotor_fastcall_005c27c0, kotor_fastcall_005c2860, kotor_fastcall_005c2900, kotor_fastcall_005c29a0,
        kotor_fastcall_005c2a40, kotor_fastcall_005c2ae0, kotor_fastcall_005c2b80, kotor_fastcall_005c2c20,
        kotor_fastcall_005c2d40, kotor_fastcall_005c2de0, kotor_fastcall_005c2e80, kotor_fastcall_005c2f20,
        kotor_fastcall_005c2fc0, kotor_fastcall_005c3060, kotor_fastcall_005c3100, kotor_fastcall_005c31a0,
        kotor_fastcall_005c3240, kotor_fastcall_005c32e0, kotor_fastcall_005c3670, kotor_fastcall_005c3710,
        kotor_fastcall_005c3830, kotor_fastcall_005c3830 /* 005C4F39 and 005C4F48: called twice (original) */,
        kotor_fastcall_005c38d0, kotor_fastcall_005c3970, kotor_fastcall_005c3a10, kotor_fastcall_005c3ab0,
        kotor_fastcall_005c3b50, kotor_fastcall_005c3bf0, kotor_fastcall_005c3d70, kotor_fastcall_005c3e10,
        kotor_fastcall_005c3eb0, kotor_fastcall_005c3f50, kotor_fastcall_005c3ff0, kotor_fastcall_005c4090,
        kotor_fastcall_005c4130, kotor_fastcall_005c41d0, kotor_fastcall_005c4270, kotor_fastcall_005c4310,
        kotor_fastcall_005c43b0, kotor_fastcall_005c4450, kotor_fastcall_005c44f0, kotor_fastcall_005c45b0,
        kotor_fastcall_005c4670};
    for (Loader f : first) if (f(self, nullptr) == 0) return 0;           // test eax,eax / je 005C5060
    if (kotor_rules_cell8_007a39dc != 2)                                  // 005C4D1F/005C4D26
        for (Loader f : client_only) if (f(self, nullptr) == 0) return 0;
    if (kotor_rules_cell8_007a39dc == 1) return 1;                        // 005C4DFE/005C4E05 -> 005C5064
    for (Loader f : rest) if (f(self, nullptr) == 0) return 0;            // ... 005C505E jne 005C5064
    return 1;                                                             // 005C5064
}

// ---------------------------------------------------------------- shutdown (not first frame)
// 005C5070 CTwoDimArrays::UnLoad2DArrays (005C5070..005C53D0), RET, EAX 1: IPRP unloads,
// then C2DA::Unload2DArray 004139E0 on every non-null table field in the original order.
int __fastcall kotor_fastcall_005c5070(void* self, void*) {
    kotor_fastcall_005c4940(self, nullptr);                               // 005C5073
    kotor_fastcall_005c4b90(self, nullptr);                               // 005C507A
    if (Table* t = ptr_at<Table>(self, 0x124)) c2da_current_unload_004139e0(t, nullptr);  // 005C507F / 005C5089
    if (Table* t = ptr_at<Table>(self, 0x120)) c2da_current_unload_004139e0(t, nullptr);  // 005C508E / 005C5098
    if (Table* t = ptr_at<Table>(self, 0xC)) c2da_current_unload_004139e0(t, nullptr);  // 005C509D / 005C50A4
    if (Table* t = ptr_at<Table>(self, 0x10)) c2da_current_unload_004139e0(t, nullptr);  // 005C50A9 / 005C50B0
    if (Table* t = ptr_at<Table>(self, 0x14)) c2da_current_unload_004139e0(t, nullptr);  // 005C50B5 / 005C50BC
    if (Table* t = ptr_at<Table>(self, 0x84)) c2da_current_unload_004139e0(t, nullptr);  // 005C50C1 / 005C50CB
    if (Table* t = ptr_at<Table>(self, 0x88)) c2da_current_unload_004139e0(t, nullptr);  // 005C50D0 / 005C50DA
    if (Table* t = ptr_at<Table>(self, 0x18)) c2da_current_unload_004139e0(t, nullptr);  // 005C50DF / 005C50E6
    if (Table* t = ptr_at<Table>(self, 0x8C)) c2da_current_unload_004139e0(t, nullptr);  // 005C50EB / 005C50F5
    if (Table* t = ptr_at<Table>(self, 0x1C)) c2da_current_unload_004139e0(t, nullptr);  // 005C50FA / 005C5101
    if (Table* t = ptr_at<Table>(self, 0x20)) c2da_current_unload_004139e0(t, nullptr);  // 005C5106 / 005C510D
    if (Table* t = ptr_at<Table>(self, 0x24)) c2da_current_unload_004139e0(t, nullptr);  // 005C5112 / 005C5119
    if (Table* t = ptr_at<Table>(self, 0x2C)) c2da_current_unload_004139e0(t, nullptr);  // 005C511E / 005C5125
    if (Table* t = ptr_at<Table>(self, 0x28)) c2da_current_unload_004139e0(t, nullptr);  // 005C512A / 005C5131
    if (Table* t = ptr_at<Table>(self, 0x90)) c2da_current_unload_004139e0(t, nullptr);  // 005C5136 / 005C5140
    if (Table* t = ptr_at<Table>(self, 0x94)) c2da_current_unload_004139e0(t, nullptr);  // 005C5145 / 005C514F
    if (Table* t = ptr_at<Table>(self, 0x98)) c2da_current_unload_004139e0(t, nullptr);  // 005C5154 / 005C515E
    if (Table* t = ptr_at<Table>(self, 0x9C)) c2da_current_unload_004139e0(t, nullptr);  // 005C5163 / 005C516D
    if (Table* t = ptr_at<Table>(self, 0xA8)) c2da_current_unload_004139e0(t, nullptr);  // 005C5172 / 005C517C
    if (Table* t = ptr_at<Table>(self, 0xA0)) c2da_current_unload_004139e0(t, nullptr);  // 005C5181 / 005C518B
    if (Table* t = ptr_at<Table>(self, 0xA4)) c2da_current_unload_004139e0(t, nullptr);  // 005C5190 / 005C519A
    if (Table* t = ptr_at<Table>(self, 0x48)) c2da_current_unload_004139e0(t, nullptr);  // 005C519F / 005C51A6
    if (Table* t = ptr_at<Table>(self, 0x30)) c2da_current_unload_004139e0(t, nullptr);  // 005C51AB / 005C51B2
    if (Table* t = ptr_at<Table>(self, 0x34)) c2da_current_unload_004139e0(t, nullptr);  // 005C51B7 / 005C51BE
    if (Table* t = ptr_at<Table>(self, 0xC0)) c2da_current_unload_004139e0(t, nullptr);  // 005C51C3 / 005C51CD
    if (Table* t = ptr_at<Table>(self, 0x4C)) c2da_current_unload_004139e0(t, nullptr);  // 005C51D2 / 005C51D9
    if (Table* t = ptr_at<Table>(self, 0xAC)) c2da_current_unload_004139e0(t, nullptr);  // 005C51DE / 005C51E8
    if (Table* t = ptr_at<Table>(self, 0xB0)) c2da_current_unload_004139e0(t, nullptr);  // 005C51ED / 005C51F7
    if (Table* t = ptr_at<Table>(self, 0xB4)) c2da_current_unload_004139e0(t, nullptr);  // 005C51FC / 005C5206
    if (Table* t = ptr_at<Table>(self, 0xB8)) c2da_current_unload_004139e0(t, nullptr);  // 005C520B / 005C5215
    if (Table* t = ptr_at<Table>(self, 0xBC)) c2da_current_unload_004139e0(t, nullptr);  // 005C521A / 005C5224
    if (Table* t = ptr_at<Table>(self, 0x50)) c2da_current_unload_004139e0(t, nullptr);  // 005C5229 / 005C5230
    if (rd32(self, 0x54) != 0) c2da_current_unload_004139e0(ptr_at<Table>(self, 0x50), nullptr);  // 005C5235 tests +54h, 005C523F unloads +50h (original quirk, kept)
    if (Table* t = ptr_at<Table>(self, 0x58)) c2da_current_unload_004139e0(t, nullptr);  // 005C5244 / 005C524B
    if (Table* t = ptr_at<Table>(self, 0x5C)) c2da_current_unload_004139e0(t, nullptr);  // 005C5250 / 005C5257
    if (Table* t = ptr_at<Table>(self, 0x60)) c2da_current_unload_004139e0(t, nullptr);  // 005C525C / 005C5263
    if (Table* t = ptr_at<Table>(self, 0x64)) c2da_current_unload_004139e0(t, nullptr);  // 005C5268 / 005C526F
    if (Table* t = ptr_at<Table>(self, 0x68)) c2da_current_unload_004139e0(t, nullptr);  // 005C5274 / 005C527B
    if (Table* t = ptr_at<Table>(self, 0x6C)) c2da_current_unload_004139e0(t, nullptr);  // 005C5280 / 005C5287
    if (Table* t = ptr_at<Table>(self, 0xC8)) c2da_current_unload_004139e0(t, nullptr);  // 005C528C / 005C5296
    if (Table* t = ptr_at<Table>(self, 0xCC)) c2da_current_unload_004139e0(t, nullptr);  // 005C529B / 005C52A5
    if (Table* t = ptr_at<Table>(self, 0xD0)) c2da_current_unload_004139e0(t, nullptr);  // 005C52AA / 005C52B4
    if (Table* t = ptr_at<Table>(self, 0x70)) c2da_current_unload_004139e0(t, nullptr);  // 005C52B9 / 005C52C0
    if (Table* t = ptr_at<Table>(self, 0xD4)) c2da_current_unload_004139e0(t, nullptr);  // 005C52C5 / 005C52CF
    if (Table* t = ptr_at<Table>(self, 0x38)) c2da_current_unload_004139e0(t, nullptr);  // 005C52D4 / 005C52DB
    if (Table* t = ptr_at<Table>(self, 0x74)) c2da_current_unload_004139e0(t, nullptr);  // 005C52E0 / 005C52E7
    if (Table* t = ptr_at<Table>(self, 0xE0)) c2da_current_unload_004139e0(t, nullptr);  // 005C52EC / 005C52F6
    if (Table* t = ptr_at<Table>(self, 0xEC)) c2da_current_unload_004139e0(t, nullptr);  // 005C52FB / 005C5305
    if (Table* t = ptr_at<Table>(self, 0xF0)) c2da_current_unload_004139e0(t, nullptr);  // 005C530A / 005C5314
    if (Table* t = ptr_at<Table>(self, 0xF4)) c2da_current_unload_004139e0(t, nullptr);  // 005C5319 / 005C5323
    if (Table* t = ptr_at<Table>(self, 0xF8)) c2da_current_unload_004139e0(t, nullptr);  // 005C5328 / 005C5332
    if (Table* t = ptr_at<Table>(self, 0xFC)) c2da_current_unload_004139e0(t, nullptr);  // 005C5337 / 005C5341
    if (Table* t = ptr_at<Table>(self, 0x100)) c2da_current_unload_004139e0(t, nullptr);  // 005C5346 / 005C5350
    if (Table* t = ptr_at<Table>(self, 0x104)) c2da_current_unload_004139e0(t, nullptr);  // 005C5355 / 005C535F
    if (Table* t = ptr_at<Table>(self, 0x108)) c2da_current_unload_004139e0(t, nullptr);  // 005C5364 / 005C536E
    if (Table* t = ptr_at<Table>(self, 0x114)) c2da_current_unload_004139e0(t, nullptr);  // 005C5373 / 005C537D
    if (Table* t = ptr_at<Table>(self, 0x110)) c2da_current_unload_004139e0(t, nullptr);  // 005C5382 / 005C538C
    if (Table* t = ptr_at<Table>(self, 0x118)) c2da_current_unload_004139e0(t, nullptr);  // 005C5391 / 005C539B
    if (Table* t = ptr_at<Table>(self, 0x7C)) c2da_current_unload_004139e0(t, nullptr);  // 005C53A0 / 005C53A7
    if (Table* t = ptr_at<Table>(self, 0x80)) c2da_current_unload_004139e0(t, nullptr);  // 005C53AC / 005C53B6
    if (Table* t = ptr_at<Table>(self, 0x128)) c2da_current_unload_004139e0(t, nullptr);  // 005C53BB / 005C53C6
    return 1;                                                             // 005C53CB
}

// 005BFCC0 CTwoDimArrays::~CTwoDimArrays (005BFCC0..005C0269), RET: delete ([vtbl+0](1))
// and zero every table field in the original order; free the two IPRP arrays after +34h.
void __fastcall kotor_fastcall_005bfcc0(void* self, void*) {
    if (Table* t = ptr_at<Table>(self, 0x124)) delete_c2da(t);  // 005BFCC4 / 005BFCD4 [vtbl+0](1)
    wr32(self, 0x124, 0);                                       // 005BFCDE
    if (Table* t = ptr_at<Table>(self, 0x120)) delete_c2da(t);  // 005BFCD6 / 005BFCEA [vtbl+0](1)
    wr32(self, 0x120, 0);                                       // 005BFCF1
    if (Table* t = ptr_at<Table>(self, 0xC)) delete_c2da(t);  // 005BFCEC / 005BFCFD [vtbl+0](1)
    wr32(self, 0xC, 0);                                       // 005BFD04
    if (Table* t = ptr_at<Table>(self, 0x10)) delete_c2da(t);  // 005BFCFF / 005BFD0D [vtbl+0](1)
    wr32(self, 0x10, 0);                                       // 005BFD14
    if (Table* t = ptr_at<Table>(self, 0x14)) delete_c2da(t);  // 005BFD0F / 005BFD1D [vtbl+0](1)
    wr32(self, 0x14, 0);                                       // 005BFD27
    if (Table* t = ptr_at<Table>(self, 0x84)) delete_c2da(t);  // 005BFD1F / 005BFD30 [vtbl+0](1)
    wr32(self, 0x84, 0);                                       // 005BFD3A
    if (Table* t = ptr_at<Table>(self, 0x88)) delete_c2da(t);  // 005BFD32 / 005BFD46 [vtbl+0](1)
    wr32(self, 0x88, 0);                                       // 005BFD4D
    if (Table* t = ptr_at<Table>(self, 0x18)) delete_c2da(t);  // 005BFD48 / 005BFD59 [vtbl+0](1)
    wr32(self, 0x18, 0);                                       // 005BFD63
    if (Table* t = ptr_at<Table>(self, 0x8C)) delete_c2da(t);  // 005BFD5B / 005BFD6C [vtbl+0](1)
    wr32(self, 0x8C, 0);                                       // 005BFD73
    if (Table* t = ptr_at<Table>(self, 0x1C)) delete_c2da(t);  // 005BFD6E / 005BFD7F [vtbl+0](1)
    wr32(self, 0x1C, 0);                                       // 005BFD86
    if (Table* t = ptr_at<Table>(self, 0x20)) delete_c2da(t);  // 005BFD81 / 005BFD8F [vtbl+0](1)
    wr32(self, 0x20, 0);                                       // 005BFD96
    if (Table* t = ptr_at<Table>(self, 0x24)) delete_c2da(t);  // 005BFD91 / 005BFD9F [vtbl+0](1)
    wr32(self, 0x24, 0);                                       // 005BFDA6
    if (Table* t = ptr_at<Table>(self, 0x2C)) delete_c2da(t);  // 005BFDA1 / 005BFDAF [vtbl+0](1)
    wr32(self, 0x2C, 0);                                       // 005BFDB6
    if (Table* t = ptr_at<Table>(self, 0x28)) delete_c2da(t);  // 005BFDB1 / 005BFDBF [vtbl+0](1)
    wr32(self, 0x28, 0);                                       // 005BFDC9
    if (Table* t = ptr_at<Table>(self, 0x90)) delete_c2da(t);  // 005BFDC1 / 005BFDD2 [vtbl+0](1)
    wr32(self, 0x90, 0);                                       // 005BFDDC
    if (Table* t = ptr_at<Table>(self, 0x94)) delete_c2da(t);  // 005BFDD4 / 005BFDE8 [vtbl+0](1)
    wr32(self, 0x94, 0);                                       // 005BFDF2
    if (Table* t = ptr_at<Table>(self, 0x98)) delete_c2da(t);  // 005BFDEA / 005BFDFE [vtbl+0](1)
    wr32(self, 0x98, 0);                                       // 005BFE08
    if (Table* t = ptr_at<Table>(self, 0x9C)) delete_c2da(t);  // 005BFE00 / 005BFE14 [vtbl+0](1)
    wr32(self, 0x9C, 0);                                       // 005BFE1E
    if (Table* t = ptr_at<Table>(self, 0xA8)) delete_c2da(t);  // 005BFE16 / 005BFE2A [vtbl+0](1)
    wr32(self, 0xA8, 0);                                       // 005BFE34
    if (Table* t = ptr_at<Table>(self, 0xA0)) delete_c2da(t);  // 005BFE2C / 005BFE40 [vtbl+0](1)
    wr32(self, 0xA0, 0);                                       // 005BFE4A
    if (Table* t = ptr_at<Table>(self, 0xA4)) delete_c2da(t);  // 005BFE42 / 005BFE56 [vtbl+0](1)
    wr32(self, 0xA4, 0);                                       // 005BFE5D
    if (Table* t = ptr_at<Table>(self, 0x48)) delete_c2da(t);  // 005BFE58 / 005BFE69 [vtbl+0](1)
    wr32(self, 0x48, 0);                                       // 005BFE70
    if (Table* t = ptr_at<Table>(self, 0x30)) delete_c2da(t);  // 005BFE6B / 005BFE79 [vtbl+0](1)
    wr32(self, 0x30, 0);                                       // 005BFE83
    if (Table* t = ptr_at<Table>(self, 0xC0)) delete_c2da(t);  // 005BFE7B / 005BFE8C [vtbl+0](1)
    wr32(self, 0xC0, 0);                                       // 005BFE93
    if (Table* t = ptr_at<Table>(self, 0x4C)) delete_c2da(t);  // 005BFE8E / 005BFE9F [vtbl+0](1)
    wr32(self, 0x4C, 0);                                       // 005BFEA9
    if (Table* t = ptr_at<Table>(self, 0xAC)) delete_c2da(t);  // 005BFEA1 / 005BFEB2 [vtbl+0](1)
    wr32(self, 0xAC, 0);                                       // 005BFEBC
    if (Table* t = ptr_at<Table>(self, 0xB0)) delete_c2da(t);  // 005BFEB4 / 005BFEC8 [vtbl+0](1)
    wr32(self, 0xB0, 0);                                       // 005BFED2
    if (Table* t = ptr_at<Table>(self, 0xB4)) delete_c2da(t);  // 005BFECA / 005BFEDE [vtbl+0](1)
    wr32(self, 0xB4, 0);                                       // 005BFEE8
    if (Table* t = ptr_at<Table>(self, 0xB8)) delete_c2da(t);  // 005BFEE0 / 005BFEF4 [vtbl+0](1)
    wr32(self, 0xB8, 0);                                       // 005BFEFE
    if (Table* t = ptr_at<Table>(self, 0xBC)) delete_c2da(t);  // 005BFEF6 / 005BFF0A [vtbl+0](1)
    wr32(self, 0xBC, 0);                                       // 005BFF14
    if (Table* t = ptr_at<Table>(self, 0xC4)) delete_c2da(t);  // 005BFF0C / 005BFF20 [vtbl+0](1)
    wr32(self, 0xC4, 0);                                       // 005BFF27
    if (Table* t = ptr_at<Table>(self, 0x34)) delete_c2da(t);  // 005BFF22 / 005BFF33 [vtbl+0](1)
    wr32(self, 0x34, 0);                                       // 005BFF38
    live_free(ptr_at<void>(self, 0x00));   // 005BFF35..005BFF3B (push [esi] / call 006FA390)
    wr32(self, 0x00, 0); wr8(self, 0x08, 0); // 005BFF44 / 005BFF46
    live_free(ptr_at<void>(self, 0x04));   // 005BFF40..005BFF49
    wr32(self, 0x04, 0); wr8(self, 0x09, 0); // 005BFF56 / 005BFF59
    if (Table* t = ptr_at<Table>(self, 0x50)) delete_c2da(t);  // 005BFF4E / 005BFF62 [vtbl+0](1)
    wr32(self, 0x50, 0);                                       // 005BFF69
    if (Table* t = ptr_at<Table>(self, 0x54)) delete_c2da(t);  // 005BFF64 / 005BFF72 [vtbl+0](1)
    wr32(self, 0x54, 0);                                       // 005BFF79
    if (Table* t = ptr_at<Table>(self, 0x58)) delete_c2da(t);  // 005BFF74 / 005BFF82 [vtbl+0](1)
    wr32(self, 0x58, 0);                                       // 005BFF89
    if (Table* t = ptr_at<Table>(self, 0x5C)) delete_c2da(t);  // 005BFF84 / 005BFF92 [vtbl+0](1)
    wr32(self, 0x5C, 0);                                       // 005BFF99
    if (Table* t = ptr_at<Table>(self, 0x60)) delete_c2da(t);  // 005BFF94 / 005BFFA2 [vtbl+0](1)
    wr32(self, 0x60, 0);                                       // 005BFFA9
    if (Table* t = ptr_at<Table>(self, 0x64)) delete_c2da(t);  // 005BFFA4 / 005BFFB2 [vtbl+0](1)
    wr32(self, 0x64, 0);                                       // 005BFFB9
    if (Table* t = ptr_at<Table>(self, 0x68)) delete_c2da(t);  // 005BFFB4 / 005BFFC2 [vtbl+0](1)
    wr32(self, 0x68, 0);                                       // 005BFFC9
    if (Table* t = ptr_at<Table>(self, 0x6C)) delete_c2da(t);  // 005BFFC4 / 005BFFD2 [vtbl+0](1)
    wr32(self, 0x6C, 0);                                       // 005BFFDC
    if (Table* t = ptr_at<Table>(self, 0xC8)) delete_c2da(t);  // 005BFFD4 / 005BFFE5 [vtbl+0](1)
    wr32(self, 0xC8, 0);                                       // 005BFFEF
    if (Table* t = ptr_at<Table>(self, 0xCC)) delete_c2da(t);  // 005BFFE7 / 005BFFFB [vtbl+0](1)
    wr32(self, 0xCC, 0);                                       // 005C0005
    if (Table* t = ptr_at<Table>(self, 0xD0)) delete_c2da(t);  // 005BFFFD / 005C0011 [vtbl+0](1)
    wr32(self, 0xD0, 0);                                       // 005C0018
    if (Table* t = ptr_at<Table>(self, 0x70)) delete_c2da(t);  // 005C0013 / 005C0024 [vtbl+0](1)
    wr32(self, 0x70, 0);                                       // 005C002E
    if (Table* t = ptr_at<Table>(self, 0xD4)) delete_c2da(t);  // 005C0026 / 005C0037 [vtbl+0](1)
    wr32(self, 0xD4, 0);                                       // 005C003E
    if (Table* t = ptr_at<Table>(self, 0x38)) delete_c2da(t);  // 005C0039 / 005C004A [vtbl+0](1)
    wr32(self, 0x38, 0);                                       // 005C0054
    if (Table* t = ptr_at<Table>(self, 0xD8)) delete_c2da(t);  // 005C004C / 005C005D [vtbl+0](1)
    wr32(self, 0xD8, 0);                                       // 005C0067
    if (Table* t = ptr_at<Table>(self, 0xDC)) delete_c2da(t);  // 005C005F / 005C0073 [vtbl+0](1)
    wr32(self, 0xDC, 0);                                       // 005C007A
    if (Table* t = ptr_at<Table>(self, 0x74)) delete_c2da(t);  // 005C0075 / 005C0086 [vtbl+0](1)
    wr32(self, 0x74, 0);                                       // 005C0090
    if (Table* t = ptr_at<Table>(self, 0xE0)) delete_c2da(t);  // 005C0088 / 005C0099 [vtbl+0](1)
    wr32(self, 0xE0, 0);                                       // 005C00A3
    if (Table* t = ptr_at<Table>(self, 0xE4)) delete_c2da(t);  // 005C009B / 005C00AF [vtbl+0](1)
    wr32(self, 0xE4, 0);                                       // 005C00B9
    if (Table* t = ptr_at<Table>(self, 0xE8)) delete_c2da(t);  // 005C00B1 / 005C00C5 [vtbl+0](1)
    wr32(self, 0xE8, 0);                                       // 005C00CF
    if (Table* t = ptr_at<Table>(self, 0xEC)) delete_c2da(t);  // 005C00C7 / 005C00DB [vtbl+0](1)
    wr32(self, 0xEC, 0);                                       // 005C00E2
    if (Table* t = ptr_at<Table>(self, 0x3C)) delete_c2da(t);  // 005C00DD / 005C00EE [vtbl+0](1)
    wr32(self, 0x3C, 0);                                       // 005C00F5
    if (Table* t = ptr_at<Table>(self, 0x78)) delete_c2da(t);  // 005C00F0 / 005C00FE [vtbl+0](1)
    wr32(self, 0x78, 0);                                       // 005C0105
    if (Table* t = ptr_at<Table>(self, 0x40)) delete_c2da(t);  // 005C0100 / 005C010E [vtbl+0](1)
    wr32(self, 0x40, 0);                                       // 005C0118
    if (Table* t = ptr_at<Table>(self, 0xF0)) delete_c2da(t);  // 005C0110 / 005C0121 [vtbl+0](1)
    wr32(self, 0xF0, 0);                                       // 005C012B
    if (Table* t = ptr_at<Table>(self, 0xF4)) delete_c2da(t);  // 005C0123 / 005C0137 [vtbl+0](1)
    wr32(self, 0xF4, 0);                                       // 005C0141
    if (Table* t = ptr_at<Table>(self, 0xF8)) delete_c2da(t);  // 005C0139 / 005C014D [vtbl+0](1)
    wr32(self, 0xF8, 0);                                       // 005C0157
    if (Table* t = ptr_at<Table>(self, 0xFC)) delete_c2da(t);  // 005C014F / 005C0163 [vtbl+0](1)
    wr32(self, 0xFC, 0);                                       // 005C016D
    if (Table* t = ptr_at<Table>(self, 0x100)) delete_c2da(t);  // 005C0165 / 005C0179 [vtbl+0](1)
    wr32(self, 0x100, 0);                                       // 005C0183
    if (Table* t = ptr_at<Table>(self, 0x104)) delete_c2da(t);  // 005C017B / 005C018F [vtbl+0](1)
    wr32(self, 0x104, 0);                                       // 005C0199
    if (Table* t = ptr_at<Table>(self, 0x108)) delete_c2da(t);  // 005C0191 / 005C01A5 [vtbl+0](1)
    wr32(self, 0x108, 0);                                       // 005C01AF
    if (Table* t = ptr_at<Table>(self, 0x10C)) delete_c2da(t);  // 005C01A7 / 005C01BB [vtbl+0](1)
    wr32(self, 0x10C, 0);                                       // 005C01C2
    if (Table* t = ptr_at<Table>(self, 0x44)) delete_c2da(t);  // 005C01BD / 005C01CE [vtbl+0](1)
    wr32(self, 0x44, 0);                                       // 005C01D8
    if (Table* t = ptr_at<Table>(self, 0x114)) delete_c2da(t);  // 005C01D0 / 005C01E1 [vtbl+0](1)
    wr32(self, 0x114, 0);                                       // 005C01EB
    if (Table* t = ptr_at<Table>(self, 0x110)) delete_c2da(t);  // 005C01E3 / 005C01F7 [vtbl+0](1)
    wr32(self, 0x110, 0);                                       // 005C0201
    if (Table* t = ptr_at<Table>(self, 0x118)) delete_c2da(t);  // 005C01F9 / 005C020D [vtbl+0](1)
    wr32(self, 0x118, 0);                                       // 005C0214
    if (Table* t = ptr_at<Table>(self, 0x7C)) delete_c2da(t);  // 005C020F / 005C0220 [vtbl+0](1)
    wr32(self, 0x7C, 0);                                       // 005C022A
    if (Table* t = ptr_at<Table>(self, 0x80)) delete_c2da(t);  // 005C0222 / 005C0233 [vtbl+0](1)
    wr32(self, 0x80, 0);                                       // 005C023D
    if (Table* t = ptr_at<Table>(self, 0x11C)) delete_c2da(t);  // 005C0235 / 005C0249 [vtbl+0](1)
    wr32(self, 0x11C, 0);                                       // 005C0253
    if (Table* t = ptr_at<Table>(self, 0x128)) delete_c2da(t);  // 005C024B / 005C025F [vtbl+0](1)
    wr32(self, 0x128, 0);                                       // 005C0261
}
}  // namespace kotor_rules
