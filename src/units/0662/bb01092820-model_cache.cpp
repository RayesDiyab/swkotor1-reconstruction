// model_cache.cpp - lane-3d-model: model cache lookup/insert, IODispatcher, Input::Read, tree type casts.
// Evidence: dumpbin of GOG swkotor.exe (lane-3d-model/d, lane-sound/full.asm); cited per behavior.
#include "d57d20b49f-model_load.hpp"
#include "22230a53be-storage.h"
#include <cstdlib>

using namespace aurora_model;

extern "C" {
// Cells owned by this lane (original .bss: zero at load).
MaxTree* volatile aurora_current_model_007fbae4 = nullptr;
volatile U aurora_model_no_delete_007a8a04 = 0;
volatile U aurora_model_variants_00828068 = 0;
List aurora_io_dispatcher_00830510 = {};
volatile U8 aurora_io_dispatcher_guard_0083051c = 0;
volatile I aurora_input_read_counter_007b932c = 0;   // only reader/writer: 004A13AF / 004A13C5
}

namespace {
// Model cache 007A8A64 {data, size, capacity}: single owner task-7 historical_storage_007a8a64
// (CleanupStorage12: head + 8 opaque bytes). Count/capacity live at +4/+8 (0046A5F0 reads 007A8A68).
CleanupStorage12& cache_cell() { return historical_storage_007a8a64; }
List* cache_list() { return reinterpret_cast<List*>(&historical_storage_007a8a64); }
I cache_count() { return get<I>(&cache_cell(), 4); }                          // [007A8A68]
MaxTree* cache_at(I i) { return get<MaxTree*>(get<unsigned char*>(&cache_cell(), 0) + 4 * i, 0); } // [[007A8A64]+i*4]
const char* tree_name(MaxTree* t) { return reinterpret_cast<const char*>(at(t, 8)); } // +08 name
}

// ---------------------------------------------------------------- tree casts / refcount
// 0043E1C0 MaxTree::AsModel: (byte[+4C] & 7F) == 2 ? this : 0 (sub/neg/sbb/not/and).
MaxTree* __fastcall aurora_tree_as_model_0043e1c0(MaxTree* self, void*) {
    return ((get<U8>(self, 0x4c) & 0x7f) == 2) ? self : nullptr;
}
// 0043E1D0 MaxTree::AsAnimation: (byte[+4C] & 7F) == 5 ? this : 0.
MaxTree* __fastcall aurora_tree_as_animation_0043e1d0(MaxTree* self, void*) {
    return ((get<U8>(self, 0x4c) & 0x7f) == 5) ? self : nullptr;
}
// 0043E130 MaxTree::GetType: zero-extended byte[+4C] & 7F.
U __fastcall aurora_tree_get_type_0043e130(MaxTree* self, void*) {
    return static_cast<U>(get<U8>(self, 0x4c)) & 0x7fu;
}
// 0043E240 Model::AddRef: ++[+54], returns new count, RET4 (argument ignored).
I __fastcall model_add_ref_0043e240(MaxTree* self, void*, void*) {
    const I count = get<I>(self, 0x54) + 1;
    put<I>(self, 0x54, count);
    return count;
}
// 0043E210 Model::DelRef: if [007A8A04] != 0 return [+54] unchanged (0043E215..0043E21C); else
// --[+54], and when it reaches 0 call MaxTree::operator delete(this) 0044AEC0 (cdecl); returns new count.
I __fastcall aurora_model_del_ref_0043e210(MaxTree* self, void*, void*) {
    if (aurora_model_no_delete_007a8a04 != 0) return get<I>(self, 0x54);
    const I count = get<I>(self, 0x54) - 1;
    put<I>(self, 0x54, count);
    if (count == 0) aurora_maxtree_delete_0044aec0(self);   // 0043E22B..0043E231
    return count;
}

