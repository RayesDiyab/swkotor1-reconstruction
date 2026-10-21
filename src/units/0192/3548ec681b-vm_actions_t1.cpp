// W7 tier t1: NWScript engine-command bodies and their unowned leaves, reconstructed from the original
// swkotor.exe (read-only, capstone 5.0.7). Native x86 translation unit: guest pointers are 32-bit words.
// Every call to an existing provider goes to its live candidate export (present in the 1122 and 1123 maps) (named next to each declaration).
// MEMORY: only live_operator_new / live_free (original 006FA7E6 / 006FA390).
// Original faults (null allocations later dereferenced, a found-but-null game object) are reproduced by performing
// the original faulting access itself (closeC: the former explicit traps were removed); nothing is replaced by an
// invented "safe" value.
#include "73dbba025f-vm_actions_t1.hpp"
#include <cstring>
#if !defined(_M_IX86)
#error W7 ACTION bodies are native x86 (32-bit guest pointers)
#endif
static_assert(sizeof(void*) == 4, "32-bit pointers");
static_assert(sizeof(W7ExoStringValue) == 8, "CExoString by value");

namespace kotor_reconstruction { struct ArrayMutations; struct StartupString; }
namespace directory_list { char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, int); }                 // 0288 CExoString::operator[]
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); }   // 0098 CExoArrayList::Allocate
namespace kotor_rules { void __fastcall kotor_fastcall_0059d7c0(void*, void*); }   // 0429 ~CSWSExpressionNode
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }                       // 0262 CExoString::CExoString()
namespace frame_control { extern unsigned message_manager_007a3a00; }                                                       // g_pVirtualMachine (0456)
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                                               // g_pAppManager (0053)
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
unsigned __fastcall recovered_vm_pop_integer_005d24c0(void*, void*, void*);          // 1196
unsigned __fastcall recovered_vm_pop_string_005d25e0(void*, void*, void*);           // 1200
unsigned __fastcall recovered_vm_push_string_005d1930(void*, void*, void*);          // 1200
unsigned __fastcall recovered_vm_outer_push_integer_005d1010(void*, void*, unsigned);// 1196
unsigned __fastcall recovered_vm_outer_push_object_005d10d0(void*, void*, unsigned); // 1196
unsigned __fastcall recovered_vm_outer_pop_object_005d10c0(void*, void*, void*);     // 1196
void* __fastcall kotor_fastcall_004ae750(void*, void*, unsigned);                    // 0530 CServerExoApp::GetGameObject
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);    // 0457 CGameObjectArray::GetGameObject
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);                          // 0459 CExoString::~CExoString
const char* __fastcall WindowCStr005e5670(void*, void*);                             // 0078 CExoString::CStr
void* __fastcall WindowCopy005e5b00(void*, void*, const void*);                      // 0078 CExoString::CExoString(const CExoString&)
void* __fastcall resman_string_from_int_005e5bc0(void*, void*, int);                 // 0266 CExoString::CExoString(int)
void* __fastcall resman_string_counted_005e5b70(void*, void*, const char*, int);     // 0267 CExoString::CExoString(const char*, int)
void* __fastcall async_assign_string_005e5c50(void*, void*, const void*);            // 0199 CExoString::operator=
int __fastcall async_string_equal_005e5200(void*, void*, const void*);               // 0188 CExoString::operator==
int __fastcall async_string_length_005e5790(void*, void*);                           // 0188 CExoString::GetLength
void* __fastcall lower_construct_005e6080(void*, void*, void*);                      // 0310 CExoString::LowerCase (this, out)
unsigned __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);            // 0168 CExoLinkedListInternal::GetAtPos
unsigned __fastcall unresolved_list_next_005e9b90(void*, void*, void**);             // 0168 CExoLinkedListInternal::GetNext
}

