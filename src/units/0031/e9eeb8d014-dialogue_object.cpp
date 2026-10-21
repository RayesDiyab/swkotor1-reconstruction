// Native closure for original 0057A470. No original-address function pointers.
#include "032bd84a83-w5_t3.hpp"
#include "c49290b0be-w5_server_env.hpp"
#include "8416480882-w5_action_bodies.hpp"
#include "c711e98a87-startup_string.h"
#include "c7df44e058-menu_continuation.h"
#include <initializer_list>
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }
extern "C" {
unsigned __fastcall gui_load_resref_is_valid_00406030(void*,void*);
unsigned __fastcall kotor_fastcall_004aea30(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_004aea40(void*,void*);
unsigned __fastcall recovered_compute_safe_location_004aeb60(void*,void*,void*,float,unsigned,unsigned,void*);
void* __fastcall recovered_object_array_004aed70(void*,void*);
void* __fastcall kotor_fastcall_004b2820(void*,void*);
void* __fastcall recovered_object_render_004cc2b0(void*,void*);
void __fastcall w5_object_clear_all_actions_004ccd80(void*,void*,unsigned);
void __fastcall w1x_object_set_orientation_004cd170(void*,void*,void*);
void __fastcall recovered_effect_apply_004d14f0(void*,void*,void*,unsigned,unsigned);
void __fastcall recovered_event_string_set_004d6ae0(void*,void*,unsigned,void*);
void __fastcall recovered_event_integer_004d76b0(void*,void*,unsigned,unsigned);
void* __fastcall recovered_creature_client_creature_004efb20(void*,void*);
void __fastcall recovered_set_activity_004f00a0(void*,void*,unsigned,unsigned,unsigned);
void* __fastcall recovered_effect_construct_00503e40(void*,void*,unsigned);
void __fastcall kotor_fastcall_005df540(void*,void*,int);
void* __fastcall kotor_fastcall_005ed580(void*,void*,unsigned);
unsigned __fastcall frame_R4_005ed690(void*,void*);
unsigned __fastcall kotor_m15_client_get_area_not_ready_005edad0(void*,void*);
unsigned __fastcall kotor_fastcall_005edc10(void*,void*);
void __fastcall kotor_fastcall_005edc30(void*,void*,unsigned,unsigned);
unsigned __fastcall kotor_fastcall_005edef0(void*,void*);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*,void*);
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void*,void*,int,float,float,void*);
void __fastcall kotor_fastcall_0062ec60(void*,void*,unsigned);
void* __fastcall frame_GetCharacter_006346c0(void*,void*,unsigned);
void* __fastcall kotor_fastcall_00634cc0(void*,void*,int);
unsigned __fastcall gui_load_slot_zero_0063e7f0(void*,void*);
// 0063D470 packaging import is finalized only against presentation owner's reviewed tier.
void __fastcall kotor_fastcall_0063d470(void*,void*);
}
namespace dialog_object_native {
using w5::U; using w5::I; using w5::Vector; using w5::bits;
using String=kotor_reconstruction::StartupString;
static_assert(sizeof(void*)==4,"Native x86 only");
U addr(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
void* ptr(U p){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(p));}
U rd(U p,U o){U x;std::memcpy(&x,ptr(p+o),4);return x;}
U r16(U p,U o){std::uint16_t x;std::memcpy(&x,ptr(p+o),2);return x;}
U r8(U p,U o){return *static_cast<unsigned char*>(ptr(p+o));}
void wr(U p,U o,U x){std::memcpy(ptr(p+o),&x,4);}
void w16(U p,U o,U x){const auto y=static_cast<std::uint16_t>(x);std::memcpy(ptr(p+o),&y,2);}
Vector read_vector(U p){Vector x;std::memcpy(&x,ptr(p),12);return x;}
float read_float(U p){return w5::flt(rd(p,0));}
U client(){return addr(w5::client_app());}
U server(){return addr(w5::server_app());}
U input(){return addr(kotor_menu_window_cells::input_007a39e4);}
U allocate(U size){return addr(live_operator_new(size));}
void release(U p){live_free(ptr(p));}
// Exact z*z + y*y + x*x sequence at 0057A77A/0057ABF1, wide 53-bit intermediates.
double squared_zyx(const Vector& p){return (double(p.z)*p.z+double(p.y)*p.y)+double(p.x)*p.x;}
float degree_radians(float d){return static_cast<float>(deg_to_rad_004a96d0(d));} // original0057AC9D fstp dword
// Read-only original constant values; no duplicate game global storage. W5 trap TU is not linked.
U literal(U a){switch(a){
case 0x746644:return w5::k_object_invalid;
case 0x74666c:return 0;
case 0x74668c:return 4;
case 0x7473b8:return 0;
case 0x7441dc:return w5::k_event_queue_type_007441dc;
case 0x745dbc:return 1;
case 0x745dc0:return 2;
case 0x745dc4:return 3;
case 0x747450:return 0x41200000;
case 0x7455bc:return 0x44610000;
// closeC: dialog_object_core.inc passes only the eleven literal addresses above (audited); 0073D764 is the default arm.
default:return 0x3f800000;}}
// Original x87 predicate boundaries: unordered skips both; equality differs.
bool move_distance_exceeds(const Vector& p){const double t=w5::flt(literal(0x747450));return squared_zyx(p)>t*t;}
bool party_distance_at_least(const Vector& p){return squared_zyx(p)>=w5::flt(literal(0x7455bc));}
U v(U self,U slot,std::initializer_list<U> values={}){
    const U* a=values.begin();
    // Original virtual getter returns EAX; setter return values are deliberately ignored.
    switch(slot){
    case 0x0c:case 0x10:case 0x2c:case 0x30:case 0x94:
        return w5::vcall<U>(ptr(self),slot);
    case 0x80:return addr(w5::vcall<void*>(ptr(self),slot,ptr(a[0])));
    case 0x88:case 0x8c:w5::vcall<void>(ptr(self),slot,ptr(a[0]));return 0;
    // closeC: the core passes only slots 0C/10/2C/30/80/88/8C/94/C0 (audited); slot C0 is the default arm.
    default:w5::vcall<void>(ptr(self),slot,a[0]);return 0;
    }
}
U c(U target,U self,std::initializer_list<U> values={}){
    const U* a=values.begin();void* s=ptr(self);
    switch(target){
    case 0x406030:return gui_load_resref_is_valid_00406030(s,nullptr);
    case 0x423b90:return addr(vector_sum_00423b90(static_cast<Vector*>(ptr(a[0])),static_cast<Vector*>(ptr(a[1])),static_cast<Vector*>(ptr(a[2]))));
    case 0x423bc0:return addr(kotor_cdecl_00423bc0(static_cast<Vector*>(ptr(a[0])),static_cast<Vector*>(ptr(a[1])),static_cast<Vector*>(ptr(a[2]))));
    case 0x423bf0:return addr(vector_component_mult_00423bf0(static_cast<Vector*>(ptr(a[0])),static_cast<Vector*>(ptr(a[1])),w5::flt(a[2])));
    case 0x4a9bb0:return addr(quaternion_rotate_004a9bb0(static_cast<Vector*>(ptr(a[0])),ptr(a[1]),static_cast<Vector*>(ptr(a[2]))));
    case 0x4ac960:return addr(quaternion_construct_004ac960(static_cast<w5::Quaternion*>(s),nullptr,Vector{w5::flt(a[0]),w5::flt(a[1]),w5::flt(a[2])},w5::flt(a[3])));
    case 0x4ae750:return addr(kotor_fastcall_004ae750(s,nullptr,a[0]));
    case 0x4ae770:return addr(kotor_fastcall_004ae770(s,nullptr,a[0]));
    case 0x4ae910:return addr(kotor_fastcall_004ae910(s,nullptr,a[0]));
    case 0x4aea30:return kotor_fastcall_004aea30(s,nullptr,a[0]);
    case 0x4aea40:return kotor_fastcall_004aea40(s,nullptr);
    case 0x4aeb60:return recovered_compute_safe_location_004aeb60(s,nullptr,ptr(a[0]),w5::flt(a[1]),a[2],a[3],ptr(a[4]));
    case 0x4aed70:return addr(recovered_object_array_004aed70(s,nullptr));
    case 0x4aed80:return addr(w5::ai_master()); // reuse canonical W5 adapter; caller passes actual server
    case 0x4b08d0:return recovered_event_enqueue_004b08d0(s,nullptr,a[0],a[1],a[2],a[3],a[4],ptr(a[5]));
    case 0x4b2820:return addr(kotor_fastcall_004b2820(s,nullptr));
    case 0x4cc2b0:return addr(recovered_object_render_004cc2b0(s,nullptr));
    case 0x4ccb00:return recovered_object_add_action_to_front_004ccb00(s,nullptr,a[0],a[1],a[2],ptr(a[3]),a[4],ptr(a[5]),a[6],ptr(a[7]),a[8],ptr(a[9]),a[10],ptr(a[11]),a[12],ptr(a[13]),a[14],ptr(a[15]),a[16],ptr(a[17]),a[18],ptr(a[19]),a[20],ptr(a[21]),a[22],ptr(a[23]),a[24],ptr(a[25]),a[26],ptr(a[27]));
    case 0x4ccd80:w5_object_clear_all_actions_004ccd80(s,nullptr,a[0]);return 0;
    case 0x4cd170:w1x_object_set_orientation_004cd170(s,nullptr,ptr(a[0]));return 0;
    case 0x4cd1a0:recovered_object_position_004cd1a0(s,nullptr,static_cast<Vector*>(ptr(a[0])),a[1],a[2],a[3]);return 0;
    case 0x4d14f0:recovered_effect_apply_004d14f0(s,nullptr,ptr(a[0]),a[1],a[2]);return 0;
    case 0x4d6ae0:recovered_event_string_set_004d6ae0(s,nullptr,a[0],ptr(a[1]));return 0;
    case 0x4d7540:return addr(recovered_event_construct_004d7540(s,nullptr));
    case 0x4d76b0:recovered_event_integer_004d76b0(s,nullptr,a[0],a[1]);return 0;
    case 0x4d8230:return kotor_fastcall_004d8230(s,nullptr,a[0],ptr(a[1]));
    case 0x4ee440:return w5_creature_get_use_range_004ee440(s,nullptr,a[0],static_cast<Vector*>(ptr(a[1])),static_cast<float*>(ptr(a[2])),a[3]);
    case 0x4ef890:return recovered_creature_incapacitated_004ef890(s,nullptr);
    case 0x4efb20:return addr(recovered_creature_client_creature_004efb20(s,nullptr));
    case 0x4f00a0:recovered_set_activity_004f00a0(s,nullptr,a[0],a[1],a[2]);return 0;
    case 0x4f6000:return w5_creature_get_is_in_use_range_004f6000(s,nullptr,a[0],w5::flt(a[1]),a[2]);
    case 0x4f87d0:w5_creature_activity_manager_004f87d0(s,nullptr,a[0]);return 0;
    case 0x4f8a50:return w5_creature_add_move_to_point_action_to_front_004f8a50(s,nullptr,a[0],static_cast<Vector*>(ptr(a[1])),a[2],a[3],a[4],w5::flt(a[5]),w5::flt(a[6]),a[7],a[8],a[9],a[10],a[11],a[12],static_cast<Vector*>(ptr(a[13])),a[14],a[15],a[16]);
    case 0x503e40:return addr(recovered_effect_construct_00503e40(s,nullptr,a[0]));
    case 0x5df540:kotor_fastcall_005df540(s,nullptr,static_cast<int>(a[0]));return 0;
    case 0x5ed580:return addr(kotor_fastcall_005ed580(s,nullptr,a[0]));
    case 0x5ed5b0:return frame_R4_005ed5b0(s,nullptr,a[0]);
    case 0x5ed690:return frame_R4_005ed690(s,nullptr);
    case 0x5ed8b0:return addr(kotor_fastcall_005ed8b0(s,nullptr));
    case 0x5eda50:return kotor_fastcall_005eda50(s,nullptr,a[0]);
    case 0x5edad0:return kotor_m15_client_get_area_not_ready_005edad0(s,nullptr);
    case 0x5edc10:return kotor_fastcall_005edc10(s,nullptr);
    case 0x5edc30:kotor_fastcall_005edc30(s,nullptr,a[0],a[1]);return 0;
    case 0x5edef0:return kotor_fastcall_005edef0(s,nullptr);
    case 0x60fb20:return addr(kotor_m15a_creature_get_server_creature_0060fb20(s,nullptr));
    case 0x62abf0:kotor_m15_ingame_start_global_fade_0062abf0(s,nullptr,static_cast<int>(a[0]),w5::flt(a[1]),w5::flt(a[2]),ptr(a[3]));return 0;
    case 0x62ac60:return static_cast<U>(static_cast<menu_continuation::InGame*>(s)->IsGlobalFading_0062ac60());
    case 0x62ec60:kotor_fastcall_0062ec60(s,nullptr,a[0]);return 0;
    case 0x6346c0:return addr(frame_GetCharacter_006346c0(s,nullptr,a[0]));
    case 0x634cc0:return addr(kotor_fastcall_00634cc0(s,nullptr,static_cast<int>(a[0])));
    case 0x63d470:kotor_fastcall_0063d470(s,nullptr);return 0;
    // closeC: every c/cc/sc target in the core is one of the cases above (audited); 0063E7F0 is the default arm.
    default:return gui_load_slot_zero_0063e7f0(s,nullptr);
    }
}
// Preserve original reload-after-inner-call ordering of manager/client/server globals.
U cc(U target,std::initializer_list<U> args){return c(target,client(),args);}
U sc(U target,std::initializer_list<U> args){return c(target,server(),args);}
#include "684ff21579-dialog_object_core.inc"
}
extern "C" unsigned __fastcall w5_action_dialog_object_0057a470(void* self,void*,void* node){
    return dialog_object_native::dialog_object(dialog_object_native::addr(self),dialog_object_native::addr(node));
}