// 0044A900 MaxTree::MaxTree(name), RET4: clear both free lists, copy name to +8 (unbounded byte copy incl. NUL),
// [+28] = 0, type byte [+4C] = 1, [+48] = 0, [+2C] = 0; inlined ResetFreeLists (token list +3C first, then +30);
// [0]/[4] = 0044A8E0/0043E190.
MaxTree* __fastcall maxtree_construct_0044a900(MaxTree* self, void*, const char* name) {
    put<U>(self, 0x34, 0); put<U>(self, 0x38, 0); put<U>(self, 0x30, 0);           // 0044A921..0044A927
    put<U>(self, 0x40, 0); put<U>(self, 0x44, 0); put<U>(self, 0x3C, 0);           // 0044A92E..0044A934
    std::strcpy(reinterpret_cast<char*>(at(self, 8)), name);                        // 0044A937..0044A94E
    put<U>(self, 0x28, 0);                                                          // 0044A950
    put<U8>(self, 0x4C, 1);                                                         // 0044A953
    put<U>(self, 0x48, 0);                                                          // 0044A956
    put<U>(self, 0x2C, 0);                                                          // 0044A959
    if (get<U>(self, 0x3C) != 0) { live_free(ptr<void>(get<U>(self, 0x3C))); put<U>(self, 0x3C, 0); } // 0044A95C..0044A96C
    put<U>(self, 0x44, 0); put<U>(self, 0x40, 0);                                   // 0044A96F..0044A972
    if (get<U>(self, 0x30) != 0) { live_free(ptr<void>(get<U>(self, 0x30))); put<U>(self, 0x30, 0); } // 0044A975..0044A985
    put<U>(self, 0x38, 0); put<U>(self, 0x34, 0);                                   // 0044A98C..0044A98F
    put<TreeDestroyFn>(self, 0, &aurora_maxtree_destructor_0044a8e0);                // 0044A993
    put<TreeParseFn>(self, 4, &aurora_maxtree_parse_field_0043e190);                 // 0044A999
    return self;
}
// 0044AA70 Model::Model(name), RET4: MaxTree base; default bounding values; type 2; InsertModel(this);
// [0]/[4] = 0044AA60/0043E1E0; [AC] = -1 then the handle returned by RequestPool(0, 1, 2, 0, &[AC]) (EAX
// stored again at 0044AB8E); [B0] = [B4] = [BC] = 0.
MaxTree* __fastcall model_construct_0044aa70(MaxTree* self, void*, const char* name) {
    maxtree_construct_0044a900(self, nullptr, name);                                // 0044AA91..0044AA96
    put<U>(self, 0x5C, 0); put<U>(self, 0x60, 0); put<U>(self, 0x58, 0);           // 0044AAA1..0044AAA7
    for (U o = 0x68; o <= 0x7C; o += 4) put<U>(self, o, 0);                         // 0044AAAA..0044AAB9
    put<U>(self, 0xBC, 0); put<U>(self, 0xC0, 0); put<U>(self, 0xB8, 0);           // 0044AABC..0044AAC8
    put<U>(self, 0x68, 0xC0A00000u);                                                // 0044AAE6 (-5.0f)
    put<U>(self, 0x6C, 0xC0A00000u);                                                // 0044AAF5 (-5.0f)
    put<U>(self, 0x70, 0xBF800000u);                                                // 0044AB04 (-1.0f)
    put<U>(self, 0x74, 0x40A00000u);                                                // 0044AB13 (5.0f)
    put<U>(self, 0x78, 0x40A00000u);                                                // 0044AB22 (5.0f)
    put<U8>(self, 0x4C, 2);                                                         // 0044AB2B
    put<U>(self, 0x64, 0);                                                          // 0044AB2F
    put<U16>(self, 0x50, 0);                                                        // 0044AB32
    put<U8>(self, 0x52, 0);                                                         // 0044AB36
    put<U8>(self, 0x53, 1);                                                         // 0044AB39
    put<U>(self, 0x80, 0x40E00000u);                                                // 0044AB3D (7.0f)
    put<U>(self, 0x54, 0);                                                          // 0044AB47
    put<U>(self, 0x84, 0x3F800000u);                                                // 0044AB4A (1.0f)
    put<U>(self, 0x7C, 0x41200000u);                                                // 0044AB54 (10.0f)
    aurora_insert_model_0046a5f0(self);                                             // 0044AB25..0044AB57
    put<U8>(self, 0x88, 0);                                                         // 0044AB69
    put<TreeDestroyFn>(self, 0, &aurora_model_destructor_0044aa60);                  // 0044AB6F
    put<TreeParseFn>(self, 4, &aurora_model_parse_field_0043e1e0);                   // 0044AB75
    put<U>(self, 0xAC, 0xFFFFFFFFu);                                                // 0044AB7C
    const U handle = aurora_pool_request_00423ee0(0, 1, 2, 0, ptr<U>(addr(at(self, 0xAC)))); // 0044AB5C..0044AB82
    put<U>(self, 0xAC, handle);                                                     // 0044AB8E
    put<U>(self, 0xB0, 0); put<U>(self, 0xB4, 0); put<U>(self, 0xBC, 0);           // 0044AB90..0044AB9C
    return self;
}