namespace {
using U = std::uint32_t;
using I = std::int32_t;
constexpr U kInvalidObject = 0x7f000000u;   // [00746644]
constexpr U kTypeModule = 3;                // [00746632]
constexpr U kTypeArea = 4;                  // [00746633]
constexpr I kPopFailed = -2001;             // 0xFFFFF82F
constexpr I kPushFailed = -2000;            // 0xFFFFF830
void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
U rd8(U a) { return *static_cast<const unsigned char*>(ptr(a)); }
void wr8(U a, U v) { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(v); }
// closeC: the original's null-pointer stores, performed as the original performs them (volatile, never elided).
void wr_fault(U a, U v) { *static_cast<volatile U*>(ptr(a)) = v; }

void* vm() { return ptr(frame_control::message_manager_007a3a00); }                  // mov ecx,[007A3A00]
U server() { return rd(word(kotor_menu_window_cells::manager_007a39fc) + 8); }       // mov ecx,[[007A39FC]+8]
// CServerExoApp::GetObjectArray 004AED70 (mov eax,[ecx+4]; mov eax,[eax+1005C]). W2 t3 exports this address
// as @recovered_object_array_004aed70@8; the same two loads are inlined here (no second exported owner).
U object_array_004aed70(U s) { return rd(rd(s + 4) + 0x1005c); }
// thiscall AsX() virtual (no stack arguments)
U virtual_cast(U object, U slot) { using F = void*(__thiscall*)(void*); return word(reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object))); }
// mov ...: if pushed -> 0 else -2000 (neg/sbb/and 7D0/add F830)
I push_result(U pushed) { return pushed ? 0 : kPushFailed; }

void string_default(void* s) { (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr); }
void string_destroy(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
char at_005e5600(void* s, int i) { return directory_list::At005e5600(static_cast<kotor_reconstruction::StartupString*>(s), nullptr, i); }

// The var table of a game object, exactly as 0053B7B3..0053B7EB / 005431A6..005431D3 select it.
U var_table_of(U object) {
  const U type = rd8(object + 8);
  if (type == kTypeArea) return virtual_cast(object, 0x28) + 0x1f4;      // AsSWSArea()->m_oVarTable
  if (type == kTypeModule) return virtual_cast(object, 0x20) + 0x9c;     // AsSWSModule()->m_oVarTable
  return object + 0x110;                                                 // CSWSObject::m_oVarTable
}
}  // namespace

