#include "40141ae87f-ini_cleanup.h"

namespace base_ini_reconstruction {

// Normal, nonthrowing destruction only. These entry carriers put the receiver
// in ECX and ignore EDX; no original FS/EH unwind machinery is reconstructed.
void __fastcall destroy_section_005ea5a0(Section* section, void*) {
    List* lines = section->lines.internal;
    if (lines) {
        destroy_list_005e9ab0(lines, nullptr);
        live_free(lines);
    }
    section->name()->~String();
}

void __fastcall destroy_cache_005ea650(Ini* ini, void*) {
    List* sections = ini->sections.internal;
    // The original dereferences the internal list even when the cache is empty.
    // A null internal list is invalid state, not successful empty destruction.
    if (sections->head) {
        do {
            Section* section = static_cast<Section*>(
                remove_head_005e9c80(sections, nullptr));
            List* lines = section->lines.internal;
            if (lines->head) {
                do {
                    String* line = static_cast<String*>(
                        remove_head_005e9c80(lines, nullptr));
                    if (line) {
                        line->~String();
                        live_free(line);
                    }
                    // Reload the wrapper after the payload destructor/free.
                    lines = section->lines.internal;
                } while (lines->head);
            }
            destroy_section_005ea5a0(section, nullptr);
            live_free(section);
            sections = ini->sections.internal;
        } while (sections->head);
    }
    bindings.assign_cstr_005e5140(ini->filename(), nullptr, "");
}

void __fastcall destroy_internal_005eb500(Ini* ini, void*) {
    destroy_cache_005ea650(ini, nullptr);
    ini->filename()->~String();
    // Save the list allocation before its destructor; do not free the Ini here.
    List* sections = ini->sections.internal;
    if (sections) {
        destroy_list_005e9ab0(sections, nullptr);
        live_free(sections);
    }
}

} // namespace base_ini_reconstruction