// ---------------------------------------------------------------- cache
// 00464090 BinarySearchModel(name, lo, hi): while hi >= lo: mid = (lo+hi)/2 (cdq/sub/sar: rounds toward 0);
// r = _stricmp(name, cache[mid]+8); 0 -> return cache[mid] (re-read, 004640DC); r < 0 -> hi = mid-1; else lo = mid+1.
MaxTree* __cdecl aurora_binary_search_model_00464090(const char* name, I lo, I hi) {
    while (hi >= lo) {                                                              // 0046409C / 004640D1
        const I mid = (lo + hi) / 2;                                                // 004640A4..004640B1
        const I r = kotor_stricmp_0070acaf(name, tree_name(cache_at(mid)));        // 004640B3..004640BB
        if (r == 0) return cache_at(mid);                                           // 004640DC..004640E2
        if (r < 0) hi = mid - 1; else lo = mid + 1;                                 // 004640C7 / 004640CE
    }
    return nullptr;                                                                 // 004640D8
}

// 00445020 CExoArrayList<DWORD>::Insert(value, index), RET8: grow when size == capacity (cap ? 2*cap : 8,
// via 005E03C0); store value at [size] and ++size (0044504C/0044504F); then shift [old size .. index+1] up
// from the element below and store value at [index] (00445060..00445074).
// [M45 compose] 00445020 body removed: existing M43 provider list_insert_00445020 (font-texture-order) wins (definition removed; declaration kept)
void __fastcall aurora_list_insert_00445020(List* self, void*, U value, I index);

// 0046A590 BinaryInsertModel(tree, lo, hi): same bisection on _stricmp(tree+8, cache[mid]+8) without the
// equality exit (r >= 0 -> lo = mid+1); inserts at lo through 00445020 on 007A8A64.
void __cdecl aurora_binary_insert_model_0046a590(MaxTree* tree, I lo, I hi) {
    while (hi >= lo) {                                                              // 0046A59A / 0046A5D2
        const I mid = (lo + hi) / 2;
        const I r = kotor_stricmp_0070acaf(tree_name(tree), tree_name(cache_at(mid))); // 0046A5A4..0046A5BE
        if (r < 0) hi = mid - 1; else lo = mid + 1;                                 // 0046A5C8..0046A5CF
    }
    aurora_list_insert_00445020(cache_list(), nullptr, addr(tree), lo);             // 0046A5D8..0046A5E3
}
// 0046A5F0 InsertModel(tree): BinaryInsertModel(tree, 0, [007A8A68]-1).
void __cdecl aurora_insert_model_0046a5f0(MaxTree* tree) {
    aurora_binary_insert_model_0046a590(tree, 0, cache_count() - 1);
}

// ---------------------------------------------------------------- IODispatcher
// 0073BE40 freeIODispatch (atexit callback): ecx = 00830510; jmp 0047ABC0.
void __cdecl aurora_free_io_dispatch_0073be40() {
    exo_array_list_free_0047abc0(&aurora_io_dispatcher_00830510, nullptr);          // lane-3d-render provider
}
// 004A0580 IODispatcher::GetRef: function-static at 00830510 guarded by bit0 of 0083051C. First call:
// set bit, zero the three dwords (004A05AA..004A05BB), atexit(0073BE40) (004A05C0); returns 00830510.
// The fs:[0] frame (handler 717E2E) only guards the static initialization; no throwing callee.
IODispatcher* __cdecl aurora_io_dispatcher_get_004a0580() {
    if ((aurora_io_dispatcher_guard_0083051c & 1u) == 0) {
        aurora_io_dispatcher_guard_0083051c = static_cast<U8>(aurora_io_dispatcher_guard_0083051c | 1u); // 004A05A4
        aurora_io_dispatcher_00830510.size = 0;                                     // 004A05B1 (00830514)
        aurora_io_dispatcher_00830510.capacity = 0;                                 // 004A05B6 (00830518)
        aurora_io_dispatcher_00830510.data = 0;                                     // 004A05BB (00830510)
        std::atexit(&aurora_free_io_dispatch_0073be40);                             // 004A05C0 (006FB332 _atexit)
    }
    return reinterpret_cast<IODispatcher*>(&aurora_io_dispatcher_00830510);
}

