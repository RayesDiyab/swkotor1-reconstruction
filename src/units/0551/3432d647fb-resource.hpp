#pragma once
#include "905c21d6b5-owner.hpp" // E1 epoch chain
namespace texture_resource {
using U=texture::U;using I=texture::I;using Basic=texture::Basic;using List=texture::List;
// Dependency views only; main6 owns all real cells and their storage/lifetimes.
struct Bindings {acquisition::ReleaseRaster const volatile* image_callback;List* scan;List* d3_on;List* d3_off;}; // E1 edit F6: typed [007A46F8] cell
extern Bindings bindings;
extern "C" {
 U __fastcall free_image_0041f570(Basic*,void*);
 U __fastcall remove_ordered_00421650(Basic*,void*);
 U __cdecl image_amount(Basic*,U);
 void* __cdecl remove_method_address();
 U __cdecl remove_raw(void*,List*,U);
}
}
