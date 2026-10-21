// 0886-gates (Claude): faithful bodies for former AREA_GATE / required(0x...) gates of the 0886 cohort unit
// (server_mainloop_native_vm_canonical_cell.cpp, area_native_load_numeric.inc vtables of CSWSArea).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   00508010 CSWSArea::GetIsIDInExcludeList(ulong id, CExoArrayList<CGameObject*>* list)    RET 8   (vtbl +04)
//   0058FD30 CSWArea::SetWind(uchar wind)                                                   RET 4   (vtbl +10)
//   0050C1E0 CSWSArea::HandleTransparentDoors(Vector* a, Vector* b, float z_lo, float z_hi, float width,
//            ulong ignore_id, ulong* blocker_id, int flag, CExoArrayList<CGameObject*>* list, int unused) RET 28h (vtbl +18)
// Globals by symbol: [007A39FC] kotor_menu_window_cells::manager_007a39fc, [007A3A28] kotor_cell_rules_007a3a28.
// .rdata bound to proven values: [00747B78] = "BlockSight" (C string literal).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using U8 = std::uint8_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
inline U rd8(U a) { return *static_cast<const unsigned char*>(ptr(a)); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
extern "C" void* kotor_cell_rules_007a3a28;                                         // [007A3A28] CSWRules*

extern "C" {
// ---------------- existing providers (c14 map, exact decorated arity) ----------------
void* __fastcall w5_server_get_door_by_id_004ae7c0(void* server, void*, U id);             // 004AE7C0 GetDoorByGameObjectID (pf_w5_drive)
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);       // 005E5A90 CExoString(char const*) (0459)
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                         // 005E5C20 ~CExoString (0459)
int __fastcall c2da_current_get_int_00413660(void* table, void*, int row, void* column, int* out); // 00413660 C2DA::GetINTEntry (0376)
void __fastcall aurora_list_add16_00671c00(void* list, void*, void* value);                // 00671C00 CExoArrayList<T*>::Add (0293)
U __fastcall area_no_nonwalk_polys_detailed_00506650(void* area, void*, void* a, void* b, float z_lo, float z_hi,
    float width, U ignore_id, U* blocker_id, int flag8, void* list, int flag10, void* walk_hit, int check_doors); // 00506650 (0526)

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_00508010(void* self, void*, U id, void* list);
U __fastcall kotor_fastcall_0058fd30(void* self, void*, U wind);
U __fastcall kotor_fastcall_0050c1e0(void* self, void*, void* a, void* b, float z_lo, float z_hi, float width,
                                     U ignore_id, U* blocker_id, int flag, void* list, int unused);
}

// ===================================================================================== 00508010
// CSWSArea::GetIsIDInExcludeList: 1 when some element of the pointer list has object id (+4) == id. `this` unused.
U __fastcall kotor_fastcall_00508010(void*, void*, U id, void* list) {
    if (list == nullptr) return 0;                                                 // 00508010..00508018
    const int size = static_cast<int>(rd(word(list) + 4));                         // 0050801A
    if (size <= 0) return 0;                                                       // 0050801D..0050801F (jle, signed)
    U cell = rd(word(list));                                                       // 00508027 data
    for (int i = 0; i < size; ++i, cell += 4) {                                    // 00508021..0050803D
        if (rd(rd(cell) + 4) == id) return 1;                                      // 00508030..00508035 -> 00508046
    }
    return 0;                                                                      // 0050803F..00508043
}

// ===================================================================================== 0058FD30
// CSWArea::SetWind: byte +AB = wind; returns 1.
U __fastcall kotor_fastcall_0058fd30(void* self, void*, U wind) {
    wr8(word(self) + 0xab, wind & 0xff);                                           // 0058FD30..0058FD34
    return 1;                                                                      // 0058FD3A..0058FD3F
}