extern "C" {
// ============================================================================================ VM outer
// 005D1000 / 005D1080 / 005D1090: mov ecx,[ecx]; jmp CVirtualMachineInternal::*
unsigned __fastcall recovered_vm_outer_pop_integer_005d1000(void* v, void*, void* out) { return recovered_vm_pop_integer_005d24c0(ptr(rd(word(v))), nullptr, out); }
unsigned __fastcall recovered_vm_outer_pop_string_005d1080(void* v, void*, void* s) { return recovered_vm_pop_string_005d25e0(ptr(rd(word(v))), nullptr, s); }
unsigned __fastcall recovered_vm_outer_push_string_005d1090(void* v, void*, void* s) { return recovered_vm_push_string_005d1930(ptr(rd(word(v))), nullptr, s); }

// ============================================================================================ CSWVarTable
// 0059B000: index in [0,96) -> (1 << (index & 31)) & bits[index >> 5]  (the raw masked bit, not 0/1)
int __fastcall recovered_var_table_get_boolean_0059b000(void* t, void*, int index) {
  if (index < 0 || index >= 0x60) return 0;
  return static_cast<int>((1u << (static_cast<U>(index) & 31u)) & rd(word(t) + 4u * (static_cast<U>(index) >> 5)));
}
// 0059B040: set or clear the bit; out of range: no effect
void __fastcall recovered_var_table_set_boolean_0059b040(void* t, void*, int index, int value) {
  if (index < 0 || index >= 0x60) return;
  const U cell = word(t) + 4u * (static_cast<U>(index) >> 5), bit = 1u << (static_cast<U>(index) & 31u);
  wr(cell, value ? (rd(cell) | bit) : (rd(cell) & ~bit));
}
// 0059B0B0: index in [0,8) -> byte at +0C+index; else 0 (AL)
unsigned char __fastcall recovered_var_table_get_number_0059b0b0(void* t, void*, int index) {
  if (index < 0 || index >= 8) return 0;
  return static_cast<unsigned char>(rd8(word(t) + 0xc + static_cast<U>(index)));
}
// 0059B0D0: index in [0,8) -> byte store of the low byte of value
void __fastcall recovered_var_table_set_number_0059b0d0(void* t, void*, int index, int value) {
  if (index < 0 || index >= 8) return;
  wr8(word(t) + 0xc + static_cast<U>(index), static_cast<U>(value) & 0xffu);
}

// ============================================================================================ CSWSExpression
// Layouts (from the bodies): CSWSExpression 0x20 {+0 number, +4/+8/+C CExoArrayList<CExoString*>, +10 case flag,
// +14 CSWSExpressionList*, +18 CExoString parsed}; CSWSExpressionList 0x0C {+0 head, +4 cursor, +8 last};
// CSWSExpressionNode 0x1C {+0 state, +4 type, +8 (not initialised), +C next, +10 CExoString, +18 alternate (type 2)}.
void* __fastcall recovered_expression_construct_0059d990(void* e, void*) {
  const U s = word(e);
  wr(s + 8, 0); wr(s + 0xc, 0); wr(s + 4, 0);
  string_default(ptr(s + 0x18));
  wr(s + 0x14, 0); wr(s + 0x10, 0); wr(s, 0);
  return e;
}
void* __fastcall recovered_expression_new_node_0059d860(void*, void*, int type) {
  void* n = live_operator_new(0x1c);
  if (!n) { wr_fault(word(n) + 4, static_cast<U>(type)); return nullptr; }   // 0059D8C3..0059D8DB: mov [0+4],type; eax=0
  const U s = word(n);
  string_default(ptr(s + 0x10));
  wr(s + 0xc, 0); wr(s, 0); wr(s + 4, static_cast<U>(type));
  return n;
}
void __fastcall recovered_expression_list_add_node_0059d820(void* l, void*, void* node) {
  const U s = word(l), n = word(node);
  if (rd(s) == 0 && rd(s + 8) == 0) { wr(s, n); wr(s + 8, n); wr(s + 4, n); return; }
  const U cursor = rd(s + 4);
  if (cursor == rd(s + 8)) wr(s + 8, n);
  wr(n + 0xc, rd(cursor + 0xc));
  wr(rd(s + 4) + 0xc, n);
  wr(s + 4, n);
}
// 0059D7D0: walk an alternate chain until a type-3 node (not freed) or the end; recurse into nested type-2 nodes
void __fastcall recovered_expression_list_delete_alternate_0059d7d0(void* l, void*, void* node) {
  U n = word(node);
  while (n) {
    const U type = rd(n + 4);
    if (type == 3) return;
    const U next = rd(n + 0xc);
    if (type == 2) recovered_expression_list_delete_alternate_0059d7d0(l, nullptr, ptr(rd(n + 0x18)));
    kotor_rules::kotor_fastcall_0059d7c0(ptr(n), nullptr);   // 0429 owner of ~CSWSExpressionNode 0059D7C0
    live_free(ptr(n));
    n = next;
  }
}
// 0059D920: free every node of the main chain (alternates through DeleteAlternate); the list object itself is not freed
void __fastcall recovered_expression_list_delete_list_0059d920(void* l, void*) {
  U n = rd(word(l));
  while (n) {
    const U next = rd(n + 0xc);
    if (rd(n + 4) == 2) recovered_expression_list_delete_alternate_0059d7d0(l, nullptr, ptr(rd(n + 0x18)));
    kotor_rules::kotor_fastcall_0059d7c0(ptr(n), nullptr);   // 0429 owner of ~CSWSExpressionNode 0059D7C0
    live_free(ptr(n));
    n = next;
  }
}
namespace {
// operator new(0Ch) + {0,0,0}. On failure the original stores the null list and only faults where it is first
// dereferenced; the dereferencing instructions below fault on the null list exactly there.
U new_list() {
  void* l = live_operator_new(0xc);
  if (l) { wr(word(l), 0); wr(word(l) + 4, 0); wr(word(l) + 8, 0); }
  return word(l);
}
U checked(U list, U /*fault_address: the original instruction that faults on a null list*/) { return list; }
struct ByteArray { U data = 0; I size = 0; I allocated = 0; };   // CExoArrayList<char> (frame +30)
struct ListArray { U data = 0; I size = 0; I allocated = 0; };   // CExoArrayList<CSWSExpressionList*> (frame +3C)
static_assert(sizeof(ListArray) == 12, "CExoArrayList layout");
void lists_allocate(ListArray& a, U n) { menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&a), nullptr, n); }
}
// 0059DAB0 CSWSExpression::ParseString(CExoString by value). Returns 1 when unchanged, 0 when parsed,
// -1000 ')' without '(', -1001 '(' left open, -1002 '|' on an empty branch, -1003 '*' at the end,
// -1004 unknown '*' escape. Error exits leak the partial graph and leave +14 pointing to the list that
// was freed on entry (original behaviour, preserved).
int __fastcall recovered_expression_parse_string_0059dab0(void* e, void*, W7ExoStringValue s) {
  const U self = word(e);
  void* const sp = &s;
  if (rd(self + 0x10) == 0) {                                   // 0059DADB: case-insensitive expressions are lowercased
    W7ExoStringValue lower{};
    (void)lower_construct_005e6080(sp, nullptr, &lower);
    (void)async_assign_string_005e5c50(sp, nullptr, &lower);
    string_destroy(&lower);
  }
  if (async_string_equal_005e5200(ptr(self + 0x18), nullptr, sp)) { string_destroy(sp); return 1; }
  if (rd(self + 0x14)) {                                        // 0059DB46: +14 is not cleared
    recovered_expression_list_delete_list_0059d920(ptr(rd(self + 0x14)), nullptr);
    if (rd(self + 0x14)) live_free(ptr(rd(self + 0x14)));
  }
  ListArray lists;
  const U first = new_list();
  lists_allocate(lists, 0x10);
  wr(lists.data + 4u * static_cast<U>(lists.size), first); ++lists.size;
  ByteArray chars;
  I depth = 0;                                                  // EDI
  I status = 0;
  const I length = async_string_length_005e5790(sp, nullptr);
  auto add_to = [&](U node, U site) { recovered_expression_list_add_node_0059d820(ptr(checked(rd(lists.data + 4u * static_cast<U>(depth)), site)), nullptr, ptr(node)); };
  for (I i = 0; i < length && status == 0; ++i) {
    const char c = at_005e5600(sp, i);
    switch (c) {
    case '*': {                                                 // 0059DC0F
      ++i;
      if (i >= length) { status = -1003; break; }
      int type = 0;
      switch (at_005e5600(sp, i)) {
      case '*': type = 1; break;
      case 'w': type = 4; break;
      case 'p': type = 5; break;
      case 'a': type = 6; break;
      case 'n': type = 7; break;
      case 'A': type = 8; break;
      default: status = -1004; break;
      }
      if (status == 0) add_to(word(recovered_expression_new_node_0059d860(e, nullptr, type)), 0x0059d820);
      break;
    }
    case '(': {                                                 // 0059DC7B
      ++depth;
      const U list = new_list();
      if (lists.size == lists.allocated) lists_allocate(lists, lists.allocated ? static_cast<U>(lists.allocated) * 2u : 0x10u);
      wr(lists.data + 4u * static_cast<U>(lists.size), list); ++lists.size;
      break;
    }
    case '|': {                                                 // 0059DCCC
      const U cur = checked(rd(lists.data + 4u * static_cast<U>(depth)), 0x0059dccf);
      if (rd(cur) == 0) { status = -1002; break; }
      const U join = word(recovered_expression_new_node_0059d860(e, nullptr, 3));
      const U fork = word(recovered_expression_new_node_0059d860(e, nullptr, 2));
      recovered_expression_list_add_node_0059d820(ptr(cur), nullptr, ptr(join));
      wr(fork + 0x18, rd(cur));
      wr(fork + 0xc, join);
      wr(cur, fork);
      wr(cur + 4, fork);
      break;
    }
    case ')': {                                                 // 0059DD37
      if (depth < 1) { status = -1000; break; }
      const U prev = checked(rd(lists.data + 4u * static_cast<U>(depth - 1)), 0x0059dd44), cur = rd(lists.data + 4u * static_cast<U>(depth));
      if (rd(prev) == 0) {
        checked(cur, 0x0059dd4c);
        wr(prev, rd(cur));
        wr(prev + 8, rd(cur + 8));
      } else {
        checked(cur, 0x0059dd68);
        const U moved = rd(rd(prev + 4) + 0xc);                   // 0059DD65..0059DD6B
        wr_fault(rd(cur + 8) + 0xc, moved);                        // 0059DD6E: empty '()' after text -> [0+0Ch] faults (original)
        if (rd(prev + 4) == rd(prev + 8)) wr(prev + 8, rd(cur + 8));
        wr(rd(prev + 4) + 0xc, rd(cur));
      }
      wr(prev + 4, rd(cur + 8));
      if (cur) live_free(ptr(cur));
      --lists.size;
      for (I k = depth; k < lists.size; ++k) wr(lists.data + 4u * static_cast<U>(k), rd(lists.data + 4u * static_cast<U>(k + 1)));
      --depth;
      break;
    }
    default: {                                                  // 0059DDD3: literal text run
      if (chars.size == chars.allocated) {
        const I capacity = chars.allocated ? chars.allocated * 2 : 0x10;
        chars.allocated = capacity;
        void* grown = live_operator_new(static_cast<U>(capacity));
        for (I k = 0; k < chars.size; ++k)                         // 0059DE13: a null buffer faults on the first copied byte
          *static_cast<volatile unsigned char*>(ptr(word(grown) + static_cast<U>(k))) = static_cast<unsigned char>(rd8(chars.data + static_cast<U>(k)));
        if (chars.data) live_free(ptr(chars.data));
        chars.data = word(grown);
      }
      *static_cast<volatile unsigned char*>(ptr(chars.data + static_cast<U>(chars.size))) = static_cast<unsigned char>(c);   // 0059DE36
      ++chars.size;
      bool flush = i + 1 >= async_string_length_005e5790(sp, nullptr);
      if (!flush) {
        const char n = at_005e5600(sp, i + 1);
        flush = n == '*' || n == '|' || n == '(' || n == ')';
      }
      if (flush) {
        const U node = word(recovered_expression_new_node_0059d860(e, nullptr, 0));
        W7ExoStringValue text{};
        (void)resman_string_counted_005e5b70(&text, nullptr, static_cast<const char*>(ptr(chars.data)), chars.size);
        (void)async_assign_string_005e5c50(ptr(node + 0x10), nullptr, &text);
        chars.size = 0;
        add_to(node, 0x0059de9c);
        string_destroy(&text);
      }
      break;
    }
    }
  }
  if (status == 0 && depth > 0) status = -1001;                 // 0059DEFD
  if (status == 0) {                                            // 0059E01A
    const U head = checked(rd(lists.data), 0x0059e01e), last = rd(head + 8);
    const U end = word(recovered_expression_new_node_0059d860(e, nullptr, 9));
    if (last) wr(last + 0xc, end);
    else recovered_expression_list_add_node_0059d820(ptr(head), nullptr, ptr(end));
    wr(self + 0x14, rd(lists.data));
    (void)async_assign_string_005e5c50(ptr(self + 0x18), nullptr, sp);
  }
  if (chars.data) live_free(ptr(chars.data));                  // the original calls _free unconditionally; _free(NULL) returns
  if (lists.data) live_free(ptr(lists.data));
  string_destroy(sp);
  return status;
}

