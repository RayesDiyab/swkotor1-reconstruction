#pragma once
#include <cstdint>
namespace kotor::server_mainloop {
using Word=std::uint32_t;using Wide=std::uint64_t;
struct Memory {virtual ~Memory()=default;virtual Word read(Word)=0;virtual void write(Word,Word)=0;virtual Word read16(Word)=0;virtual void write16(Word,Word)=0;virtual Word read8(Word)=0;};
// All providers are explicit. No success defaults or silent required callbacks.
struct Ports {virtual ~Ports()=default;
 virtual void update_004b1ee0(Word)=0;
 virtual void tick_004adbd0(Word)=0;
 virtual void ai_mode_005d57d0(Word,Word)=0;
 virtual void ai_update_005d5650(Word,Word)=0;
 virtual void party_update_004b6da0(Word)=0;
 virtual void party_positions_004b4e80(Word)=0;
 virtual void net_update_004b0b70(Word)=0;
 virtual void update_state_004b6950(Word,Word)=0;
 virtual Word gui_005ed690(Word)=0;
 virtual Word timer_004aede0(Word)=0;
 virtual void timer_fields_004ade40(Word,Word,Word)=0;
 virtual void timer_split_004add90(Word,Word,Word,Word,Word,Word)=0;
 virtual Word module_004b14f0(Word)=0;
 virtual void module_date_004c4100(Word,Word,Word,Word,Word,Word,Word)=0;
 virtual void set_date_004aebc0(Word,Word,Word,Word,Word,Word,Word,Word)=0;
 virtual void set_tick_004af0c0(Word,Word)=0;
 virtual void set_day_004af0a0(Word,Word)=0;
 virtual void party_flag_00565530(Word,Word)=0;
 virtual Word transition_004ba920(Word,Word)=0;
 virtual Word player_id_004aea40(Word)=0;
 virtual Word creature_004ae770(Word,Word)=0;
 virtual Word creature_area_004cb120(Word)=0;
 virtual Word client_busy_005edad0(Word)=0;
 virtual Word can_save_004b2520(Word)=0;
 virtual void empty_005b3190(Word)=0;
 virtual void literal_005e5140(Word,Word)=0;
 virtual Word save_004b58a0(Word,Word,Word,Word,Word)=0;
 virtual void destroy_005e5c20(Word)=0;
 virtual void fade_0062abf0(Word,Word,Word,Word,Word)=0;
 virtual void client_mode_005edc80(Word,Word)=0;
 virtual void input_005ed310(Word)=0;
 virtual void gui_mode_0062b410(Word,Word)=0;
 virtual Word client_internal_005ed530(Word)=0;
 virtual void reset_00604d70(Word)=0;
 virtual void save_done_004b3000(Word)=0;
 virtual void player_notice_0056c7f0(Word,Word)=0;
 virtual void unload_phase_004b3110(Word)=0;
 virtual void log_00401430(Word)=0;
 virtual void copy_string_005e5b00(Word,Word)=0;
 virtual Word load_mode_005edfb0(Word)=0;
 virtual Word copy_module_004b1680(Word,Word,Word,Word)=0;
 virtual Word allocate_006fa7e6(Word)=0;
 virtual Word construct_module_004c84a0(Word,Word,Word,Word)=0;
 virtual void assign_005e5c50(Word,Word)=0;
 virtual Word start_module_004c9050(Word,Word,Word)=0;
 virtual void error_004ba5b0(Word,Word)=0;
 virtual void party_reset_005639c0(Word)=0;
 virtual Word area_step_004c5720(Word,Word,Word)=0;
 virtual Word finish_module_004c5880(Word)=0;
 virtual Word message_004aedb0(Word)=0;
 virtual void status_0056c980(Word,Word,Word)=0;
 virtual void notify_00568310(Word,Word)=0;
 virtual void notify_00579920(Word,Word)=0;
 virtual Word list_at_005e9b60(Word,Word)=0;
 virtual void list_next_005e9b90(Word,Word)=0;
 virtual void player_ready_00576360(Word,Word)=0;
 virtual void free_006fa390(Word)=0;
 virtual Wide time_005e9260(Word)=0;
 virtual Word timeout_004b4ab0(Word,Wide)=0;
 virtual void shutdown_004b69c0(Word,Word,Word)=0;
 virtual void quit_00401280(Word)=0;
 virtual void tail_004b1e10(Word,Wide)=0;
};
// scratch is an addressable aligned 0x80-byte invocation-local region.
// Original ordinary control flow 004BABB0..004BB336 (1,926 bytes).
Word mainloop_004babb0(Memory&,Ports&,Word self,Word scratch);
}