// ===================================================================================== 0050C1E0
// CSWSArea::HandleTransparentDoors (vtbl +18h, called by NoNonWalkPolysDetailed when its flag10 == 1):
// the blocker found by the caller is a door; when its doortypes/genericdoors "BlockSight" entry is zero and a
// list is given, the door is added to the exclude list and the detailed test is re-run with the same segment.
// Quirk kept: the GetINTEntry output slot is the stack slot of the blocker_id argument (the pointer value is the
// "previous" value), so a failed lookup leaves a nonzero value there and the function returns 0.
U __fastcall kotor_fastcall_0050c1e0(void* self, void*, void* a, void* b, float z_lo, float z_hi, float width,
                                     U ignore_id, U* blocker_id, int flag, void* list, int) {
    const U id = *blocker_id;                                                      // 0050C1FA..0050C1FE
    if (id == 0x7f000000u) return 0;                                               // 0050C201..0050C20A -> 0050C314
    void* const server = *reinterpret_cast<void* const*>(
        static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8); // 0050C210..0050C216
    void* const door = w5_server_get_door_by_id_004ae7c0(server, nullptr, id);    // 0050C219..0050C21A
    if (door == nullptr) return 0;                                                 // 0050C221..0050C223
    int slot = static_cast<int>(word(blocker_id));                                 // [esp+48h]: argument slot reused
    unsigned char column[8];                                                       // CExoString {data,length}
    const U two_da = rd(word(kotor_cell_rules_007a3a28) + 0xb8);                   // [[007A3A28]+B8] CTwoDimArrays
    if (rd8(word(door) + 0x2a0) == 0) {                                            // 0050C229..0050C236
        frame_LogStringCtor_005e5a90(column, nullptr, "BlockSight");               // 0050C231..0050C23C ([00747B78])
        c2da_current_get_int_00413660(ptr(rd(two_da + 0x28)), nullptr,
            static_cast<int>(rd8(word(door) + 0x2a1)), column, &slot);            // 0050C241..0050C26C (result discarded)
    } else {
        frame_LogStringCtor_005e5a90(column, nullptr, "BlockSight");               // 0050C277..0050C27B
        c2da_current_get_int_00413660(ptr(rd(two_da + 0x24)), nullptr,
            static_cast<int>(rd8(word(door) + 0x2a0)), column, &slot);            // 0050C280..0050C2AB
    }
    frame_LogStringDtor_005e5c20(column, nullptr);                                 // 0050C2B4..0050C2BC
    if (slot != 0) return 0;                                                       // 0050C2C1..0050C2C7
    if (list == nullptr) return 0;                                                 // 0050C2C9..0050C2CF
    using Slot10 = void*(__thiscall*)(void*);
    void* const value = reinterpret_cast<Slot10>(rd(rd(word(door)) + 0x10))(door); // 0050C2D1..0050C2D5 vtbl +10
    aurora_list_add16_00671c00(list, nullptr, value);                              // 0050C2D8..0050C2DB
    return area_no_nonwalk_polys_detailed_00506650(self, nullptr, a, b, z_lo, z_hi, width, ignore_id, blocker_id,
                                                   flag, list, 0, nullptr, 0);     // 0050C2E0..0050C30D
}