// ============================================================================================ CSWSObject listen
// 004CD6C0: first expression in +18C[0..+190) whose number (+0) equals `number`
void* __fastcall recovered_object_get_listen_expression_004cd6c0(void* o, void*, int number) {
  const U self = word(o), count = rd(self + 0x190), data = rd(self + 0x18c);
  for (I i = 0; i < static_cast<I>(count); ++i) {
    const U e = rd(data + 4u * static_cast<U>(i));
    if (static_cast<I>(rd(e)) == number) return ptr(e);
  }
  return nullptr;
}
// 004CF8F0: find or append the expression for `number`, then parse a copy of `string` into it (result ignored)
void __fastcall recovered_object_set_listen_expression_004cf8f0(void* o, void*, void* string, int number) {
  const U self = word(o);
  U e = word(recovered_object_get_listen_expression_004cd6c0(o, nullptr, number));
  if (!e) {
    void* block = live_operator_new(0x20);
    e = block ? word(recovered_expression_construct_0059d990(block, nullptr)) : 0u;   // 004CF92E..004CF93B: edi = 0 on failure
    const U list = self + 0x18c;
    wr_fault(e, static_cast<U>(number));                           // 004CF943: mov [edi],ebx (faults when edi = 0)
    if (rd(list + 4) == rd(list + 8)) {
      const U allocated = rd(list + 8);
      menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(list)), nullptr, allocated ? allocated * 2u : 0x10u);
    }
    wr(rd(list) + 4u * rd(list + 4), e);
    wr(list + 4, rd(list + 4) + 1);
  }
  W7ExoStringValue copy{};
  (void)WindowCopy005e5b00(&copy, nullptr, string);
  (void)recovered_expression_parse_string_0059dab0(ptr(e), nullptr, copy);
}

