// Original 00685610 — CSWGuiMainInterface::ShowActionIcon
// Reconstruction of the complete original entry-to-return body.
//
// Original span: 00685610..006856B8.
//
// ABI: thiscall plus five stack arguments, ret 0x14.
//   ECX = CSWGuiMainInterface* self
//   EDX = unused (project fastcall shim slot)
//   stack+4  = int* count             (icon slot cursor)
//   stack+8  = const CResRef* primary (specific icon)
//   stack+12 = const CResRef* fallback(category default icon)
//   stack+16 = U32 active             (nonzero when a current combat action exists)
//   stack+20 = U32 force              (nonzero skips the empty-primary early return)
//
// Export name under the project's fastcall-symbol convention:
//   @kotor_fastcall_00685610@28
//
// Original behavior (all branches):
//   if (force == 0 && primary == empty CResRef) return;
//   if (primary->IsValid()) {
//       n = *count;                       // fault here if count is null
//       SetFillImage(self + 0x62CC + n*0x140, primary, 0);
//   } else {
//       n = *count;                       // fault here if count is null
//       SetFillImage(self + 0x62CC + n*0x140, fallback, 0);
//   }
//   if (active != 0 || *count != 0)
//       *(float*)(self + 0x62D8 + (*count)*0x140) = 1.0f;
//   else
//       *(float*)(self + 0x62D8) = 0.25f;
//   n = *count;
//   flags = (U32*)(self + 0x625C + n*0x140 + 0x44);
//   ++*count;                             // increment before the flags OR
//   *flags |= 2;
//
// No allocation, no local object, no x87, no SEH. No null/vtable guards:
// a null self/primary/fallback faults exactly where the original does; a null
// count faults only at the post-IsValid *count read, as in the original.
// The empty-CResRef comparison uses the selected real default CResRef provider,
// which zeroes sixteen bytes, matching the original's 0x73D71C static cell.
//
// This TU defines only original 00685610.

// Real selected providers (1566 map). Each declaration is ABI-identical to the
// original call: ECX = receiver, stack arguments in original order.
extern "C" unsigned __fastcall c2da_not_equal_text_00406060(
    const void* self, void* /*edx*/, const void* other);
extern "C" unsigned __fastcall gui_load_resref_is_valid_00406030(
    const void* self, void* /*edx*/);
extern "C" void __fastcall gui_load_border_params_set_fill_00414c00(
    void* self, void* /*edx*/, const void* source, unsigned zero);
extern "C" void* __fastcall c2da_default_ref_00405ed0(
    void* self, void* /*edx*/);

// Selected 00405ED0 is the C++-mangled default CResRef constructor at
// 0062B990. Bind the C declaration to that exact selected symbol; no address
// is used. (The 1566 map also carries the @c2da_default_ref_00405ed0@8 alias;
// the directive is inert when that alias is already defined.)
#pragma comment(linker, "/alternatename:@c2da_default_ref_00405ed0@8=?default_00405ed0@ResRef@kotor_menu_10_19@@QAEPAU12@XZ")

extern "C" void __fastcall kotor_fastcall_00685610(
    void* self,
    void* /*edx*/,
    int* count,
    const void* primary,
    const void* fallback,
    unsigned active,
    unsigned force)
{
    if (force == 0) {
        // Original compares against the static sixteen-byte empty CResRef at
        // 0x73D71C. A real default-constructed CResRef is the same value.
        unsigned char empty[16];
        c2da_default_ref_00405ed0(empty, nullptr);
        if (c2da_not_equal_text_00406060(primary, nullptr, empty) == 0) {
            return;
        }
    }

    unsigned char* const base = static_cast<unsigned char*>(self);

    // IsValid is called before *count is read; the *count load inside each
    // branch is the original's first count access and its null-count fault.
    if (gui_load_resref_is_valid_00406030(primary, nullptr) != 0) {
        const unsigned index = static_cast<unsigned>(*count);
        gui_load_border_params_set_fill_00414c00(
            base + 0x62CC + index * 0x140, nullptr, primary, 0);
    } else {
        const unsigned index = static_cast<unsigned>(*count);
        gui_load_border_params_set_fill_00414c00(
            base + 0x62CC + index * 0x140, nullptr, fallback, 0);
    }

    if (active != 0 || *count != 0) {
        const unsigned index = static_cast<unsigned>(*count);
        *reinterpret_cast<float*>(base + 0x62D8 + index * 0x140) = 1.0f;
    } else {
        *reinterpret_cast<float*>(base + 0x62D8) = 0.25f;
    }

    const unsigned index = static_cast<unsigned>(*count);
    unsigned* const flags =
        reinterpret_cast<unsigned*>(base + 0x625C + index * 0x140 + 0x44);
    ++*count;
    *flags |= 2u;
}
