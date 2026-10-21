// lane-m59-hostexit REPLACEMENT COPY of M58 unit game-prepare-game-prepare (game-prepare/game_prepare.cpp sha 12c0357d...).
// Only change (HX-1): PreparedOwner::release/drain use release_scope_match() (formatter in its prepare_release drain
// window) instead of the prepare-time heap_and_scope_match() (formatter claim-admitted), which can never hold inside
// FormatterScope::prepare_release. Diag ids, exit codes and every other check are unchanged.
// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit game-prepare-game-prepare
// (game-prepare/game_prepare.cpp sha de7faa61...). Only change: kotor_host_diag* report calls (D94 proposal)
// before each existing false return / fail_closed_release; compound conditions split into sequential ifs
// in the SAME short-circuit order; attach results captured once in a local before the unchanged compare.
// lane-m48-b fork CLIENT REPLACEMENT COPY of the M47 stage unit game-prepare-game-prepare (game-prepare/game_prepare.cpp).
// Only change: removal of the vestigial kotor_sound_publish_audio_providers() call (see comment at the former call site).
// lane-prepare. Genuine game_prepare / game_release for the M41-stage
// game_lifetime.h (unchanged since M40, sha 5eb94639). Composes existing
// stage providers plus lane-prepare producers; every still-missing producer is an
// explicit extern in game_prepare_producers.h (link-time UNDEF, never a value).
// Base (00404276/00404284) and Manager (004042BB/004042C9) are NOT constructed
// here: reconstructed WinMain 004041F0 does that on its own route.
#include "7239dd00bc-game_prepare_producers.h"
#include "b8f725f719-functional_runtime.h"
#include "5a637bf0d2-release_contract.h"
#include "726a0f6ebd-allocator_live.h"
#include "5aa995fd0c-modern_locale.h"
#include "d66394d979-canonical_formatter_owner.h"
#include "998d166954-window_shared_cells.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
#include "0f74fbe8f3-manager_cell.h"
#include "806487833e-resource_cell_joins.h"   // task22_strings / task10_resource_cells attach (M41 stage root)
#include <cstddef>
#include <new>

namespace kotor_functional {
using namespace kotor_game_prepare;
namespace {
class PreparedOwner;
PreparedOwner* active_owner = nullptr; // Sole modern ownership record, no game cell.

using Global = client_services_start::Global;
constexpr unsigned word_index(Global g) noexcept { return static_cast<unsigned>(g); }

class PreparedOwner {
    // Partial-acquisition record. Each flag is set immediately after the step
    // that created the ownership, before the next opaque/throwing operation.
    enum class Phase { Acquiring, Bound, Releasing, Released };
    Phase phase_ = Phase::Acquiring;
    void* const borrowed_heap_;
    PreparedGame* const result_;

    bool strings_constructed_ = false;      // three Shell CExoStrings
    // Naturally aligned durable Views storage; no member is active until the
    // existing construct() provider placement-constructs it (no alignas pad).
    union { kotor_menu_live_views::Views views_slot_; };
    kotor_menu_live_views::Views* views_ = nullptr;
    // Native entry frames: original storage is WinMain's own stack frame
    // (004041F6 sub esp,84h) and InitGameApp's. Default initialization only;
    // no invented contents. Read-before-write audit remains an open gate.
    kotor_winmain::Frame winmain_frame_;
    kotor_window_shell::Frame shell_frame_;
    kotor_winmain::Context winmain_{};
    kotor_window_shell::Context shell_{};
    client_services_start::Globals start_{}; // Address metadata only.
    client_services_start::Globals* prior_start_ = nullptr;
    bool start_published_ = false;
    client_services::Dependencies prior_client_{};
    bool client_published_ = false;
    bool video_attached_ = false;
    bool video_cells_attached_ = false;
    task22_strings::Attachment strings_join_{};
    task10_resource_cells::Attachment resource_join_{};
    bool strings_joined_ = false;
    bool resource_joined_ = false;
    bool bias_published_ = false;
    kotor_live_publication::Providers window_{};
    bool window_sources_recorded_ = false;  // true once acquire may own anything
    GameLifetime lifetime_;

