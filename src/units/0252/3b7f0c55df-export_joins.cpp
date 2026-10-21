// Native x86 symbol joins only. The guarded link census must establish that
// each requested name is undefined and its real owner is defined exactly once.
// No callable wrapper, copied body, data object, initializer or va_list forwarding.
#include "6e25a82454-startup_views.h"
#include "4ebe22b2dc-dependencies.h"
#include "c534281817-startup_string.h"
#include "66a5366142-menu_targets.h"
#include "9281d0a58c-warning_runtime.h"
#include "7b8c81fcff-allocator_live.h"
#include "c7e191db10-native_strings.h"
#include "c30aaa70eb-resources.h"
#include <type_traits>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error These aliases require the inspected MSVC x86 names and ABI.
#endif
using BorrowedString = kotor_menu_10_19::String;
using LifetimeString = kotor_reconstruction::StartupString;
using FormatterString = kotor_reconstruction::StartupString;
static_assert(sizeof(void*) == 4 && sizeof(BorrowedString) == 8 &&
              alignof(BorrowedString) == 4 && sizeof(LifetimeString) == 8 &&
              alignof(LifetimeString) == 1 && sizeof(FormatterString) == 8 &&
              alignof(FormatterString) == 1, "Inspected native string views");
static_assert(offsetof(BorrowedString, bytes) == 0 &&
              offsetof(LifetimeString, data) == 0 &&
              offsetof(LifetimeString, capacity) == 4 &&
              offsetof(FormatterString, data) == 0 &&
              offsetof(FormatterString, capacity) == 4, "Same two native words");
using Allocation = void* (__cdecl*)(std::uint32_t);
static_assert(std::is_same<decltype(&live_operator_new), Allocation>::value &&
              std::is_same<decltype(&kotor_menu_10_19::dependencies::allocate_006fa7e6), Allocation>::value,
              "006FA7E6 is operator_new, not the distinct malloc policy");
using BorrowedFormat = void (__cdecl*)(BorrowedString*, const char*, ...);
using OwnerFormat = void (__cdecl*)(FormatterString*, const char*, ...);
static_assert(std::is_same<decltype(&kotor_menu_10_19::dependencies::format_005e5680), BorrowedFormat>::value &&
              std::is_same<decltype(&kotor_reconstruction::StartupFormat005e5680), OwnerFormat>::value,
              "Both native entries are cdecl variadic with the same two fixed words");
using BorrowedText = const char* (__thiscall BorrowedString::*)();
using BorrowedCopy = BorrowedString* (__thiscall BorrowedString::*)(const BorrowedString*);
using OwnerText = char* (__fastcall*)(LifetimeString*, void*);
using OwnerCopy = LifetimeString* (__fastcall*)(LifetimeString*, void*, const LifetimeString*);
static_assert(std::is_same<decltype(&BorrowedString::text_005e5670), BorrowedText>::value &&
              std::is_same<decltype(&BorrowedString::copy_005e5b00), BorrowedCopy>::value &&
              std::is_same<decltype(&WindowCStr005e5670), OwnerText>::value &&
              std::is_same<decltype(&WindowCopy005e5b00), OwnerCopy>::value,
              "ECX receiver; fastcall EDX is unused by both owners; RET0/RET4 equal");
using BorrowedRefDefault = kotor_menu_10_19::ResRef* (__thiscall kotor_menu_10_19::ResRef::*)();
static_assert(std::is_same<decltype(&kotor_menu_10_19::ResRef::default_00405ed0), BorrowedRefDefault>::value &&
              sizeof(kotor_menu_10_19::ResRef) == 16 && sizeof(menu_resources::Ref) == 16,
              "00405ED0: ECX receiver, sixteen native bytes, RET0, EAX receiver");
// /alternatename resolves only an otherwise undefined name. The delivery guard
// rejects an overlapping exporter; this is not a runtime fallback. Receiver,
// return address, stack, registers, EAX and all variadic words reach the owner
// unchanged, because the linker uses the same function address, with no thunk.
// Borrowed String is a native eight-byte storage view; this does not establish
// a complete portable C++ object model or construct/publish a production table.
#pragma comment(linker, "/alternatename:?allocate_006fa7e6@dependencies@kotor_menu_10_19@@YAPAXI@Z=_live_operator_new")
#pragma comment(linker, "/alternatename:?default_005b3190@String@kotor_menu_10_19@@QAEPAU12@XZ=??0StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:?destroy_005e5c20@String@kotor_menu_10_19@@QAEXXZ=??1StartupString@kotor_reconstruction@@QAE@XZ")
#pragma comment(linker, "/alternatename:?format_005e5680@dependencies@kotor_menu_10_19@@YAXPAUString@2@PBDZZ=?StartupFormat005e5680@kotor_reconstruction@@YAXPAUStartupString@1@PBDZZ")
#pragma comment(linker, "/alternatename:?text_005e5670@String@kotor_menu_10_19@@QAEPBDXZ=@WindowCStr005e5670@8")
#pragma comment(linker, "/alternatename:?copy_005e5b00@String@kotor_menu_10_19@@QAEPAU12@PBU12@@Z=@WindowCopy005e5b00@12")
#pragma comment(linker, "/alternatename:?default_00405ed0@ResRef@kotor_menu_10_19@@QAEPAU12@XZ=@c2da_default_ref_00405ed0@8")
// lane-tlk: M40 nominal member -> canonical FetchInternal (0041E1A0). Consumer service-retained-21
// (simple_0041e8f0, 0041E948 call 0041E1A0). thiscall RET0Ch (ECX=this, 3 stack words, EAX int)
// == __fastcall(Tlk*, EDX unused, Word, StringResource*, int) RET0Ch; same function address, no thunk.
#pragma comment(linker, "/alternatename:?fetch_0041e1a0@Tlk@kotor_menu_10_19@@QAEHIPAUStringResource@2@H@Z=?FetchInternal0041e1a0@tlk_core@@YIHPAUTlk@kotor_menu_10_19@@PAXIPAUStringResource@1@H@Z")
