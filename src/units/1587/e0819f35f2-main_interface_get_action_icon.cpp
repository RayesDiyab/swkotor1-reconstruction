// Original 00686FB0 — CSWGuiMainInterface::GetActionIcon
// Reconstruction of the complete original entry-to-return body.
//
// Original span: 00686FB0..0068741C (Ghidra 0x510 including the three jump
// tables at 00687420 / 00687468 / 00687494 and the 52-byte index map at
// 00687434).
//
// ABI: thiscall plus six stack arguments, ret 0x18.
//   ECX = CSWGuiMainInterface* self       (unused)
//   EDX = unused (project fastcall shim slot)
//   stack+4  = CSWSCreature* creature     (unused)
//   stack+8  = action node*               ([node+0] is the action id)
//   stack+12 = combat action object*      (nullable in the id-0x3F branch)
//   stack+16 = unused pointer             (all call sites pass &outer-position)
//   stack+20 = CResRef* outSpecific       (specific icon)
//   stack+24 = CResRef* outGeneric        (category default icon)
//
// Export name under the project's fastcall-symbol convention:
//   @kotor_fastcall_00686fb0@32
//
// Dispatch:
//   id = node ? *(int*)node : 0x0C;  if ((unsigned)(id-0x0C) > 0x33) return;
//   0x0C -> type = action[+0x10]-1 (table2)
//   0x0F -> spell id node[+0x38] -> spell[+0x10] / i_powerm
//   0x2E -> item id  node[+0x38] -> item icon    / i_useitemm
//   [corrected 2026-10-21, c57 M-3: these two cases use EAX as loaded at
//   00686FE8 = stack arg 2 (the node), not the combat action at stack+12;
//   UpdateActionQueue passes action = 0 for a queued use-item when the round
//   has no current action (0068A175), so reading action[+0x38] faulted at
//   0x38.]
//   0x3F -> action==0: i_attack / i_attackm
//           else type = action[+0x10]-1 (table3)
//   other action ids return with both outputs reset to empty.
//
// Table2 (id 0x0C), by type: 0 i_attack/i_attackm, 5 i_equip/i_equipm,
// 6 i_unequip/i_equipm, 8 spell(action[+0x24])[+0x10]/i_powerm,
// 9 item(action[+0x64]) icon/i_useitemm, 10 feat(action[+0x5C])[+0x18]/i_featm.
// Table3 (id 0x3F) is the same, except type 10 reads the zero-extended word
// action[+0x5C]; types 1..4 and 7 return.
//
// Both outputs are reset to empty through the real 00406290 provider at entry,
// before the id check, so every early/default return leaves them empty.
//
// Fault contract preserved: id 0x0C dereferences a null action (before its
// type range check); ids 0x0F/0x2E read the node, which is non-null whenever
// they are reached (the id was read from it); id 0x0F and the two
// spell cases dereference a null GetSpell result at +0x10 [corrected
// 2026-10-20: only id 0x0F does; the two type-8 spell cases at 00687118 and
// 006872B6 test it and return]; the item/feat cases
// return on a null lookup. No guards are added.
//
// This TU defines only original 00686FB0.

#include <cstdint>

// Real selected providers (1566 map). Declarations match the original call
// ABI: ECX = receiver, EDX = unused project slot, remaining arguments on the
// stack in original order.
extern "C" void* __fastcall unresolved_ref_assign_00406290(
    void* temp, void* /*edx*/, void* out, const char* text);
extern "C" void* __fastcall ref_assign_copy_00406120(
    void* temp, void* /*edx*/, void* out, const void* source);
extern "C" void* __fastcall recovered_item_lookup_004ae760(
    void* server, void* /*edx*/, unsigned id);
extern "C" void* __fastcall kotor_fastcall_005556b0(
    void* item, void* /*edx*/, void* out);
extern "C" void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(
    void* spell_array, void* /*edx*/, int id);
