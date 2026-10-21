// KOTOR 1 reconstruction — original 00687E40
// CSWGuiMainInterface::InitializeMap
//
// Original body: 00687E40..00687FCD (0x18D), thiscall, ECX=this, no stack
// arguments, RET 0. SEH handler 0072F420; two CExoString locals (module name
// first, formatted map name second); one transient CResRef whose 16-byte stack
// slot is reused for the label extent.
//
// This TU is a replacement body only. The current selected candidate maps the
// original name @kotor_m1_opt_main_interface_initialize_map_00687e40@8 to a
// production trap at 00728200 (0804-m1-opt-feedback.obj:
// push 0x83D700; push 0x687E40; call _kotor_unreachable_trap 007157D0; int3).
// Integration must remove/rebind that trap and assign this body to 00687E40.
// No existing provider, trap or shared header is edited.
//
// Faithfulness notes:
// - App manager is the selected real storage cell
//   kotor_menu_window_cells::manager_007a39fc (original [007A39FC]).
// - GetModule is the selected real adapter server_get_module_004ae6b0
//   (original 004AE6B0, selected 006BB920). It is called with ECX=[app+8].
// - GetClientOptions is the selected real body kotor_fastcall_005ed700
//   (original 005ED700, selected 00617750). It is called with ECX=[app+4].
// - module+0x30 CResRef is copied to the first CExoString by the selected real
//   c2da_ref_to_string_00405f70 (original 00405F70, selected 0062A960,
//   RET 4).
// - The format string is the exact "lbl_map%s" (original 0x747774) and the
//   formatter is the selected real kotor_menu_10_19::dependencies::
//   format_005e5680 (original 005E5680, selected 00625CC0, cdecl variadic).
// - map = [module+0x218]. The count at [map+4] is tested with `cmp ...,0; jbe`,
//   i.e. a zero-only test. A negative/nonzero count enters the fill/extent block.
// - The transient CResRef is built by the selected real
//   async_ref_construct_string_00406d60 (original 00406D60, selected 0062A450,
//   RET 4). It is passed to the selected real
//   gui_load_border_params_set_fill_00414c00 (original 00414C00, selected
//   006321D0, RET 8), then the same 16-byte stack slot is overwritten with
//   {0,0,width,height}. The union below preserves that slot reuse and order.
// - Border params are at this+0x5E70 (LBL_MAP border+0x14). The original clears
//   bits 0-1 of params+0x1C (fill style) after SetFillImage.
// - The scaled extent preserves the original ordered x87 sequences:
//     fild [this+0x6094]; fmul [map+0x14]; genuine ST0->EAX  (height)
//     fild [this+0x6090]; fmul [map+0x14]; genuine ST0->EAX  (width)
//   then {0,0,width,height} is written over the CResRef slot. Source calls the
//   selected `_genuine_x87_ftol_006fae8c` directly through a local PC53
//   double-store/reload seam; there is no binary32 narrowing or width-first
//   evaluation. This TU MUST be built with `/arch:IA32`, precise FP and `/EHs`;
//   do not use `__ftol2_sse`/SSE cvttss2si or bind 006FAE8C itself.
// - SetExtent is the original label vtable slot +4, called with ECX=this+0x5E00
//   and one Extent* stack argument. The selected label vtable 0082CBD8 has
//   +4 = 006332D0 (real).
// - Options flags: if options != null, bit1 of this+0x5D04 (LBL_MAPBORDER+0x44)
//   and this+0x5F84 (LBL_ARROW+0x44) is set iff the map block ran and
//   (options+0x14 & 8) != 0; otherwise it is cleared. The original uses the
//   xor/and/2 idiom, reproduced below.
// - Cleanup: first CExoString (map name) then second (module name); the two
//   ScopedString objects below produce that reverse declaration-order cleanup
//   on both normal exit and unwind (the original's EH states 0 then 1). The
//   transient CResRef has no destructor and is not destroyed.
// - Null module/map/label pointers fault as original; no guards are added.
//
// No original absolute address is dereferenced. All external names are selected
// real storage/function symbols or the replacement body's own name.

#include <cstddef>
#include <cstdint>
#include <cstring>

// Selected genuine conversion: operand in ST0, no stack args, pops ST0, low
// dword in EAX. The C identifier intentionally has no leading underscore.
extern "C" int __cdecl genuine_x87_ftol_006fae8c();

namespace {

using U32 = std::uint32_t;
using I32 = std::int32_t;

// Object-relative field access. Integer address arithmetic preserves the
// original's unchecked field faults (null module/map/label) instead of relying
// on C++ pointer arithmetic on invalid pointers. No absolute address is used.
template <class T>
T load_at(const void* base, std::ptrdiff_t offset) {
    const std::intptr_t address =
        static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(base)) + offset;
    T value;
    std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(T));
    return value;
}

template <class T>
void store_at(void* base, std::ptrdiff_t offset, T value) {
    const std::intptr_t address =
        static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(base)) + offset;
    std::memcpy(reinterpret_cast<void*>(address), &value, sizeof(T));
}

}  // namespace

