// Private full source for the remaining four module-owner bodies:
//   00508710 CSWSArea::ClearObjects
//   004C68A0 CSWSModule::~CSWSModule
//   004C8940 CSWSModule deleting destructor
//   004C6BC0 NVThunkModuleGameObject_destructor
//
// Source-only. Included after the frozen v4 + 004C44D0 sources. No table/
// module_native edit, no new original body/address, no object-dtor/004F0FD0.
//
// 004C68A0 carries the mapped original 18-entry field/list/node unwind cleanup
// through a private scoped C++ guard. The C-linkage throw-through exception-
// specification/recipe boundary (/EHs vs /EHsc) is documented separately.
// 00508710 executes the genuine actual-receiver virtual slot0; selected object
// destructor coverage (creature 004F8410/004F0FD0, player/AOE/store/encounter/
// traps/base) remains an integration gate, not a source omission.

namespace kotor_list {
struct Internal;
}

namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);
}

extern "C" void* __fastcall kotor_fastcall_004ae780(void*, void*, Word);
// kotor_fastcall_004d8230 is declared by the selected v5 TU:
// unsigned char __fastcall(void*,void*,unsigned,void**).
extern "C" Word __fastcall recovered_object_array_004aed70(void*, void*);
extern "C" void __fastcall recovered_object_array_erase_004d8290(
    void*, void*, Word);
extern "C" void __fastcall world_journal_entry_destroy_004c4030(void*, void*);
extern "C" const void* kotor_vtable_cgameobject_007457a0[];

extern "C" void __fastcall case40_00508710(void*, void*);
extern "C" void __fastcall case40_004c68a0(void*, void*);
extern "C" void* __fastcall case40_004c8940(void*, void*, Word);
extern "C" void* __fastcall case40_004c6bc0(void*, void*, Word);
extern "C" void __fastcall case40_00578c30(void*, void*);
extern "C" void __fastcall case40_0059a980(void*, void*);
extern "C" void __fastcall case40_004aeb90(void*, void*);
extern "C" void __fastcall case40_004c4c60(void*, void*);
extern "C" void __fastcall case40_004c44d0(
    void*, void*, kotor_reconstruction::StartupString*);

namespace {

using Case40ArrayDtor2 =
    void (__fastcall*)(kotor_reconstruction::StartupString*, void*);

}  // namespace

// 00508710 CSWSArea::ClearObjects. thiscall/no args/RET0.
extern "C" void __fastcall case40_00508710(void* area, void*) {
  NativeMemory m;
  const Word s = word(area);
  const Word server = m.read(m.read(0x7A39FC) + 8);
  const Word array = recovered_object_array_004aed70(ptr(server), nullptr);
  int count = static_cast<int>(m.read(s + 0x194));
  for (int i = count - 1; i >= 0; --i) {
    const Word id = m.read(m.read(s + 0x190) + static_cast<Word>(i * 4));
    // Original reserves the out slot with the incoming this; the lookup
    // overwrites it on both the found and not-found paths.
    void* out = ptr(s);
    (void)kotor_fastcall_004d8230(ptr(array), nullptr, id, &out);
    if (out) {
      using ObjectDtor = void* (__thiscall*)(void*, Word);
      ObjectDtor dtor =
          reinterpret_cast<ObjectDtor>(m.read(m.read(word(out))));
      (void)dtor(out, 1);
    }
  }
  m.write(s + 0x194, 0);
}

namespace {

// Private 004C68A0 cleanup guard. The stages are the original unwind-map live
// sets in cleanup order; step() advances the stage BEFORE running the action, so
// a throwing destructor's own object is never re-destroyed. The guard is armed
// after the +0x1D8 check and before 00409B80, matching original state 0xF.
struct Case40ModuleCleanup {
  enum Stage {
    Done,
    BaseHelper,
    BaseGameObject,
    List28,
    List2C,
    List48,
    Loc4C,
    String5C,
    Loc64,
    Table8C,
    ArrayB0,
    Free130,
    Free13C,
    Journal1CC,
    Free1DC,
    String1F8,
    String204,
    NodeString,
    NodeLoc8
  };

  NativeMemory& m;
  Word self;
  Word element;
  Stage stage;

  Case40ModuleCleanup(NativeMemory& mm, Word s)
      : m(mm), self(s), element(0), stage(String204) {}