extern "C" void* __fastcall kotor_rules_get_feat_00550c00(
    void* rules, void* /*edx*/, unsigned feat_id);

// Selected 00406120 is a C++-mangled fastcall global in
// resource_ref_assignment. Bind the C declaration above to that exact selected
// symbol; no address is used.
#pragma comment(linker, "/alternatename:@ref_assign_copy_00406120@16=?assign_00406120@resource_ref_assignment@@YIPAURef@menu_resources@@PAU23@PAX0PBU23@@Z")

// Selected 00550C00 is a C++-mangled fastcall global in kotor_rules.
#pragma comment(linker, "/alternatename:@kotor_rules_get_feat_00550c00@12=?kotor_fastcall_00550c00@kotor_rules@@YIPAXPAX0G@Z")

// Selected storage cells (1566 map): app pointer cell and rules pointer cell.
// The app cell is a C++-mangled pointer variable; declare it with its real
// namespace/name so the compiler emits the selected symbol.
namespace menu_continuation {
struct AppManager;
extern AppManager* app_manager_007a39fc;
}  // namespace menu_continuation

extern "C" void* kotor_cell_rules_007a3a28;

namespace {

unsigned read_u32(const void* base, unsigned offset)
{
    return *reinterpret_cast<const unsigned*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

unsigned read_u16(const void* base, unsigned offset)
{
    return *reinterpret_cast<const unsigned short*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

unsigned char read_u8(const void* base, unsigned offset)
{
    return *reinterpret_cast<const unsigned char*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

void assign_text(void* out, const char* text)
{
    // Original call shape: ECX = destination output, stack arg1 = stack
    // temporary, stack arg2 = text. The provider writes the output first and
    // then copies it into the temporary.
    unsigned char temporary[16];
    unresolved_ref_assign_00406290(out, nullptr, temporary, text);
}

void assign_ref(void* out, const void* source)
{
    // Same original shape for the CResRef-to-CResRef assignment: ECX = output,
    // stack arg1 = stack temporary, stack arg2 = source.
    unsigned char temporary[16];
    ref_assign_copy_00406120(out, nullptr, temporary, source);
}

void* server_app()
{
    unsigned char* const app =
        reinterpret_cast<unsigned char*>(menu_continuation::app_manager_007a39fc);
    return *reinterpret_cast<void**>(
        reinterpret_cast<std::uintptr_t>(app) + 8);
}

void* spell_array()
{
    unsigned char* const rules = static_cast<unsigned char*>(kotor_cell_rules_007a3a28);
    return *reinterpret_cast<void**>(
        reinterpret_cast<std::uintptr_t>(rules) + 0x8C);
}

void set_spell_icon(void* out_specific, void* out_generic, unsigned spell_id,
                    bool null_test)
{
    // Action id 0x0F (006873D8) adds 0x10 to the GetSpell result without a
    // null test. The combat-action type-8 cases (00687118 / 006872B6) test
    // the result and return with both outputs left empty on null.
    void* const spell = kotor_m15b_spell_array_get_spell_0059b6d0(
        spell_array(), nullptr, static_cast<int>(spell_id));
    if (null_test && spell == nullptr) {
        return;
    }
    assign_ref(out_specific,
               reinterpret_cast<const void*>(
                   reinterpret_cast<std::uintptr_t>(spell) + 0x10));
    assign_text(out_generic, "i_powerm");
}

void set_item_icon(void* out_specific, void* out_generic, unsigned item_id)
{
    void* const item = recovered_item_lookup_004ae760(server_app(), nullptr, item_id);
    if (item == nullptr) {
        return;
    }
    // Original: GetIcon writes a 16-byte stack temporary, then the real
    // 00406120 copy assigns that temporary to out_specific.
    unsigned char icon[16];
    kotor_fastcall_005556b0(item, nullptr, icon);
    assign_ref(out_specific, icon);
    assign_text(out_generic, "i_useitemm");
}

void set_feat_icon(void* out_specific, void* out_generic, unsigned feat_id)
{
    void* const feat = kotor_rules_get_feat_00550c00(
        kotor_cell_rules_007a3a28, nullptr, feat_id);
    if (feat == nullptr) {
        return;
    }
    assign_ref(out_specific,
               reinterpret_cast<const void*>(
                   reinterpret_cast<std::uintptr_t>(feat) + 0x18));
    assign_text(out_generic, "i_featm");
}

}  // namespace

extern "C" void __fastcall kotor_fastcall_00686fb0(
    void* /*self*/,
    void* /*edx*/,
    void* /*creature*/,
    const void* node,
    const void* action,
    void* /*unused4*/,
    void* outSpecific,
    void* outGeneric)
{
    // Entry reset of both outputs, in original order (specific, then generic).
    assign_text(outSpecific, "");
    assign_text(outGeneric, "");

    int id = 0x0C;
    if (node != nullptr) {
        id = static_cast<int>(read_u32(node, 0));
    }
    // Original: sub ecx,0x0C; cmp ecx,0x33; ja return (32-bit unsigned wrap).
    const unsigned id_index = static_cast<unsigned>(id) - 0x0Cu;
    if (id_index > 0x33u) {
        return;
    }

    switch (id) {
    case 0x0C: {
        // No null test on action: the byte load faults first, exactly as the
        // original. The feat id is also loaded before the type range check.
        const unsigned type = static_cast<unsigned>(read_u8(action, 0x10)) - 1u;
        const unsigned feat_id = read_u32(action, 0x5C);
        if (type > 0xAu) {
            return;
        }
        switch (type) {
        case 0:
            assign_text(outSpecific, "i_attack");
            assign_text(outGeneric, "i_attackm");
            return;
        case 5:
            assign_text(outSpecific, "i_equip");
            assign_text(outGeneric, "i_equipm");
            return;
        case 6:
            assign_text(outSpecific, "i_unequip");
            assign_text(outGeneric, "i_equipm");
            return;
        case 8:
            set_spell_icon(outSpecific, outGeneric, read_u32(action, 0x24), true);
            return;
        case 9:
            set_item_icon(outSpecific, outGeneric, read_u32(action, 0x64));
            return;
        case 10:
            set_feat_icon(outSpecific, outGeneric, feat_id);
            return;
        default:
            return;
        }
    }
    case 0x0F:
        // 006873D8: mov eax,[eax+0x38] with EAX = node (00686FE8). No null
        // test on the GetSpell result.
        set_spell_icon(outSpecific, outGeneric, read_u32(node, 0x38), false);
        return;
    case 0x2E:
        // 00687384: mov eax,[eax+0x38] with EAX = node (00686FE8).
        set_item_icon(outSpecific, outGeneric, read_u32(node, 0x38));
        return;
    case 0x3F: {
        if (action == nullptr) {
            assign_text(outSpecific, "i_attack");
            assign_text(outGeneric, "i_attackm");
            return;
        }
        const unsigned type = static_cast<unsigned>(read_u8(action, 0x10)) - 1u;
        if (type > 0xAu) {
            return;
        }
        switch (type) {
        case 0:
            assign_text(outSpecific, "i_attack");
            assign_text(outGeneric, "i_attackm");
            return;
        case 5:
            assign_text(outSpecific, "i_equip");
            assign_text(outGeneric, "i_equipm");
            return;
        case 6:
            assign_text(outSpecific, "i_unequip");
            assign_text(outGeneric, "i_equipm");
            return;
        case 8:
            set_spell_icon(outSpecific, outGeneric, read_u32(action, 0x24), true);
            return;
        case 9:
            set_item_icon(outSpecific, outGeneric, read_u32(action, 0x64));
            return;
        case 10:
            // Original zero-extends the word at +0x5C for this branch.
            set_feat_icon(outSpecific, outGeneric, read_u16(action, 0x5C));
            return;
        default:
            return;
        }
    }
    default:
        return;
    }
}
