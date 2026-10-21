#include "9f5a6fa2ed-start_module.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Tier 9: 004BA920 CServerExoAppInternal::StartNewModule(CExoString* module name).
namespace w1::start_module {
namespace {
constexpr U kModulesAlias = 0x73d90c;      // "MODULES:"
constexpr U kModuleSave = 0x745128;        // "modulesave" (C2DA resref)
constexpr U kResModule = 0x7db, kResRim = 0xbba;   // resource types checked by Exists (MOD / RIM)
U client(Ports& p) { return p.read(p.global(kAppManager) + 4); }
}
U get_module_004b14f0(U internal, U scratch, Ports& p) {
  U id = p.read(internal + 0x10060);                                // current module id
  if (p.read(internal + 0x1b918) == id) return p.read(internal + 0x1b91c);   // 004B14F4..004B1500: cached
  p.write(scratch, 0);
  p.call(0x4d8230, p.read(internal + 0x1005c), {id, scratch});      // GetGameObject (result code ignored)
  U o = p.read(scratch);
  if (!o) { p.write(internal + 0x1b918, kInvalidObject); p.write(internal + 0x1b91c, 0); return 0; }
  p.write(internal + 0x1b918, p.read(internal + 0x10060));
  U m = p.vcall(o, 0x20, {});                                       // AsSWSModule
  p.write(internal + 0x1b91c, m);
  return m;
}
U start_new_module_004ba920(U internal, U name, U e, Ports& p) {
  // e == esp after the four pushes (frame 0x2C): +0x10 CExoString, +0x14.. CResRef (16 bytes), +0x20 CExoString,
  // +0x38 the argument slot (reused for the C2DA pointer).
  if (p.read(internal + 0x100b4) == 1) return 0;                    // 004BA93E..004BA947: a module change is already pending
  U cl = client(p);
  U hint = p.call(0x5ee090, cl, {name, 0});                         // CClientExoApp::GetNextLoadScreenHintSTRREF
  p.call(0x5edf00, cl, {hint});                                     // SetLoadScreenHint
  p.call(0x5ee040, client(p), {0});                                 // SetLoadScreenHintUpdateTimer
  p.call(0x5edc80, client(p), {0});                                 // SetCanSendDialog
  p.call(0x62ec60, p.call(0x5ed690, client(p), {}), {0});           // GetInGameGui()->SetGlobalDialogState(0) (push 0 precedes)
  p.call(0x6332b0, p.call(0x5ed690, client(p), {}), {});            // GetInGameGui()->CloseDialog
  p.call(0x5d5e70, p.global(0x7a39ec), {});                         // CExoSound::StopAllOneShots
  p.call(0x5edb20, client(p), {});                                  // PlayLoadModuleMovies
  p.call(0x5e5a90, e + 0x10, {kModulesAlias});
  p.call(0x408800, p.global(0x7a39e8), {e + 0x10});                 // CExoResMan::AddResourceDirectory("MODULES:")
  p.call(0x5e5c20, e + 0x10, {});
  p.call(0x406d60, e + 0x14, {name});                               // CResRef(name)
  bool found = p.call(0x408bc0, p.global(0x7a39e8), {e + 0x14, kResModule, 0}) != 0;   // Exists(.mod)
  if (!found) {
    p.call(0x406d60, e + 0x14, {name});
    found = p.call(0x408bc0, p.global(0x7a39e8), {e + 0x14, kResRim, 0}) != 0;         // Exists(.rim)
  }
  p.call(0x5e5a90, found ? e + 0x20 : e + 0x10, {kModulesAlias});
  p.call(0x4088d0, p.global(0x7a39e8), {found ? e + 0x20 : e + 0x10});   // RemoveResourceDirectory("MODULES:")
  p.call(0x5e5c20, found ? e + 0x20 : e + 0x10, {});
  if (!found) return 0;                                             // 004BAA82
  p.write(internal + 0x100b4, 1); p.write(internal + 0x1007c, 1);
  p.call(0x4b4f50, internal, {});                                   // ForceRunAreaOnExitScripts
  if (!p.call(0x4b2ba0, internal, {}) || !p.call(0x4b2e70, internal, {})) {   // StorePlayerCharacters / StoreCurrentModule
    p.write(internal + 0x1007c, 0);
    return 0;
  }
  U mem = p.call(0x6fa7e6, 0, {0x54});                              // new C2DA("modulesave", 0)
  U table = 0;
  if (mem) {
    p.call(0x406d80, e + 0x14, {kModuleSave});                      // CResRef("modulesave") (temporary)
    table = p.call(0x413cc0, mem, {e + 0x14, 0});                   // C2DA::C2DA(CResRef, 0)
  }
  if (table && !p.call(0x4143b0, table, {})) { p.vcall(table, 0, {1}); table = 0; }   // Load2DArray failed: delete
  p.call(0x4b2380, internal, {name, table});                        // DoModuleEnterSaveCleanup
  if (p.call(0x4b14f0, internal, {})) {                             // GetModule
    if (!table || p.call(0x4b5050, internal, {name, table})) p.call(0x4b8300, internal, {name, table});   // AutoSave / DoPCAutosave
  }
  if (table) p.vcall(table, 0, {1});                                // scalar deleting destructor
  return p.call(0x4ba820, internal, {name});                        // ExecuteLoadModule
}
}
