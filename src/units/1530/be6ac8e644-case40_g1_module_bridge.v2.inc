// Private case40 G1 bridge v2 (harness/interface code, not an original body).
// Included at the end of the selected 0886 TU so it can use the actual
// NativeMemory/NativePorts constructors; it does not copy or redeclare them.
// Constructs the real CSWSModule through the selected 0886 adapter and performs
// the exact original 004BB0BA publication. Case40 normal end_m01aa (type 0x809)
// domain: the mainloop's copy_module/start_module/error and 0xbc1 load-game
// block are deliberately not driven here.
//
// v2: replace the raw 8-byte CExoString storage/manual dtor with a minimal
// real-ctor/real-dtor RAII object so a C++ throw after construction cannot skip
// cleanup. No catch, guard, fallback or fabricated state. The inner scope closes
// before the interface output writes, preserving v1's dtor order.
extern "C" unsigned __fastcall recovered_case40_module_build_publish(
    void* server_internal, void* /*edx*/, const char* name, unsigned event_arg,
    void** out_module, unsigned* out_module_id) {
  NativeMemory m;
  NativePorts p(m);
  Word instance = 0;
  Word id = 0;
  {
    struct CExoStringScope {
      alignas(8) unsigned char storage[8];
      explicit CExoStringScope(const char* text) {
        frame_LogStringCtor_005e5a90(storage, nullptr, text);
      }
      ~CExoStringScope() {
        frame_LogStringDtor_005e5c20(storage, nullptr);
      }
      CExoStringScope(const CExoStringScope&) = delete;
      CExoStringScope& operator=(const CExoStringScope&) = delete;
      Word word_value() { return word(storage); }
    } module_name(name);

    // Original mainloop: instance = raw ? construct(raw, name, 0, event) : 0.
    // The literal 0 and event_arg positions are the original call-site order.
    const Word raw = p.allocate_006fa7e6(0x224);
    instance = raw ? p.construct_module_004c84a0(
        raw, module_name.word_value(), 0, event_arg) : 0;
    // Original 004BB0B3 reads [module+0x20] before 004BB0BA stores it. If the
    // 0x224 allocation failed, instance==0 and this read faults at 0x20.
    id = m.read(instance + 0x20);
    m.write(word(server_internal) + 0x10060, id);
  }  // module_name dtor here, before the interface output writes
  *out_module = ptr(instance);
  *out_module_id = id;
  return 1;
}