    // lane-m59-hostexit: the owner/heap/Locale part shared by the prepare-time and release-time checks.
    bool heap_and_locale_match() const noexcept {
        return active_owner == this && source_heap_identity() && live_globals.heap &&
            borrowed_heap_ && *live_globals.heap == borrowed_heap_ && locale_identity();
    }
    // Prepare-time scope: the formatter owner is Active (admits claims). Unchanged predicate.
    bool heap_and_scope_match() const noexcept {
        return heap_and_locale_match() && kotor_canonical_string_format::formatter_claim_admitted();
    }
    // lane-m59-hostexit (HX-1): release-time scope. Release (and the drain it runs) is reached only from
    // run()'s ~GameLease -> FormatterScope::prepare_release(release_and_observe_formatter_consumers, &prepared),
    // which verifies identity()+Active, sets Phase::CheckingDrain BEFORE invoking the drain callback, and re-checks
    // identity()+CheckingDrain after it (std::terminate otherwise). So during release the formatter is never
    // claim-admitted (Active) and never drain-admitted (DrainAdmitted, set only after the callback returns):
    // the former heap_and_scope_match() (formatter_claim_admitted) was false on EVERY release -> exit 90 at
    // release.prepared.entry after a clean original shutdown (run 20261007T230550468179Z). The heap/Locale/owner
    // identity is still required; the formatter clause now requires the drain window instead of claim admission,
    // so a stray release while the formatter still admits claims (or after it was drain-admitted) still refuses.
    bool release_scope_match() const noexcept {
        return heap_and_locale_match() && !kotor_canonical_string_format::formatter_claim_admitted() &&
            !kotor_canonical_string_format::ini_detach_admitted();
    }
    bool result_matches() const noexcept {
        return result_ && result_->lifetime == &lifetime_ && result_->start == &start_ &&
            (!result_->window || result_->window == kotor_menu_window_publication);
    }
    // GameDrain operation: the genuine window/worker owner drains its objects
    // while Views/Contexts/Start/client/GUI tables and the nine joins are live.
    static bool __cdecl drain(void* p) noexcept {
        auto* self = static_cast<PreparedOwner*>(p); // Actual this-owner transport.
        if (!self || !self->release_scope_match()) return false; // HX-1: drain runs only inside release
        if (!self->window_sources_recorded_) return true; // nothing acquired
        return kotor_window_sources_drain(self->window_.owner);
    }
    void destroy_shell_strings() noexcept {
        // atexit LIFO of initterm order 0073ACE0 < 0073AD40 < 0073AD60
        // (__xc entries at 0078D008/0078D014/0078D018): Copyright, GameName,
        // BuildNumber -> 0073B760, 0073B750, 0073B720 -> 005E5C20 each.
        auto& g = views_->shell;
        kotor_shell_string_destroy_005e5c20(&g.caption_prefix_7a3cdc, nullptr);
        kotor_shell_string_destroy_005e5c20(&g.title_7a3ccc, nullptr);
        kotor_shell_string_destroy_005e5c20(&g.graphics_version_7a3cc4, nullptr);
    }
public:
    PreparedOwner(void* heap, PreparedGame& result) noexcept
        : borrowed_heap_(heap), result_(&result) {}
    PreparedOwner(const PreparedOwner&) = delete;
    PreparedOwner& operator=(const PreparedOwner&) = delete;
    ~PreparedOwner() {
        if (phase_ != Phase::Released || active_owner == this || views_ ||
            start_published_ || client_published_ || window_sources_recorded_) {
            ;
            fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
        }
    }
    void record_result() noexcept {
        // Before the first opaque/throwing producer operation, including failure.
        result_->lifetime = &lifetime_;
        result_->window = nullptr;
        result_->start = &start_;
    }
    bool prepare(const StartupArguments& args) {
        if (!heap_and_scope_match()) { ; return false; }
        if (!result_matches()) { ; return false; }
        if (client_services_start::globals) { ; return false; }
        // 1. Initial: PE image values, then CRT argc/argv (0070238F/00702395),
        //    then static CExoString construction (0073ACE0/0073AD40/0073AD60).
        kotor_menu_live_views::Initial initial{kotor_image_winmain_initial,
            kotor_image_shell_initial, kotor_image_reinit_initial, kotor_image_message_initial};
        initial.winmain.argc = static_cast<kotor_menu_live_views::Word>(args.argc);
        initial.winmain.argv = static_cast<kotor_menu_live_views::Word>(
            reinterpret_cast<std::uintptr_t>(args.argv)); // borrowed CRT argv, never freed
        kotor_shell_string_construct_005b3190(&initial.shell.graphics_version, nullptr);
        kotor_shell_string_construct_005b3190(&initial.shell.title, nullptr);
        kotor_shell_string_construct_005b3190(&initial.shell.caption_prefix, nullptr);
        // 2. Views (existing provider, checks the eight canonical cells first).
        //    The constructed {0,0} String words move into the durable Globals;
        //    the temporaries in `initial` own no allocation.
        views_ = kotor_menu_live_views::construct(&views_slot_, sizeof(views_slot_),
            *kotor_menu_window_shared_cells, initial);
        if (!views_) { ; return false; }
        // 007B9274: AurMakeCurrent 0044EDF2 writes the Views member WinMain reads
        // at 00404576 (one storage); published for lane-routes aur_make_current::cells.
        publish_make_current_bias(&views_->winmain.bias_7b9274);
        bias_published_ = true;
        // F1: ReadAndSetVideoMode 005F5C18/005F5C23 (StartServices 005F905F) and
        // SetVideoMode store width/depth into these same words. W-5 (lane-m50-vtables): width is the single 0078D1D4
        // owner kotor_renderer_gl_cells::width_0078d1d4 (also written by 0044EA00 and read by the renderer/GUI).
        if (!options_video::attach_saved_video_mode(&kotor_renderer_gl_cells::width_0078d1d4, &views_->winmain.depth_78d1e0)) {
            ;
            return false;
        }
        video_attached_ = true;
        // D46: lane-config video_mode_bindings cells (ReadVideoModeSettings 005F0F88
        // writes 0078D1DC; SetVideoMode 005F18B7/005F18C1 write depth/width).
        if (!options_video::attach_video_mode_cells(&kotor_renderer_gl_cells::width_0078d1d4, &views_->reinit.mode_78d1dc,
                                                    &views_->winmain.depth_78d1e0)) {
            ;
            return false;
        }
        video_cells_attached_ = true;
        strings_constructed_ = true;
        // 3. Contexts over the durable Views and owned entry frames.
        winmain_ = {&views_->winmain, &winmain_frame_,
                    {this, &kotor_winmain_invoke, &kotor_winmain_read_word}};
        shell_ = {&views_->shell, &kotor_shell_constants, &shell_frame_,
                  {this, &kotor_shell_invoke}};
        // 4. StartServices address table. Base/Manager/Input/Aurora/Width/Height
        //    and depth are joined by GameLifetime::bind (base_gui_join::attach).
        start_.words[word_index(Global::Resource)] = &kotor_resource_demand::resource_manager_007a39e8;
        start_.words[word_index(Global::Interface)] = &kotor_cell_interface_007a39f0;
        start_.words[word_index(Global::Tlk)] = &kotor_cell_tlk_007a3a08;
        start_.words[word_index(Global::TlkClient)] = &kotor_cell_tlk_client_007a3a0c;
        start_.words[word_index(Global::AllowWindowed)] = &kotor_cell_allow_windowed_007a3a30;
        start_.logging_byte_008328e1 = &kotor_cell_logging_008328e1;
        prior_start_ = client_services_start::globals;
        client_services_start::globals = &start_;
        start_published_ = true;
        // 5. client_services::dependencies (consumers: Manager ctor 00401160,
        //    Client ctor 005EE2F0, Initialize 005F51C0, input 005F6B10).
        auto& d = client_services::dependencies;
        prior_client_ = d;
        client_published_ = true;
        d.allocate_006fa7e6 = &live_operator_new;         // existing task23 flag1 006FA7E6 provider
        d.tick_0073d0f8 = &Tick0073d0f8;                   // [lane-prepare] 00401188 IAT GetTickCount
        d.stats_004010c0 = &ConstructReentrantServerStats004010c0;      // [lane-prepare]
        d.object_table_004ae5d0 = &ConstructObjectTableManager004ae5d0; // [lane-prepare]
        d.internal_005fbfe0 = &ConstructClientInternal005fbfe0;         // [lane-prepare]
        d.rules_00552c50 = &kotor_original::kotor_fastcall_00552c50;        // [lane-prepare] rules_c2da.cpp
        d.load_screen_0067a710 = &kotor_original::kotor_fastcall_0067a710;   // [lane-prepare] load_screen.cpp
        d.string_005e5a90 = &ConstructStringText005e5a90;
        d.destroy_string_005e5c20 = &DestroyString005e5c20;
        d.get_int_00413660 = &kotor_rules_c2da::ClientGetIntEntry00413660; // adapter -> lane-startup-misc 00413660 (D38)
        d.gui_initialize_00632720 = &kotor_original::kotor_fastcall_00632720; // [lane-prepare] client_callees.cpp
        d.event_descriptions_005eeb10 = &SetEventDescriptions005eeb10;             // [lane-r4-lowers]
        d.movie_playing_0070c1f0 = &kotor_fastcall_0070c1f0;               // [lane-routes]
        d.unpauseable_0062b410 = &SetGlobalUnpauseableState0062b410; // [lane-r4-lowers]
        d.clear_events_005df4e0 = &kotor_fastcall_005df4e0;
        d.sound_mode_005d5e80 = &table29_required::gui_sound_mode_005d5e80; // [lane-sound] donor task-29 sound-mode-closure-r2
        d.input_active_005df540 = &kotor_fastcall_005df540;
        d.keyboard_off_005f2980 = &kotor_fastcall_005f2980;
        d.show_mouse_0061fa00 = &ShowMouse0061fa00;             // [lane-r4-lowers]
        d.rules_007a3a28 = &kotor_cell_rules_007a3a28;
        d.sound_007a39ec = &kotor_cell_sound_007a39ec;             // [lane-sound] owner
        d.sound_mode_0074c5f8 = &kotor_rdata_sound_mode_0074c5f8;   // [lane-sound] owner
        // lane-m48-b fork CLIENT: the former kotor_sound_publish_audio_providers() call is removed. audio::providers is
        // constant-initialized with all 19 slots in sound/audio_retyped.cpp (service-audio-15; sound_owners.cpp:48), so
        // there is nothing to publish and no lane defines that (non-original) publisher symbol.
        // F10: menu_normal / retained string and resource slots are immediates in the
        // original (0062FF38 call 005B3190; 0062FF6C/0062FF71 push 5E5C20/5B3190),
        // used by the module ctor 00643F40 (005F8D89) and CGuiInGame 0062FED0
        // (005F9472) inside StartServices - so they must be published before WinMain.
        const auto strings_result =
            task22_strings::attach(strings_join_, menu_normal_40_49::providers, menu_normal_retained_40_49::providers);
        if (strings_result != task22_strings::Result::PreparedFiveStringSlots) {
            ;
            return false;
        }
        strings_joined_ = true;
        const auto resource_result = task10_resource_cells::attach(resource_join_, strings_join_);
        if (resource_result != task10_resource_cells::Result::PreparedManagerCells) {
            ;
            return false;
        }
        resource_joined_ = true;
        // input_007a39e4 is joined by bind (previous value restored by finish).
        // 6. Window sources. Acquisition may throw; it records providers.owner
        //    first, so mark recorded before the call.
        window_sources_recorded_ = true;
        if (!kotor_window_sources_acquire(this, views_, &winmain_, &shell_, &window_)) {
            ;
            return false;
        }
        if (!heap_and_scope_match()) { ; return false; }
        // 7. Existing transactional bind: nine joins then window attach.
        GameBindings b{views_, &winmain_, &shell_, &start_, &client_services::dependencies,
                       &menu_normal_40_49::providers, &window_};
        if (!lifetime_.bind(b)) { ; return false; }
        phase_ = Phase::Bound;
        result_->window = kotor_menu_window_publication;
        return true; // Metadata prepared; not whole-game readiness.
    }
    void release(PreparedGame& result) noexcept {
        if (&result != result_ || phase_ == Phase::Releasing || phase_ == Phase::Released ||
            !release_scope_match() || !result_matches()) { // HX-1: release-time scope (formatter CheckingDrain)
            ;
            fail_closed_release(ReleaseRefusal::InvalidMetadata);
        }
        phase_ = Phase::Releasing; // Refuse recursive prepare/release.
        if (!lifetime_.detach_windows()) {
            ;
            fail_closed_release(ReleaseRefusal::WindowOwnershipRetained);
        }
        if (lifetime_.phase() == GameLifetime::Phase::Empty) {
            // bind never succeeded: drain any partial window-source acquisition.
            if (!drain(this)) {
                ;
                fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
            }
        } else if (!lifetime_.finish({this, &drain})) {
            ;
            fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
        }
        window_sources_recorded_ = false;
        window_ = {};
        if (!release_scope_match()) { // HX-1
            ;
            fail_closed_release(ReleaseRefusal::InvalidMetadata);
        }
        // Reverse of acquisition: client table, Start table, Strings, Views.
        // Reverse of acquisition (consumers drained by finish above).
        if (resource_joined_) {
            if (!task10_resource_cells::detach(resource_join_, true)) {
                ;
                fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
            }
            resource_joined_ = false;
        }
        if (strings_joined_) {
            if (!task22_strings::detach(strings_join_, true)) {
                ;
                fail_closed_release(ReleaseRefusal::AggregateOwnershipRetained);
            }
            strings_joined_ = false;
        }
        if (client_published_) {
            client_services::dependencies = prior_client_;
            client_published_ = false;
        }
        if (start_published_) {
            if (client_services_start::globals != &start_) {
                ;
                fail_closed_release(ReleaseRefusal::InvalidMetadata);
            }
            client_services_start::globals = prior_start_;
            start_published_ = false;
        }
        if (views_) {
            // Strings still use the live source heap family (005E5C20 -> 006FA390).
            if (strings_constructed_) destroy_shell_strings();
            strings_constructed_ = false;
            if (bias_published_) { publish_make_current_bias(nullptr); bias_published_ = false; }
            if (video_cells_attached_) { options_video::detach_video_mode_cells(); video_cells_attached_ = false; }
            if (video_attached_) { options_video::detach_saved_video_mode(); video_attached_ = false; }
            views_->~Views();
            views_ = nullptr;
        }
        result = {};
        phase_ = Phase::Released;
    }
};
}
}

