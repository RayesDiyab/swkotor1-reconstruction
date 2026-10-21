// Lane vmA: C-linkage wrappers over existing 0886-unit glue (no new behaviour), for CreateObject 0052F820
// (claude_vm_vmA_endar.cpp).  The coordinator includes this file at the end of
// src0886\server_mainloop_native_vm_canonical_cell.cpp, after "claude_0886_exports.inc".
// Each wrapper instantiates exactly the glue the unit's area loaders already use for the same address.

// 00587A70 CSWSPlaceable::LoadFromTemplate(CResRef, int) thiscall RET 8 -> 1150 port via the 0886 NativePlaceableTemplate
// glue (same call NativeAreaPlaceables::template_load makes; scratch = 0x20 bytes: s+0xC struct, s+0x10/s+0x18 strings).
extern "C" unsigned __fastcall kotor_fastcall_00587a70(void* placeable, void*, void* resref, unsigned flag){
  NativeMemory m; NativePlaceableTemplate p(m); alignas(8) unsigned scratch[8];
  return placeable_template_00587a70(m,p,word(placeable),word(resref),flag,word(scratch));
}

// 005C7F30 CSWSWaypoint::LoadWaypoint(CResGFF*, CResStruct*) thiscall RET 8 -> 1136 port via the 0886 NativeWaypointLoad
// glue (same call NativeAreaWaypoints::load makes, scratch 0x58).
extern "C" unsigned __fastcall kotor_fastcall_005c7f30(void* waypoint, void*, void* gff, void* root){
  NativeMemory m; NativeWaypointLoad p(m); alignas(8) unsigned char scratch[0x58];
  return waypoint_load_005c7f30(m,p,word(waypoint),word(gff),word(root),word(scratch));
}
