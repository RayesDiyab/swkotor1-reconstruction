#include "c24fb05a15-w2_broadcast.hpp"
#include <cstdint>
#include <cstring>
namespace w2::broadcast {
namespace {
using I = std::int32_t;
double f64(Word bits) { float f; std::memcpy(&f, &bits, 4); return static_cast<double>(f); }
float f32(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
Word bits_of(float f) { Word w; std::memcpy(&w, &f, 4); return w; }
constexpr float kHearAll = 100.0f;      // [00741838]
constexpr Word kEventDialog = 0x0a;     // [007441DC]
}

// ===================================================================== 0059D8E0 / 0059D960
void clear_alternate_0059d8e0(Memory& m, Word expression, Word n) {
    for (; n != 0; n = m.read(n + 0xc)) {
        const Word type = m.read(n + 4);
        if (type == 3) return;                                            // 0059D8F6
        if (type == 2) clear_alternate_0059d8e0(m, expression, m.read(n + 0x18));
        m.write(n, 0);
    }
}
void clear_graph_0059d960(Memory& m, Word expression) {
    for (Word n = m.read(m.read(expression + 0x14)); n != 0; n = m.read(n + 0xc)) {   // list dereferenced unchecked
        if (m.read(n + 4) == 2) clear_alternate_0059d8e0(m, expression, m.read(n + 0x18));
        m.write(n, 0);
    }
}

// ===================================================================== 0059E170 TestString
// Frame: str (lowered/copied text) at s, temporary at s+8, CExoArrayList<node*> stack {data,size,cap} at s+0x10.
Word test_string_0059e170(Memory& m, Ports& p, Word self, Word text, Word capture, Word s) {
    const Word str = s, tmp = s + 8, stk = s + 0x10;
    p.str_default_005b3190(str);
    if (m.read(self + 0x10) == 0) {                                       // 0059E19B: case-insensitive
        p.str_assign_005e5c50(str, p.str_lower_005e6080(text, tmp));
        p.str_destroy_005e5c20(tmp);
    } else {
        p.str_assign_005e5c50(str, text);
    }
    const Word list = m.read(self + 0x14);
    if (list == 0) { p.str_destroy_005e5c20(str); return 0; }            // 0059E1E3
    Word node = m.read(list);
    m.write(stk, 0); m.write(stk + 4, 0); m.write(stk + 8, 0);
    const Word sentinel = p.expr_new_node_0059d860(self, 3);            // 0059E228
    p.array_grow_005e03c0(stk, 0x10);
    auto data = [&]() { return m.read(stk); };
    auto size = [&]() { return m.read(stk + 4); };
    m.write(data() + 4 * size(), sentinel); m.write(stk + 4, size() + 1);
    Word pos = 0;                                                         // [esp+14h]
    Word top = 0;                                                         // [esp+10h]
    const Word len = p.str_length_005e5790(str);
    auto push = [&](Word n) {                                             // inline CExoArrayList::Add (grow x2 / 10h)
        if (size() == m.read(stk + 8)) { const Word cap = m.read(stk + 8); p.array_grow_005e03c0(stk, cap != 0 ? cap * 2 : 0x10); }
        m.write(data() + 4 * size(), n); m.write(stk + 4, size() + 1); ++top;
    };
    auto release_and = [&](Word freed, Word result) {                    // free popped/first node, ClearGraph, free stack
        if (freed != 0) { p.str_destroy_005e5c20(freed + 0x10); p.free_006fa390(freed); }   // 0059E3C8 / 0059E6FC
        clear_graph_0059d960(m, self);
        p.free_006fa390(data());
        p.str_destroy_005e5c20(str);
        return result;
    };
    auto success = [&]() -> Word {                                       // 0059E67D / 0059E784
        const Word first = m.read(data());
        if (first != 0) { p.str_destroy_005e5c20(first + 0x10); p.free_006fa390(first); }
        for (Word n = m.read(m.read(self + 0x14)); n != 0; n = m.read(n + 0xc)) {   // inlined ClearGraph
            if (m.read(n + 4) == 2)
                for (Word d = m.read(n + 0x18); d != 0 && m.read(d + 4) != 3; d = m.read(d + 0xc)) {
                    if (m.read(d + 4) == 2) clear_alternate_0059d8e0(m, self, m.read(d + 0x18));
                    m.write(d, 0);
                }
            m.write(n, 0);
        }
        p.free_006fa390(data());
        p.str_destroy_005e5c20(str);
        return 1;
    };
    if (node == 0) return success();
    enum Step { Visit, Advance, Fail };
    Step step = Visit;
    for (;;) {
        if (step == Advance) {                                            // 0059E372
            node = m.read(node + 0xc);
            m.write(node, 0);
            step = Visit;
        }
        if (step == Fail) {                                               // 0059E51C
            for (;;) {
                const Word popped = m.read(data() + 4 * top);
                pos = m.read(popped + 8);
                const Word n = size() - 1;
                m.write(stk + 4, n);
                for (Word k = top; static_cast<I>(k) < static_cast<I>(n); ++k) m.write(data() + 4 * k, m.read(data() + 4 * (k + 1)));
                --top;
                if (static_cast<I>(top) < 0) {                            // 0059E744
                    p.str_destroy_005e5c20(popped + 0x10); p.free_006fa390(popped);
                    clear_graph_0059d960(m, self);
                    p.free_006fa390(data());
                    p.str_destroy_005e5c20(str);
                    return 0;
                }
                if (m.read(popped + 4) != 0) { node = popped; break; }   // literals are popped through
            }
            step = Visit;
        }
        m.write(node, m.read(node) + 1);                                  // 0059E26B
        const Word type = m.read(node + 4);
        if (type > 9 || type == 3) { step = Advance; continue; }
        switch (type) {
        case 0: {                                                         // 0059E2F0 literal
            push(node);
            const Word tl = p.str_length_005e5790(node + 0x10);
            bool ok = true;
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(tl); ++k) {
                const Word a = p.str_at_005e5600(str, pos + k) & 0xffu;
                const Word b = p.str_at_005e5600(node + 0x10, k) & 0xffu;
                if (a != b) { ok = false; break; }
            }
            if (!ok) { step = Fail; break; }
            pos += tl;
            m.write(node + 8, pos);
            step = Advance;
            break;
        }
        case 1: {                                                         // 0059E388 "**"
            push(node);
            if (pos == len) {
                if (static_cast<I>(m.read(node)) > 1) return release_and(m.read(data()), 0);   // 0059E3C6
            } else if (static_cast<I>(m.read(node)) > 1) {
                ++pos;
            }
            m.write(node + 8, pos);
            step = Advance;
            break;
        }
        case 2: {                                                         // 0059E422 fork
            m.write(node + 8, pos);
            push(node);
            const Word visits = m.read(node);
            if (visits == 1) { step = Advance; break; }
            if (visits == 2) { node = m.read(node + 0x18); m.write(node, 0); step = Visit; break; }
            p.array_delete_at_006857a0(stk, top);                         // 0059E505
            --top;
            step = Fail;
            break;
        }
        case 9: {                                                         // 0059E287 end
            if (pos != len) { step = Fail; break; }
            if (capture == 0) return success();
            const Word caps = self + 4;                                   // CExoArrayList<CExoString*>
            for (Word k = 0; static_cast<I>(k) < static_cast<I>(m.read(self + 8)); ++k) {
                const Word e = m.read(m.read(self + 4) + 4 * k);
                if (e != 0) { p.str_destroy_005e5c20(e); p.free_006fa390(e); }
            }
            if (m.read(caps) != 0) { p.free_006fa390(m.read(caps)); m.write(caps, 0); m.write(caps + 8, 0); }   // 0059E2E6
            m.write(caps + 4, 0);                                         // 0059E570 (size only when data was null)
            auto append = [&](Word v) {
                if (m.read(caps + 4) == m.read(caps + 8)) { const Word cap = m.read(caps + 8); p.array_grow_005e03c0(caps, cap != 0 ? cap * 2 : 0x10); }
                m.write(m.read(caps) + 4 * m.read(caps + 4), v); m.write(caps + 4, m.read(caps + 4) + 1);
            };
            Word prev = 0;                                                // the capture argument slot, reused
            {
                const Word mem = p.allocate_006fa7e6(8);
                Word whole = 0;
                if (mem != 0) { p.str_copy_005e5b00(mem, text); whole = mem; }
                append(whole);
            }
            for (Word k = 1; static_cast<I>(k) < static_cast<I>(size()); ++k) {
                const Word n = m.read(data() + 4 * k);
                if (m.read(n + 4) == 2) continue;                         // 0059E5EB
                const Word part = m.read(n + 8) - prev;
                const Word mem = p.allocate_006fa7e6(8);
                Word sub = 0;
                if (mem != 0) { p.str_counted_005e5b70(mem, p.str_cstr_005e5670(text) + prev, part); sub = mem; }
                append(sub);
                prev = m.read(m.read(data() + 4 * k) + 8);
            }
            return success();
        }
        default: {                                                        // 4..8 character classes (0059E483)
            if (pos == len) return release_and(m.read(data()), 0);       // 0059E6F6
            p.array_add_00671c00(stk, node);
            ++top;
            const I c = static_cast<std::int8_t>(p.str_at_005e5600(str, pos) & 0xffu);
            bool match = false;
            if (type == 6) match = c >= 'a' && c <= 'z';
            if (!match && type == 7) match = c >= '0' && c <= '9';
            if (!match && type == 8) match = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
            if (!match && type == 4) match = c == ' ';
            if (!match && type == 5) match = c == '.' || c == '?' || c == '!';
            if (match) { ++pos; m.write(node + 8, pos); step = Advance; break; }
            p.array_delete_at_006857a0(stk, top);
            --top;
            step = Fail;
            break;
        }
        }
    }
}

// ===================================================================== 004CD700
Word test_listen_expression_004cd700(Memory& m, Ports& p, Word self, Word text, Word s) {
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(self + 0x190)); ++i) {
        const Word e = m.read(m.read(self + 0x18c) + 4 * i);
        if (e == 0) continue;
        if (test_string_0059e170(m, p, e, text, 1, s) != 0) return m.read(m.read(m.read(self + 0x18c) + 4 * i));
    }
    return 0xffffffffu;
}