extern "C" bool __cdecl kotor_functional_game_prepare(
    const kotor_functional::StartupArguments* args, kotor_functional::PreparedGame* result) {
    using namespace kotor_functional;
    // The original single || condition, split in identical order (pure reads / identity queries).
    if (!args) { ; return false; }
    if (!result) { ; return false; }
    if (result->lifetime || result->window || result->start) { ; return false; }
    if (active_owner) { ; return false; }
    if (!source_heap_identity()) { ; return false; }
    if (!locale_identity()) { ; return false; }
    if (!kotor_canonical_string_format::formatter_claim_admitted()) { ; return false; }
    if (!kotor_menu_window_shared_cells) { ; return false; }
    if (!kotor_menu_live_views::canonical(*kotor_menu_window_shared_cells)) { ; return false; }
    if (!live_globals.heap || !*live_globals.heap) { ; return false; }
    if (client_services_start::globals) { ; return false; }
    // Same game Win32 heap family as every other source owner.
    void* storage = live_operator_new(static_cast<std::uint32_t>(sizeof(PreparedOwner)));
    if (!storage) { ; return false; }
    auto* owner = ::new(storage) PreparedOwner(*live_globals.heap, *result);
    active_owner = owner;
    owner->record_result();
    return owner->prepare(*args);
}

extern "C" void __cdecl kotor_functional_game_release(kotor_functional::PreparedGame* result) noexcept {
    using namespace kotor_functional;
    if (!result) { ; fail_closed_release(ReleaseRefusal::InvalidMetadata); }
    if (!active_owner) {
        if (result->lifetime || result->window || result->start) {
            ;
            fail_closed_release(ReleaseRefusal::PreparedOwnershipRetained);
        }
        return; // No aggregate ownership acquired; no fabricated drain.
    }
    auto* owner = active_owner;
    owner->release(*result); // Any false genuine drain terminates with owner retained.
    active_owner = nullptr;
    owner->~PreparedOwner();
    live_free(owner); // Exact source allocator family, after all owner lifetimes.
}