// ============================================================================================ player list
// 004B4C50 / 004B4C80 (donor gog-dialogue first_pc_action.cpp @bb622021): the cursor at +100F8 is written even
// when the head is null; a null GetAtPos/GetNext payload yields OBJECT_INVALID; the payload's +38 is the PC id.
unsigned __fastcall recovered_server_internal_first_pc_004b4c50(void* internal, void*) {
  const U self = word(internal), wrapper = rd(self + 0x10064), head = rd(rd(wrapper));
  wr(self + 0x100f8, head);
  if (!head) return kInvalidObject;
  const U player = unresolved_list_at_pos_005e9b60(ptr(rd(wrapper)), nullptr, ptr(head));
  return player ? rd(player + 0x38) : kInvalidObject;
}
unsigned __fastcall recovered_server_internal_next_pc_004b4c80(void* internal, void*) {
  const U self = word(internal);
  if (!rd(self + 0x100f8)) return kInvalidObject;
  const U player = unresolved_list_next_005e9b90(ptr(rd(rd(self + 0x10064))), nullptr, static_cast<void**>(ptr(self + 0x100f8)));
  return player ? rd(player + 0x38) : kInvalidObject;
}
unsigned __fastcall recovered_server_first_pc_004aea10(void* s, void*) { return recovered_server_internal_first_pc_004b4c50(ptr(rd(word(s) + 4)), nullptr); }
unsigned __fastcall recovered_server_next_pc_004aea20(void* s, void*) { return recovered_server_internal_next_pc_004b4c80(ptr(rd(word(s) + 4)), nullptr); }

