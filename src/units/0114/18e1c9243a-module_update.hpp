#pragma once
#include "93d73ae698-w1_ports.hpp"
// G06 module/area update pump. Frames: frame+N == the original [esp+N] after the prologue named per function.
namespace w1::module_update {
// 004AE990 CServerExoApp::GetActivePauseState (-> 004B1DA0): 2 when bit 1 of [internal+0x10078], else bit 0. AL only.
U active_pause_state_004ae990(U server_app, Ports& p);
// 004ADD50 CWorldTimer::ConvertFromCalendarDay(day, year*, month*, day_of_month*), thiscall ret 0x10 (receiver unused).
void calendar_day_004add50(U day, U year_out, U month_out, U dom_out, Ports& p);
// 004ADD90 CWorldTimer::ConvertFromTimeOfDay(ms, hour*, minute*, second*, millisecond*), thiscall ret 0x14.
void time_of_day_004add90(U timer, U ms, U hour_out, U minute_out, U second_out, U msec_out, Ports& p);
// 004C6210 CSWSModule::UpdateTime(day, ms, elapsed), thiscall ret 0xC. frame = 0x38 bytes (after `push edi` 004C621A;
// +0x2C/+0x30/+0x34 are the argument slots, reused by the original as locals).
void update_time_004c6210(U module, U day, U ms, U elapsed, U frame, Ports& p);
// 004C6480 CSWSModule::TimeStopSanityCheck, thiscall ret. frame = 0x10 bytes (+0xC position).
void time_stop_check_004c6480(U module, U frame, Ports& p);
// 00508050 CSWSArea::SetCurrentWeather(BYTE weather, int keep), thiscall ret 8. frame = 8 bytes: +0 the weather argument
// slot (dword as pushed), +4 the keep argument slot (reused as the player-list position).
void set_weather_00508050(U area, U weather, U keep, U frame, Ports& p);
// 00508A30 CSWSArea::AIUpdate, thiscall ret; returns 1 when the heartbeat ran. frame = 0x1C bytes
// (+4 day, +8 ms, +0xC elapsed, +0x10 day delta, +0x14 SetCurrentWeather frame).
U area_ai_update_00508a30(U area, U frame, Ports& p);
// 004C6C90 CSWSModule::AIUpdate, thiscall ret. frame = 0x5C bytes (+0x14 UpdateTime frame, +0x4C TimeStopSanityCheck frame).
U module_ai_update_004c6c90(U module, U frame, Ports& p);
// 004C6DC0 CSWSModule::DoUpdate, thiscall ret: alternates module AIUpdate and the current area's AIUpdate. frame = 0x5C bytes.
U module_do_update_004c6dc0(U module, U frame, Ports& p);
// 004CE6E0 CSWSObject::ReportOverflow(WORD code, int value, int limit, int unused), thiscall ret 0x10.
// Debug-mode only feedback to every player creature. frame = 4 bytes (list position).
void report_overflow_004ce6e0(U object, U code, U value, U limit, U frame, Ports& p);
}
