#pragma once
#include "cf1b7598d7-async_load.h"
#include "8d6a860064-add_key.h"
// Opaque allocation identity follows Main's sole constructor; no Key overlay.
extern "C" void __fastcall async_unload_key_0040d2e0(void*,void*);
extern "C" __declspec(noinline) async_load::Word __fastcall
async_remove_key_00407830(async_load::Manager*,void*,async_load::String*,async_load::Word);
