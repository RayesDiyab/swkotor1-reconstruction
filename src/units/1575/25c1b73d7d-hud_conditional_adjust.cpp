// A5 private reconstruction of original 006851B0
// CSWGuiMainInterfaceChar::Adjust — thiscall(float), RET 4.
//
// New TU only. No shared header, vtable table, provider or Draw TU is edited.
// Original span 006851B0..00685330. Selected vtable slots:
//   this+0x28 + i*0x140 (6 labels)   : +04 -> 00417780
//   this+0x7A8 + i*0x150 (2 progress): +04 -> 00419300
//   this+0xCC8 (CSWGuiCharButton)    : +04 -> 00417A20
// The x87 block mirrors the original: k = 1 - delta*4 is carried in ST1 across
// both original _ftol2 conversions; the selected _ftol2 body is the real
// 0062ACF0 -> 0061DC20 original-protocol converter.

#include <cstdint>
#include <cstring>

namespace {

struct Extent {
    std::uint32_t v[4];
};

using SetExtentFn = void (__fastcall*)(void*, void*, const Extent*);

inline void set_extent(void* object, const Extent* extent) {
    void** vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<SetExtentFn>(vtable[1])(object, nullptr, extent);
}

std::int16_t read_s16(const std::uint8_t* p) {
    std::int16_t value;
    std::memcpy(&value, p, sizeof(value));
    return value;
}

}  // namespace

extern "C" void __cdecl genuine_x87_ftol_006fae8c();

extern "C" void __fastcall kotor_fastcall_006851b0(void* self, void*, float delta) {
    std::uint8_t* base = static_cast<std::uint8_t*>(self);

    Extent bar;
    std::memcpy(&bar, base + 0x7AC, sizeof(bar));

    const int e9a = read_s16(base + 0xE9A);
    const int e9e = read_s16(base + 0xE9E);
    const int e98 = read_s16(base + 0xE98);
    const int e9c = read_s16(base + 0xE9C);

    // Keep the operand values in memory for the inline-x87 block.
    volatile float delta_v = delta;
    volatile float k_four = 4.0f;
    volatile float k_one = 1.0f;
    volatile int diff_x = e9e - e9a;
    volatile int diff_y = e9c - e98;
    volatile int left_word = e9a;
    volatile int top_word = e98;
    volatile int bar_left = static_cast<int>(bar.v[0]);
    volatile int bar_top = static_cast<int>(bar.v[1]);
    // Do not name these dx/dy: DX is a reserved 16-bit register name in __asm.
    volatile int adjust_x = 0;
    volatile int adjust_y = 0;
    // The selected _ftol2 body clobbers EAX/ECX/EDX. Force a stack reload of the
    // object pointer after the inline call instead of trusting register liveness.
    void* volatile self_reload = self;

    __asm {
        fld dword ptr [delta_v]
        fmul dword ptr [k_four]
        fsubr dword ptr [k_one]
        fild dword ptr [diff_x]
        fmul st(0), st(1)
        fiadd dword ptr [left_word]
        fisub dword ptr [bar_left]
        call genuine_x87_ftol_006fae8c
        mov [adjust_x], eax
        fild dword ptr [diff_y]
        fmul st(0), st(1)
        fiadd dword ptr [top_word]
        fisub dword ptr [bar_top]
        call genuine_x87_ftol_006fae8c
        mov [adjust_y], eax
        fstp st(0)
    }

    const int xadj = adjust_x;
    const int yadj = adjust_y;
    std::uint8_t* base_after = static_cast<std::uint8_t*>(self_reload);

    Extent e;
    for (int i = 0; i < 6; ++i) {
        void* child = base_after + 0x28 + i * 0x140;
        std::memcpy(&e, static_cast<std::uint8_t*>(child) + 4, sizeof(e));
        e.v[0] += static_cast<std::uint32_t>(xadj);
        e.v[1] += static_cast<std::uint32_t>(yadj);
        set_extent(child, &e);
    }

    for (int i = 0; i < 2; ++i) {
        void* child = base_after + 0x7A8 + i * 0x150;
        std::memcpy(&e, static_cast<std::uint8_t*>(child) + 4, sizeof(e));
        e.v[0] += static_cast<std::uint32_t>(xadj);
        e.v[1] += static_cast<std::uint32_t>(yadj);
        set_extent(child, &e);
    }

    void* button = base_after + 0xCC8;
    std::memcpy(&e, static_cast<std::uint8_t*>(button) + 4, sizeof(e));
    e.v[0] += static_cast<std::uint32_t>(xadj);
    e.v[1] += static_cast<std::uint32_t>(yadj);
    set_extent(button, &e);
}
