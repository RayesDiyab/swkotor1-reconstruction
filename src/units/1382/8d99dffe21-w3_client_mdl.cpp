// lane-w3 unit w3-client-mdl: the model-node control entry used by the area day/night lighting
//   0044B900 AurControlPart(char const*)          ("<gob> <part> <field> <values>")
//   0044B140 MdlNode::ConJoinDispatch             (type-exact node clone factory dispatcher)
//   00445350 / 0044A4A0 / 0044A390 / 0044A2A0 / 0044A5E0 / 0044A850 / 0044AD20 / 0044AE60 / 0043FD70
//            the per-type "new node with this node's translated name" factories (unlabelled in the Ghidra export;
//            the label map places them inside the neighbouring functions)
// Every node constructor, FindGob, the field parser and the controller dispatch are REAL runtime1115 render-core
// owners and are only called here.
#include "dd06b37d22-w3_client.h"

using namespace kotor_w3;

extern "C" {
void* __cdecl find_gob_00440460(const char* name);                                       // 00440460 FindGob
const char* __cdecl skip_blanks_00463510(const char* text);                              // 00463510 advanceStringToWhitespace
void __fastcall mdl_node_parse_field_dispatch_0043fa10(void* node, void*, const char* text);   // 0043FA10 RET 4
void __fastcall aurora_node_control_dispatch_0043f890(void* node, void*, void* part, U a, U b, U c);   // 0043F890 RET 10h
void __fastcall aurora_node_destruct_0044a040(void* node, void*);                       // 0044A040 ~MdlNode
const char* __fastcall aurora_node_translate_name_00438a00(void* node, void*);          // 00438A00 TranslateNodeName
void* __fastcall mdl_node_construct_00445210(void* self, void*, const char* name);      // 00445210 MdlNode(char*) RET 4
void* __fastcall mdl_node_light_construct_0044a3f0(void* self, void*, const char* name);
void* __fastcall mdl_node_emitter_construct_0044a300(void* self, void*, const char* name);
void* __fastcall mdl_node_trimesh_construct_0044a640(void* self, void*, const char* name);
void* __fastcall mdl_node_skin_construct_0044ac70(void* self, void*, const char* name);
void* __fastcall mdl_node_anim_mesh_construct_0044abc0(void* self, void*, const char* name);
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);
// lane-w3 explicit trap (w3_traps.cpp): MdlNodeTrigger::MdlNodeTrigger is absent from runtime1115.
void* __fastcall recovered_mdl_node_trigger_construct_0049d7b0(void* self, void*, const char* name);
}

namespace {
const char kControlFormat_00741420[] = "%s%s%n";                     // .rdata 00741420
using Ctor = void* (__fastcall*)(void*, void*, const char*);
// Shared shape of the nine factories: operator new(size); if allocated, ctor(new, TranslateNodeName(this)); an
// optional type word store; EAX = constructed node (0 when the allocation failed).
void* node_factory(void* self, U size, Ctor ctor, int type_word) noexcept(false) {
    void* const memory = live_operator_new(size);                     // push size; call 006FA7E6
    if (memory == nullptr) return nullptr;                            // xor eax,eax; cmp esi,eax; je
    void* const node = ctor(memory, nullptr, aurora_node_translate_name_00438a00(self, nullptr));
    if (type_word >= 0) {                                             // camera / reference factories only
        st<U16>(memory, 0, static_cast<U16>(type_word));              // mov word ptr [esi], n
        return memory;                                                // mov eax, esi
    }
    return node;                                                      // EAX = constructor's return
}
}  // namespace