// ============================================================================================ ACTION bodies
// 00542FB0 SetListening(object, int): object +19C = value when the object array resolves the id
int __fastcall recovered_vm_set_listening_00542fb0(void*, void*, int, unsigned) {
  U id = 0, value = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return kPopFailed;
  if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) return kPopFailed;
  void* object = nullptr;
  if (kotor_fastcall_004d8230(ptr(object_array_004aed70(server())), nullptr, id, &object) == 0 && object) wr(word(object) + 0x19c, value);
  return 0;
}
// 00543030 SetListenString(object, string, int): CSWSObject::SetListenExpression(string, number)
int __fastcall recovered_vm_set_listen_string_00543030(void*, void*, int, unsigned) {
  W7ExoStringValue text;
  string_default(&text);
  U id = 0, number = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id) || !recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &text) ||
      !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &number)) {
    string_destroy(&text);
    return kPopFailed;
  }
  void* object = nullptr;
  if (kotor_fastcall_004d8230(ptr(object_array_004aed70(server())), nullptr, id, &object) == 0 && object)
    recovered_object_set_listen_expression_004cf8f0(object, nullptr, &text, static_cast<int>(number));
  string_destroy(&text);
  return 0;
}
// 00543130 SetLocalValue(object, int index, int value): 680 boolean, 682 number (value must fit a signed byte)
int __fastcall recovered_vm_set_local_value_00543130(void*, void*, int routine, unsigned) {
  U id = 0, index = 0, value = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id) || !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &index) ||
      !recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value))
    return kPopFailed;
  const U object = word(kotor_fastcall_004ae750(ptr(server()), nullptr, id));
  if (!object) return 0;
  const U table = var_table_of(object);
  if (!table) return 0;
  if (routine == 0x2a8) {
    recovered_var_table_set_boolean_0059b040(ptr(table), nullptr, static_cast<int>(index), static_cast<int>(value));
  } else if (routine == 0x2aa) {
    const I v = static_cast<I>(value);
    if (v >= -0x80 && v <= 0x7f) recovered_var_table_set_number_0059b0d0(ptr(table), nullptr, static_cast<int>(index), v);
    else { (void)WindowCStr005e5670(ptr(object + 0x18), nullptr); (void)WindowCStr005e5670(ptr(object + 0x18), nullptr); }  // 0054320C: results unused
  }
  return 0;
}
// 0053B760 GetLocalValue(object, int index): 679 boolean (raw masked bit), 681 number (zero-extended byte)
int __fastcall recovered_vm_get_local_value_0053b760(void*, void*, int routine, unsigned) {
  U id = 0, index = 0, result = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return kPopFailed;
  if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &index)) return kPopFailed;
  const U object = word(kotor_fastcall_004ae750(ptr(server()), nullptr, id));
  if (object) {
    const U table = var_table_of(object);
    if (table) {
      if (routine == 0x2a7) result = static_cast<U>(recovered_var_table_get_boolean_0059b000(ptr(table), nullptr, static_cast<int>(index)));
      else if (routine == 0x2a9) result = recovered_var_table_get_number_0059b0b0(ptr(table), nullptr, static_cast<int>(index));
    }
  }
  return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result));
}
// 0053C720 GetIsObjectValid(object) (donor gog-dialogue object_valid_action.cpp @bb622021)
int __fastcall recovered_vm_get_is_object_valid_0053c720(void*, void*, int, unsigned) {
  U id = 0;
  if (!recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &id)) return kPopFailed;
  const U valid = kotor_fastcall_004ae750(ptr(server()), nullptr, id) ? 1u : 0u;
  return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, valid));
}
// 0053EBC0 IntToString(int) (donor mac-vm-diagnostics diagnostics.cpp @c099c4f6)
int __fastcall recovered_vm_int_to_string_0053ebc0(void*, void*, int, unsigned) {
  W7ExoStringValue result;
  string_default(&result);
  U value = 0;
  if (!recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &value)) { string_destroy(&result); return kPopFailed; }
  W7ExoStringValue temporary{};
  (void)resman_string_from_int_005e5bc0(&temporary, nullptr, static_cast<int>(value));
  (void)async_assign_string_005e5c50(&result, nullptr, &temporary);
  string_destroy(&temporary);
  const U pushed = recovered_vm_outer_push_string_005d1090(vm(), nullptr, &result);
  string_destroy(&result);
  return pushed ? 0 : kPushFailed;
}
// 0053C920 GetFirstPC / GetNextPC: no pops; any other routine pushes OBJECT_INVALID
int __fastcall recovered_vm_get_pc_0053c920(void*, void*, int routine, unsigned) {
  U id = kInvalidObject;
  if (routine == 0x224) id = recovered_server_first_pc_004aea10(ptr(server()), nullptr);
  else if (routine == 0x225) id = recovered_server_next_pc_004aea20(ptr(server()), nullptr);
  return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id));
}
// 0053E220 GetUserDefinedEventNumber: object = this+8 (script owner); -1 when the id is not in the array
int __fastcall recovered_vm_get_user_defined_event_number_0053e220(void* self, void*, int, unsigned) {
  U number = 0xffffffffu;
  void* found = nullptr;
  if (kotor_fastcall_004d8230(ptr(object_array_004aed70(server())), nullptr, rd(word(self) + 8), &found) == 0) {
    const U object = word(found);
    switch (*static_cast<const volatile unsigned char*>(ptr(object + 8))) {   // 0053E251: movzx eax,[ecx+8] (faults on a null object)
    case 3: number = rd(virtual_cast(object, 0x20) + 0x148); break;   // module
    case 4: number = rd(virtual_cast(object, 0x28) + 0x1dc); break;   // area
    case 13: number = rd(virtual_cast(object, 0x5c) + 0x314); break;
    default: number = rd(object + 0x150); break;
    }
  }
  return push_result(recovered_vm_outer_push_integer_005d1010(vm(), nullptr, number));
}
}