// Selected real app-manager storage (original [007A39FC]; 0053-window-shared-
// cells.obj, selected 0095236C). The selected symbol is
// ?manager_007a39fc@kotor_menu_window_cells@@3RAXA; this `void* volatile`
// declaration matches the selected source declaration and its mangling.
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

// Selected real CExoString / CResRef interface (original 0012-startup-strings,
// 0200-ref-bodies, 0125-canonical-assignment-format). These declarations mirror
// the selected startup_views.h declarations; the member/namespace names produce
// the exact selected decorated symbols. They are declarations only; the bodies
// are the selected real providers.
namespace kotor_menu_10_19 {

struct String {
    alignas(4) unsigned char bytes[8];
    String* __thiscall default_005b3190();   // 00615F80, real
    const char* __thiscall text_005e5670();  // 0061EFA0, real
    void __thiscall destroy_005e5c20();      // 00615F90, real
};

struct ResRef {
    alignas(4) unsigned char bytes[16];
};

namespace dependencies {
// 00625CC0, real cdecl variadic formatter.
void __cdecl format_005e5680(String* destination, const char* format, ...);
}

}  // namespace kotor_menu_10_19

static_assert(sizeof(kotor_menu_10_19::String) == 8,
              "CExoString is the selected 8-byte object");
static_assert(sizeof(kotor_menu_10_19::ResRef) == 16,
              "CResRef is the selected 16-byte object");

