#include "0b7f360fa8-run_actions.hpp"
#include "0c3a23cb81-action_queue.hpp"
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// 0057F4A0 CSWSObject::RunActions: the action-queue runner. W1 owns the runner and dispatch table; the per-type
// bodies are W5's (lane-w5 ACTION-ABI.md): body(receiver, node) thiscall ret 4, EAX = result code.
namespace w1::run_actions {
extern "C" unsigned __fastcall w5_action_party_follow_leader_00511130(void*,void*,void*);
namespace {
constexpr U kInProgress = 1;     // [00745DBC]
constexpr U kComplete = 2;       // [00745DC0]
constexpr U kFailed = 3;         // [00745DC4]
constexpr U kRequeue = 4;        // [00745DC8]
constexpr U kTimeBudget = 1000;  // [00745DCC] high-resolution units per RunActions call
constexpr U kDebugQueueDump = 0x8327fc;    // BSS debug flag (no writer in the original .text)
constexpr U kCameraRestored = 0x7a2288;    // .data cell (initial 1), also read by the client camera code
enum Receiver : U { kObject, kCreature };
struct Case { U body; U gate; Receiver receiver; };
// Derived from the jump table at 005801E4 (65 entries, type-1 index) and each case's first instructions.
// gate 0 = any object type, 5 = creature only, 59 = creature or placeable.
Case case_for(U type) {
  switch (type) {
  case 0x01: return {0x51f4f0, 5, kCreature};   case 0x02: return {0x5101a0, 5, kCreature};
  case 0x03: return {0x5155b0, 5, kCreature};   case 0x04: return {0x50ff20, 5, kCreature};
  case 0x05: return {0x51d600, 5, kCreature};   case 0x06: return {0x57d080, 0, kObject};
  case 0x07: return {0x517410, 5, kCreature};   case 0x08: return {0x510fd0, 5, kCreature};
  case 0x09: return {0x513830, 5, kCreature};   case 0x0a: return {0x510670, 5, kCreature};
  case 0x0b: return {0x513ec0, 5, kCreature};   case 0x0e: return {0x57b430, 0, kObject};
  case 0x10: return {0x513f60, 5, kCreature};   case 0x11: return {0x5103b0, 5, kCreature};
  case 0x12: return {0x5108a0, 5, kCreature};   case 0x13: return {0x50fb60, 5, kCreature};
  case 0x14: return {0x57d490, 0, kObject};     case 0x15: return {0x57bbf0, 0, kObject};
  case 0x16: return {0x5137c0, 5, kCreature};   case 0x17: return {0x57cf00, 0, kObject};
  case 0x18: return {0x57a470, 0, kObject};     case 0x19: return {0x519570, 5, kCreature};
  case 0x1a: return {0x518c40, 5, kCreature};   case 0x1b: return {0x50e400, 5, kCreature};
  case 0x1c: return {0x50e900, 5, kCreature};   case 0x1d: return {0x519e30, 5, kCreature};
  case 0x1e: return {0x57b5b0, 0, kObject};     case 0x1f: return {0x57b290, 0, kObject};
  case 0x20: return {0x57b320, 0, kObject};     case 0x21: return {0x57b3d0, 0, kObject};
  case 0x22: return {0x57b6a0, 0, kObject};     case 0x23: return {0x57bae0, 59, kObject};
  case 0x24: return {0x510c20, 5, kCreature};   case 0x25: return {0x57b530, 0, kObject};
  case 0x26: return {0x57d9d0, 0, kObject};     case 0x27: return {0x57bec0, 0, kObject};
  case 0x28: return {0x57e8c0, 0, kObject};     case 0x29: return {0x5140d0, 5, kCreature};
  case 0x2a: return {0x5140e0, 5, kCreature};   case 0x2b: return {0x5140d0, 5, kCreature};
  case 0x2c: return {0x50fd50, 5, kCreature};   case 0x2d: return {0x515ac0, 5, kCreature};
  case 0x2f: return {0x57c7b0, 0, kObject};     case 0x30: return {0x51d110, 5, kCreature};
  case 0x31: return {0x50fc90, 5, kCreature};   case 0x32: return {0x514270, 5, kCreature};
  case 0x33: return {0x51e6a0, 5, kCreature};   case 0x34: return {0x5140f0, 5, kCreature};
  case 0x35: return {0x5141b0, 5, kCreature};   case 0x36: return {0x5140d0, 5, kCreature};
  case 0x37: return {0x5132e0, 5, kCreature};   case 0x38: return {0x517a60, 5, kCreature};
  case 0x3a: return {0x5136b0, 5, kCreature};   case 0x3c: return {0x511080, 5, kCreature};
  case 0x3e: return {0x57ce00, 0, kObject};     case 0x3f: return {0x5b6210, 5, kCreature};
  /* 0x40: special-cased in run_actions (0057FAC6) */  case 0x41: return {0x51b420, 5, kCreature};
  default: return {0, 0, kObject};               // 0x0D, 0x39, 0x3B and out of range: no body (result 3)
  }
}
void debug_queue_dump(U o, U f, Ports& p) {       // 0057F4E3..0057F635 (only with the BSS debug flag set)
  p.call(0x5b3190, f + 0x20, {});
  p.call(0x5e5670, o + 0x18, {});
  if (p.vcall(o, 0x30, {})) {
    for (int k = 0; k < 9; ++k) (void)(k < 8 ? p.vcall(o, 0x30, {}) : 0);
    U cr = p.vcall(o, 0x30, {});
    if (p.read(p.read(cr + 0x9c8) + 0x958)) p.vcall(o, 0x30, {});
    p.vcall(o, 0x30, {});
  }
  U position = p.read(p.read(o + 0xfc));
  if (position) {
    p.write(f + 0x10, position);
    p.call(0x5e5140, f + 0x20, {0x73d5d8});                         // = " "
    while (p.read(f + 0x10)) {
      p.call(0x5b3190, f + 0x28, {});
      U data = p.call(0x5e9b60, p.read(o + 0xfc), {p.read(f + 0x10)});
      p.call(0x5e5680, f + 0x28, {0x749430, p.read(data), p.read16(data + 0x6c)});   // " %d(%d)"
      U sum = p.call(0x5e5d10, f + 0x20, {f + 0x30, f + 0x28});
      p.call(0x5e5c50, f + 0x20, {sum});
      p.call(0x5e5c20, f + 0x30, {});
      p.call(0x5e9b90, p.read(o + 0xfc), {f + 0x10});
      p.call(0x5e5c20, f + 0x28, {});
    }
    p.call(0x5e5670, f + 0x20, {});
  }
  p.call(0x5e5c20, f + 0x20, {});
}
void destroy_node(U node, Ports& p) {
  action_queue::action_node_destroy_004cc4d0(node, p);
  p.call(0x6fa390, 0, {node});
}
}
U run_actions_0057f4a0(U o, U day, U ms, U start_lo, U start_hi, U f, Ports& p) {
  U result = kFailed;                                               // ebp = [00745DC4]
  const U kind = p.read8(o + 8);                                    // ebx / [esp+14]
  p.write(f + 0x1c, 0);                                             // stop flag
  p.write(f + 0x18, 0);                                             // last action type
  if (p.global(kDebugQueueDump)) debug_queue_dump(o, f, p);
  if (p.read(p.read(o + 0xfc))) {                                   // 0057F63A..0057F6B3 party leader camera restore
    U client = p.read(p.global(kAppManager) + 4);
    U id = p.read(o + 4);
    U leader = p.read(p.call(0x5ed8b0, client, {}) + 0x24);
    if (p.call(0x5eda50, client, {id}) == leader &&
        p.read8(p.call(0x5ed700, p.read(p.global(kAppManager) + 4), {}) + 0x6d) == 5) {
      p.set_global(kCameraRestored, 1);
      p.call(0x641dc0, p.call(0x5ed530, p.read(p.global(kAppManager) + 4), {}), {});   // CSWCModule::RestoreCamera
      p.call(0x5eda60, p.read(p.global(kAppManager) + 4), {0, 1});  // SetInputClass(0, 1)
    }
  }
  for (;;) {                                                        // 0057F6C0
    if (p.read(p.read(o + 0xfc))) {
      U node = p.call(0x5e9c80, p.read(o + 0xfc), {});               // RemoveHead
      if (!p.read(o + 0xbc) && !p.read(o + 0xc0)) { p.write(o + 0xbc, day); p.write(o + 0xc0, ms); }
      p.write(o + 0x1e4, node);
      U type = p.read(node);
      p.write(f + 0x18, type);
      p.write16(o + 0x80, type & 0xffff);
      result = kFailed;
      if (type == 0x0c) {                                           // 0057FB07..0057FB5E
        if (kind == 5) {
          p.write(o + 0x84, p.read(node + 0x38));
          U self = p.vcall(o, 0x30, {});
          result = p.call(0x5bbbf0, self, {node});                  // AIActionPhysicalAttacks
          p.write(o + 0x84, kInvalidObject);
          if (result != kInProgress)
            p.call(0x4ade40, p.call(0x4aede0, server(p), {}), {f + 0x10, f + 0x20});   // refreshed time is not used
        }
      } else if (type == 0x0f) {                                    // 0057F80C..0057F865
        if (kind == 5) {
          result = p.call(0x514af0, p.vcall(o, 0x30, {}), {node});
          if (result == kFailed || result == kComplete)
            p.call(0x4d35c0, p.read(p.vcall(o, 0x30, {}) + 0x9c8), {});   // CSWSCombatRound::GetSpellActionPending
        } else if (kind == 9) {
          result = p.call(0x584ec0, p.vcall(o, 0x48, {}), {node});  // CSWSPlaceable::AIActionCastSpell
        }
      } else if (type == 0x2e) {                                    // 0057F8DE..0057F923
        if (kind == 5) {
          result = p.call(0x50f170, p.vcall(o, 0x30, {}), {node});
          if (result != kInProgress) {
            p.write(p.vcall(o, 0x30, {}) + 0x524, kInvalidObject);
            p.write(p.vcall(o, 0x30, {}) + 0x528, kInvalidObject);
          }
        }
      } else if (type == 0x3d) {                                    // 0057F748..0057F7A3
        if (p.call(0x5ed550, p.read(p.global(kAppManager) + 4), {}) == kInvalidObject) result = kInProgress;
        else if (kind == 5 && p.vcall(o, 0x30, {}) && p.read(p.vcall(o, 0x30, {}) + 0xa88))
          result = w5_action_party_follow_leader_00511130(reinterpret_cast<void*>(p.vcall(o, 0x30, {})), nullptr, reinterpret_cast<void*>(node)); // AIActionPartyFollowLeader; receiver and node are 32-bit U addresses preserved as pointers; no 0x511130 trap
      } else if (type == 0x40) {                                    // 0057FAC6..0057FB02
        if (kind == 5 && p.vcall(o, 0x30, {}) && p.read(p.vcall(o, 0x30, {}) + 0xa88))
          result = p.call(0x510ab0, p.vcall(o, 0x30, {}), {node}); // AIActionCheckMoveToFollowRadius
      } else {
        Case k = case_for(type);
        bool gate = k.gate == 0 || (k.gate == 5 && kind == 5) || (k.gate == 59 && (kind == 5 || kind == 9));
        if (k.body && gate) {
          U receiver = k.receiver == kCreature ? p.vcall(o, 0x30, {}) : o;
          if (k.body == 0x5140d0) { (void)receiver; result = kFailed; }   // 005140D0: mov eax,[00745DC4]; ret 4 (W1-owned)
          else result = p.call(k.body, receiver, {node});
        }
      }
      // 0057FE08: debug combat-round dump (BSS debug flag)
      if (p.global(kDebugQueueDump) && p.vcall(o, 0x30, {}) && p.read(p.vcall(o, 0x30, {}) + 0xa88)) {
        U cr = p.vcall(o, 0x30, {});
        p.call(0x4d42e0, p.read(p.vcall(o, 0x30, {}) + 0x9c8), {0xa, cr + 0x18});
        U target = p.read(p.vcall(o, 0x30, {}) + 0x50c);
        if (target != kInvalidObject) {
          U t = p.call(0x4ae750, server(p), {target});
          if (t && p.vcall(t, 0x30, {})) {
            U tc = p.vcall(t, 0x30, {});
            p.call(0x4d42e0, p.read(tc + 0x9c8), {0x1e, tc + 0x18});
          }
        }
      }
      p.write(o + 0x1e4, 0);                                        // 0057FE9E..0057FED5
      p.write(ai_master(p) + 0x7c, p.read16(o + 0x80));
      p.write16(o + 0x80, 0xffff);
      p.write(o + 0xa8, day);
      p.write(o + 0xac, ms);
      if (result == kRequeue) {
        p.call(0x5e9dc0, p.read(o + 0xfc), {node});                 // AddTail
      } else if (result == kInProgress) {
        p.call(0x5e9d70, p.read(o + 0xfc), {node});                 // AddHead
      } else {                                                      // 0057FF15..0057FF66 complete / failed
        if (p.vcall(o, 0xc4, {}) != kInvalidObject && p.read(f + 0x18) != 0x0c) {
          p.vcall(o, 0xc4, {});
          p.vcall(o, 0xc0, {kInvalidObject});
        }
        destroy_node(node, p);
        p.write(o + 0xbc, 0);
        p.write(o + 0xc0, 0);
      }
    } else if (p.vcall(o, 0x30, {})) {                              // 0057FF6B..0058000E queue empty: clear creature targets
      U cr = p.vcall(o, 0x30, {});
      for (U off : {0x50cu, 0x504u, 0x524u, 0x528u}) if (p.read(cr + off) != kInvalidObject) p.write(cr + off, kInvalidObject);
      p.write(p.vcall(o, 0x30, {}) + 0x508, kInvalidObject);
      U move = p.read(p.vcall(o, 0x30, {}) + 0x340);
      p.write(move + 0x254, kInvalidObject);
      p.write(move + 0x258, 0); p.write(move + 0x25c, 0); p.write(move + 0x260, 0);
    }
    // 00580011: stop conditions
    if (result != kInProgress) {
      std::uint64_t now = p.call64(0x5e9260, p.read(p.global(0x7a39e0) + 4), {});
      std::uint64_t spent = now - (static_cast<std::uint64_t>(start_hi) << 32 | start_lo);
      U last = p.read(f + 0x18);
      if (spent > kTimeBudget || last == 5 || last == 0x30 || last == 1 || last == 0x33) p.write(f + 0x1c, 1);
    } else {
      p.write(f + 0x1c, 1);
    }
    if (!p.read(p.read(o + 0xfc)) || p.read(f + 0x1c)) break;      // 00580065..00580076
  }
  // 0058007C: queue / effect overflow guards
  U count = p.read(p.read(o + 0xfc) + 8);
  if (count > 0x4b) {
    p.call(0x4ce6e0, o, {0xa1, count, 0x4b, 0});                    // ReportOverflow
    count = p.read(p.read(o + 0xfc) + 8);
    if (count > 0x1f4) {
      p.call(0x4ce6e0, o, {0xa4, count, 0x1f4, 1});
      U keep_culling = 1;
      while (p.read(p.read(o + 0xfc)) && keep_culling == 1) {      // 005800D7..00580123
        U node = p.call(0x5e9c80, p.read(o + 0xfc), {});
        if (p.vcall(o, 0x74, {node, keep_culling}) == keep_culling) {
          if (node) destroy_node(node, p);
        } else {
          keep_culling = 0;
          p.call(0x5e9d70, p.read(o + 0xfc), {node});
        }
      }
      if (p.read(p.read(o + 0xfc) + 8) > 0x3e8) {                   // 00580125..0058016A drop everything
        while (p.read(p.read(o + 0xfc))) {
          U node = p.call(0x5e9c80, p.read(o + 0xfc), {});
          if (node) destroy_node(node, p);
        }
      }
    }
  }
  U effects = p.read(o + 0x128);                                    // 0058016C..005801CA
  if (sgt(effects, 0x1f4)) {
    p.call(0x4ce6e0, o, {0xa2, effects, 0x1f4, 0});
    effects = p.read(o + 0x128);
    if (sgt(effects, 0x3e8)) {
      p.call(0x4ce6e0, o, {0xa5, effects, 0x3e8, 1});
      p.call(0x4d0870, o, {1});                                     // RemoveSomeEffectsOfDurationType(WORD [007473C0] = 1)
      p.call(0x4d0870, o, {2});                                     // RemoveSomeEffectsOfDurationType(WORD [007473C4] = 2)
    }
  }
  return 0;                                                         // EAX is not defined by the original epilogue
}
}
