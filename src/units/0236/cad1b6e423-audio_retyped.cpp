// 005F1F70 CClientExoAppInternal::InitializeSoundOptions on re-typed providers.
// Control flow and quirks are the M40 task-33 donor's (audio.cpp 7d789a13...7661,
// AUDIO_MUTANT=0), evidence 005F1F70-005F297E; only the provider types changed.
#include "0c26cce844-audio_retyped.hpp"
#include "dfa85b0422-sound_path.h"                      // kotor_sound_path CExoSound bodies
#include "9252d3b484-warning_runtime.h"         // kotor_reconstruction::StartupFormat005e5680
#include "5c6f651f13-window_shared_cells.h"
#include "39c670fc66-allocator_live.h"
extern "C" void* kotor_cell_sound_007a39ec;   // lane-sound owner (sound_owners.cpp)
extern "C" double __cdecl kotor_cdecl_006fc5c0(const char* text);   // atof 006FC5C0 port (claude_vm_actions_strings)
#include <cstdlib>
#include <cstring>
#include <new>

namespace audio {
// Every slot bound to its sole existing producer at static-initialization time.
Providers providers = {
    &Ini::construct_005e6750, &Ini::read_005e67c0, &Ini::write_005e67d0, &Ini::destroy_005e67e0,
    &kotor_menu20::String::as_int_005e5630,                      // service-menu20-4.obj
    &kotor_sound_path::StringAsFloat005e5650,                    // this unit
    &kotor_reconstruction::StartupFormat005e5680,                // canonical-assignment-format.obj
    &live_operator_new,                                          // live-policy.obj (006FA7E6)
    &kotor_sound_path::SoundConstruct005d5bc0,
    &kotor_sound_path::SoundSetNonStreamingEffects005d6440,
    &kotor_sound_path::SoundSetStreamingEffects005d6430,
    &kotor_sound_path::SoundSetBias005d6320,
    &kotor_sound_path::SoundGetEax005d6450,
    &kotor_sound_path::SoundGetBias005d6380,
    &kotor_sound_path::SoundVoices2d005d63a0,
    &kotor_sound_path::SoundVoices3d005d63b0,
    &kotor_menu20::ClientApp::get_options_005ed700,              // service-menu20-4.obj
    &kotor_menu_window_cells::manager_007a39fc,                  // window-shared-cells.obj
    &kotor_cell_sound_007a39ec,                                  // lane-sound owner
};
namespace {
const char* const keys[] = {"Sound Init", "Number 2D Voices", "Number 3D Voices", "EAX", "Force Software",
                            "Environment Effects Nonstreaming", "Environment Effects Streaming", "2D3D Bias"};
// Raw storage so the original's reads of destroyed String words (005F20F4/005F20F9
// write only AL over them) stay byte reads of storage, never of a dead object.
struct Slot {
    alignas(4) unsigned char bytes[sizeof(String)];
    String* get() { return std::launder(reinterpret_cast<String*>(bytes)); }
    void text(const char* p) { ::new (static_cast<void*>(bytes)) String(p); }   // 005E5A90
    void destroy() { get()->~StartupString(); }                                  // 005E5C20
    U word0() const { U v; std::memcpy(&v, bytes, 4); return v; }
};
int transaction(Ini& ini, String& out, unsigned index, bool write, Slot& key, Slot& section, Slot& file) {
    key.text(keys[index]); section.text("Sound Options"); file.text(".\\swkotor.ini");
    int r = (ini.*(write ? providers.ini_write : providers.ini_read))(&out, file.get(), section.get(), key.get());
    file.destroy(); section.destroy(); key.destroy();
    return r;
}
int transaction(Ini& ini, String& out, unsigned index, bool write) {
    Slot key, section, file;
    return transaction(ini, out, index, write, key, section, file);
}
U as_int(String& s) { return static_cast<U>((reinterpret_cast<kotor_menu20::String&>(s).*providers.as_int)()); }
void* client() {                                                       // [[007A39FC]+4]
    void* manager = *providers.manager_slot; void* c; std::memcpy(&c, static_cast<unsigned char*>(manager) + 4, 4);
    return c;
}
unsigned char* settings(void* c) {
    return static_cast<unsigned char*>((static_cast<kotor_menu20::ClientApp*>(c)->*providers.settings)());
}
}

extern "C" U __stdcall audio_005f1f70() {
    alignas(4) unsigned char ini_bytes[sizeof(Ini)];
    Ini& ini = *reinterpret_cast<Ini*>(ini_bytes);
    (ini.*providers.ini_ctor)();                                        // 005F1F96
    Slot unused_slot, out_slot;
    ::new (static_cast<void*>(unused_slot.bytes)) String();            // 005F1FA5 (005B3190)
    ::new (static_cast<void*>(out_slot.bytes)) String();               // 005F1FB3 (005B3190)
    String& out = *out_slot.get();
    U eax_mode = 3, force_software = 0, sound_init = 0;                // 005F1FC7 / 005F1FCF / 005F1F9B
    Slot key0, section0, file0;
    if (transaction(ini, out, 0, false, key0, section0, file0)) sound_init = as_int(out); // 005F2019-005F2056
    providers.format(&out, "%d", 1U); transaction(ini, out, 0, true, key0, section0, file0); // 005F2064-005F20BD
    U voices2d = (section0.word0() & 0xffffff00U) | 24U;               // 005F20F4
    U voices3d = (key0.word0() & 0xffffff00U) | 16U;                   // 005F20F9
    if (transaction(ini, out, 1, false)) voices2d = (voices2d & 0xffffff00U) | (as_int(out) & 255U); // 005F2183
    if (transaction(ini, out, 2, false)) voices3d = (voices3d & 0xffffff00U) | (as_int(out) & 255U); // 005F2215
    if (sound_init) { eax_mode = 0; force_software = 1; }               // 005F2219-005F222A
    else {
        if (transaction(ini, out, 3, false)) eax_mode = as_int(out);    // 005F22BD
        if (transaction(ini, out, 4, false)) {
            force_software = as_int(out);                               // 005F234A
            std::memcpy(settings(client()) + 0x4c, &force_software, 4); // 005F234F-005F235F
        }
    }
    void* memory = providers.allocate(4);                               // 005F2364
    U hardware = (force_software == 0) ? 1U : 0U;                       // 005F237F-005F2381
    *providers.sound_slot = memory ? providers.sound_ctor(memory, nullptr, voices2d, voices3d, eax_mode, hardware) : nullptr; // 005F2392-005F23A8
    if (transaction(ini, out, 5, false)) { U n = as_int(out); providers.set_nonstream(*providers.sound_slot, nullptr, n); } // 005F2439
    if (transaction(ini, out, 6, false)) { U n = as_int(out); providers.set_stream(*providers.sound_slot, nullptr, n); }    // 005F24D3
    if (transaction(ini, out, 7, false)) { float n = providers.as_float(&out, nullptr); providers.set_bias(*providers.sound_slot, nullptr, n); } // 005F2561-005F2570
    U v = providers.get_eax(*providers.sound_slot, nullptr);            // 005F257B
    providers.format(&out, "%d", v); transaction(ini, out, 3, true);
    void* saved_client = client();                                      // 005F2612-005F2618
    v = providers.get_eax(*providers.sound_slot, nullptr);              // 005F2621
    settings(saved_client)[0x49] = static_cast<unsigned char>(v);       // 005F262C-005F2636
    providers.format(&out, "%d", force_software); transaction(ini, out, 4, true);
    double bias = providers.get_bias(*providers.sound_slot, nullptr);   // 005F26D0 (ST0)
    providers.format(&out, "%.2f", bias); transaction(ini, out, 7, true);
    providers.format(&out, "%d", providers.get_2d(*providers.sound_slot, nullptr) & 255U); transaction(ini, out, 1, true);
    providers.format(&out, "%d", providers.get_3d(*providers.sound_slot, nullptr) & 255U); transaction(ini, out, 2, true);
    providers.format(&out, "%d", 0U); transaction(ini, out, 0, true);   // 005F28AC-005F2911
    out_slot.destroy();                                                 // 005F2948
    unused_slot.destroy();                                              // 005F2956
    (ini.*providers.ini_dtor)();                                        // 005F2967
    return 0;
}
}

namespace kotor_sound_path {
// 005E5650 CExoString::AsFLOAT: data ? atof(data) (006FC5C0) : [0073D700] (= 0.0f).
// Lane snapshot: atof bound to the faithful port of the original static-CRT atof 006FC5C0 (claude_vm_actions_strings
// _kotor_cdecl_006fc5c0: _fltin2 / __strgtold12 / __ld12tod) instead of UCRT atof.
float __fastcall StringAsFloat005e5650(kotor_reconstruction::StartupString* self, void*) {
    const char* data = self->data;
    if (!data) return 0.0f;                                             // 005E5660
    return static_cast<float>(kotor_cdecl_006fc5c0(data));              // 005E5656-005E565F (ST0 narrowed by consumer)
}
}