namespace {
// ReadSync's 16-byte stack Input object (004A15B0/004A15D0: sub esp,10h; [local+0C] = 0).
// +00/+04 receive the binary MDL/MDX sizes (004A1288/004A129B); +08 is never written or read on this path.
struct InputStorage { U mdl_size; U mdx_size; U unused08; U zero0c; };
static_assert(sizeof(InputStorage) == 16, "004A15B0 sub esp,10h");
}

// 004A15B0 IODispatcher::ReadSync(name), RET4: local Input, [local+0C] = 0 (004A15BC), Input::Read 004A13A0.
MaxTree* __fastcall aurora_io_read_sync_name_004a15b0(IODispatcher*, void*, const char* name) {
    InputStorage input;
    input.zero0c = 0;
    return aurora_input_read_name_004a13a0(reinterpret_cast<InputObject*>(&input), nullptr, name);
}
// 004A15D0 IODispatcher::ReadSync(FILE* mdl, FILE* mdx), RET8: same local, Input::Read 004A14B0.
MaxTree* __fastcall aurora_io_read_sync_file_004a15d0(IODispatcher*, void*, std::FILE* mdl, std::FILE* mdx) {
    InputStorage input;
    input.zero0c = 0;
    return aurora_input_read_file_004a14b0(reinterpret_cast<InputObject*>(&input), nullptr, mdl, mdx);
}

// 004A13A0 Input::Read(name), RET4. ++[007B932C] is outside this lane: see note below.
// mdl = AurResGet(name, ".mdl", 0, 1); mdx = AurResGet(name, ".mdx", 0, 1) (004A13CB / 004A13DC).
// !mdl -> 0 (mdx descriptor left registered: original quirk). head = GetDataBytes(4, mdl); !head -> 0.
// head[0] == 0 (binary): FreeDataBytes(mdl, head); InputBinary::Read(this, mdl, mdx, 0) -> AsModel.
// else (ASCII): mdx ? AurResFree(mdx, 0); FreeDataBytes(mdl, head); AurResFree(mdl, 0);
// sprintf(buf, "%s%s", name, ".mdl"); save [007FBAE4]; dofile(buf); result = [007FBAE4]; restore.
MaxTree* __fastcall aurora_input_read_name_004a13a0(InputObject* self, void*, const char* name) {
    aurora_input_read_counter_007b932c = aurora_input_read_counter_007b932c + 1;    // 004A13AF..004A13C5
    void* const mdl = aurres_get_0044c740(name, ".mdl", nullptr, 1);               // 004A13B6..004A13CB (740CA8)
    void* const mdx = aurres_get_0044c740(name, ".mdx", nullptr, 1);               // 004A13D0..004A13DC (743944)
    if (mdl == nullptr) return nullptr;                                             // 004A13E4..004A13F5
    unsigned char* const head = static_cast<unsigned char*>(aurres_data_bytes_0044bd10(4, mdl)); // 004A13F9
    if (head == nullptr) return nullptr;                                            // 004A1406..004A1414
    if (head[0] == 0) {                                                             // 004A1417
        aurres_free_data_bytes_0044bd50(mdl, head);                                 // 004A141E
        MaxTree* const tree = aurora_input_binary_read_004a1260(self, nullptr,
            static_cast<Resource*>(mdl), static_cast<Resource*>(mdx), 0);           // 004A142A..004A142E
        return aurora_tree_as_model_0043e1c0(tree, nullptr);                        // 004A1433..004A1435
    }
    if (mdx != nullptr) aurres_release_0044c8f0(mdx, 0);                            // 004A1447..004A144E
    aurres_free_data_bytes_0044bd50(mdl, head);                                     // 004A1458
    aurres_release_0044c8f0(mdl, 0);                                                // 004A1460
    char buffer[0x80];                                                              // [esp+28h]: 0x84-byte frame
    std::sprintf(buffer, "%s%s", name, ".mdl");                                     // 004A1465..004A1475 (740CCC)
    MaxTree* const saved = aurora_current_model_007fbae4;                           // 004A147A
    aurres_dofile_0044c980(buffer);                                                 // 004A1485
    MaxTree* const result = aurora_current_model_007fbae4;                          // 004A148A
    aurora_current_model_007fbae4 = saved;                                          // 004A1493
    return result;
}

