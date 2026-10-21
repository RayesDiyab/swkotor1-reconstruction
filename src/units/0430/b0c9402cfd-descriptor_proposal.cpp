#include "001abaa274-modern_boundaries.h"
#include "90c0a5b35a-allocator_live.h"
#include "50062ae20e-native_strings.h"
#include "73922e061e-canonical_formatter_owner.h"
#include "31abfab473-warning_runtime.h"
#include <type_traits>
namespace {
using String=kotor_reconstruction::StartupString;
// Transport from the existing R20 borrowed-assignment donor; canonical method
// retains algorithm ownership. No function-pointer convention casts.
String* __fastcall assign(String* self,void*,const String* rhs){
 return self->Assign005e5c50(const_cast<String*>(rhs));
}
const char* __fastcall text(String* self,void*){
 return WindowCStr005e5670(self,nullptr);
}
}
namespace tlk_host {
tlk_file::Bindings MakeFileBindings(const char* mode){
 return {&file_internal::exofile_construct_005e68d0,
         &file_internal::exofile_seek_beginning_005e6ad0,
         &file_internal::exofile_read_005e6960,
         &file_internal::internal_destroy_005eb900,&live_free,mode};
}
file_internal::EngineBindings MakeFileEngineBindings(file_internal::EngineBindings required){
 required.allocate_006fa7e6=&live_operator_new;
 required.assign_005e5c50=&assign;
 required.text_005e5670=&text;
 required.format_005e5680=&kotor_reconstruction::StartupFormat005e5680;
 return required;
}
}
// Main owns the only tlk_file::bindings, file_internal::engine/file_ops cells,
// actual descriptor lifetimes, publication order and successful drain. This
// proposal defines no cells, singleton or original data literals.
static_assert(std::is_same<decltype(file_internal::EngineBindings::allocate_006fa7e6),decltype(&live_operator_new)>::value,"one canonical allocation route");
