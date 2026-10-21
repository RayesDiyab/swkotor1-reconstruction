// G11-quicksave lane: full original 006323A0..0063271A body (890 bytes).
//
// Original behavior (original VAs are identity labels only; no numeric address
// is read or aliased):
//   entry: push ebp/mov ebp,esp/and esp,-8, 23-state /EHs frame
//   ECX (self) and EDX are not read; [ebp+8]=saveId (int), [ebp+0xC]=suffix
//   call B's 0062FBE0(saveId, suffix) (__stdcall ret 8)
//   non-zero -> success block (returns 1); zero -> cold block (returns 0)
//
// Success block exact order (every step is a separate statement; no provider
// or cleanup order depends on argument evaluation order):
//   game_name "GAMEINPROGRESS:"; alias list; resolve into game; game_name
//   Destroy; future_name "FUTUREGAME:"; alias list; resolve into future;
//   future_name Destroy; nuke_name "GAMEINPROGRESS:"; nuke(1,1); nuke_name
//   scope dtor; future length-1/left temp/assign/dtor; game length-1/left
//   temp/assign/dtor; CStr(game); CStr(future); move_file_a(future, game);
//   return 1 (future then game dtors).
//
// Cold block exact order (every step is a separate statement):
//   default save_name/save_path; save_id != -1 -> CStr(suffix) then Format
//   ("%06d - %s"), else Assign(suffix); AssignText "SAVES:"; explicit concat
//   temp/assign/dtor; hide_load_screen([app_manager+4],0,1); construct wb+,
//   CORRUPT, backslash; explicit concat1 and concat2; construct file; destroy
//   p2,p1,slash,corrupt,wb; write("CORRUPT"); flush; return 0 (file dtor, then
//   the already-dead temporaries are no-ops, then save_path/save_name dtors).
//
// No guards, skips, traps, alternate returns or partial branches are added.
// The source-level scopes reproduce the original construction/destruction
// order; MSVC chooses its own unwind-state numbering (the original table has
// 23 states) and must destroy exactly the constructed objects on every path.
#include "e848c4c6cb-g11_quicksave.h"

namespace {

// [0x7A39E0]+0x0C: the ResolveFileName alias-list this pointer.
void* alias_list() {
    return *reinterpret_cast<void**>(
        static_cast<unsigned char*>(g11_exo_base) + 0x0C);
}

// [[0x7A39FC]+4]: the CGuiInGame client internal passed to HideLoadScreen.
void* client_internal() {
    return reinterpret_cast<void**>(g11_app_manager)[1];
}

}  // namespace

extern "C" int __fastcall load_screen_unpack_quick_save_game_006323a0(
    void*, void*, int save_id, void* suffix) {
    if (g11_copy_quick_save(save_id, suffix) != 0) {
        // 1. GAMEINPROGRESS: name temp -> resolve -> explicit destroy.
        g11_quicksave::ExoString game_name("GAMEINPROGRESS:");
        void* game_aliases = alias_list();
        g11_quicksave::ExoString game =
            g11_quicksave::resolve(game_aliases, game_name, 0xFFFFu);
        game_name.Destroy();

        // 2. FUTUREGAME: name temp -> resolve -> explicit destroy.
        g11_quicksave::ExoString future_name("FUTUREGAME:");
        void* future_aliases = alias_list();
        g11_quicksave::ExoString future =
            g11_quicksave::resolve(future_aliases, future_name, 0xFFFFu);
        future_name.Destroy();

        // 3. Nuke name temp -> nuke -> scope destroy.
        {
            g11_quicksave::ExoString nuke_name("GAMEINPROGRESS:");
            void* nuke_name_ptr = nuke_name.raw();
            void* resman = g11_resource_manager;
            void* const one_value =
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(1u));
            g11_nuke_directory(resman, nullptr, nuke_name_ptr, one_value,
                               one_value);
        }

        // 4. Future then game: length -> left temp -> assign -> scope destroy.
        {
            int future_length = future.Length() - 1;
            g11_quicksave::ExoString future_left =
                g11_quicksave::left(future, future_length);
            future = future_left;
        }
        {
            int game_length = game.Length() - 1;
            g11_quicksave::ExoString game_left =
                g11_quicksave::left(game, game_length);
            game = game_left;
        }

        // 5. CStr(game) then CStr(future), then move(future, game).
        const char* game_path = game.CStr();
        const char* future_path = future.CStr();
        g11_move_file_a(future_path, game_path);
        return 1;
    }

    // Cold (copy failed) block.
    g11_quicksave::ExoString save_name;  // original 0x58 slot
    g11_quicksave::ExoString save_path;  // original 0x60 slot

    if (save_id != -1) {
        char* suffix_cstr = g11_exostring_cstr(suffix, nullptr);
        void* save_name_raw = save_name.raw();
        g11_exostring_format(save_name_raw, "%06d - %s", save_id, suffix_cstr);
    } else {
        save_name.AssignRaw(suffix);
    }

    save_path.AssignText("SAVES:");

    {
        g11_quicksave::ExoString joined = save_path + save_name;
        save_path = joined;
    }

    {
        void* internal = client_internal();
        g11_hide_load_screen(internal, nullptr, 0, 1);
    }

    // Five original argument temporaries, explicitly named and constructed in
    // the original order, then the file.
    g11_quicksave::ExoString wb("wb+");
    g11_quicksave::ExoString corrupt("CORRUPT");
    g11_quicksave::ExoString slash("\\");
    g11_quicksave::ExoString p1 = save_path + slash;  // concat1
    g11_quicksave::ExoString p2 = p1 + corrupt;       // concat2
    void* const p2_path = p2.raw();
    void* const wb_name = wb.raw();
    g11_quicksave::ExoFile file(p2_path, 0u, wb_name);

    // Destroy the five temporaries before Write/Flush, exactly as the original.
    // Destroy() marks an object dead before its provider call, so a throwing
    // destroy is not retried and the scope-exit destructors are no-ops.
    p2.Destroy();
    p1.Destroy();
    slash.Destroy();
    corrupt.Destroy();
    wb.Destroy();

    file.Write("CORRUPT");
    file.Flush();
    return 0;
}