  ~Case40ModuleCleanup() {
    while (stage != Done) step();
  }

  void free_clear(Word off) {
    Word p = m.read(self + off);
    live_free(ptr(p));
    m.write(self + off, 0);
  }

  void destroy_list_field(Word off) {
    Word list = m.read(self + off);
    if (list) {
      base_ini_reconstruction::destroy_list_005e9ab0(
          reinterpret_cast<kotor_list::Internal*>(list), nullptr);
      live_free(ptr(list));
    }
  }

  void destroy_journal() {
    Word p = m.read(self + 0x1CC);
    if (p) {
      Word header = p - 4;
      unsigned n = m.read(header);
      c2da_destroy_array_006fa852(
          ptr(p), 0x24, n,
          reinterpret_cast<Case40ArrayDtor2>(
              &world_journal_entry_destroy_004c4030));
      live_free(ptr(header));
    }
    m.write(self + 0x1CC, 0);
  }

  void step() {
    switch (stage) {
      case NodeLoc8:
        stage = NodeString;
        exo_locstring_destroy_005ea000(ptr(element + 8), nullptr);
        break;
      case NodeString:
        stage = String204;
        frame_LogStringDtor_005e5c20(ptr(element), nullptr);
        break;
      case String204:
        stage = String1F8;
        frame_LogStringDtor_005e5c20(ptr(self + 0x204), nullptr);
        break;
      case String1F8:
        stage = Free1DC;
        frame_LogStringDtor_005e5c20(ptr(self + 0x1F8), nullptr);
        break;
      case Free1DC:
        stage = Journal1CC;
        free_clear(0x1DC);
        break;
      case Journal1CC:
        stage = Free13C;
        destroy_journal();
        break;
      case Free13C:
        stage = Free130;
        free_clear(0x13C);
        break;
      case Free130:
        stage = ArrayB0;
        free_clear(0x130);
        break;
      case ArrayB0:
        stage = Table8C;
        c2da_destroy_array_006fa852(
            ptr(self + 0xB0), 8, 15,
            reinterpret_cast<Case40ArrayDtor2>(&frame_LogStringDtor_005e5c20));
        break;
      case Table8C:
        stage = Loc64;
        case40_0059a980(ptr(self + 0x8C), nullptr);
        break;
      case Loc64:
        stage = String5C;
        exo_locstring_destroy_005ea000(ptr(self + 0x64), nullptr);
        break;
      case String5C:
        stage = Loc4C;
        frame_LogStringDtor_005e5c20(ptr(self + 0x5C), nullptr);
        break;
      case Loc4C:
        stage = List48;
        exo_locstring_destroy_005ea000(ptr(self + 0x4C), nullptr);
        break;
      case List48:
        stage = List2C;
        destroy_list_field(0x48);
        break;
      case List2C:
        stage = List28;
        destroy_list_field(0x2C);
        break;
      case List28:
        stage = BaseGameObject;
        destroy_list_field(0x28);
        break;
      case BaseGameObject:
        stage = BaseHelper;
        m.write(self + 0x1C,
                reinterpret_cast<Word>(kotor_vtable_cgameobject_007457a0));
        break;
      case BaseHelper:
        stage = Done;
        case40_004c4c60(ptr(self), nullptr);
        break;
      case Done:
        break;
    }
  }
};

}  // namespace

