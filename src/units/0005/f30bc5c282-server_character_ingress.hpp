#pragma once
// Declarations derived from the selected committed implementation.
#include "00a0946121-resources.h"
using IngressRef16 = menu_resources::Ref;
static_assert(sizeof(IngressRef16) == 16);
extern "C" unsigned __fastcall recovered_server_login_005244e0( void* message, void*, void* player, unsigned minor);
extern "C" unsigned __fastcall recovered_server_character_004ae700( void* outer, void*, unsigned minor, void* player, IngressRef16 reference, const void* bytes, unsigned length);
extern "C" unsigned __fastcall recovered_server_character_internal_004b7470( void* internal, void*, unsigned minor, void* player, IngressRef16 reference, const void* bytes, unsigned length);
extern "C" unsigned __fastcall recovered_file_write_005e69a0( void* file, void*, const void* bytes, unsigned size, unsigned count);