namespace {

struct Extent {
    I32 left;
    I32 top;
    I32 width;
    I32 height;
};
static_assert(sizeof(Extent) == 16, "CSWGuiExtent is four dwords");

// The original CResRef at E+0x24 is overwritten by the extent at E+0x24.
// One union preserves that exact stack-slot reuse and the build/use order.
union RefOrExtent {
    kotor_menu_10_19::ResRef fill;
    Extent extent;
};
static_assert(sizeof(RefOrExtent) == 16, "transient ref/extent slot is 16 bytes");

// Two real CExoString locals with the selected real destructor. Declaration
// order is module name first (original E+0x1C) then map name (E+0x14); the
// compiler destroys map name then module name on both normal exit and unwind,
// matching the original cleanup order. The compiler's own EH states provide the
// original two-stage unwind; no manual state variable is used.
struct ScopedString {
    kotor_menu_10_19::String value;
    ScopedString();
    ~ScopedString();
    ScopedString(const ScopedString&) = delete;
    ScopedString& operator=(const ScopedString&) = delete;
};

__declspec(noinline) ScopedString::ScopedString() {
    value.default_005b3190();  // 00615F80, real
}

__declspec(noinline) ScopedString::~ScopedString() {
    value.destroy_005e5c20();  // 00615F90, real
}

// Accepted local thin ABI seam. The binary64/PC53 product is stored once and
// reloaded to ST0, then the selected genuine ST0-consuming conversion is called;
// there is no binary32 narrowing and no compiler-chosen __ftol2_sse path.
int ftol2_X87(double value) {
    int result;
    volatile double v = value;
    __asm {
        fld qword ptr [v]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

using SetExtentFn = void (__fastcall*)(void* label, void* unused_edx,
                                       const Extent* extent);

}  // namespace

// Selected real providers. Each plain C name is the lane's established import
// name; the integration alias file maps it to the selected decorated symbol.
extern "C" void* __fastcall server_get_module_004ae6b0(void* server,
                                                      void* unused_edx);
extern "C" void* __fastcall kotor_fastcall_005ed700(void* app,
                                                    void* unused_edx);
extern "C" void __fastcall c2da_ref_to_string_00405f70(void* reference,
                                                       void* unused_edx,
                                                       void* out_string);
extern "C" void* __fastcall async_ref_construct_string_00406d60(
    void* reference, void* unused_edx, const void* string);
extern "C" void __fastcall gui_load_border_params_set_fill_00414c00(
    void* border_params, void* unused_edx, const void* reference, int force);

extern "C" __declspec(noinline) void __fastcall kotor_fastcall_00687e40(
    void* self_, void* /*unused_edx*/) {
    unsigned char* const self = static_cast<unsigned char*>(self_);

    // [007A39FC] is the selected real manager cell; [app+8] is the server.
    void* const app = kotor_menu_window_cells::manager_007a39fc;
    void* const module =
        server_get_module_004ae6b0(load_at<void*>(app, 0x08), nullptr);

    // Original B0: first CExoString at E+0x1C, second at E+0x14, then EH state 1.
    ScopedString module_name;
    ScopedString map_name;

    // Original B1: module+0x30 CResRef -> module_name; CStr; Format map_name.
    c2da_ref_to_string_00405f70(
        reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(module) + 0x30),
        nullptr, &module_name.value);
    kotor_menu_10_19::dependencies::format_005e5680(
        &map_name.value, "lbl_map%s", module_name.value.text_005e5670());

    // Original B2: map=[module+0x218]; zero-only [map+4] gate.
    bool map_built = false;
    void* const map = load_at<void*>(module, 0x218);
    if (load_at<U32>(map, 0x04) != 0) {
        // Original B3: scale, transient CResRef, SetFillImage, clear fill bits,
        // x87 extent, label SetExtent.
        const float scale = load_at<float>(map, 0x14);

        RefOrExtent scratch;
        async_ref_construct_string_00406d60(&scratch.fill, nullptr,
                                            &map_name.value);

        void* const border_params = self + 0x5E70;
        gui_load_border_params_set_fill_00414c00(border_params, nullptr,
                                                 &scratch.fill, 0);

        // and dword [params+0x1C], 0xFFFFFFFC
        U32 fill_flags = load_at<U32>(border_params, 0x1C);
        fill_flags &= ~3u;
        store_at<U32>(border_params, 0x1C, fill_flags);

        // Original order: height self+6094 -> binary32 map scale -> genuine
        // ST0 conversion completes before width self+6090 is loaded. The signed
        // integer and product are not narrowed to binary32; the accepted local
        // seam preserves the PC53 value through one double store/reload.
        const I32 height = ftol2_X87(
            static_cast<double>(load_at<I32>(self, 0x6094)) *
            static_cast<double>(scale));
        const I32 width = ftol2_X87(
            static_cast<double>(load_at<I32>(self, 0x6090)) *
            static_cast<double>(scale));

        // Overwrite the same 16-byte slot that held the CResRef.
        scratch.extent = Extent{0, 0, width, height};

        // Original B3 tail: label at this+0x5E00, vtable slot +4.
        void* const label = self + 0x5E00;
        void* const label_vtable = load_at<void*>(label, 0);
        const SetExtentFn set_extent = reinterpret_cast<SetExtentFn>(
            load_at<void*>(label_vtable, 0x04));
        // F8: the original leaves EDX=&extent at the 00687F23..F30 call.
        set_extent(label, &scratch.extent, &scratch.extent);
        map_built = true;
    }

    // Original B4: options gate and bit1 update for LBL_MAPBORDER/LBL_ARROW.
    void* const options =
        kotor_fastcall_005ed700(load_at<void*>(app, 0x04), nullptr);
    if (options != nullptr) {
        const U32 visible =
            (map_built &&
             (load_at<unsigned char>(options, 0x14) & 8u) != 0)
                ? 1u
                : 0u;

        // Original xor/and/2 idiom, first [this+0x5D04].
        U32 bit = visible;
        bit += bit;
        U32 old = load_at<U32>(self, 0x5D04);
        bit ^= old;
        bit &= 2u;
        old ^= bit;
        store_at<U32>(self, 0x5D04, old);

        // Same idiom, [this+0x5F84].
        bit = visible;
        bit += bit;
        old = load_at<U32>(self, 0x5F84);
        bit ^= old;
        bit &= 2u;
        old ^= bit;
        store_at<U32>(self, 0x5F84, old);
    }

    // ScopedString destruction order: map_name then module_name.
}

// Integration/rebind notes:
// - Target original: 00687E40, thiscall RET 0 (lane name @...@8).
// - Selected replacement source name: kotor_fastcall_00687e40.
// - Exact integrator export/binding: @kotor_fastcall_00687e40@8 =
//   _kotor_fastcall_00687e40 (the lane export generator creates the decorated
//   name from this C definition; if using alternatename directly, alias
//   @kotor_fastcall_00687e40@8 to _kotor_fastcall_00687e40).
// - Remove/rebind the current 00687E40 production trap
//   (@kotor_m1_opt_main_interface_initialize_map_00687e40@8 at 00728200) and
//   bind this body to 00687E40. The trap's symbol is a different name, so the
//   planned @kotor_fastcall_00687e40@8 export does not duplicate it; still do
//   not link both bodies at the same address.
// - Imports (selected real): manager_007a39fc (0095236C);
//   server_get_module_004ae6b0 (006BB920); kotor_fastcall_005ed700 (00617750);
//   c2da_ref_to_string_00405f70 (0062A960); String::default_005b3190
//   (00615F80); String::text_005e5670 (0061EFA0); String::destroy_005e5c20
//   (00615F90); format_005e5680 (00625CC0); async_ref_construct_string_00406d60
//   (0062A450); gui_load_border_params_set_fill_00414c00 (006321D0); label
//   vtable 0082CBD8 slot +4 -> 006332D0.
// - Build: compile this TU with x87/PC53 (`/arch:IA32`, precise FP, `/EHs`) and
//   let the local seam call `_genuine_x87_ftol_006fae8c` (0206/0064 genuine
//   owners) directly. Do not bind 006FAE8C itself, use `__ftol2_sse`/an SSE2
//   conversion, add FP-state control, or invent another provider/alias object.
// - No original bytes, no absolute-address ports, no stub/trap/guard, no
//   fabricated success/failure values.
