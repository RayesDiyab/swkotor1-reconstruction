// Lane W1 integration request for the 0886 owner (server_mainloop_native_vm_canonical_cell.cpp).
// #include this file at the END of that TU (after vm_entering_commands_native.inc). It adds extern "C"
// fastcall-with-dummy-EDX wrappers over existing portable providers whose native glue (NativeMemory,
// NativeAreaHeight, NativeCreatureForcepoints, native_area_safe_position) is TU-local to 0886.
// No new behaviour; each wrapper only instantiates the glue 0886 already uses for the same address.
extern "C" void __fastcall w1x_object_set_hp_004cc280(void* object, void*, unsigned hp) {
  NativeMemory m; kotor::server_mainloop::creature_hitpoints_set_004cc280(m, word(object), hp);
}
// Returns the ComputeHeight result as IEEE single bits (the portable provider's contract).
extern "C" unsigned __fastcall w1x_area_height_bits_004bc380(void* area, void*, void* point, unsigned flag, unsigned* room) {
  NativeMemory m; NativeAreaHeight p(m); unsigned scratch[4];
  return kotor::server_mainloop::area_height_bits_004bc380(m, p, word(area), word(point), flag, word(room), word(scratch));
}
extern "C" unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e) {
  return native_area_safe_position(word(area), a, b, c, d, e);
}
// CSWSCreatureStats::GetSkillRank(skill, versus object, base_only); original return is AL.
extern "C" unsigned __fastcall w1x_creature_skill_rank_005aa570(void* stats, void*, unsigned skill, void* versus, unsigned base_only) {
  NativeMemory m; NativeCreatureSkill p(m);
  return kotor::server_mainloop::creature_skill_005aa570(m, p, word(stats), skill, word(versus), base_only);
}
// CExoArrayList<unsigned long>::AddUnique(value), via 0886's native_unique_append glue.
extern "C" void __fastcall w1x_unique_append_006449d0(void* list, void*, unsigned value) {
  native_unique_append(word(list), value);
}
// CSWSObject::SetOrientation(const Vector*), via the 1018 portable provider 0886 already uses (object_orientation_004cd170).
extern "C" void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* direction) {
  NativeMemory m; kotor::server_mainloop::object_orientation_004cd170(m, word(object), word(direction));
}
// CExoArrayList<pointer>::Insert(value, index), via the 1032 provider and 0886's NativeEffectInsert glue.
extern "C" void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, unsigned value, unsigned index) {
  NativeMemory m; NativeEffectInsert p; kotor::server_mainloop::effect_insert_00616940(m, p, word(list), value, index);
}
// Original return is AX-only (see creature_forcepoints.hpp).
extern "C" unsigned __fastcall w1x_creature_max_fp_004fd490(void* creature, void*) {
  NativeMemory m; NativeCreatureForcepoints p(m);
  return kotor::server_mainloop::creature_forcepoints_004fd490(m, p, word(creature));
}
// CServerExoAppInternal::LoadModule(name, player), via 0886's load_module_004b95b0 and its NativeLoadRequest glue
// (the same call 0886's server_system_native.inc load_module port makes).
extern "C" unsigned __fastcall w1x_server_load_module_004b95b0(void* internal, void*, void* name, unsigned player) {
  NativeMemory m; NativeLoadRequest p(m); alignas(8) unsigned char scratch[0xe8];
  return kotor::server_mainloop::load_module_004b95b0(m, p, word(internal), word(name), player, word(scratch));
}