// 004A14B0 Input::Read(FILE* mdl, FILE* mdx), RET8. mdlres = AurResGet(FILE* mdl) 0044C870; !mdlres -> 0.
// head = GetDataBytes(4, mdlres); !head -> 0. Binary: FreeDataBytes; mdxres = AurResGet(FILE* mdx);
// InputBinary::Read(this, mdlres, mdxres, 0) -> AsModel. ASCII: FreeDataBytes; save [007FBAE4];
// interpret each line: skip ' '/'\t'; skip empty and '#'; strip trailing \n \r \t ' '; FuncInterp 0044C1F0;
// then AurResFree(mdlres, 0); result = [007FBAE4]; restore.
extern "C" char* __cdecl aurres_func_interp_0044c1f0(char* line);   // FuncInterp (ASCII console/model interpreter)
extern "C" char* __cdecl resource_next_0044bfa0();               // AurResGetNextLine (lane-aurres)
MaxTree* __fastcall aurora_input_read_file_004a14b0(InputObject* self, void*, std::FILE* mdl, std::FILE* mdx) {
    void* const mdl_res = aurres_get_file_0044c870(mdl);                            // 004A14B6..004A14B9
    if (mdl_res == nullptr) return nullptr;                                         // 004A14C3..004A14CB
    unsigned char* const head = static_cast<unsigned char*>(aurres_data_bytes_0044bd10(4, mdl_res)); // 004A14CE..004A14D1
    if (head == nullptr) return nullptr;                                            // 004A14D9..004A14DB
    if (head[0] == 0) {                                                             // 004A14DD
        aurres_free_data_bytes_0044bd50(mdl_res, head);                             // 004A14E4
        void* const mdx_res = aurres_get_file_0044c870(mdx);                        // 004A14E9..004A14EE
        MaxTree* const tree = aurora_input_binary_read_004a1260(self, nullptr,
            static_cast<Resource*>(mdl_res), static_cast<Resource*>(mdx_res), 0);   // 004A14F6..004A14FC
        return aurora_tree_as_model_0043e1c0(tree, nullptr);                        // 004A1501..004A1503
    }
    aurres_free_data_bytes_0044bd50(mdl_res, head);                                 // 004A150D..004A1510
    MaxTree* const saved = aurora_current_model_007fbae4;                           // 004A1515
    for (char* line = resource_next_0044bfa0(); line != nullptr; line = resource_next_0044bfa0()) { // 004A151E / 004A157E
        while (*line == ' ' || *line == '\t') ++line;                               // 004A1527..004A1534
        if (*line == 0 || *line == '#') continue;                                   // 004A1536..004A153F
        char* end = line + std::strlen(line) - 1;                                   // 004A1541..004A154F
        while (end >= line && (*end == '\n' || *end == '\r' || *end == '\t' || *end == ' ')) { // 004A1553..004A1573
            *end = 0;
            --end;
        }
        aurres_func_interp_0044c1f0(line);                                            // 004A1575..004A1576
    }
    aurres_release_0044c8f0(mdl_res, 0);                                            // 004A1587..004A158A
    MaxTree* const result = aurora_current_model_007fbae4;                          // 004A158F
    aurora_current_model_007fbae4 = saved;                                          // 004A1597
    return result;
}