// =====================================================================================================
// CSWSArea / CSWArea / CResHelper<CResARE,2012> destruction (vtbl slot 0 of 00747BFC / 00749B38 / 0074767C and
// the NVThunk slots of 00747BF8 / 00747B88), reached from the module destructor 004C68A0 (area slot0(1)).
//   0050DFB0 / 0058FF60 / 00506E00 scalar deleting destructors (flags) RET 4, EAX = this
//   0050D350 / 0050D360 thunks: this -= 100h / 11Ch, jmp 0050DFB0
//   0050D370 ~CSWSArea, 0050C0E0 CSWSArea::UnloadArea, 0058F7A0 ~CSWArea, 00506BD0 ~CResHelper  RET 0
//   006056A0 CSWCArea::DetachFromServerArea, 004C3150 CSWSModule::ClearAreaCache,
//   004AEBA0 CServerExoApp::ClearAreaCache -> 004B1860 CServerExoAppInternal::ClearAreaCache      RET 0
// Original vtable stores (each destructor re-installs its class table) bind to the 0886 unit's rebuilt tables
// through claude_0886_area_table(original address) (area_native_load_numeric.inc), never the original address.
// [0073D71C] = "" (C string literal).
// EH: ~CSWSArea registers member-destruction unwind states (10h..0); the bound member destructors do not throw
// in normal operation, so the unwind replay is not reproduced (the normal-path order is kept exactly).
namespace frame_control { extern unsigned resource_manager_007a39e8; }           // [007A39E8] CExoResMan*
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);             // 005E9C80 CExoLinkedListInternal::RemoveHead (0091)
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);             // 005E9AB0 ~CExoLinkedList<uint> (0091)
}
extern "C" {
void __cdecl live_free(void*);                                                    // 006FA390
void* __cdecl claude_0886_area_table(unsigned original);                         // 0886 unit: rebuilt table for an original vtable address
void* __fastcall server_get_module_004ae6b0(void* server, void*);                // 004AE6B0 CServerExoApp::GetModule (0530)
U __fastcall recovered_module_remove_tag_004c5ef0(void* module, void*, void* tag, U id); // 004C5EF0 RemoveObjectFromLookupTable (1011)
void* __fastcall recovered_object_array_004aed70(void* server, void*);           // 004AED70 GetObjectArray
U __fastcall recovered_object_array_erase_004d8290(void* array, void*, U id);    // 004D8290 CGameObjectArray::Delete (0963)
void __fastcall recovered_pathfind_shutdown_005cfd90(void* pathfind, void*);     // 005CFD90 CPathfindInformation::Shutdown
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy); // 006FA852 eh_vector_destructor_iterator (0186)
void __fastcall exo_locstring_destroy_005ea000(void* string, void*);             // 005EA000 ~CExoLocString (0265)
void __fastcall case40_0059a980(void* table, void*);                             // 0059A980 ~CSWSScriptVarTable (0886 unit)
U __fastcall c2da_not_equal_text_00406060(void* ref, void*, const char* text);    // 00406060 CResRef::operator!=(char const*) (0264)
void __fastcall c2da_cancel_00409b00(void* res, void*);                           // 00409B00 CRes::CancelRequest (0264)
U __fastcall c2da_release_00409cf0(void* manager, void*, void* res);             // 00409CF0 CExoResMan::ReleaseResObject (0264)

void* __fastcall kotor_fastcall_0050dfb0(void* self, void*, U flags);
void* __fastcall kotor_fastcall_0050d350(void* self, void*, U flags);
void* __fastcall kotor_fastcall_0050d360(void* self, void*, U flags);
void __fastcall kotor_fastcall_0050d370(void* self, void*);
void __fastcall kotor_fastcall_0050c0e0(void* self, void*);
void* __fastcall kotor_fastcall_0058ff60(void* self, void*, U flags);
void __fastcall kotor_fastcall_0058f7a0(void* self, void*);
void* __fastcall kotor_fastcall_00506e00(void* self, void*, U flags);
void __fastcall kotor_fastcall_00506bd0(void* self, void*);
void __fastcall kotor_fastcall_006056a0(void* client_area, void*);
void __fastcall kotor_fastcall_004c3150(void* module, void*);
void __fastcall kotor_fastcall_004aeba0(void* server, void*);
void __fastcall kotor_fastcall_004b1860(void* internal, void*);
}

