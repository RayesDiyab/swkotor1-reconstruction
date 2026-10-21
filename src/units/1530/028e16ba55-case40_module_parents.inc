// Private module G1 parent bodies (source-only, not a table).
// Included at the end of the selected 0886 TU after the accepted four-body
// include. No trap/guard/partial body.
//
// 004AEB90 CServerExoApp::ClearLastModuleFound:
//   mov ecx,[ecx+4]; jmp 004B1560;
// The +4 field is the server-internal receiver. 004B1560 is the separately
// owned exact setter export; reuse it, do not inline or duplicate its body.

extern "C" void __fastcall
recovered_server_internal_clear_last_module_found_004b1560(void*, void*);

extern "C" void __fastcall case40_004aeb90(void* server_outer, void*) {
  void* internal = *reinterpret_cast<void**>(
      static_cast<unsigned char*>(server_outer) + 4);
  recovered_server_internal_clear_last_module_found_004b1560(internal, nullptr);
}
