// Per-type action bodies (W5). Generated from lane-w5/source/w5_action_bodies.hpp.
// With W1_LINK_W5_ACTIONS defined the runner calls W5's exports; otherwise each body traps with its address.
extern "C" unsigned __fastcall w5_action_dialog_object_0057a470(void*, void*, void*);
extern "C" unsigned __fastcall w5_action_do_command_0057b530(void*, void*, void*);   // claude_w5_do_command.cpp
#if defined(W1_LINK_W5_ACTIONS)
extern "C" {
unsigned __fastcall w5_action_move_to_point_0051f4f0(void*, void*, void*);
unsigned __fastcall w5_action_check_move_to_object_005101a0(void*, void*, void*);
unsigned __fastcall w5_action_check_move_away_from_object_005155b0(void*, void*, void*);
unsigned __fastcall w5_action_check_inter_area_pathfinding_0050ff20(void*, void*, void*);
unsigned __fastcall w5_action_jump_to_point_0051d600(void*, void*, void*);
unsigned __fastcall w5_action_play_animation_0057d080(void*, void*, void*);
unsigned __fastcall w5_action_pick_up_item_00517410(void*, void*, void*);
unsigned __fastcall w5_action_equip_item_00510fd0(void*, void*, void*);
unsigned __fastcall w5_action_drop_item_00513830(void*, void*, void*);
unsigned __fastcall w5_action_check_move_to_point_00510670(void*, void*, void*);
unsigned __fastcall w5_action_unequip_item_00513ec0(void*, void*, void*);
unsigned __fastcall w5_action_physical_attacks_005bbbf0(void*, void*, void*);
unsigned __fastcall w5_action_speak_0057b430(void*, void*, void*);
unsigned __fastcall w5_action_cast_spell_00514af0(void*, void*, void*);
unsigned __fastcall w5_action_wait_for_end_of_round_00513f60(void*, void*, void*);
unsigned __fastcall w5_action_check_move_to_object_radius_005103b0(void*, void*, void*);
unsigned __fastcall w5_action_check_move_to_point_radius_005108a0(void*, void*, void*);
unsigned __fastcall w5_action_change_facing_object_0050fb60(void*, void*, void*);
unsigned __fastcall w5_action_open_door_0057d490(void*, void*, void*);
unsigned __fastcall w5_action_close_door_0057bbf0(void*, void*, void*);
unsigned __fastcall w5_action_orient_camera_005137c0(void*, void*, void*);
unsigned __fastcall w5_action_play_sound_0057cf00(void*, void*, void*);
unsigned __fastcall w5_action_dialog_object_0057a470(void*, void*, void*);
unsigned __fastcall w5_action_disarm_mine_00519570(void*, void*, void*);
unsigned __fastcall w5_action_recover_mine_00518c40(void*, void*, void*);
unsigned __fastcall w5_action_flag_mine_0050e400(void*, void*, void*);
unsigned __fastcall w5_action_examine_mine_0050e900(void*, void*, void*);
unsigned __fastcall w5_action_set_mine_00519e30(void*, void*, void*);
unsigned __fastcall w5_action_wait_0057b5b0(void*, void*, void*);
unsigned __fastcall w5_action_pause_dialog_0057b290(void*, void*, void*);
unsigned __fastcall w5_action_resume_dialog_0057b320(void*, void*, void*);
unsigned __fastcall w5_action_speak_str_ref_0057b3d0(void*, void*, void*);
unsigned __fastcall w5_action_give_item_0057b6a0(void*, void*, void*);
unsigned __fastcall w5_action_take_item_0057bae0(void*, void*, void*);
unsigned __fastcall w5_action_encounter_creature_destroy_self_00510c20(void*, void*, void*);
unsigned __fastcall w5_action_do_command_0057b530(void*, void*, void*);
unsigned __fastcall w5_action_unlock_object_0057d9d0(void*, void*, void*);
unsigned __fastcall w5_action_lock_object_0057bec0(void*, void*, void*);
unsigned __fastcall w5_action_use_object_0057e8c0(void*, void*, void*);
unsigned __fastcall w5_action_rest_005140e0(void*, void*, void*);
unsigned __fastcall w5_action_check_move_away_from_location_0050fd50(void*, void*, void*);
unsigned __fastcall w5_action_random_walk_00515ac0(void*, void*, void*);
unsigned __fastcall w5_action_item_cast_spell_0050f170(void*, void*, void*);
unsigned __fastcall w5_action_set_commandable_0057c7b0(void*, void*, void*);
unsigned __fastcall w5_action_jump_to_object_0051d110(void*, void*, void*);
unsigned __fastcall w5_action_change_facing_point_0050fc90(void*, void*, void*);
unsigned __fastcall w5_action_counter_spell_00514270(void*, void*, void*);
unsigned __fastcall w5_action_drive_0051e6a0(void*, void*, void*);
unsigned __fastcall w5_action_appear_005140f0(void*, void*, void*);
unsigned __fastcall w5_action_disappear_005141b0(void*, void*, void*);
unsigned __fastcall w5_action_force_follow_object_005132e0(void*, void*, void*);
unsigned __fastcall w5_action_heal_00517a60(void*, void*, void*);
unsigned __fastcall w5_action_check_force_follow_object_005136b0(void*, void*, void*);
unsigned __fastcall w5_action_area_wait_00511080(void*, void*, void*);
unsigned __fastcall w5_action_party_follow_leader_00511130(void*, void*, void*);
unsigned __fastcall w5_action_bark_string_0057ce00(void*, void*, void*);
unsigned __fastcall w5_action_combat_005b6210(void*, void*, void*);
unsigned __fastcall w5_action_check_move_to_follow_radius_00510ab0(void*, void*, void*);
unsigned __fastcall w5_action_surrender_to_enemies_0051b420(void*, void*, void*);
}
#define W1_ACTION_BODY(fn, label) return fn(ptr(s), nullptr, ptr(a[0]))
#else
#define W1_ACTION_BODY(fn, label) trap(t, label " (W5 action body; build with W1_LINK_W5_ACTIONS)")
#endif
extern "C" unsigned __fastcall w5_action_appear_005140f0(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_disappear_005141b0(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_drop_item_00513830(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_encounter_creature_destroy_self_00510c20(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_equip_item_00510fd0(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_heal_00517a60(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_pick_up_item_00517410(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_rest_005140e0(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_surrender_to_enemies_0051b420(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_unequip_item_00513ec0(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_wait_for_end_of_round_00513f60(void*, void*, void*);   // claude_w5_creature_actions.cpp
extern "C" unsigned __fastcall w5_action_area_wait_00511080(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_change_facing_object_0050fb60(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_change_facing_point_0050fc90(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_force_follow_object_005136b0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_inter_area_pathfinding_0050ff20(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_away_from_location_0050fd50(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_away_from_object_005155b0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_to_follow_radius_00510ab0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_to_object_005101a0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_to_object_radius_005103b0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_to_point_00510670(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_check_move_to_point_radius_005108a0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_force_follow_object_005132e0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_jump_to_object_0051d110(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_jump_to_point_0051d600(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_orient_camera_005137c0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_random_walk_00515ac0(void*, void*, void*);   // claude_w5_movement_actions.cpp
extern "C" unsigned __fastcall w5_action_bark_string_0057ce00(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_close_door_0057bbf0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_give_item_0057b6a0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_lock_object_0057bec0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_pause_dialog_0057b290(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_play_animation_0057d080(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_play_sound_0057cf00(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_resume_dialog_0057b320(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_set_commandable_0057c7b0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_speak_0057b430(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_speak_str_ref_0057b3d0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_take_item_0057bae0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_unlock_object_0057d9d0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_use_object_0057e8c0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_wait_0057b5b0(void*, void*, void*);   // claude_w5_object_actions.cpp
extern "C" unsigned __fastcall w5_action_combat_005b6210(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_counter_spell_00514270(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_disarm_mine_00519570(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_examine_mine_0050e900(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_flag_mine_0050e400(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_recover_mine_00518c40(void*, void*, void*);   // claude_w5_combat_actions.cpp
extern "C" unsigned __fastcall w5_action_move_to_point_0051f4f0(void*, void*, void*);   // claude_w5_pathfind_walk.cpp
extern "C" unsigned __fastcall w5_action_physical_attacks_005bbbf0(void*, void*, void*);   // claude_combat_attack.cpp
extern "C" unsigned __fastcall w5_action_cast_spell_00514af0(void*, void*, void*);   // claude_combat_spell.cpp
extern "C" unsigned __fastcall w5_action_item_cast_spell_0050f170(void*, void*, void*);   // claude_combat_spell.cpp
extern "C" unsigned __fastcall w5_action_set_mine_00519e30(void*, void*, void*);   // claude_combat_spell_mine.cpp
extern "C" unsigned __fastcall w5_action_open_door_0057d490(void*, void*, void*);   // existing linked body
extern "C" unsigned __fastcall w5_action_drive_0051e6a0(void*, void*, void*);   // existing linked body
extern "C" unsigned __fastcall w5_action_party_follow_leader_00511130(void*, void*, void*);   // existing linked body
