// Private module G1 nested bodies (reconstructed full bodies, not a table).
// Included at the end of the selected 0886 TU after module_native.inc so the
// selected NativeMemory/module_initial_vtable and selected provider declarations
// are visible. No trap/guard/partial body is present.
//
// Source-closed here:
//   00578C30 CSWSAreaMap::~CSWSAreaMap
//   0059A980 CSWSScriptVarTable::~CSWSScriptVarTable
//   004C4C60 CResHelper<CResIFO,2014>::~CResHelper
//   004C4F30 CResHelper<CResIFO,2014>::~CResHelper deleting destructor
//
// Still root-gated and NOT here: 00508710 (indirect object dtor set),
// 004C44D0 (005EE0C0), 004AEB90 (004B1560), 004C68A0/004C8940/004C6BC0.

// Selected providers not declared by the selected TU. c2da_destroy_array's
// declaration comes from ai_event_cleanup_native_declarations.hpp; do not
// redeclare it here.
extern "C" void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);
extern "C" void __fastcall kotor_fastcall_0059ea30(void*, void*);

namespace {

using Case40ArrayDtor =
    void (__fastcall*)(kotor_reconstruction::StartupString*, void*);

struct Case40CResIFO {
  const void* methods;   // +0x00
  Word requested;        // +0x04
  void* resource;        // +0x08
  unsigned char reference[16];  // +0x0C
};
static_assert(sizeof(Case40CResIFO) == 0x1C,
              "CResHelper<CResIFO,2014> layout");

// 004C8470-equivalent unwind cleanup: read [table], destroy the entry array,
// free its header, then clear [table]. The original ECX is the table's `this`,
// so the unwind clears the table's [0], not just a local copy.
struct Case40ScriptVarArrayScope {
  void* table;
  bool live;
  Case40ScriptVarArrayScope(void* t, bool l) : table(t), live(l) {}
  ~Case40ScriptVarArrayScope() {
    if (!live || !table) return;
    const Word base = *reinterpret_cast<Word*>(table);
    if (!base) return;
    const Word header = base - 4;
    const unsigned count = *reinterpret_cast<unsigned*>(ptr(header));
    c2da_destroy_array_006fa852(
        ptr(base), 0x10, count,
        reinterpret_cast<Case40ArrayDtor>(&kotor_fastcall_0059ea30));
    live_free(ptr(header));
    *reinterpret_cast<Word*>(table) = 0;
  }
};

}  // namespace

// 00578C30 CSWSAreaMap::~CSWSAreaMap. thiscall, no args, ret 0.
// Faithful quirk: +4 is cleared only when +0 was non-null.
extern "C" void __fastcall case40_00578c30(void* self, void*) {
  unsigned char* s = static_cast<unsigned char*>(self);
  void* p = *reinterpret_cast<void**>(s);
  if (!p) return;
  live_free(p);
  *reinterpret_cast<void**>(s) = nullptr;
  *reinterpret_cast<void**>(s + 4) = nullptr;
}

// 0059A980 CSWSScriptVarTable::~CSWSScriptVarTable. thiscall, no args, ret 0.
// Entries are 0x10 bytes: name +0, type +8, value +0xC. Type 3 value is a
// CExoString, type 5 is a 004015A0-owned object; both are destroyed then freed.
// The array itself is destroyed through 006FA852 with selected 0059EA30, then
// its header is freed. [this+4] is deliberately not cleared.
extern "C" void __fastcall case40_0059a980(void* self, void*) {
  unsigned char* s = static_cast<unsigned char*>(self);
  const Word base = *reinterpret_cast<Word*>(s);
  Case40ScriptVarArrayScope holder{s, true};
  const int count = *reinterpret_cast<int*>(s + 4);
  for (int i = 0; i < count; ++i) {
    unsigned char* e =
        static_cast<unsigned char*>(ptr(base + static_cast<Word>(i * 0x10)));
    if (*reinterpret_cast<Word*>(e + 8) == 3) {
      Word p = *reinterpret_cast<Word*>(e + 0xC);
      if (p) {
        frame_LogStringDtor_005e5c20(ptr(p), nullptr);
        live_free(ptr(p));
      }
    }
    if (*reinterpret_cast<Word*>(e + 8) == 5) {
      Word p = *reinterpret_cast<Word*>(e + 0xC);
      if (p) {
        kotor_m15e_slot090_void0_004015a0(ptr(p), nullptr);
        live_free(ptr(p));
      }
    }
  }
  if (base) {
    const Word header = base - 4;
    const unsigned n = *reinterpret_cast<unsigned*>(ptr(header));
    c2da_destroy_array_006fa852(
        ptr(base), 0x10, n,
        reinterpret_cast<Case40ArrayDtor>(&kotor_fastcall_0059ea30));
    live_free(ptr(header));
    holder.live = false;
  }
  *reinterpret_cast<Word*>(s) = 0;
}

// 004C4C60 CResHelper<CResIFO,2014>::~CResHelper. thiscall, no args, ret 0.
// Store the selected module_initial_vtable ARRAY ADDRESS (00859A6C), not the
// original 007458D4 and not the slot value 0x573B70 (the 004C4F30 trap).
extern "C" void __fastcall case40_004c4c60(void* self, void*) {
  Case40CResIFO* h = static_cast<Case40CResIFO*>(self);
  h->methods = static_cast<const void*>(module_initial_vtable);
  void* resource = h->resource;
  if (!resource) return;
  if (!c2da_not_equal_text_00406060(h->reference, nullptr, "")) return;
  if (h->requested) {
    c2da_cancel_00409b00(resource, nullptr);
    h->requested = 0;
  }
  NativeMemory m;
  const Word manager = m.read(0x7A39E8);
  if (c2da_release_00409cf0(ptr(manager), nullptr, resource)) return;
  // Selected ifo_vtable slot0 is the three-argument __fastcall thunk
  // 0056DC60 -> 00630E10 (ECX=resource, EDX unused, one stack flag, ret 4).
  using Destroy = void* (__fastcall*)(void*, void*, Word);
  Destroy destroy =
      reinterpret_cast<Destroy>(m.read(m.read(word(resource))));
  destroy(resource, nullptr, 1);
  h->resource = nullptr;
}

// 004C4F30 CResHelper<CResIFO,2014>::~CResHelper deleting destructor.
// ECX=this, one stack flags argument, ret 4, returns this.
extern "C" void* __fastcall case40_004c4f30(void* self, void*, Word flags) {
  case40_004c4c60(self, nullptr);
  if (flags & 1) live_free(self);
  return self;
}
