// 00647C10 CSWCCreatureStats::CheckCustomPortrait
//
// Source-faithful reconstruction of swkotor.exe 00647C10..00647CDF.
// Derived from the oracle bytes (SHA-256
// 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435):
//
//   647C10  push -1 / push 72A338 / fs:[0] prologue
//   647C25  sub esp,0x20
//   647C28  push esi
//   647C29  lea ecx,[esp+4]                 ; local CSWPortrait A
//   647C2D  call 00678EC0                   ; A = CSWPortrait()
//   647C32  mov esi,[esp+0x34]              ; esi = arg1 (portrait_out)
//   647C36  push esi                        ; source
//   647C37  lea eax,[esp+0x18]              ; local CResRef B
//   647C3B  push eax                        ; out
//   647C3C  lea ecx,[esp+0xc]               ; dest = A
//   647C48  call 00406120                   ; A = lower16(*portrait_out), B = A
//   647C4D  push 0                          ; staged exists arg3
//   647C4F  push 3                          ; staged exists arg2
//   647C51  push 3                          ; get_portrait size
//   647C53  lea ecx,[esp+0x20]              ; out = B
//   647C57  push ecx
//   647C58  lea ecx,[esp+0x14]              ; ECX = A
//   647C5C  call 00678EF0                   ; B = A.GetPortrait(3); EAX = B
//   647C61  mov ecx,[007A39E8]              ; canonical resource manager
//   647C67  push eax
//   647C68  call 00408BC0                   ; exists(B,3,0)
//   647C6D  test eax,eax
//   647C6F  je 647C97
//   647C71  lea ecx,[esp+4]                 ; A destructor
//   647C75  mov [esp+2C],-1                 ; EH state -1
//   647C7D  call 004015A0                   ; no-op destructor
//   647C82  mov eax,1
//   647C87..647C96                          ; restore fs:0, add esp,2C, ret
//   647C97  mov eax,[esp+38]                ; is_default_id
//   647C9B  test eax,eax
//   647C9D  mov ecx,esi                     ; dest = portrait_out
//   647C9F  je 647CAD
//   647CA1  push 00751C0C                   ; "po_hu_f_99_" (nonzero)
//   647CAB  jmp 647CB7
//   647CAD  push 00751C00                   ; "po_hu_m_99_" (zero)
//   647CB7  call 00406290                   ; portrait_out = lower(literal); B = out
//   647CBC  lea ecx,[esp+4]                 ; A destructor
//   647CC0  mov [esp+2C],-1                 ; EH state -1
//   647CC8  call 004015A0                   ; no-op destructor
//   647CCD  xor eax,eax
//   647CD1..647CDE                          ; restore fs:0, add esp,2C, ret
//
// ABI: __cdecl, no receiver, two 4-byte stack arguments, RET0.  The callers
// (0066588D and 00668AE3) remove the two argument dwords with `add esp,8`;
// the second caller reuses those same words as the by-value CResRef given to
// SetPortrait 005EE480 (RET16).
//
// Exception contract: the exported target is noexcept(false).  It calls the
// potentially throwing constructor and GetPortrait seams and the C++ manager
// method, and has no catch.  The declaration must override the compiler's
// C-function nonthrow assumption so a provider failure propagates to the
// caller and the local A destructor runs on unwind after construction.
//
// The two 16-byte locals are the original's stack objects.  Local A has the
// original non-trivial ctor/dtor (00678EC0/004015A0), so its destructor runs on
// both normal exits and on unwind after construction; local B is the plain
// 16-byte CResRef sink shared by 00406120 and 00678EF0/00406290.
//
// Engine quirks preserved:
//  * No null check on portrait_out or on the canonical manager cell.
//  * 00406120 lowercases all 16 bytes without stopping at NUL (selected
//    helper 006FB766 is `_kotor_lower_006fb766`, the CRT tolower wrapper).
//  * The exists result is tested as any-nonzero.
//  * The found path leaves portrait_out untouched.
//  * is_default_id is tested as any-nonzero; 0066588D/00668AE3 supply 0/1.
#include "48ddfadb13-portrait_00647c10.h"
#include "798b250dab-portrait_00647c10_providers.h"

#include <cstdint>

namespace {

// 16-byte original CSWPortrait local A.  Constructor/destructor forward to the
// selected providers with the original thiscall receiver in ECX.  __forceinline
// keeps the emitted call at the original call sites (00647C2D/00647C7D/
// 00647CC8); if a compiler declines, the call survives one typed forwarder and
// behavior is unchanged.  The destructor is noexcept because 004015A0 is a
// single `ret`.
struct alignas(4) CSWPortrait {
    std::uint8_t bytes[16];

    __forceinline CSWPortrait() {
        kotor_m15b_portrait_construct_00678ec0(this, nullptr);
    }

    __forceinline ~CSWPortrait() noexcept {
        kotor_fastcall_004015a0(this, nullptr);
    }
};

// 16-byte original CResRef local B (plain storage; no destructor call in the
// original cleanup paths).
struct alignas(4) CResRefStorage {
    std::uint8_t bytes[16];
};

static_assert(sizeof(CSWPortrait) == 16, "original local A is 16 bytes");
static_assert(sizeof(CResRefStorage) == 16, "original local B is 16 bytes");
static_assert(alignof(CSWPortrait) == 4, "4-byte CResRef alignment");
static_assert(alignof(CResRefStorage) == 4, "4-byte CResRef alignment");
static_assert(sizeof(void*) == 4, "x86 ABI");

constexpr char kDefaultMalePortrait[] = "po_hu_m_99_";
constexpr char kDefaultFemalePortrait[] = "po_hu_f_99_";

}  // namespace

extern "C" int __cdecl
recovered_client_creature_stats_check_custom_portrait_00647c10(
    void* portrait_out, unsigned int is_default_id) noexcept(false) {
    // 00647C29..00647C2D: local A construction.
    CSWPortrait portrait;

    // 00647C37..00647C48: local B is the hidden-return sink of 00406120.
    CResRefStorage image;
    (void)ref_assign_copy_00406120(&portrait, nullptr, &image, portrait_out);

    // 00647C4D..00647C5C: 00678EF0 receives (out = B, size = 3); its return
    // value is the pointer the original pushes to 00408BC0 (00647C67).
    void* const derived =
        kotor_m15e_portrait_get_portrait_00678ef0(&portrait, nullptr, &image, 3);

    // 00647C61: the canonical manager cell is read through its owner accessor
    // (selected 0060D820 reads the canonical 008EECE8 cell; no raw original VA).
    menu_resources::Manager* const manager =
        menu_resources::BorrowCanonicalManager007a39e8();

    // 00647C67..00647C6F: exists(derived, 3, null).  Any nonzero is a hit.
    if (manager->exists_00408bc0(
            reinterpret_cast<menu_resources::Ref*>(derived), 3, nullptr) != 0) {
        return 1;  // 00647C82; local A destructor runs at 00647C7D
    }

    // 00647C97..00647CC0: overwrite portrait_out with the gender default.  The
    // same local B slot is reused as 00406290's out argument.
    (void)unresolved_ref_assign_00406290(
        reinterpret_cast<menu_resources::Ref*>(portrait_out), nullptr,
        reinterpret_cast<menu_resources::Ref*>(&image),
        (is_default_id != 0) ? kDefaultFemalePortrait : kDefaultMalePortrait);

    return 0;  // 00647CD1; local A destructor runs at 00647CC8
}