// ---------------------------------------------------------------- FindModel / LoadModel
// 00464110 FindModel(name): null or _stricmp(name,"NULL")==0 -> 0. Cache hit (BinarySearchModel over
// [0, count-1]) -> that model. Miss: save [007FBAE4], store 0, r = IODispatcher::ReadSync(GetRef(), name);
// m = r ? AsModel(r) : [007FBAE4]; restore. m && _stricmp(m+8, name) != 0 -> append
// "Names Differ: %s %s" to AR_ERROR.LOG (sprintf into a 256-byte local first). Returns m.
MaxTree* __cdecl aurora_find_model_00464110(const char* name) {
    if (name == nullptr) return nullptr;                                            // 0046411F..00464122
    if (kotor_stricmp_0070acaf(name, "NULL") == 0) return nullptr;                  // 00464128..00464138 (73EE04)
    MaxTree* const hit = aurora_binary_search_model_00464090(name, 0, cache_count() - 1); // 0046413E..00464148
    if (hit != nullptr) return hit;                                                 // 00464150..00464152
    MaxTree* const saved = aurora_current_model_007fbae4;                           // 00464158
    aurora_current_model_007fbae4 = nullptr;                                        // 0046415E (eax == 0 here)
    IODispatcher* const dispatcher = aurora_io_dispatcher_get_004a0580();           // 00464163
    MaxTree* const read = aurora_io_read_sync_name_004a15b0(dispatcher, nullptr, name); // 00464168..0046416B
    MaxTree* const model = read ? aurora_tree_as_model_0043e1c0(read, nullptr)      // 00464174..00464176
                                : aurora_current_model_007fbae4;                    // 0046417D
    aurora_current_model_007fbae4 = saved;                                          // 00464186
    if (model == nullptr) return nullptr;                                           // 0046418C
    if (kotor_stricmp_0070acaf(tree_name(model), name) != 0) {                      // 00464191..0046419D (push esi; push edi)
        std::FILE* const log = std::fopen("AR_ERROR.LOG", "w");                    // 004641A0..004641AA (741F00, 73D868)
        char line[0x100];                                                           // sub esp,100h
        std::sprintf(line, "Names Differ: %s %s", tree_name(model), name);          // 004641AF..004641BD (741EEC)
        std::fprintf(log, "Names Differ: %s %s", tree_name(model), name);           // 004641C2..004641CA (006FAF01)
        std::fclose(log);                                                           // 004641CF..004641D0
    }
    return model;
}

// 00464200 LoadModel(FILE* mdl, FILE* mdx): !mdl -> 0. Save/zero [007FBAE4] (00464214), r = ReadSync(GetRef(),
// mdl, mdx); m = r ? AsModel(r) : [007FBAE4]; restore; !m -> 0. Scan the cache: for each entry e != m
// with _stricmp(e+8, m+8) == 0: Model::~Model(m) 0043F790, MaxTree::operator delete(m) 0044AEC0, and return
// the cache entry re-read at the same index (004642A3). Otherwise return m.
MaxTree* __cdecl aurora_load_model_00464200(std::FILE* mdl, std::FILE* mdx) {
    if (mdl == nullptr) return nullptr;                                             // 00464205..0046420C
    MaxTree* const saved = aurora_current_model_007fbae4;                           // 0046420E
    aurora_current_model_007fbae4 = nullptr;                                        // 00464214
    IODispatcher* const dispatcher = aurora_io_dispatcher_get_004a0580();           // 0046421E
    MaxTree* const read = aurora_io_read_sync_file_004a15d0(dispatcher, nullptr, mdl, mdx); // 00464223..0046422B
    MaxTree* const model = read ? aurora_tree_as_model_0043e1c0(read, nullptr) : aurora_current_model_007fbae4;
    aurora_current_model_007fbae4 = saved;                                          // 00464246
    if (model == nullptr) return nullptr;                                           // 0046424C
    for (I i = 0; i < cache_count(); ++i) {                                         // 00464251..00464289 (count re-read)
        MaxTree* const entry = cache_at(i);                                         // 00464260..00464266
        if (entry == model) continue;                                               // 00464269
        if (kotor_stricmp_0070acaf(tree_name(entry), tree_name(model)) == 0) {      // 0046426D..0046427F
            aurora_model_destroy_0043f790(model, nullptr);                          // 00464290..00464292
            aurora_maxtree_delete_0044aec0(model);                                  // 00464297..00464298
            return cache_at(i);                                                     // 0046429D..004642A3
        }
    }
    return model;
}
