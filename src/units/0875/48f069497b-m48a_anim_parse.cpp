// lane-m48-a: ASCII .mdl animation field parser and its two small callees.
//   0046A410 Animation::InternalParseField(line)            thiscall RET 4 (D1: fastcall(self, edx, line) @12)
//   004638F0 Parse<char[32]>(line, AurKey<char[32]>*)       cdecl, 2 DWORDs
//   00468970 CExoArrayList<AurKey<char[32]>>::Insert(key, index) thiscall RET 28h (key by value: 9 DWORDs)
//   004639B0 CExoArrayList<AurKey<char[32]>>::Allocate(capacity) thiscall RET 4
// Evidence: swkotor.exe GOG K1 dumpbin /disasm (lane-m48-a/d/0046A410.txt, d/004638F0.txt, d/0046393C.txt,
// d/00468970.txt, d/004639B0.txt). .rdata strings (pinned exe bytes): 00742EE0 "animlength", 00740CD4 "%s %f",
// 00742ED8 "event", 00742ED0 "length", 0073E2E4 "%f", 00742EC4 "transtime", 00742EB8 "animroot", 0073D5D0 "%s",
// 00742A8C "node", 00741EAC "%f%n".
// Providers (existing M47 symbols): 006FAFF0 kotor_strncmp_006faff0 (tlk-parse), 006FAE08 kotor_sscanf_006fae08 and
// 00463530 firstword_00463530 (font-txi-readers), 0070ACAF kotor_stricmp_0070acaf; 004680E0 ParseNode stays the M47
// unresolved name _aurora_parse_node_004680e0 (ASCII-MDL node parser family; see PACKET open_gates).
// Allocator family: 006FA7E6 -> live_operator_new, 006FA390 -> live_free (M41). Reached only while parsing ASCII
// models through the MaxTree +04 parse-fn cell (m47_anim_lookat.cpp 00481880); binary MDL (menu path) never calls it.
#include <cstdint>
#include <cstring>

extern "C" {
std::int32_t __cdecl kotor_strncmp_006faff0(const char* first, const char* second, std::uint32_t count);
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);
char* __cdecl firstword_00463530(char*);
__declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
void __cdecl aurora_parse_node_004680e0(char* text, void* tree);
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
}

namespace {
using U = std::uint32_t; using I = std::int32_t;
struct AurKeyName { float time; char name[32]; };                  // AurKey<char[32]>: 0x24 bytes (rep movs 9 dwords)
static_assert(sizeof(AurKeyName) == 0x24, "AurKey<char[32]>");
struct AurKeyNameList { AurKeyName* data; I size; I capacity; };   // CExoArrayList {data, size, capacity}
template<class T> T& field(void* base, U offset) { return *reinterpret_cast<T*>(static_cast<char*>(base) + offset); }
// The original copies with an inline byte loop from the sscanf result taken as an address (0046A550 / 00463930):
// that path runs only when sscanf("%s") returns 0, which cannot happen (it returns 1 or EOF); kept for fidelity.
void copy_from_result(char* destination, int result) {
    const char* source = reinterpret_cast<const char*>(static_cast<std::intptr_t>(result));
    for (I i = 0;; ++i) { const char c = source[i]; destination[i] = c; if (c == '\0') break; }
}
} // namespace

