#include "35e3e29e81-live_publication.h"
#include "2b112e8e36-global_aliases.h"
#include <cstdint>

namespace kotor_live_publication {
namespace {
bool owner(const Attachment& a) noexcept {
    return &a == kotor_menu_window_attachment &&
           kotor_menu_window_publication == &a.publication;
}
bool aliases(const kotor_menu_live_views::Views& v) noexcept {
    const auto* c = kotor_menu_window_shared_cells;
    if (!c || !kotor_menu_live_views::canonical(*c)) return false;
    return v.winmain.manager_7a39fc.address() == c->manager_007a39fc &&
        v.winmain.reinit_7a3a2c.address() == c->reinit_007a3a2c &&
        v.winmain.height_78d1d8.address() == c->height_0078d1d8 &&
        v.winmain.hwnd_7a39d8.address() == c->render_007a39d8 &&
        v.shell.top_window_7a39d4.address() == c->top_007a39d4 &&
        v.shell.render_window_7a39d8.address() == c->render_007a39d8 &&
        v.shell.instance_7a39c0.address() == c->instance_007a39c0 &&
        v.reinit.aurora_7a39f8.address() == c->aurora_007a39f8 &&
        v.reinit.manager_7a39fc.address() == c->manager_007a39fc &&
        v.reinit.input_7a39e4.address() == c->input_007a39e4 &&
        v.reinit.top_7a39d4.address() == c->top_007a39d4 &&
        v.reinit.render_7a39d8.address() == c->render_007a39d8 &&
        v.reinit.instance_7a39c0.address() == c->instance_007a39c0 &&
        v.reinit.reinit_7a3a2c.address() == c->reinit_007a3a2c &&
        v.message.render_7a39d8.address() == c->render_007a39d8 &&
        v.message.input_7a39e4.address() == c->input_007a39e4 &&
        v.message.manager_7a39fc.address() == c->manager_007a39fc &&
        v.message.aurora_7a39f8.address() == c->aurora_007a39f8 &&
        v.message.reinit_7a3a2c.address() == c->reinit_007a3a2c &&
        v.message.height_78d1d8.address() == c->height_0078d1d8;
}
bool live(Attachment& a) noexcept {
    return owner(a) && a.phase != Phase::Empty && a.views &&
        aliases(*a.views) && a.providers.ready(a.providers.owner);
}
bool overlap(const void* p, std::size_t n, const void* q, std::size_t m) noexcept {
    const auto x = reinterpret_cast<std::uintptr_t>(p);
    const auto y = reinterpret_cast<std::uintptr_t>(q);
    return x <= y ? y - x < n : x - y < m;
}
template<class C> bool overlaps_list(const void* frame, std::size_t size,
                                    Invocation<C>* node) noexcept {
    for (; node; node = node->previous)
        if (overlap(frame, size, node->context->frame, sizeof(*node->context->frame)))
            return true;
    return false;
}
bool frame_free(Attachment& a, const void* frame, std::size_t n) noexcept {
    return !overlap(frame, n, a.publication.winmain->frame,
                    sizeof(*a.publication.winmain->frame)) &&
        !overlap(frame, n, a.publication.shell->frame,
                 sizeof(*a.publication.shell->frame)) &&
        !overlaps_list(frame,n,a.reinit) && !overlaps_list(frame,n,a.message) &&
        !overlaps_list(frame,n,a.retiring_reinit) && !overlaps_list(frame,n,a.retiring_message);
}
template<class C> bool contains(Invocation<C>* top, Invocation<C>* p) noexcept {
    for (; top; top = top->previous) if (top == p) return true;
    return false;
}
template<class C> C* enter(Attachment& a, Invocation<C>* p, Invocation<C>*& top,
                          Invocation<C>* retired, C*& slot) noexcept {
    if (!p) return nullptr;
    // Reused live nodes cannot safely be released as a rejected new lease.
    if (p->active || contains(top,p) || contains(retired,p)) {
        ++a.violations; return nullptr;
    }
    auto* c = p->context;
    if (!c || !c->frame || !frame_free(a,c->frame,sizeof(*c->frame))) {
        ++a.violations; return nullptr;
    }
    p->previous = top; p->prior_bound = slot; p->active = true; top = p;
    slot = c; // Ready before any source/native callback can run.
    return c;
}
template<class C> void leave(Attachment& a, C* c, Invocation<C>*& top,
                            Invocation<C>*& retiring, C*& slot,
                            void (__cdecl* release)(void*,Invocation<C>*) noexcept) noexcept {
    if (!top || top->context != c) { ++a.violations; return; }
    auto* p = top; top = p->previous;

    slot = p->prior_bound;



    auto* old_retiring = retiring;
    p->previous = old_retiring; retiring = p; p->active = false;
    // Matching owner cleanup runs after restoring the outer Context. Retiring
    // frames remain unavailable if cleanup synchronously invokes a callback.
    release(a.providers.owner,p);
    retiring = old_retiring; // Do not read the released node again.
}
kotor_reinit::Context* __cdecl enter_reinit() {
    auto* a = kotor_menu_window_attachment;
    if (!a || !live(*a)) return nullptr;
    auto* p = a->providers.acquire_reinit(a->providers.owner);
    if (!p) return nullptr;
    auto* c = p->context;
    if (!c || c->globals != &a->views->reinit || !c->constants ||
        !c->frame || !c->dependencies.invoke || !c->dependencies.read_word) {
        ++a->violations;
        if (!p->active && !contains(a->reinit,p) && !contains(a->retiring_reinit,p))
            a->providers.release_reinit(a->providers.owner,p);
        return nullptr;
    }
    return enter(*a,p,a->reinit,a->retiring_reinit,kotor_reinit::bound_context);
}
void __cdecl leave_reinit(kotor_reinit::Context* c) noexcept {
    auto* a = kotor_menu_window_attachment;
    if (a && owner(*a)) leave(*a,c,a->reinit,a->retiring_reinit,
        kotor_reinit::bound_context,a->providers.release_reinit);
}
kotor_message::Context* __cdecl enter_message(std::uint32_t hwnd,std::uint32_t msg,
                                             std::uint32_t wp,std::uint32_t lp) {
    auto* a = kotor_menu_window_attachment;
    if (!a || !live(*a)) return nullptr;
    auto* p = a->providers.acquire_message(a->providers.owner,hwnd,msg,wp,lp);
    if (!p) return nullptr;
    auto* c = p->context;
    if (!c || c->globals != &a->views->message || !c->frame ||
        !c->dependencies.invoke || !c->dependencies.read_word || !c->dependencies.write_word) {
        ++a->violations;
        if (!p->active && !contains(a->message,p) && !contains(a->retiring_message,p))
            a->providers.release_message(a->providers.owner,p);
        return nullptr;
    }
    return enter(*a,p,a->message,a->retiring_message,kotor_message::bound_context);
}
void __cdecl leave_message(kotor_message::Context* c) noexcept {
    auto* a = kotor_menu_window_attachment;
    if (a && owner(*a)) leave(*a,c,a->message,a->retiring_message,
        kotor_message::bound_context,a->providers.release_message);
}
}
bool attach(Attachment& a, kotor_menu_live_views::Views& v,
            kotor_winmain::Context& w, kotor_window_shell::Context& s,
            const Providers& p) noexcept {
    if (!owner(a) || a.phase != Phase::Empty || a.views || a.reinit || a.message ||
        a.retiring_reinit || a.retiring_message || a.publication.winmain ||
        a.publication.shell || a.publication.enter_reinit || a.publication.leave_reinit ||
        a.publication.enter_message || a.publication.leave_message ||
        !aliases(v) ||

        w.globals != &v.winmain ||

        !w.frame ||
        !w.dependencies.invoke || !w.dependencies.read_word ||
        s.globals != &v.shell || !s.constants || !s.frame || !s.dependencies.invoke ||
        overlap(w.frame,sizeof(*w.frame),s.frame,sizeof(*s.frame)) ||
        !p.owner || !p.ready || !p.acquire_reinit || !p.release_reinit ||
        !p.acquire_message || !p.release_message || !p.quiesce || !p.ready(p.owner))
        return false;
    a.views = &v; a.providers = p;
    a.prior_winmain = kotor_winmain::bound_context;
    a.prior_shell = kotor_window_shell::bound_context;
    a.prior_reinit = kotor_reinit::bound_context;
    a.prior_message = kotor_message::bound_context;
    a.publication = {&w,&s,enter_reinit,leave_reinit,enter_message,leave_message};
    kotor_winmain::bound_context = &w; kotor_window_shell::bound_context = &s;
    a.phase = Phase::Live;
    return true;
}
bool detach(Attachment& a) noexcept {
    if (!owner(a)) return false;
    if (a.phase == Phase::Empty) return true;
    if (a.quiescing) return false;

    if (a.reinit || a.message || a.retiring_reinit || a.retiring_message) return false;

    a.phase = Phase::Draining;
    // DestroyWindow may synchronously call WndProc; keep the descriptor, outer
    // bindings, frame hooks and providers valid throughout real quiescence.
    a.quiescing = true;
    const bool drained = a.providers.quiesce(a.providers.owner);
    a.quiescing = false;
    if (!drained) return false;
    if (a.reinit || a.message || a.retiring_reinit || a.retiring_message ||
        kotor_winmain::bound_context != a.publication.winmain ||
        kotor_window_shell::bound_context != a.publication.shell ||
        kotor_reinit::bound_context != a.prior_reinit ||
        kotor_message::bound_context != a.prior_message) return false;
    kotor_winmain::bound_context = a.prior_winmain;
    kotor_window_shell::bound_context = a.prior_shell;
    a.publication = {};
    a.views = nullptr; a.providers = {};
    a.prior_winmain = nullptr; a.prior_shell = nullptr;
    a.prior_reinit = nullptr; a.prior_message = nullptr;
    a.phase = Phase::Empty;
    return true;
}
}