// 004C68A0 CSWSModule::~CSWSModule. thiscall/no args/RET0.
extern "C" void __fastcall case40_004c68a0(void* self, void*) {
  NativeMemory m;
  const Word s = word(self);

  m.write(s, reinterpret_cast<Word>(module_primary_vtable));
  m.write(s + 0x1C, reinterpret_cast<Word>(module_secondary_vtable));

  const bool release = m.read(s + 0x1D8) == 0;
  Case40ModuleCleanup cleanup(m, s);
  if (release) {
    c2da_release_demand_00409b80(ptr(m.read(s + 8)), nullptr);
  }

  // [this+0x48]: node +0x10 locstring, +8 locstring, +0 CExoString, free.
  {
    Word list = m.read(s + 0x48);
    while (m.read(list) != 0) {
      Word e = word(base_ini_reconstruction::remove_head_005e9c80(
          reinterpret_cast<kotor_list::Internal*>(list), nullptr));
      if (e) {
        cleanup.element = e;
        cleanup.stage = Case40ModuleCleanup::NodeLoc8;
        exo_locstring_destroy_005ea000(ptr(e + 0x10), nullptr);
        cleanup.step();  // element +8
        cleanup.step();  // element +0
        live_free(ptr(e));
      }
    }
  }

  const Word server = m.read(m.read(0x7A39FC) + 8);

  // Area from [this+0x40]: ClearObjects then actual slot0(1).
  {
    void* area = kotor_fastcall_004ae780(
        ptr(server), nullptr, m.read(s + 0x40));
    if (area) {
      case40_00508710(area, nullptr);
      using AreaDtor = void* (__thiscall*)(void*, Word);
      AreaDtor dtor =
          reinterpret_cast<AreaDtor>(m.read(m.read(word(area))));
      (void)dtor(area, 1);
    }
  }

  // [this+0x2C]: remove-head/free.
  {
    Word list = m.read(s + 0x2C);
    while (m.read(list) != 0) {
      Word e = word(base_ini_reconstruction::remove_head_005e9c80(
          reinterpret_cast<kotor_list::Internal*>(list), nullptr));
      live_free(ptr(e));
    }
  }

  // [this+0x28]: remove-head, destroy locstring at +0, free.
  {
    Word list = m.read(s + 0x28);
    while (m.read(list) != 0) {
      Word e = word(base_ini_reconstruction::remove_head_005e9c80(
          reinterpret_cast<kotor_list::Internal*>(list), nullptr));
      if (e) {
        exo_locstring_destroy_005ea000(ptr(e), nullptr);
        live_free(ptr(e));
      }
    }
  }

  // [this+0x5C] resources and empty re-assign.
  case40_004c44d0(
      self, nullptr,
      reinterpret_cast<kotor_reconstruction::StartupString*>(ptr(s + 0x5C)));
  GuiAssign005e5140(ptr(s + 0x5C), nullptr, "");

  // [this+0x58]/[this+0x54] freed, not cleared (original).
  {
    Word p = m.read(s + 0x58);
    if (p) live_free(ptr(p));
    p = m.read(s + 0x54);
    if (p) live_free(ptr(p));
  }

  // [this+0x218] area map.
  {
    Word p = m.read(s + 0x218);
    if (p) {
      case40_00578c30(ptr(p), nullptr);
      live_free(ptr(p));
      m.write(s + 0x218, 0);
    }
  }

  // [this+0x20] object ID erased from the server object array.
  {
    const Word array =
        recovered_object_array_004aed70(ptr(server), nullptr);
    recovered_object_array_erase_004d8290(
        ptr(array), nullptr, m.read(s + 0x20));
  }

  case40_004aeb90(ptr(server), nullptr);
  m.write(s + 0x220, 0);
  m.write(s + 0x21C, 0x7F000000u);

  cleanup.stage = Case40ModuleCleanup::String204;
  cleanup.step();  // +0x204
  cleanup.step();  // +0x1F8
  cleanup.step();  // +0x1DC
  cleanup.step();  // +0x1CC array/header
  cleanup.step();  // +0x13C
  cleanup.step();  // +0x130
  cleanup.step();  // +0xB0 inline 15 strings
  cleanup.step();  // +0x8C
  cleanup.step();  // +0x64
  cleanup.step();  // +0x5C
  cleanup.step();  // +0x4C
  cleanup.step();  // +0x48 list
  cleanup.step();  // +0x2C list
  cleanup.step();  // +0x28 list
  cleanup.step();  // base table store
  cleanup.step();  // 004C4C60, stage Done before call
}

// 004C8940 deleting destructor. ECX=this, one stack flags, RET4, returns this.
extern "C" void* __fastcall case40_004c8940(
    void* self, void*, Word flags) {
  case40_004c68a0(self, nullptr);
  if (flags & 1) live_free(self);
  return self;
}

// 004C6BC0 adjustor. ECX=embedded object, one stack flags, end-to-end RET4.
// Outer flags are cleaned by this wrapper's RET4; the inner call has its own
// copy cleaned by 004C8940's RET4. Return the adjusted object pointer.
extern "C" void* __fastcall case40_004c6bc0(
    void* embedded, void*, Word flags) {
  return case40_004c8940(
      static_cast<unsigned char*>(embedded) - 0x1C, nullptr, flags);
}
