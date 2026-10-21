#pragma once
// W2 t6 (fork G): CSWSObject::BroadcastDialog 004CD760 and its listen-pattern match chain.
//   004CD760 CSWSObject::BroadcastDialog(CExoString const& text, float range, int flag) RET 0Ch (no EAX contract)
//   004CD700 CSWSObject::TestListenExpression(CExoString const& text) RET 4 -> matching expression number or -1
//   0059E170 CSWSExpression::TestString(CExoString const& text, int capture) RET 8 -> 1 match / 0
//   0059D8E0 CSWSExpression::ClearAlternate(node) RET 4,  0059D960 CSWSExpression::ClearGraph() RET 0
// Layouts (shared with W7's parser, lane-w7 vm_actions_t1.cpp):
//   CSWSExpression {+0 number, +4/+8/+C CExoArrayList<CExoString*> captures, +10 case flag, +14 CSWSExpressionList*, +18 text}
//   CSWSExpressionList {+0 head, +4 cursor, +8 last}
//   CSWSExpressionNode 0x1C {+0 visit count, +4 type, +8 position, +C next, +10 CExoString, +18 alternate}
//   node types: 0 literal, 1 "**", 2 fork (alt chain at +18), 3 join, 4 *w ' ', 5 *p .?!, 6 *a a-z, 7 *n 0-9, 8 *A a-z0-9, 9 end
//   CSWSObject: +4 id, +8C area id, +90 position, +18C/+190 listen expressions (CExoArrayList<CSWSExpression*>), +19C listening
//   CSWSArea: +190/+194 object ids sorted by x.
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::broadcast {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    // BroadcastDialog
    virtual Word object_array_004aed70(Word server) = 0;
    virtual Word game_object_004d8230(Word array, Word id, Word out) = 0;          // AL status (0 = found)
    virtual Word vcall0(Word object, Word slot) = 0;                               // 28h AsArea, 30h AsCreature
    virtual void area_first_index_00506f20(Word area, Word out, Word x_bits) = 0;
    virtual Word visible_element_00517a20(Word creature, Word id) = 0;
    virtual void vector_sub_00423bc0(Word out, Word a, Word b) = 0;                // cdecl out = a - b
    virtual Word listen_distance_004efd20(Word creature) = 0;                      // float bits (ST0)
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual void free_006fa390(Word block) = 0;
    virtual Word event_construct_004d7540(Word storage) = 0;
    virtual Word listen_expression_004cd6c0(Word object, Word number) = 0;         // W7
    virtual void event_set_integer_004d76b0(Word event, Word index, Word value) = 0;
    virtual void event_set_string_004d6ae0(Word event, Word index, Word string) = 0;
    virtual Word ai_master_004aed80(Word server) = 0;
    virtual void add_event_004b08d0(Word master, Word a, Word b, Word caller, Word target, Word type, Word event) = 0;
    // TestString
    virtual void str_default_005b3190(Word str) = 0;
    virtual Word str_lower_005e6080(Word src, Word out) = 0;
    virtual void str_assign_005e5c50(Word dst, Word src) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual Word str_length_005e5790(Word str) = 0;
    virtual Word str_at_005e5600(Word str, Word index) = 0;                        // AL
    virtual void str_copy_005e5b00(Word dst, Word src) = 0;
    virtual void str_counted_005e5b70(Word dst, Word text, Word length) = 0;
    virtual Word str_cstr_005e5670(Word str) = 0;
    virtual Word expr_new_node_0059d860(Word expression, Word type) = 0;           // W7
    virtual void array_grow_005e03c0(Word list, Word capacity) = 0;
    virtual void array_add_00671c00(Word list, Word value) = 0;
    virtual void array_delete_at_006857a0(Word list, Word index) = 0;
};

// s: >= 0x70 bytes scratch (frame locals at s, TestString scratch at s + 0x50).
void broadcast_dialog_004cd760(Memory&, Ports&, Word self, Word text, Word range_bits, Word flag, Word s);
// s: >= 0x20 bytes scratch (TestString's).
Word test_listen_expression_004cd700(Memory&, Ports&, Word self, Word text, Word s);
// s: >= 0x20 bytes scratch (lowered text CExoString, temporary, frame CExoArrayList).
Word test_string_0059e170(Memory&, Ports&, Word expression, Word text, Word capture, Word s);
void clear_alternate_0059d8e0(Memory&, Word expression, Word node);
void clear_graph_0059d960(Memory&, Word expression);
}
