// W2 G07 native exports (thiscall + callee cleanup exported as __fastcall with unused EDX).
#include "6d05e68233-w2_entry_native.hpp"
using namespace w2::native;
namespace e = w2::entry;
extern "C" {
// 004B3EC0 CServerExoAppInternal::UpdateClientGameObjectsForPlayer(player, force, time lo, time hi) RET 10h
void __fastcall recovered_update_client_objects_004b3ec0(void* internal, void*, void* player, unsigned force, unsigned time_lo, unsigned time_hi) {
    Mem m; EntryPorts p(m); e::update_client_objects_004b3ec0(m, p, word(internal), word(player), force, time_lo, time_hi);
}
// 00578520 SendServerToPlayerGameObjUpdate(player) RET 4
unsigned __fastcall recovered_game_object_update_00578520(void* self, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x40] = {}; return e::game_object_update_00578520(m, p, word(self), word(player), word(s));
}
// 00578200 SendServerToPlayerGameObjUpdate(player, id) RET 8
unsigned __fastcall recovered_game_object_update_00578200(void* self, void*, void* player, unsigned id) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x40] = {}; return e::game_object_update_00578200(m, p, word(self), word(player), id, word(s));
}
// 0056F430 SendServerToPlayerGameObjUpdate_ObjControl(recipient, id) RET 8
unsigned __fastcall recovered_object_control_0056f430(void* self, void*, unsigned recipient, unsigned id) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return e::object_control_0056f430(m, p, word(self), recipient, id, word(s));
}
// 00578060 ComputeGameObjectUpdateForObject RET 10h
void __fastcall recovered_compute_for_object_00578060(void* self, void*, void* player, void* observer, void* array, unsigned id) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x20] = {}; e::compute_for_object_00578060(m, p, word(self), word(player), word(observer), word(array), id, word(s));
}
// 00578130 ComputeGameObjectUpdateForCategory RET 1Ch
unsigned __fastcall recovered_compute_for_category_00578130(void* self, void*, unsigned category, unsigned limit, void* player, void* observer, void* array, void* list, unsigned count) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x20] = {};
    return e::compute_for_category_00578130(m, p, word(self), category, limit, word(player), word(observer), word(array), word(list), count, word(s));
}
// 00567BA0 WriteGameObjUpdate_WorkRemaining RET 10h
void __fastcall recovered_work_remaining_00567ba0(void* self, void*, void* object, void* area, unsigned stage, unsigned total) {
    Mem m; EntryPorts p(m); e::work_remaining_00567ba0(m, p, word(self), word(object), word(area), stage, total);
}
// 00572990 WriteGameObjUpdate_MinorGUIPanels RET 4
void __fastcall recovered_minor_gui_panels_00572990(void* self, void*, void* player) { Mem m; EntryPorts p(m); e::minor_gui_panels_00572990(m, p, word(self), word(player)); }
// 0056DF60 DeleteLastUpdateObjectsInOtherAreas RET 4
void __fastcall recovered_delete_other_areas_0056df60(void* self, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x10] = {}; e::delete_other_areas_0056df60(m, p, word(self), word(player), word(s));
}
// 00567850 SortObjectsForGameObjectUpdate RET 10h
void* __fastcall recovered_sort_objects_00567850(void* self, void*, void* player, void* observer, void* array, unsigned* count) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x20] = {};
    return ptr(e::sort_objects_00567850(m, p, word(self), word(player), word(observer), word(array), word(count), word(s)));
}
// 00567780 SelectCategoryForGameObject RET 8
unsigned __fastcall recovered_select_category_00567780(void*, void*, void* object, void* observer) { Mem m; EntryPorts p(m); return e::select_category_00567780(m, p, word(object), word(observer)); }
// 005071B0 / 005071E0 CSWSArea::GetFirst/NextObjectInArea RET 4
unsigned __fastcall recovered_area_first_object_005071b0(void* area, void*, unsigned* out) { Mem m; return e::area_first_object_005071b0(m, word(area), word(out)); }
unsigned __fastcall recovered_area_next_object_005071e0(void* area, void*, unsigned* out) { Mem m; return e::area_next_object_005071e0(m, word(area), word(out)); }
// 00561820 CSWSPlayer::GetLastUpdateObject RET 4
void* __fastcall recovered_last_update_object_00561820(void* player, void*, unsigned id) {
    Mem m; EntryPorts p(m); Word slot = 0; return ptr(e::last_update_object_00561820(m, p, word(player), id, word(&slot)));
}
}
