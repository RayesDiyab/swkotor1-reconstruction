// 1127 integrator glue: W5 t3 import names whose owners export the same function under a different name.
// A strong definition of the W5 import name overrides W5's /alternatename trap fallback (w5_trap_* / w5_t2_trap_*).
//   00503590 CGameEffect::~CGameEffect   W5 @w5_import_game_effect_destroy_00503590@8 -> W1 tier6b @recovered_effect_destroy_00503590@8
//            (W5 source/w5_foreign.hpp:22 (void* effect, void*) == W1 (void*, void*); W5 frees the block itself afterwards)
//   004C5FE0 CSWSModule::FindObjectByTagOrdinal
//            W5 @w5_t2_module_find_object_by_tag_ordinal_004c5fe0@16 (module, edx, void* tag, U ordinal)
//            -> W7 t5 @recovered_module_find_object_by_tag_004c5fe0@16 (module, edx, const void* tag, int nth): same order, same dwords
// thiscall-as-fastcall with dummy EDX on both sides.
extern "C" {
void __fastcall recovered_effect_destroy_00503590(void*, void*);
unsigned __fastcall recovered_module_find_object_by_tag_004c5fe0(void* module, void*, const void* tag, int nth);

void __fastcall w5_import_game_effect_destroy_00503590(void* effect, void*) {
  recovered_effect_destroy_00503590(effect, nullptr);
}
unsigned __fastcall w5_t2_module_find_object_by_tag_ordinal_004c5fe0(void* module, void*, void* tag, unsigned ordinal) {
  return recovered_module_find_object_by_tag_004c5fe0(module, nullptr, tag, static_cast<int>(ordinal));
}
}
