// lane-w3 composition bridge (Memory/Ports owners). INTEGRATION: #include this file as the LAST line of
// runtime1115 source/startup/source/server_mainloop_native_vm_canonical_cell.cpp (after every *_native.inc), so that
// NativeMemory, NativeLayoutLifecycle, NativeAreaPrimitive, NativeModuleHelpers and NativeFactionMember are visible.
// These are binding adapters only: each forwards to the EXISTING runtime1115 owner of the address; no original body
// is re-implemented and no address credit is claimed. Allocation inside these owners keeps the composition's own
// convention (::operator new / ::operator delete through the native ports), exactly as the server-side area load
// already uses them, so objects created here must be destroyed through their own tables (CLYT slot 0).
extern "C" {
// 004ADBA0 CWorldTimer::SetMinutesPerHour(BYTE) -> server_mainloop::timer_scale_004adba0 (0892-module_helpers.obj)
void __fastcall recovered_bridge_world_timer_set_minutes_per_hour_004adba0(void* timer, void*, Word minutes) {
    NativeMemory m; NativeModuleHelpers p(m);
    kotor::server_mainloop::timer_scale_004adba0(m, p, word(timer), minutes);
}
// 0058F6B0 CSWArea::CSWArea -> server_mainloop::area_base_0058f6b0 (0903-area_primitives.obj)
void* __fastcall recovered_bridge_area_base_construct_0058f6b0(void* self, void*) {
    NativeMemory m; NativeAreaPrimitive p(m);
    return ptr(kotor::server_mainloop::area_base_0058f6b0(m, p, word(self)));
}
// 0058F800 CSWArea::InitializeStaticObjects -> server_mainloop::area_initialize_0058f800 (0903-area_primitives.obj)
void __fastcall recovered_bridge_area_initialize_static_objects_0058f800(void* self, void*) {
    NativeMemory m;
    kotor::server_mainloop::area_initialize_0058f800(m, word(self));
}
// operator new(60h) + 005DE770 CLYT::CLYT -> server_mainloop::layout_construct_005de770 (0943-layout_lifecycle.obj),
// allocated exactly as the existing NativeAreaContent port does (CLYT slot 0 releases it with ::operator delete).
void* __fastcall recovered_bridge_layout_new_005de770(void*, void*) {
    void* const memory = ::operator new(0x60);
    NativeMemory m; NativeLayoutLifecycle p;
    alignas(8) unsigned char scratch[0x30];
    return ptr(kotor::server_mainloop::layout_construct_005de770(m, p, word(memory), word(scratch)));
}
// 005DF140 CLYT::LoadLayout(CResRef const&) -> server_mainloop::layout_load_005df140
Word __fastcall recovered_bridge_layout_load_005df140(void* layout, void*, void* resref) {
    NativeLayoutLifecycle p;
    alignas(8) unsigned char scratch[0x10];
    return kotor::server_mainloop::layout_load_005df140(p, word(layout), word(resref), word(scratch));
}
// 005A4C50 GetProperty0x24 (CLYT room count) -> server_mainloop::layout_count_005a4c50
Word __fastcall recovered_bridge_layout_room_count_005a4c50(void* layout, void*) {
    NativeMemory m;
    return kotor::server_mainloop::layout_count_005a4c50(m, word(layout));
}
// 005DE470 CLYT::GetRoomName(CResRef* out, int) -> server_mainloop::layout_reference_at_005de470 (0947-layout_extract.obj)
void* __fastcall recovered_bridge_layout_room_name_005de470(void* layout, void*, void* out, Word index) {
    NativeMemory m;
    return ptr(kotor::server_mainloop::layout_reference_at_005de470(m, word(layout), word(out), index));
}
// 005DE4A0 CLYT::GetRoomPosition(Vector* out, int) -> server_mainloop::layout_position_at_005de4a0
void* __fastcall recovered_bridge_layout_room_position_005de4a0(void* layout, void*, void* out, Word index) {
    NativeMemory m;
    return ptr(kotor::server_mainloop::layout_position_at_005de4a0(m, word(layout), word(out), index));
}
// 005DE450 CLYT::UnloadLayout -> server_mainloop::layout_release_005de450
void __fastcall recovered_bridge_layout_unload_005de450(void* layout, void*) {
    NativeMemory m; NativeLayoutLifecycle p;
    kotor::server_mainloop::layout_release_005de450(m, p, word(layout));
}
// 006449D0 CExoArrayList<unsigned long>::AddUnique -> server_mainloop::unique_append_006449d0 (1039-faction_member_helpers.obj)
void __fastcall recovered_bridge_dword_array_add_unique_006449d0(void* list, void*, Word value) {
    NativeMemory m; NativeFactionMember p(m);
    kotor::server_mainloop::unique_append_006449d0(m, p, word(list), value);
}
}
