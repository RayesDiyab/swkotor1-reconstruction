// W2 t6 (fork G) native binding + exports: BroadcastDialog 004CD760, TestListenExpression 004CD700,
// CSWSExpression::TestString 0059E170, ClearAlternate 0059D8E0, ClearGraph 0059D960.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "c24fb05a15-w2_broadcast.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); }
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void* __fastcall recovered_object_array_004aed70(void*, void*);                                              // W2
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, unsigned, void**);                           // CGameObjectArray::GetGameObject
unsigned __fastcall recovered_area_index_00506f20(void*, void*, unsigned*, unsigned);                       // CSWSArea::GetFirstObjectIndiceByX (float bits)
void* __fastcall recovered_creature_visible_element_00517a20(void*, void*, unsigned);
float* __cdecl kotor_cdecl_00423bc0(float*, const float*, const float*);                                    // Vector operator- (out = a - b)
float __fastcall recovered_creature_listen_distance_004efd20(void*, void*);
void* __fastcall recovered_event_construct_004d7540(void*, void*);
void* __fastcall recovered_object_get_listen_expression_004cd6c0(void*, void*, int);                       // W7
void __fastcall recovered_event_integer_004d76b0(void*, void*, unsigned, unsigned);
void __fastcall recovered_event_string_set_004d6ae0(void*, void*, unsigned, void*);
unsigned __fastcall recovered_event_enqueue_004b08d0(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, void*);
void* __fastcall lower_construct_005e6080(void*, void*, void*);                                            // CExoString::LowerCase(out)
void* __fastcall async_assign_string_005e5c50(void*, void*, void*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
int __fastcall async_string_length_005e5790(void*, void*);
char __fastcall c2da_current_string_index_005e5600(void*, void*, int);
void* __fastcall WindowCopy005e5b00(void*, void*, const void*);
void* __fastcall resman_string_counted_005e5b70(void*, void*, const char*, int);
char* __fastcall async_string_cstr_005e5670(void*, void*);
void* __fastcall recovered_expression_new_node_0059d860(void*, void*, int);                                // W7
void __fastcall gui_load_array_add_00671c00(void*, void*, unsigned);
void __fastcall deleteAt_006857a0(void*, void*, int);
}
namespace {
using w2::broadcast::Word;
inline void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct Mem final : kotor::server_mainloop::Memory {
    Word read(Word a) override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    Word read16(Word a) override { std::uint16_t v; std::memcpy(&v, ptr(a), 2); return v; }
    void write16(Word a, Word v) override { auto x = static_cast<std::uint16_t>(v); std::memcpy(ptr(a), &x, 2); }
    Word read8(Word a) override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
};
struct Native final : w2::broadcast::Ports {
    [[noreturn]] void trap(Word a, const char* w) override { kotor_unreachable_trap(a, w); }
    Word app_007a39fc() override { return word(kotor_menu_window_cells::manager_007a39fc); }
    Word object_array_004aed70(Word s) override { return word(recovered_object_array_004aed70(ptr(s), nullptr)); }
    Word game_object_004d8230(Word a, Word id, Word out) override { return kotor_fastcall_004d8230(ptr(a), nullptr, id, static_cast<void**>(ptr(out))); }
    Word vcall0(Word o, Word slot) override { using F = void*(__thiscall*)(void*); Word vt; std::memcpy(&vt, ptr(o), 4); Word f; std::memcpy(&f, ptr(vt + slot), 4); return word(reinterpret_cast<F>(ptr(f))(ptr(o))); }
    void area_first_index_00506f20(Word area, Word out, Word x) override { (void)recovered_area_index_00506f20(ptr(area), nullptr, static_cast<unsigned*>(ptr(out)), x); }
    Word visible_element_00517a20(Word c, Word id) override { return word(recovered_creature_visible_element_00517a20(ptr(c), nullptr, id)); }
    void vector_sub_00423bc0(Word out, Word a, Word b) override { (void)kotor_cdecl_00423bc0(static_cast<float*>(ptr(out)), static_cast<const float*>(ptr(a)), static_cast<const float*>(ptr(b))); }
    Word listen_distance_004efd20(Word c) override { const float d = recovered_creature_listen_distance_004efd20(ptr(c), nullptr); Word b; std::memcpy(&b, &d, 4); return b; }
    Word allocate_006fa7e6(Word n) override { return word(live_operator_new(n)); }
    void free_006fa390(Word b) override { live_free(ptr(b)); }
    Word event_construct_004d7540(Word st) override { return word(recovered_event_construct_004d7540(ptr(st), nullptr)); }
    Word listen_expression_004cd6c0(Word o, Word n) override { return word(recovered_object_get_listen_expression_004cd6c0(ptr(o), nullptr, static_cast<int>(n))); }
    void event_set_integer_004d76b0(Word e, Word i, Word v) override { recovered_event_integer_004d76b0(ptr(e), nullptr, i, v); }
    void event_set_string_004d6ae0(Word e, Word i, Word s) override { recovered_event_string_set_004d6ae0(ptr(e), nullptr, i, ptr(s)); }
    Word ai_master_004aed80(Word s) override { Mem m; return m.read(m.read(s + 4) + 0x10044); }   // 004AED80: [[this+4]+10044h] (no free export)
    void add_event_004b08d0(Word ms, Word a, Word b, Word c, Word t, Word ty, Word e) override { (void)recovered_event_enqueue_004b08d0(ptr(ms), nullptr, a, b, c, t, ty, ptr(e)); }
    void str_default_005b3190(Word s) override { (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(ptr(s)), nullptr); }
    Word str_lower_005e6080(Word src, Word out) override { return word(lower_construct_005e6080(ptr(src), nullptr, ptr(out))); }
    void str_assign_005e5c50(Word d, Word s) override { (void)async_assign_string_005e5c50(ptr(d), nullptr, ptr(s)); }
    void str_destroy_005e5c20(Word s) override { frame_LogStringDtor_005e5c20(ptr(s), nullptr); }
    Word str_length_005e5790(Word s) override { return static_cast<Word>(async_string_length_005e5790(ptr(s), nullptr)); }
    Word str_at_005e5600(Word s, Word i) override { return static_cast<unsigned char>(c2da_current_string_index_005e5600(ptr(s), nullptr, static_cast<int>(i))); }
    void str_copy_005e5b00(Word d, Word s) override { (void)WindowCopy005e5b00(ptr(d), nullptr, ptr(s)); }
    void str_counted_005e5b70(Word d, Word t, Word n) override { (void)resman_string_counted_005e5b70(ptr(d), nullptr, static_cast<const char*>(ptr(t)), static_cast<int>(n)); }
    Word str_cstr_005e5670(Word s) override { return word(async_string_cstr_005e5670(ptr(s), nullptr)); }
    Word expr_new_node_0059d860(Word e, Word t) override { return word(recovered_expression_new_node_0059d860(ptr(e), nullptr, static_cast<int>(t))); }
    void array_grow_005e03c0(Word l, Word n) override { menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(l)), nullptr, n); }
    void array_add_00671c00(Word l, Word v) override { gui_load_array_add_00671c00(ptr(l), nullptr, v); }
    void array_delete_at_006857a0(Word l, Word i) override { deleteAt_006857a0(ptr(l), nullptr, static_cast<int>(i)); }
};
}
namespace b = w2::broadcast;
extern "C" {
void __fastcall recovered_object_broadcast_dialog_004cd760(void* self, void*, void* text, float range, int flag) {
    Mem m; Native p; alignas(8) unsigned char s[0x80] = {};
    Word bits; std::memcpy(&bits, &range, 4);
    b::broadcast_dialog_004cd760(m, p, word(self), word(text), bits, static_cast<Word>(flag), word(s));
}
int __fastcall recovered_object_test_listen_expression_004cd700(void* self, void*, void* text) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {};
    return static_cast<int>(b::test_listen_expression_004cd700(m, p, word(self), word(text), word(s)));
}
int __fastcall recovered_expression_test_string_0059e170(void* expression, void*, void* text, int capture) {
    Mem m; Native p; alignas(8) unsigned char s[0x20] = {};
    return static_cast<int>(b::test_string_0059e170(m, p, word(expression), word(text), static_cast<Word>(capture), word(s)));
}
void __fastcall recovered_expression_clear_alternate_0059d8e0(void* expression, void*, void* node) { Mem m; b::clear_alternate_0059d8e0(m, word(expression), word(node)); }
void __fastcall recovered_expression_clear_graph_0059d960(void* expression, void*) { Mem m; b::clear_graph_0059d960(m, word(expression)); }
}