extern "C" {
// 004639B0 Allocate(capacity): capacity = n; new block (n*24h); first DWORD of every element = 0; copy `size` old
// elements; free the old block. A NULL block leaves data = 0 (004639EE) and the copy writes through it as original.
void __fastcall aur_key_name_list_allocate_004639b0(AurKeyNameList* self, void* /*unused_edx*/, I capacity) {
    AurKeyName* const old = self->data;                               // 004639BC ebp = [ebx]
    self->capacity = capacity;                                        // 004639C2
    auto* const fresh = static_cast<AurKeyName*>(live_operator_new(static_cast<U>(capacity) * 0x24u)); // 004639B7..004639C5
    if (fresh != nullptr && capacity - 1 >= 0)                        // 004639CD..004639D8
        for (I k = 0; k < capacity; ++k) field<U>(&fresh[k], 0) = 0u; // 004639E0..004639EA mov dword ptr [ecx],0
    self->data = fresh;                                               // 004639F0
    for (I i = 0; i < self->size; ++i) self->data[i] = old[i];        // 004639F2..00463A17 (size and data re-read)
    if (old != nullptr) live_free(old);                               // 00463A1A..00463A1F
}                                                                     // 00463A2A ret 4

// 00468970 Insert(key, index): append at [size], size++, shift [index..old size) up by one from the top, store key.
void __fastcall aur_key_name_list_insert_00468970(AurKeyNameList* self, void* /*unused_edx*/, AurKeyName key, I index) {
    const I old_size = self->size;                                    // 00468974 ebp = [ebx+4]
    if (old_size == self->capacity)                                   // 00468977..0046897E
        aur_key_name_list_allocate_004639b0(self, nullptr, self->capacity != 0 ? self->capacity * 2 : 8); // 00468980..00468990
    self->data[self->size] = key;                                     // 00468995..004689AD
    self->size = self->size + 1;                                      // 004689AF..004689B5
    if (old_size > index)                                             // 004689B3..004689B8 (signed)
        for (I slot = old_size; slot != index; --slot) self->data[slot] = self->data[slot - 1]; // 004689BA..004689D6
    self->data[index] = key;                                          // 004689D8..004689E9
}                                                                     // 004689EF ret 28h

// 004638F0 Parse<char[32]>(line, key): "%f%n" into key->time; the %n target is the caller's key-pointer argument
// slot ([esp+10h]), so when %f does not convert the slot still holds the key pointer bits (reproduced).
void __cdecl parse_aur_key_name_004638f0(const char* line, AurKeyName* key) {
    I consumed = static_cast<I>(reinterpret_cast<std::uintptr_t>(key)); // [esp+10h] = argument slot (pointer bits)
    kotor_sscanf_006fae08(line, "%f%n", &key->time, &consumed);       // 004638FA..00463906 (fmt 00741EAC)
    const char* text = line + consumed;                               // 0046390B..0046390F lea eax,[ecx+esi]
    if (text != nullptr)                                              // 00463915..00463917 jne 00463940
        while (*text == ' ' || *text == '\t') ++text;                 // 00463940..0046394D
    const int result = kotor_sscanf_006fae08(text, "%s", key->name);  // 00463919..00463923 (fmt 0073D5D0)
    if (result == 0) copy_from_result(key->name, result);             // 0046392B..00463938
}

// 0046A410 Animation::InternalParseField(line). Fields: +50 length (float), +54 transtime (float), +58 animroot
// (char[]), +78 events CExoArrayList<AurKey<char[32]>>.
void __fastcall animation_internal_parse_field_0046a410(void* self, void* /*unused_edx*/, char* line) {
    char name[0x84];                                                  // [esp+34h] frame area (sub esp,0A4h)
    if (kotor_strncmp_006faff0("animlength", line, 10u) == 0)         // 0046A421..0046A435 (00742EE0)
        kotor_sscanf_006fae08(line + 10, "%s %f", name, &field<float>(self, 0x50)); // 0046A437..0046A449 (00740CD4)
    if (kotor_strncmp_006faff0("event", line, 5u) == 0) {             // 0046A451..0046A463 (00742ED8)
        AurKeyName key;                                               // [esp+10h]
        key.time = 0.0f;                                              // 0046A465 mov [esp+10h],eax (0)
        key.name[0] = '\0';                                           // 0046A469 mov [esp+14h],al
        parse_aur_key_name_004638f0(line + 5, &key);                  // 0046A46D..0046A476
        AurKeyNameList& events = field<AurKeyNameList>(self, 0x78);
        const I size = events.size;                                   // 0046A47B edx = [ebp+7Ch]
        I index = 0;                                                  // 0046A481
        for (; index < size; ++index)                                 // 0046A483..0046A4A3
            if (!(key.time > events.data[index].time)) break;         // fld/fcomp; test ah,41h; jne (below, equal, NaN)
        aur_key_name_list_insert_00468970(&events, nullptr, key, index); // 0046A4A5..0046A4B9
    }
    if (kotor_stricmp_0070acaf("length", firstword_00463530(line)) == 0)    // 0046A4BE..0046A4D4 (00742ED0)
        kotor_sscanf_006fae08(line + 6, "%f", &field<float>(self, 0x50));   // 0046A4D6..0046A4E3 (0073E2E4)
    if (kotor_stricmp_0070acaf("transtime", firstword_00463530(line)) == 0) // 0046A4EB..0046A501 (00742EC4)
        kotor_sscanf_006fae08(line + 9, "%f", &field<float>(self, 0x54));   // 0046A503..0046A510
    if (kotor_stricmp_0070acaf("animroot", firstword_00463530(line)) == 0) { // 0046A518..0046A52E (00742EB8)
        char* const root = &field<char>(self, 0x58);                  // 0046A530 lea esi,[ebp+58h]
        const int result = kotor_sscanf_006fae08(line + 8, "%s", root); // 0046A533..0046A53D (0073D5D0)
        if (result == 0) copy_from_result(root, result);              // 0046A545..0046A558
    }
    if (kotor_strncmp_006faff0(line, "node", 4u) == 0)                // 0046A55A..0046A56C (00742A8C)
        aurora_parse_node_004680e0(line + 4, self);                   // 0046A56E..0046A573
}                                                                     // 0046A585 ret 4
} // extern "C"
