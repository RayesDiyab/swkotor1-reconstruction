// Private full reconstruction of 004C44D0 CSWSModule::RemoveModuleResources.
// Source-only. Included after the frozen v4 source; no table/module_native edit.
//
// ABI: thiscall, one CExoString* stack argument, RET 4.
// The incoming ECX (self) is not read; the input is only the name string.
//
// The body uses real scoped CExoString objects. Every construction, assignment,
// concatenation, format, cstr and destruction calls the selected real provider;
// there is no raw-byte/manual-destructor substitute, catch, guard, fake success
// or partial arm. The named `suffix` is constructed first/destroyed last; the
// named `lips` is constructed at the LIPS block and destroyed before `suffix`,
// after the final 005EE0C0 reset. All other strings are block-scoped
// temporaries, so their normal/unwind destruction is reverse construction.

namespace kotor_menu_10_19 {
struct String {
  String* __thiscall default_005b3190();
};
}

extern "C" void __fastcall resman_remove_encapsulated_00408820(
    void*, void*, void*);
extern "C" void __fastcall kotor_fastcall_00408830(void*, void*, void*);
// async_ref_construct_string_00406d60 is provided by the selected
// vm_run_native_declarations.hpp typed provider + inline void* adapter.
extern "C" void __fastcall
recovered_client_reset_module_resources_loaded_005ee0c0(void*, void*);

namespace {

struct Case40CExoString {
  void* text;
  unsigned length;

  Case40CExoString() {
    reinterpret_cast<kotor_menu_10_19::String*>(this)->default_005b3190();
  }
  explicit Case40CExoString(const char* literal) {
    (void)frame_LogStringCtor_005e5a90(this, nullptr, literal);
  }
  explicit Case40CExoString(const kotor_reconstruction::StartupString* other) {
    reinterpret_cast<kotor_menu_10_19::String*>(this)->default_005b3190();
    (void)async_assign_string_005e5c50(
        this, nullptr, const_cast<kotor_reconstruction::StartupString*>(other));
  }
  Case40CExoString(const Case40CExoString& other) {
    reinterpret_cast<kotor_menu_10_19::String*>(this)->default_005b3190();
    (void)async_assign_string_005e5c50(
        this, nullptr, const_cast<Case40CExoString*>(&other));
  }
  Case40CExoString& operator=(const Case40CExoString& other) {
    (void)async_assign_string_005e5c50(
        this, nullptr, const_cast<Case40CExoString*>(&other));
    return *this;
  }
  void assign_from(const kotor_reconstruction::StartupString* other) {
    (void)async_assign_string_005e5c50(
        this, nullptr, const_cast<kotor_reconstruction::StartupString*>(other));
  }
  ~Case40CExoString() {
    frame_LogStringDtor_005e5c20(this, nullptr);
  }
  operator kotor_reconstruction::StartupString*() {
    return reinterpret_cast<kotor_reconstruction::StartupString*>(this);
  }
  operator const kotor_reconstruction::StartupString*() const {
    return reinterpret_cast<const kotor_reconstruction::StartupString*>(this);
  }
};

// Selected 005E5D10 contract: ECX=left, stack=(out,right), returns out.
Case40CExoString concat(const Case40CExoString& left,
                        const Case40CExoString& right) {
  Case40CExoString out;
  (void)GuiConcat005e5d10(
      reinterpret_cast<kotor_reconstruction::StartupString*>(
          const_cast<Case40CExoString*>(&left)),
      nullptr,
      reinterpret_cast<kotor_reconstruction::StartupString*>(&out),
      reinterpret_cast<const kotor_reconstruction::StartupString*>(&right));
  return out;
}

}  // namespace