namespace {
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void set_table(U object, U original) { wr(object, word(claude_0886_area_table(original))); }
void* server_app() {
    return *reinterpret_cast<void* const*>(static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8);
}
// free(p) of a member followed by the original's zero store
void free_member(U self, U offset) { live_free(ptr(rd(self + offset))); wr(self + offset, 0); }
// delete[] of a CExoString array member (8-byte elements, count cookie at -4)
void delete_strings(U self, U offset) {
    const U data = rd(self + offset);
    if (data != 0) {
        c2da_destroy_array_006fa852(ptr(data), 8, static_cast<int>(rd(data - 4)),
                                    reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
        live_free(ptr(data - 4));
    }
}
}

// ===================================================================================== 0050DFB0 / 0050D350 / 0050D360
void* __fastcall kotor_fastcall_0050dfb0(void* self, void*, U flags) {
    kotor_fastcall_0050d370(self, nullptr);                                       // 0050DFB1..0050DFB3
    if ((flags & 1) != 0) live_free(self);                                        // 0050DFB8..0050DFC0
    return self;                                                                  // 0050DFC8..0050DFCB
}
void* __fastcall kotor_fastcall_0050d350(void* self, void*, U flags) {           // NVThunkAreaResHelper_destructor
    return kotor_fastcall_0050dfb0(static_cast<unsigned char*>(self) - 0x100, nullptr, flags); // 0050D350..0050D356
}
void* __fastcall kotor_fastcall_0050d360(void* self, void*, U flags) {           // NVThunkAreaGameObject_destructor
    return kotor_fastcall_0050dfb0(static_cast<unsigned char*>(self) - 0x11c, nullptr, flags); // 0050D360..0050D366
}

// ===================================================================================== 0050D370 ~CSWSArea
void __fastcall kotor_fastcall_0050d370(void* area, void*) {
    const U s = word(area);
    set_table(s, 0x747bfc);                                                       // 0050D398
    set_table(s + 0x100, 0x747bf8);                                               // 0050D39E
    set_table(s + 0x11c, 0x747b88);                                               // 0050D3A5
    kotor_fastcall_0050c0e0(area, nullptr);                                       // 0050D3B7
    (void)recovered_object_array_erase_004d8290(recovered_object_array_004aed70(server_app(), nullptr), nullptr,
                                                rd(s + 0x120));                   // 0050D3BC..0050D3D2
    const U pathfind = rd(s + 0x1b0);                                             // 0050D3D7
    if (pathfind != 0) {                                                          // 0050D3DF..0050D3E1
        recovered_pathfind_shutdown_005cfd90(ptr(pathfind), nullptr);             // 0050D3E3..0050D3E5
        live_free(ptr(pathfind));                                                 // 0050D3EA..0050D3EB
        wr(s + 0x1b0, 0);                                                         // 0050D3F3
    }
    const U resource = rd(s + 0x208);                                             // 0050D3F9
    if (resource != 0) {                                                          // 0050D3FF..0050D401
        set_table(resource, 0x747470);                                            // 0050D404 (inline base dtor)
        live_free(ptr(resource));                                                 // 0050D403..0050D40A
        wr(s + 0x208, 0);                                                         // 0050D412
    }
    if (rd(s + 0x23c) != 0) { live_free(ptr(rd(s + 0x23c))); wr(s + 0x23c, 0); wr(s + 0x238, 0); } // 0050D418..0050D431
    if (rd(s + 0x244) != 0) { live_free(ptr(rd(s + 0x244))); wr(s + 0x244, 0); wr(s + 0x240, 0); } // 0050D437..0050D450
    if (rd(s + 0x25c) != 0) { delete_strings(s, 0x25c); wr(s + 0x25c, 0); }      // 0050D456..0050D47D
    if (rd(s + 0x260) != 0) { live_free(ptr(rd(s + 0x260))); wr(s + 0x260, 0); } // 0050D483..0050D496
    if (rd(s + 0x264) != 0) { live_free(ptr(rd(s + 0x264))); wr(s + 0x264, 0); } // 0050D49C..0050D4AF
    if (rd(s + 0x2d0) != 0) {                                                     // 0050D4B5..0050D4BD
        kotor_fastcall_006056a0(ptr(rd(s + 0x2d0)), nullptr);                    // 0050D4BF
        wr(s + 0x2d0, 0);                                                         // 0050D4C4
    }
    kotor_fastcall_004c3150(server_get_module_004ae6b0(server_app(), nullptr), nullptr); // 0050D4CA..0050D4DA
    kotor_fastcall_004aeba0(server_app(), nullptr);                               // 0050D4DF..0050D4E7
    free_member(s, 0x2a0);                                                        // 0050D4EC..0050D4FD
    delete_strings(s, 0x294); wr(s + 0x294, 0);                                   // 0050D503..0050D532
    delete_strings(s, 0x288); wr(s + 0x288, 0);                                   // 0050D538..0050D564
    free_member(s, 0x27c);                                                        // 0050D56A..0050D57B
    delete_strings(s, 0x270); wr(s + 0x270, 0);                                   // 0050D581..0050D5B6
    case40_0059a980(ptr(s + 0x1e4), nullptr);                                    // 0050D5B0..0050D5C1 ~CSWSScriptVarTable
    free_member(s, 0x1a0);                                                        // 0050D5C6..0050D5D7
    free_member(s, 0x190);                                                        // 0050D5DD..0050D601
    c2da_destroy_array_006fa852(ptr(s + 0x164), 8, 4,
                                reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 0050D5F1..0050D60C CExoString[4]
    frame_LogStringDtor_005e5c20(ptr(s + 0x158), nullptr);                       // 0050D611..0050D61C
    exo_locstring_destroy_005ea000(ptr(s + 0x150), nullptr);                     // 0050D621..0050D62C
    frame_LogStringDtor_005e5c20(ptr(s + 0x148), nullptr);                       // 0050D631..0050D63C
    const U list = rd(s + 0x144);                                                 // 0050D641
    if (list != 0) {                                                              // 0050D647..0050D64E
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(ptr(list)), nullptr); // 0050D650..0050D652
        live_free(ptr(list));                                                     // 0050D657..0050D658
    }
    free_member(s, 0x12c);                                                        // 0050D660..0050D671
    set_table(s + 0x11c, 0x7457a0);                                               // 0050D67C (CGameObject base)
    kotor_fastcall_00506bd0(ptr(s + 0x100), nullptr);                            // 0050D67A..0050D68A
    kotor_fastcall_0058f7a0(area, nullptr);                                       // 0050D68F..0050D699
}

// ===================================================================================== 0050C0E0 UnloadArea
void __fastcall kotor_fastcall_0050c0e0(void* area, void*) {
    const U s = word(area);
    free_member(s, 0x34);                                                         // 0050C0E4..0050C0F3
    free_member(s, 0x40);                                                         // 0050C0ED..0050C0FF
    free_member(s, 0x1c);                                                         // 0050C0FB..0050C10B
    free_member(s, 0x20);                                                         // 0050C107..0050C117
    free_member(s, 0x24);                                                         // 0050C113..0050C123
    free_member(s, 0x28);                                                         // 0050C11F..0050C131
    while (rd(rd(s + 0x144)) != 0) {                                              // 0050C12B..0050C13B, 0050C161..0050C169
        void* const node = base_ini_reconstruction::remove_head_005e9c80(
            static_cast<kotor_list::Internal*>(ptr(rd(s + 0x144))), nullptr);     // 0050C140..0050C146
        if (node != nullptr) {                                                    // 0050C14D..0050C14F
            exo_locstring_destroy_005ea000(node, nullptr);                        // 0050C151..0050C153 (005063B0 jmp 005EA000)
            live_free(node);                                                      // 0050C158..0050C159
        }
    }
    free_member(s, 0x160);                                                        // 0050C16C..0050C178
    void* const module = server_get_module_004ae6b0(server_app(), nullptr);       // 0050C17E..0050C189
    if (module != nullptr)                                                        // 0050C18E..0050C190
        (void)recovered_module_remove_tag_004c5ef0(module, nullptr, ptr(s + 0x158), rd(s + 0x120)); // 0050C192..0050C1A2
    const U objects = rd(s + 0x230);                                              // 0050C1A7
    if (objects != 0) {                                                           // 0050C1AD..0050C1AF
        if (rd(objects - 4) != 0) {                                               // 0050C1B1..0050C1B9
            using VectorDelete = void*(__thiscall*)(void*, U);
            (void)reinterpret_cast<VectorDelete>(rd(rd(objects)))(ptr(objects), 3); // 0050C1BB..0050C1BF (delete[])
        } else {
            live_free(ptr(objects - 4));                                          // 0050C1CA..0050C1CB
        }
    }
    wr(s + 0x230, 0);                                                             // 0050C1C1 / 0050C1D3
}

// ===================================================================================== 0058FF60 / 0058F7A0
void* __fastcall kotor_fastcall_0058ff60(void* self, void*, U flags) {
    kotor_fastcall_0058f7a0(self, nullptr);                                       // 0058FF61..0058FF63
    if ((flags & 1) != 0) live_free(self);                                        // 0058FF68..0058FF70
    return self;                                                                  // 0058FF78..0058FF7B
}
void __fastcall kotor_fastcall_0058f7a0(void* area, void*) {                     // ~CSWArea: frees only, no zero stores
    const U s = word(area);
    set_table(s, 0x749b38);                                                       // 0058F7A7
    static const U members[] = {0x1c, 0x20, 0x24, 0x28, 0x18, 0x34, 0x40, 0x4c, 0x50}; // 0058F7A3..0058F7F5
    for (const U offset : members) live_free(ptr(rd(s + offset)));
}

// ===================================================================================== 00506E00 / 00506BD0
void* __fastcall kotor_fastcall_00506e00(void* self, void*, U flags) {
    kotor_fastcall_00506bd0(self, nullptr);                                       // 00506E01..00506E03
    if ((flags & 1) != 0) live_free(self);                                        // 00506E08..00506E10
    return self;                                                                  // 00506E18..00506E1B
}
// ~CResHelper<CResARE,2012>: +4 demanded flag, +8 CRes*, +C CResRef. Releases the resource through the resource
// manager; deletes it (virtual slot 0, flag 1) only when the manager did not take it back.
void __fastcall kotor_fastcall_00506bd0(void* helper, void*) {
    const U s = word(helper);
    set_table(s, 0x74767c);                                                       // 00506BD8
    if (rd(s + 8) == 0) return;                                                   // 00506BD3..00506BDE
    if (c2da_not_equal_text_00406060(ptr(s + 0xc), nullptr, "") == 0) return;     // 00506BE0..00506BEF ([0073D71C])
    if (rd(s + 4) != 0) {                                                         // 00506BF1..00506BF6
        c2da_cancel_00409b00(ptr(rd(s + 8)), nullptr);                            // 00506BF8..00506BFB
        wr(s + 4, 0);                                                             // 00506C00
    }
    if (c2da_release_00409cf0(ptr(frame_control::resource_manager_007a39e8), nullptr, ptr(rd(s + 8))) != 0) return; // 00506C07..00506C18
    const U resource = rd(s + 8);                                                 // 00506C1A
    if (resource != 0) {                                                          // 00506C1D..00506C1F
        using Destroy = void*(__thiscall*)(void*, U);
        (void)reinterpret_cast<Destroy>(rd(rd(resource)))(ptr(resource), 1);     // 00506C21..00506C25
    }
    wr(s + 8, 0);                                                                 // 00506C27
}

// ===================================================================================== small cache clears
void __fastcall kotor_fastcall_006056a0(void* client_area, void*) { wr(word(client_area) + 0x27c, 0); } // 006056A0
void __fastcall kotor_fastcall_004c3150(void* module, void*) {                   // 004C3150..004C3164
    wr(word(module) + 0x220, 0);
    wr(word(module) + 0x21c, 0x7f000000u);
}
void __fastcall kotor_fastcall_004b1860(void* internal, void*) {                 // 004B1860..004B1874
    wr(word(internal) + 0x1b924, 0);
    wr(word(internal) + 0x1b920, 0x7f000000u);
}
void __fastcall kotor_fastcall_004aeba0(void* server, void*) {                   // 004AEBA0: [ecx+4]; jmp 004B1860
    kotor_fastcall_004b1860(ptr(rd(word(server) + 4)), nullptr);
}

// ===================================================================================== 004D7E90
// CGameObjectArray::AddObjectAtPos(ulong id, CGameObject* object) RET 8, AL status (0 inserted, 1 bad id, 4 null
// object). Hash buckets [this]+((id>>31)<<12 | id&FFF)*4 of {key, object, next} nodes sorted by descending key;
// counters +4 (low ids) / +0Ch (high ids) per half. An equal key displaces the stored object, which is re-inserted
// under the next counter id (high bit restored). Same transcription as the file-local 0963 body_004d7e90
// (claude_0963_bodies.inc), exported here with C linkage for the 0886 area constructor port.
// Quirk kept: a failed node allocation is written through (null) like the original.
extern "C" void* __cdecl live_operator_new(unsigned);                                          // 006FA7E6
extern "C" unsigned char __fastcall kotor_fastcall_004d7e90(void* array, void*, U id, void* object);
unsigned char __fastcall kotor_fastcall_004d7e90(void* array, void*, U id, void* object) {
    const U a = word(array);
    U obj = word(object);
    if (obj == 0) return 4;                                                       // 004D7E90..004D7E9C, 004D7F97..004D7F9D
    U arg = id;                                                                   // [esp+14h]
    for (;;) {
        const U top = (arg >> 31) & 1;                                            // 004D7EA6..004D7EAB ebx
        const U c = top & 0xffff;                                                 // 004D7EAE movzx ecx,bx
        const U key = arg & 0x7fffffff;                                           // 004D7EB1, 004D7EBD esi
        const U bucket = (c << 12) + (arg & 0xfff);                               // 004D7EB3..004D7EC3 edi
        if ((key & 0x7f000000) != 0) {                                            // 004D7EC5..004D7ECB
            if (key < 0x7f000001u || key > 0x7fffffffu) return 1;                 // 004D7ECD..004D7EDF, 004D7FA0
            if (rd(a + c * 4 + 0xc) >= key) wr(a + c * 4 + 0xc, key - 1);         // 004D7EE5..004D7EEE
            arg = rd(a + c * 4 + 0xc);                                            // 004D7EF2..004D7EF6
        } else {
            if (key > 0x7effffffu) return 1;                                      // 004D7EFC..004D7F02
            if (rd(a + c * 4 + 4) <= key) wr(a + c * 4 + 4, key + 1);             // 004D7F08..004D7F11
            arg = rd(a + c * 4 + 4);                                              // 004D7F15..004D7F19
        }
        void* const memory = live_operator_new(12);                               // 004D7F1D..004D7F26
        U node = 0;
        if (memory != nullptr) { node = word(memory); wr(node, 0); wr(node + 4, 0); wr(node + 8, 0); } // 004D7F29..004D7F37
        volatile U* const n = reinterpret_cast<volatile U*>(ptr(node));          // 004D7F39: null node written through
        n[0] = key; n[1] = obj; n[2] = 0;                                         // 004D7F3B..004D7F44
        U cur = rd(rd(a) + bucket * 4);                                           // 004D7F47..004D7F4A
        if (cur == 0) { wr(rd(a) + bucket * 4, node); return 0; }                 // 004D7F4D..004D7F4F, 004D7FA9..004D7FB5
        U prev = 0;                                                               // 004D7F51
        bool appended = false;
        while (rd(cur) > key) {                                                   // 004D7F53..004D7F55
            prev = cur; cur = rd(cur + 8);                                        // 004D7F57..004D7F59
            if (cur == 0) { wr(prev + 8, node); appended = true; break; }         // 004D7F5C..004D7F69
        }
        if (appended) return 0;
        if (rd(cur) != key) {                                                     // 004D7F6C
            if (prev == 0) {                                                      // 004D7FB8..004D7FBA
                wr(node + 8, rd(rd(a) + bucket * 4));                             // 004D7FBC..004D7FC2
                wr(rd(a) + bucket * 4, node);                                     // 004D7FC5..004D7FC8
                return 0;
            }
            wr(node + 8, rd(prev + 8));                                           // 004D7FD4..004D7FD9
            wr(prev + 8, node);                                                   // 004D7FDD..
            return 0;
        }
        const U displaced = rd(cur + 4);                                          // 004D7F6E
        live_free(ptr(node));                                                     // 004D7F71..004D7F77
        if (c != 0) arg |= 0x80000000u;                                           // 004D7F7A..004D7F7F
        obj = displaced;                                                          // 004D7F8D
        if (displaced == 0) return 4;                                             // 004D7F87..004D7F91, 004D7F97..004D7F9D
    }
}
