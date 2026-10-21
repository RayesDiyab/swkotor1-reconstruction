#include "18e1c9243a-module_update.hpp"
#include "4a8ca74ca0-leaves.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
namespace w1::module_update {
namespace {
constexpr U kHeartbeatInterval = 0x1770;     // 6000 ms (immediate)
constexpr U kEffectTimeStop = 0x40;          // [0074677C] .rdata = 64
U world_timer(Ports& p) { return p.call(0x4aede0, server(p), {}); }
}
U active_pause_state_004ae990(U server_app, Ports& p) {
  U flags = p.read8(p.read(server_app + 4) + 0x10078);              // 004B1DA0
  if (flags & 2) return 2;
  return (flags & 1) ? 1 : 0;
}
void calendar_day_004add50(U day, U year_out, U month_out, U dom_out, Ports& p) {
  p.write(dom_out, day % 0x1c + 1);                                 // 004ADD50..004ADD62
  U months = day / 0x1c;
  p.write(month_out, months % 0xc + 1);
  U year = months / 0xc;
  p.write(year_out, year);
  if (year > 0x7fff) p.write(year_out, 0x53c);                      // 004ADD72..004ADD81
}
void time_of_day_004add90(U timer, U ms, U hour_out, U minute_out, U second_out, U msec_out, Ports& p) {
  p.write(msec_out, ms % 0x3e8);                                    // 004ADD90..004ADDD6
  U seconds = ms / 0x3e8;
  p.write(second_out, seconds % 0x3c);
  U minutes = seconds / 0x3c;
  U per_hour = p.read8(timer + 0x38);                               // minutes per hour; 0 faults (#DE) as in the original
  p.write(minute_out, minutes % per_hour);
  p.write(hour_out, minutes / per_hour);
}
void update_time_004c6210(U m, U day, U ms, U elapsed, U e, Ports& p) {
  p.write(e + 0x2c, day); p.write(e + 0x30, ms); p.write(e + 0x34, elapsed);
  (void)world_timer(p);                                             // 004C6234 (receiver unused by 004ADD50)
  calendar_day_004add50(day, e + 0x18, e + 0x14, e + 0x10, p);
  U timer = world_timer(p);                                         // 004C6262
  time_of_day_004add90(timer, ms, e + 0xc, e + 0x24, e + 0x20, e + 0x1c, p);
  U dawn = p.read8(m + 0x1a1), dusk = p.read8(m + 0x1a2), hour = p.read(e + 0xc);
  U phase;                                                          // 004C626E..004C62B2: 1 day, 2 night, 3 dawn, 4 dusk
  if (dawn == dusk) phase = 1;
  else if (hour == dawn) phase = 3;
  else if (hour == dusk) phase = 4;
  else if (dawn < dusk) phase = (hour < dawn || hour > dusk) ? 2 : 1;
  else phase = hour < dusk ? 1 : (hour <= dawn ? 2 : 1);
  U current = p.read8(m + 0x1bc);
  if (current == 3 || current == 4) p.write(m + 0x1b8, p.read(m + 0x1b8) + elapsed);   // 004C62B4..004C62DC
  else p.write(m + 0x1b8, 0);
  U changed = 0;                                                    // 004C62E2..004C634E: byte in the day slot
  if (current != phase) { changed = 1; p.write8(m + 0x1bc, phase); }
  if (p.read(m + 0x1b4) != hour) { changed |= 2; p.write(m + 0x1b4, hour); }
  if (p.read(m + 0x1b0) != p.read(e + 0x10)) { changed |= 4; p.write(m + 0x1b0, p.read(e + 0x10)); }
  if (p.read(m + 0x1ac) != p.read(e + 0x14)) { changed |= 8; p.write(m + 0x1ac, p.read(e + 0x14)); }
  if (p.read(m + 0x1a8) != p.read(e + 0x18)) { changed |= 0x10; p.write(m + 0x1a8, p.read(e + 0x18)); }
  p.write8(e + 0x2c, changed);
  if (!changed) return;
  U list = player_list(p);                                          // 004C635C..004C63D9
  U position = p.read(p.read(list));
  p.write(e + 0x30, position);
  if (!position) return;
  U flags = p.read(e + 0x2c);                                       // pushed as the whole dword slot (ebp)
  do {
    U player = p.call(0x5e9b60, p.read(list), {position});
    if (player)
      p.call(0x56a5f0, sws_message(p), {player, flags, p.read(e + 0x18), p.read(e + 0x14), p.read(e + 0x10), p.read(e + 0xc),
                                        p.read8(m + 0x1bc), p.read(m + 0x1b8)});   // SendServerToPlayerModuleUpdate_Time
    p.call(0x5e9b90, p.read(list), {e + 0x30});
    position = p.read(e + 0x30);
  } while (position);
}
void time_stop_check_004c6480(U, U e, Ports& p) {
  U list = p.call(0x4ae9d0, server(p), {});                         // GetActiveExclusionList
  U position = p.read(p.read(list));
  p.write(e + 0xc, position);
  bool stopped = false;
  while (position) {                                                // 004C64A1..004C64FD
    if (stopped) return;
    U id = p.call(0x5e9b60, p.read(list), {position});
    U cr = p.call(0x4ae770, server(p), {id});
    if (cr && sgt(p.read(cr + 0x128), 0)) {
      U cell = p.read(cr + 0x124);
      for (U n = p.read(cr + 0x128); n; --n, cell += 4)
        if (p.read16(p.read(cell) + 8) == kEffectTimeStop) stopped = true;
    }
    p.call(0x5e9b90, p.read(list), {e + 0xc});
    position = p.read(e + 0xc);
  }
  if (!stopped) p.call(0x4ae960, server(p), {1});                   // TogglePauseState(1)
}
void set_weather_00508050(U a, U weather, U keep, U e, Ports& p) {
  p.write(e, weather); p.write(e + 4, keep);
  U k = keep;                                                       // ebx
  if ((weather & 0xff) == 0xff) {                                   // 00508063..005080BB
    U target = p.read8(a + 0x221), current = p.read8(a + 0x20c);
    if (target == current) return;
    if (p.read8(a + 4) & 1) {                                       // interior
      p.write8(a + 0x20c, 0xff);
      if (target == 1) { p.write8(e, 1); k = 0; }
      else if (target == 2) { p.write8(e, 2); k = 0; }
    } else if (target == 1) {
      if (current == 2) { p.write8(e, current); k = 1; } else { p.write8(e, 1); k = 0; }
    } else if (target == 2) {
      if (current == 1) { p.write8(e, 1); k = 1; } else { p.write8(e, 2); k = 0; }
    }
  }
  U list = player_list(p);                                          // 005080C1..00508162
  if (list) {
    U head = p.read(p.read(list));
    p.write(e + 4, head);
    U player = head ? p.read(head + 8) : 0;                         // inline GetAtPos(head)
    U position = head;
    while (position) {
      if (player) {
        U object = p.call(0x561080, player, {});
        if (object && p.call(0x4cb120, object, {}) &&
            p.read(p.call(0x4cb120, object, {}) + 0x120) == p.read(a + 0x120))
          p.call(0x56d1e0, sws_message(p), {player, p.read(e), k});   // SendServerToPlayerArea_Weather
      }
      player = p.call(0x5e9b90, p.read(list), {e + 4});
      position = p.read(e + 4);
    }
  }
  p.write8(a + 0x221, k ? (p.read(e) & 0xff) : 0);                  // 00508163..00508171
}
U area_ai_update_00508a30(U a, U e, Ports& p) {
  p.call(0x4ade40, world_timer(p), {e + 4, e + 8});
  p.call(0x4ae460, world_timer(p), {p.read(e + 4), p.read(e + 8), p.read(a + 0x218), p.read(a + 0x21c), e + 0x10, e + 0xc});
  p.write(a + 0x218, p.read(e + 4));
  p.write(a + 0x21c, p.read(e + 8));
  if (p.read8(a + 0x220) == 0xff && !(p.read8(a + 4) & 1)) {         // 00508A8B..00508B88 outdoor random weather
    U countdown = p.read(a + 0x214);
    if (!countdown) {
      S roll = static_cast<S>(p.call(0x6fc447, 0, {})) % 100;
      U rain = p.read8(a + 0xa8), snow = p.read8(a + 0xa9), kind = 0;
      if (rain) kind = 1;
      else if (snow) kind = 2;
      if (kind) {
        S chance = static_cast<S>(kind == 1 ? rain : snow);
        bool change = false;
        if (roll < chance) { if (!p.read(a + 0x210)) { p.write8(a + 0x20c, kind); p.write(a + 0x210, 1); change = true; } }
        else if (p.read(a + 0x210)) { p.write8(a + 0x20c, kind); p.write(a + 0x210, 0); change = true; }
        if (change) set_weather_00508050(a, p.read8(a + 0x20c), p.read(a + 0x210), e + 0x14, p);
      }
      p.write(a + 0x214, 0x493e0);                                   // 300000 ms
    } else {
      U elapsed = p.read(e + 0xc);
      p.write(a + 0x214, countdown > elapsed ? countdown - elapsed : 0);
    }
  }
  p.call(0x4ae460, world_timer(p), {p.read(e + 4), p.read(e + 8), p.read(a + 0x13c), p.read(a + 0x140), e + 0x10, e + 0xc});
  if (p.read(e + 0xc) < kHeartbeatInterval) return 0;               // 00508BC5..00508BCD
  if (p.read(a + 0x140))                                            // OnHeartbeat
    p.call(0x5d0fc0, p.global(kVirtualMachine), {a + 0x164, p.read(a + 0x120), 1});
  p.write(a + 0x13c, p.read(e + 4));
  p.write(a + 0x140, p.read(e + 8));
  return 1;
}
U module_ai_update_004c6c90(U m, U e, Ports& p) {
  p.call(0x4ade40, world_timer(p), {e + 4, e + 8});
  p.call(0x4ae460, world_timer(p), {p.read(e + 4), p.read(e + 8), p.read(m + 0x128), p.read(m + 0x12c), e + 0x10, e + 0xc});
  if (p.read(e + 0xc) >= kHeartbeatInterval) {                      // 004C6CEB..004C6D34
    if (p.read(m + 0x12c)) p.call(0x5d0fc0, p.global(kVirtualMachine), {m + 0xb0, p.read(m + 0x20), 1});
    p.write(m + 0x128, p.read(e + 4));
    p.write(m + 0x12c, p.read(e + 8));
    return 1;
  }
  p.call(0x4ae460, world_timer(p), {p.read(e + 4), p.read(e + 8), p.read(m + 0x1c0), p.read(m + 0x1c4), e + 0x10, e + 0xc});
  update_time_004c6210(m, p.read(e + 4), p.read(e + 8), p.read(e + 0xc), e + 0x14, p);
  p.write(m + 0x1c0, p.read(e + 4));
  p.write(m + 0x1c4, p.read(e + 8));
  if ((active_pause_state_004ae990(server(p), p) & 0xff) == 1) time_stop_check_004c6480(m, e + 0x4c, p);
  return 0;
}
U module_do_update_004c6dc0(U m, U e, Ports& p) {
  if (p.read(m + 0x44)) {                                           // 004C6DC3..004C6DD2 (tail call)
    p.write(m + 0x44, 0);
    return module_ai_update_004c6c90(m, e, p);
  }
  U area = p.call(0x4ae780, server(p), {p.read(m + 0x40)});         // GetAreaByGameObjectID
  U r = area;
  if (area) r = area_ai_update_00508a30(area, e, p);
  p.write(m + 0x44, 1);
  return r;
}
void report_overflow_004ce6e0(U o, U code, U value, U limit, U e, Ports& p) {
  if (debug_mode(p) != 1) return;                                   // 004CE703..004CE70B
  U messages = sws_message(p);
  U list = player_list(p);
  if (!messages || !list) return;
  U position = p.read(p.read(list));
  p.write(e, position);
  while (position) {                                                // 004CE760..004CE884
    U player = p.call(0x5e9b60, p.read(list), {position});
    if (player && p.call(0x561080, player, {})) {
      U creature = p.vcall(p.call(0x561080, player, {}), 0x30, {});   // AsSWSCreature
      if (creature) {
        U mem = p.call(0x6fa7e6, 0, {0x34});
        U msg = mem ? leaves::message_data_construct_004d69f0(mem, p) : 0;
        p.call(0x4d76b0, msg, {0, value});
        p.call(0x4d76b0, msg, {1, limit});
        p.call(0x4d7780, msg, {0, kInvalidObject});
        p.call(0x4d7780, msg, {1, p.read(o + 4)});
        p.call(0x4d6ae0, msg, {1, o + 0x18});                       // SetString(1, tag)
        if (code == 0xa3) {                                         // AI-overflow report: AI master counters
          U aim = ai_master(p);
          p.call(0x4d6ae0, msg, {2, aim + 0x60});
          p.call(0x4d76b0, msg, {2, p.read(aim + 0x7c)});
          U v = p.read(aim + 0x74) + p.read(aim + 0x78) * 2;
          v = p.read(aim + 0x70) + v * 2;
          v = p.read(aim + 0x6c) + v * 2;
          v = p.read(aim + 0x68) + v * 2;
          p.call(0x4d76b0, msg, {3, v});
        }
        leaves::send_feedback_004ede10(creature, code, msg, p);
      }
    }
    p.call(0x5e9b90, p.read(list), {e});
    position = p.read(e);
  }
}
}