extern "C" void __fastcall case40_004c44d0(
    void* /*self*/, void*, kotor_reconstruction::StartupString* name) {
  NativeMemory m;
  menu_resources::Manager* manager =
      reinterpret_cast<menu_resources::Manager*>(m.read(0x7A39E8));

  // 004C44FB: remove the incoming module name first.
  resman_remove_encapsulated_00408820(manager, nullptr, name);

  // 004C4504..004C455A: suffix = name after the first ':' or the whole name.
  Case40CExoString suffix;
  const int colon = find_005e5f20(name, nullptr, 0x3A, 0);
  if (colon == -1) {
    suffix.assign_from(name);
  } else {
    const unsigned length = async_string_length_005e5790(name, nullptr);
    Case40CExoString part;
    (void)base_ini_reconstruction::right_005e6170(
        name, nullptr,
        reinterpret_cast<kotor_reconstruction::StartupString*>(&part),
        static_cast<int>(length - static_cast<unsigned>(colon) - 1u));
    suffix = part;
  }

  // 004C455F..004C456A: 0063E7C0 is exactly xor eax,eax; ret 4. Call the
  // selected provider for its contract, then use its known actual result 0.
  // (Avoid a void-to-unsigned function-pointer call under /W4 /WX.)
  packed_return_false_0063e7c0(manager, nullptr, &suffix);
  const unsigned edi = 0;

  // 004C456F..004C459C: bare CURRENTGAME: directory.
  {
    Case40CExoString literal("CURRENTGAME:");
    (void)manager->directory_00408800(literal);
  }

  // 004C45A1..004C45EC: CURRENTGAME:<suffix> remove image.
  {
    Case40CExoString literal("CURRENTGAME:");
    Case40CExoString out = concat(literal, suffix);
    kotor_fastcall_00408830(manager, nullptr, &out);
  }

  // 004C45F1..004C463C: CURRENTGAME:<suffix> remove encapsulated.
  {
    Case40CExoString literal("CURRENTGAME:");
    Case40CExoString out = concat(literal, suffix);
    resman_remove_encapsulated_00408820(manager, nullptr, &out);
  }

  // 004C4641..004C466C: bare CURRENTGAME: release.
  {
    Case40CExoString literal("CURRENTGAME:");
    kotor_fastcall_004088d0(manager, nullptr, &literal);
  }

  // 004C4671..004C469C: bare MODULES: directory.
  {
    Case40CExoString literal("MODULES:");
    (void)manager->directory_00408800(literal);
  }

  // 004C46A1..004C4710: ref(suffix), exists 0x7DB; true arm.
  {
    unsigned char ref[16];
    (void)async_ref_construct_string_00406d60(ref, nullptr, &suffix);
    if (manager->exists_00408bc0(
            reinterpret_cast<menu_resources::Ref*>(ref), 0x7DB, nullptr)) {
      Case40CExoString literal("MODULES:");
      Case40CExoString out = concat(literal, suffix);
      resman_remove_encapsulated_00408820(manager, nullptr, &out);
    } else {
      // 004C4715..004C4770: ref(suffix + "_s"), exists 0xBBA. The probe/ref
      // are destroyed before the branch.
      unsigned esi;
      {
        Case40CExoString underscore("_s");
        Case40CExoString probe = concat(suffix, underscore);
        unsigned char ref_s[16];
        (void)async_ref_construct_string_00406d60(ref_s, nullptr, &probe);
        esi = manager->exists_00408bc0(
            reinterpret_cast<menu_resources::Ref*>(ref_s), 0xBBA, nullptr);
      }
      if (esi) {
        if (edi == 0) {
          // 004C4785..004C480F: MODULES:<suffix>_s remove image.
          Case40CExoString underscore("_s");
          Case40CExoString literal("MODULES:");
          Case40CExoString base = concat(literal, suffix);
          Case40CExoString out = concat(base, underscore);
          kotor_fastcall_00408830(manager, nullptr, &out);
        } else {
          // 004C4815..004C485F: LIVE<edi>:MODULES\<suffix> remove image.
          Case40CExoString fmt;
          StartupFormat005e5680(
              reinterpret_cast<kotor_reconstruction::StartupString*>(&fmt),
              "LIVE%d:MODULES\\", static_cast<int>(edi));
          Case40CExoString out = concat(fmt, suffix);
          kotor_fastcall_00408830(manager, nullptr, &out);
        }
      }
    }
  }

  // 004C4871..004C489C: bare MODULES: release.
  {
    Case40CExoString literal("MODULES:");
    kotor_fastcall_004088d0(manager, nullptr, &literal);
  }

  // 004C48A5..004C4907: long-lived lips = LIPS:<suffix>_loc; then localization.
  Case40CExoString lips;
  StartupFormat005e5680(
      reinterpret_cast<kotor_reconstruction::StartupString*>(&lips),
      "LIPS:%s_loc", WindowCStr005e5670(&suffix, nullptr));
  resman_remove_encapsulated_00408820(manager, nullptr, &lips);
  {
    Case40CExoString literal("LIPS:localization");
    resman_remove_encapsulated_00408820(manager, nullptr, &literal);
  }

  // 004C490C..004C4A08: ref(suffix + "_a"), exists 0xBBA; true arm RIMS.
  {
    unsigned exists;
    {
      Case40CExoString underscore("_a");
      Case40CExoString probe = concat(suffix, underscore);
      unsigned char ref[16];
      (void)async_ref_construct_string_00406d60(ref, nullptr, &probe);
      exists = manager->exists_00408bc0(
          reinterpret_cast<menu_resources::Ref*>(ref), 0xBBA, nullptr);
    }
    if (exists) {
      Case40CExoString underscore("_a");
      Case40CExoString literal("RIMS:");
      Case40CExoString base = concat(literal, suffix);
      Case40CExoString out = concat(base, underscore);
      kotor_fastcall_00408830(manager, nullptr, &out);
    }
  }

  // 004C4A0D..004C4B09: ref(suffix + "_adx"), exists 0xBBA; true arm RIMS.
  {
    unsigned exists;
    {
      Case40CExoString underscore("_adx");
      Case40CExoString probe = concat(suffix, underscore);
      unsigned char ref[16];
      (void)async_ref_construct_string_00406d60(ref, nullptr, &probe);
      exists = manager->exists_00408bc0(
          reinterpret_cast<menu_resources::Ref*>(ref), 0xBBA, nullptr);
    }
    if (exists) {
      Case40CExoString underscore("_adx");
      Case40CExoString literal("RIMS:");
      Case40CExoString base = concat(literal, suffix);
      Case40CExoString out = concat(base, underscore);
      kotor_fastcall_00408830(manager, nullptr, &out);
    }
  }

  // 004C4B0E..004C4B3A: HD0:players remove encapsulated.
  {
    Case40CExoString literal("HD0:players");
    resman_remove_encapsulated_00408820(manager, nullptr, &literal);
  }

  // 004C4B3F..004C4B47: final client reset, last external call.
  void* client = reinterpret_cast<void*>(m.read(m.read(0x7A39FC) + 4));
  recovered_client_reset_module_resources_loaded_005ee0c0(client, nullptr);
  // 004C4B54/004C4B65: lips then suffix destructors; then RET 4.
}