extern "C" {
void* __fastcall recovered_mdl_node_join_base_00445350(void* self, void*) noexcept(false) {          // type 0x0001
    return node_factory(self, 0x50, mdl_node_construct_00445210, -1);                               // 00445350..004453A3
}
void* __fastcall recovered_mdl_node_join_light_0044a4a0(void* self, void*) noexcept(false) {         // type 0x0003
    return node_factory(self, 0xAC, mdl_node_light_construct_0044a3f0, -1);                         // 0044A4A0..0044A4F6
}
void* __fastcall recovered_mdl_node_join_emitter_0044a390(void* self, void*) noexcept(false) {       // type 0x0005
    return node_factory(self, 0x130, mdl_node_emitter_construct_0044a300, -1);                      // 0044A390..0044A3E6
}
void* __fastcall recovered_mdl_node_join_camera_0044a2a0(void* self, void*) noexcept(false) {        // type 0x0009
    return node_factory(self, 0x50, mdl_node_construct_00445210, 9);                                // 0044A2A0..0044A2FA
}
void* __fastcall recovered_mdl_node_join_reference_0044a5e0(void* self, void*) noexcept(false) {     // type 0x0011
    return node_factory(self, 0x74, mdl_node_construct_00445210, 0x11);                             // 0044A5E0..0044A63A
}
void* __fastcall recovered_mdl_node_join_trimesh_0044a850(void* self, void*) noexcept(false) {       // 0x21/0x121/0x221/0x821
    return node_factory(self, 0x19C, mdl_node_trimesh_construct_0044a640, -1);                      // 0044A850..0044A8A6
}
void* __fastcall recovered_mdl_node_join_skin_0044ad20(void* self, void*) noexcept(false) {          // type 0x0061
    return node_factory(self, 0x200, mdl_node_skin_construct_0044ac70, -1);                         // 0044AD20..0044AD76
}
void* __fastcall recovered_mdl_node_join_anim_mesh_0044ae60(void* self, void*) noexcept(false) {     // type 0x00A1
    return node_factory(self, 0x1D4, mdl_node_anim_mesh_construct_0044abc0, -1);                    // 0044AE60..0044AEB6
}
void* __fastcall recovered_mdl_node_join_trigger_0043fd70(void* self, void*) noexcept(false) {       // type 0x0401
    return node_factory(self, 0x50, recovered_mdl_node_trigger_construct_0049d7b0, -1);             // 0043FD70..0043FDC3
}

// 0044B140 MdlNode::ConJoinDispatch: ECX node; dispatch on the signed 16-bit node-type word. Each original case masks
// ECX with (type & K) == K, which always holds for the exact case value, so the node itself is passed.
void* __fastcall recovered_mdl_node_con_join_dispatch_0044b140(void* node, void*) noexcept(false) {
    const I32 type = static_cast<I32>(ld<std::int16_t>(node, 0));     // 0044B140..0044B143 movsx
    switch (type) {
    case 0x001: return recovered_mdl_node_join_base_00445350(node, nullptr);       // 0044B169 (byte table 0044B29C)
    case 0x003: return recovered_mdl_node_join_light_0044a4a0(node, nullptr);      // 0044B16E
    case 0x005: return recovered_mdl_node_join_emitter_0044a390(node, nullptr);    // 0044B181
    case 0x009: return recovered_mdl_node_join_camera_0044a2a0(node, nullptr);     // 0044B194
    case 0x011: return recovered_mdl_node_join_reference_0044a5e0(node, nullptr);  // 0044B1A7
    case 0x021: return recovered_mdl_node_join_trimesh_0044a850(node, nullptr);    // 0044B1BA
    case 0x061: return recovered_mdl_node_join_skin_0044ad20(node, nullptr);       // 0044B1CD
    case 0x0A1: return recovered_mdl_node_join_anim_mesh_0044ae60(node, nullptr);  // 0044B210
    case 0x121: return recovered_mdl_node_join_trimesh_0044a850(node, nullptr);    // 0044B1FA
    case 0x221: return recovered_mdl_node_join_trimesh_0044a850(node, nullptr);    // 0044B223
    case 0x401: return recovered_mdl_node_join_trigger_0043fd70(node, nullptr);    // 0044B266
    case 0x821: return recovered_mdl_node_join_trimesh_0044a850(node, nullptr);    // 0044B24E
    default: return nullptr;                                                       // 0044B24B
    }
}

// 0044B900 AurControlPart(char const* command) cdecl -> AL. "<gob><part><n>" are scanned with "%s%s%n"; the part's
// node is cloned through ConJoinDispatch, the remainder of the command is parsed into the clone and applied to the
// part through MdlNode::ControlDispatch, and the clone is destroyed.
U8 __cdecl recovered_aur_control_part_0044b900(const char* command) noexcept(false) {
    char gob_name[0x40];                                              // {E-0x80}
    char part_name[0x40];                                             // {E-0x40}
    I32 consumed = 0;                                                 // {E-0x84} (written by %n)
    part_name[0] = 0;                                                 // 0044B924
    gob_name[0] = 0;                                                  // 0044B929
    if (kotor_sscanf_006fae08(command, kControlFormat_00741420, gob_name, part_name, &consumed) < 2) return 0;   // 0044B90F..0044B939
    if (gob_name[0] == 0) return 0;                                   // 0044B93F..0044B945
    if (part_name[0] == 0) return 0;                                  // 0044B94B..0044B951
    void* const gob = find_gob_00440460(gob_name);                    // 0044B953..0044B958
    if (gob == nullptr) return 0;                                     // 0044B960..0044B962
    void* const part = vslot<void* (__fastcall*)(void*, void*, const char*)>(gob, 0x10C)(gob, nullptr, part_name);   // 0044B964..0044B96D GetPart
    if (part == nullptr) return 0;                                    // 0044B975..0044B977
    void* const clone = recovered_mdl_node_con_join_dispatch_0044b140(rp(part, 4), nullptr);   // 0044B979..0044B97D [part+4] node
    mdl_node_parse_field_dispatch_0043fa10(clone, nullptr, skip_blanks_00463510(command + consumed));   // 0044B982..0044B996
    aurora_node_control_dispatch_0043f890(clone, nullptr, part, 0, 0, 0x3F800000u);   // 0044B99B..0044B9A7 (part, 0, 0, 1.0f)
    if (clone != nullptr) {                                           // 0044B9AC..0044B9AE
        aurora_node_destruct_0044a040(clone, nullptr);                // 0044B9B0..0044B9B2
        live_free(clone);                                             // 0044B9B7..0044B9B8
    }
    return 1;                                                         // 0044B9C2
}
}  // extern "C"
