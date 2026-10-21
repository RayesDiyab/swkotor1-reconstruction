// Private reconstruction of two original leaf setters.
//
// 004B1560 CServerExoAppInternal::ClearLastModuleFound:
//   mov dword ptr [ecx+0001B918h],7F000000h
//   mov dword ptr [ecx+0001B91Ch],0
//   ret
// 005EE0C0 CClientExoApp::ResetModuleResourcesLoaded:
//   mov eax,dword ptr [ecx+4]
//   mov byte ptr [eax+00000498h],0
//   ret
//
// This file is compiled only by the separate leaf TU
// (source/module_two_setters_tu.cpp). It is not included into the selected 0886
// translation unit. The original functions have no prologue, call, frame,
// cookie, EH/SEH, x87, global access or allocation. Do not add a null guard,
// trap or return value.

extern "C" void __fastcall recovered_server_internal_clear_last_module_found_004b1560(
    void* self, void*) {
  unsigned char* internal = static_cast<unsigned char*>(self);
  *reinterpret_cast<unsigned int*>(internal + 0x1B918) = 0x7F000000u;
  *reinterpret_cast<unsigned int*>(internal + 0x1B91C) = 0u;
}

extern "C" void __fastcall recovered_client_reset_module_resources_loaded_005ee0c0(
    void* self, void*) {
  unsigned char* internal = *reinterpret_cast<unsigned char**>(
      static_cast<unsigned char*>(self) + 4);
  internal[0x498] = 0;
}
