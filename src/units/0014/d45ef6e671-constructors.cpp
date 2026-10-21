#include "e7667768c0-internal.h"
#include <new>
#include <cstring>
#include "4743e70ebd-allocator_live.h"
namespace file_internal {
namespace {
// Support lifetimes corresponding only to the authenticated original C++
// unwind actions. No legacy handler, foreign exception or CRT state emulation.
struct AllocationCleanup {
 Internal* value;bool released=false;
 ~AllocationCleanup() noexcept {if(!released)live_free(value);}
};
struct InternalCleanup {
 Internal* value;bool completed=false;
 ~InternalCleanup() noexcept {if(!completed)value->~Internal();}
};
struct ResolvedOutput {
 alignas(4) unsigned char storage[sizeof(String)];bool live=false;
 String* address(){return reinterpret_cast<String*>(storage);}
 ~ResolvedOutput() noexcept {if(live)address()->~StartupString();}
 void destroy(){live=false;address()->~StartupString();}
};
}

File* __fastcall exofile_construct_005e68d0(File* self,void*,String* name,Word type,String* mode){
 auto* storage=static_cast<Internal*>(engine->allocate_006fa7e6(20));
 AllocationCleanup cleanup{storage};
 Internal* constructed=storage?internal_construct_005eb840(storage,nullptr,name,type,mode):nullptr;
 cleanup.released=true;
 self->internal=constructed;
 return self;
}
Internal* __fastcall internal_construct_005eb840(Internal* self,void*,String* name,Word type,String* mode){
 ::new (self) Internal; // Native C++ member construction handles name/mode partial completion.
 InternalCleanup cleanup{self};
 self->stream=nullptr;engine->assign_005e5c50(&self->mode,nullptr,mode);
 ResolvedOutput output;auto* resolved=output.address();
 void* base=*engine->exo_base_007a39e0;void* alias=nullptr;std::memcpy(&alias,static_cast<unsigned char*>(base)+12,4);
 String* result=engine->resolve_005e68a0(alias,nullptr,resolved,name,type);
 output.live=true; // Original state2 begins only after resolver return.
 engine->assign_005e5c50(&self->name,nullptr,result);output.destroy();
 self->stream=file_ops->open(engine->text_005e5670(&self->name,nullptr),engine->text_005e5670(mode,nullptr));
 cleanup.completed=true;return self;
}
}
