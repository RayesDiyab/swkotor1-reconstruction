#include "40141ba4d9-manager_destructor.h"
#include "c46300269d-allocator_live.h"
#include <cstring>

namespace menu_resources {
void __fastcall DestroyManager00408670(Manager* self,void*) {
    // Original loads the first handle before publishing the quit word.
    Word handle=self->word(0x38);
    self->word(0x48,1);
    // Even INVALID_HANDLE_VALUE and zero are passed to ResumeThread.
    (void)manager_resume_thread_0073d1dc(handle);
    handle=self->word(0x38); // actual caller reload after the opaque import
    if(handle!=0xffffffffu) {
        (void)manager_wait_single_0073d05c(handle,0xffffffffu);
        (void)manager_close_handle_0073d200(self->word(0x38));
    }
    // These are actual selected member types, whose lifetimes must already
    // have been started by the genuine Manager constructor owner.
    auto* text=std::launder(reinterpret_cast<String*>(self->bytes+0x4c));
    text->~StartupString();
    auto* critical=std::launder(reinterpret_cast<kotor_startup::CriticalSection*>(self->bytes+0x40));
    critical->Destroy();
    critical->~CriticalSection(); // trivial support lifetime end; no new entry
    // Each next slot is loaded after all cleanup of the preceding slot.
    // List cleanup frees nodes only; the caller then frees the internal.
    for(unsigned offset=0x24;;offset-=4) {
        kotor_list::Internal* list;
        std::memcpy(&list,self->bytes+offset,sizeof list);
        if(list) {
            base_ini_reconstruction::destroy_list_005e9ab0(list,nullptr);
            list->~Internal(); // trivial support lifetime end
            live_free(list);
        }
        if(offset==0x10)break;
    }
    // No handle, ID, quit, list pointer, payload or Manager allocation clear.
    // Original EH/FS state transitions and unwind handlers are not reproduced.
}
}
