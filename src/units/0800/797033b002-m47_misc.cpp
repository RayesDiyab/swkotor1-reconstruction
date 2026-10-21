// lane-m47-3d misc: CSWGuiScene::~CSWGuiScene 004165E0 (consumers 3d-render-scene3d 004175A3, gui-load-gui-objects).
// Exact consumer declaration (stage 3d/render/scene3d.h:473, gui-load/gui_objects.h:269):
//   extern "C" void __fastcall gui_scene_destruct_004165e0(void* gui_scene, void*);   original __thiscall, plain RET (0041670C).
// Evidence: original swkotor.exe 004165E0..0041670C (local disassembly lane-m47-3d/d/004165E0.txt).
#include <cstdint>
#include <cstring>
#include "6c81d8874d-scene3d.h"   // canonical lane-3d-render header (+ vtables_gen.h / vtables2_gen.h typed tables)

namespace menu_resources { struct Manager; }
// Owner: manager-cell-owner (manager-owned/manager_cell.h:6), identical declaration.
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; }
// Owner: r4-lowers-r4-log-joins (frame-gui/source/mainloop.h:20,25-26), identical declarations / 8-byte CExoString view.
namespace frame_log { struct String {unsigned data,length;}; }
extern "C" {
void __fastcall frame_LogStringCtor_005e5a90(frame_log::String*,void*,const char*) noexcept(false);
void __fastcall frame_LogStringDtor_005e5c20(frame_log::String*,void*);
// Owner: startup46-resman-leaves (startup46/resman_leaves.cpp:14) CExoResMan::RemoveResourceImageFile, thiscall RET 4.
void __fastcall kotor_fastcall_00408830(void*, void*, void*);
// Owner: gui-load-gui-vtables (gui-load/gui_objects.h:235, inside its extern "C" block at global scope).
extern const struct GuiLoadDrawableMethods gui_load_drawable_methods_0073e324;
}

namespace {
using kotor_scene3d::U; using kotor_scene3d::U8; using kotor_scene3d::I;
using kotor_scene3d::field; using kotor_scene3d::PointerList;
// Live vptr reads through the published typed tables (D21/D41; same accessors as stage 3d/render/scene3d.cpp:88-92).
inline const kotor_scene3d::SceneVTable* scene_vt(void* s) { return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(s); }
inline const kotor_scene3d::CameraVTable* camera_vt(void* c) { return *static_cast<const kotor_scene3d::CameraVTable* const volatile*>(c); }
inline const aurora_model::GobVTable* gob_vt(void* g) { return *static_cast<const aurora_model::GobVTable* const volatile*>(g); }
inline U8* manager_bytes() {   // mov reg,[007A39E8h] re-read at every use (0041665B / 0041667C / 00416699 / 004166A2 / 004166BC / 004166DF)
    return reinterpret_cast<U8*>(kotor_resource_demand::resource_manager_007a39e8);
}
// Temporary CExoString of the RemoveResourceImageFile argument: EH states 2 / 3 (0041667E / 004166C7) destroy it on
// unwind, state 1 after the explicit ~CExoString (0041668B / 004166D5) -> RAII lease emits that cleanup (D30-FINAL).
struct StringLease {
    frame_log::String object;
    explicit StringLease(const char* text) { frame_LogStringCtor_005e5a90(&object, nullptr, text); }   // 00416672 / 004166B7
    ~StringLease() { frame_LogStringDtor_005e5c20(&object, nullptr); }                                 // 00416694 / 004166DA
    StringLease(const StringLease&) = delete; StringLease& operator=(const StringLease&) = delete;
};
void remove_image_file(const char* name) {
    StringLease name_string(name);
    kotor_fastcall_00408830(kotor_resource_demand::resource_manager_007a39e8, nullptr, &name_string.object);   // 00416686 / 004166CC (ECX re-read)
}
}

// [M45 compose] 004165E0 body removed: single body = lane-m47-core core47-gui-manager-leaves (GUI manager leaves owner); same extern C symbol (definition removed; declaration kept)
extern "C" void __fastcall gui_scene_destruct_004165e0(void* gui_scene, void*);