// ===================================================================== 004CD760 BroadcastDialog
// Frame locals (orig): +0C object, +10 index, +14 area, +18 area game object, +1C self, +20 object array,
// +24 distance^2 (float), +28 object id, +2C..+34 object position copy, +38..+40 difference vector.
void broadcast_dialog_004cd760(Memory& m, Ports& p, Word self, Word text, Word range_bits, Word flag, Word s) {
    const Word obj_out = s + 0x0c, idx = s + 0x10, area_go = s + 0x18, opos = s + 0x2c, diff = s + 0x38;
    m.write(idx, 0xffffffffu); m.write(obj_out, 0); m.write(area_go, 0);
    const Word server = m.read(p.app_007a39fc() + 8);
    (void)p.game_object_004d8230(p.object_array_004aed70(server), m.read(self + 0x8c), area_go);
    const Word area = m.read(area_go) != 0 ? p.vcall0(m.read(area_go), 0x28) : 0;
    const Word objects = p.object_array_004aed70(server);
    if (area == 0) return;
    p.area_first_index_00506f20(area, idx, bits_of(static_cast<float>(f64(m.read(self + 0x90)) - f64(range_bits))));
    if (static_cast<I>(m.read(idx)) < 0) return;
    const double range = f64(range_bits);
    while (static_cast<I>(m.read(idx)) < static_cast<I>(m.read(area + 0x194))) {   // 004CD810
        const Word i = m.read(idx);
        m.write(idx, i + 1);
        const Word id = m.read(m.read(area + 0x190) + 4 * i);
        if ((p.game_object_004d8230(objects, id, obj_out) & 0xffu) != 0) return;   // [0074666C] = 0
        const Word o = m.read(obj_out);
        if (o == 0 || id == m.read(self + 4)) continue;
        m.write(opos, m.read(o + 0x90)); m.write(opos + 4, m.read(o + 0x94)); m.write(opos + 8, m.read(o + 0x98));
        const double reach = range + f64(m.read(self + 0x90));
        const double ox = f64(m.read(opos));
        if (reach < ox) return;                                           // 004CD893: sorted by x, done (ordered only)
        const double dz = f64(m.read(self + 0x98)) - f64(m.read(opos + 8));
        const double dy = f64(m.read(self + 0x94)) - f64(m.read(opos + 4));
        const double dx = f64(m.read(self + 0x90)) - ox;
        const double d2 = (dx * dx + dy * dy) + dz * dz;
        const float d2f = static_cast<float>(d2);
        const double r2 = range * range;
        if (!(d2 <= r2)) continue;                                        // 004CD8D8 (unordered skips)
        if (m.read(o + 0x19c) == 0) continue;
        bool hear = false;
        if (p.vcall0(o, 0x30) != 0 && p.vcall0(self, 0x30) != 0) {
            const Word e = p.visible_element_00517a20(p.vcall0(o, 0x30), m.read(self + 4));
            if (e == 0) continue;
            const Word fl = m.read8(e + 4);
            if ((fl & 2u) == 0) continue;
            if (flag != 0 && (fl & 1u) == 0 && !(d2f < kHearAll)) continue;   // 004CD93B
            hear = true;
        } else if (p.vcall0(o, 0x30) == 0 || p.vcall0(self, 0x30) != 0) {
            hear = true;
        } else {                                                          // listener is a creature, speaker is not
            p.vector_sub_00423bc0(diff, self + 0x90, opos);
            const double ld = f64(p.listen_distance_004efd20(p.vcall0(o, 0x30)));
            const double x = f64(m.read(diff)), y = f64(m.read(diff + 4)), z = f64(m.read(diff + 8));
            const double e2 = (z * z + y * y) + x * x;
            if (ld * ld < e2) continue;                                   // 004CD9B1 (unordered proceeds)
            hear = true;
        }
        (void)hear;
        const Word match = test_listen_expression_004cd700(m, p, o, text, s + 0x50);
        if (static_cast<I>(match) < 0) continue;
        const Word mem = p.allocate_006fa7e6(0x34);
        const Word ev = mem != 0 ? p.event_construct_004d7540(mem) : 0;
        if (ev == 0) p.trap(0x004cd9f2, "W2: BroadcastDialog event allocation failed (original writes through null)");
        m.write16(ev, 7);                                                 // event type
        p.event_set_integer_004d76b0(ev, 0, m.read(p.listen_expression_004cd6c0(o, match) + 8));
        p.event_set_integer_004d76b0(ev, 1, match);
        for (Word k = 1; static_cast<I>(k) < static_cast<I>(m.read(p.listen_expression_004cd6c0(o, match) + 8)); ++k)
            p.event_set_string_004d6ae0(ev, k, m.read(m.read(p.listen_expression_004cd6c0(o, match) + 4) + 4 * k));
        p.add_event_004b08d0(p.ai_master_004aed80(server), 0, 0, m.read(self + 4), id, kEventDialog, ev);
    }
}
}
