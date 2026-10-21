#pragma once
// lane-packed-file: CExoPackedFile family (abstract 0073E128, CExoResFile/BIF
// 0073E1B0, CExoEncapsulatedFile/ERF-MOD-HAK 0073E1F8, CExoResourceImageFile/RIM
// 0073E240). Objects are raw observed-offset storage (no nominal class is claimed):
//   +00 method table  +04 CExoString name (8)  +0C u16  +10 file/image size
//   +14 CExoFile* sync  +18 CExoFile* async  +1C ref count (signed)
//   +20 async ref count (signed)  +24 sync open flag  +28 async open flag
//   +2C header loaded
// CExoResFile (0x38): +30 BIFF header (0x14 alloc)  +34 entry table (16-byte rows)
// CExoEncapsulatedFile (0x44): +30 CExoLocString (8)  +38 ERF header (0xA0 alloc)
//   +3C resource list (8-byte rows: offset,size)  +40 u8 archive mode (0..4)
// CExoResourceImageFile (0x34, built inline by 0040F990): +30 whole-file image
//
// Slot transport (DECISIONS.md D1): every original slot is __thiscall (ECX receiver,
// callee pop). Slots are typed ABI-identical __fastcall(self, unused EDX, ...);
// callers pass nullptr for EDX. No function-pointer casts anywhere in this lane.
#include "1f4ee3e3f4-startup_string.h"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace kotor_packed_file {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
using DeletingDestroy = void* (__fastcall*)(void* self, void* unused_edx, Word flags);   // RET4
using Effect = Word (__fastcall*)(void* self, void* unused_edx);                          // RET0
using Getter = void* (__fastcall*)(void* self, void* unused_edx);                         // RET0
using Sized = Word (__fastcall*)(void* self, void* unused_edx, Word key);                 // RET4
using Transfer = Word (__fastcall*)(void* self, void* unused_edx, Word key, void* buffer, Word size, Word unused); // RET10h
using AsyncTransfer = void (__fastcall*)(void* self, void* unused_edx, Word key, void* buffer, Word size, Word unused); // RET10h, EAX unclaimed
using LoadHeader = Word (__fastcall*)(void* self, void* unused_edx, Word mode);           // RET4, low byte read
using Locate = void* (__fastcall*)(void* self, void* unused_edx, Word key);               // RET4 (RIM +44)

// Seventeen slots common to all four tables (0x00..0x40), original order.
struct Methods {
    DeletingDestroy deleting_destroy;  // +00
    Effect add_ref;                    // +04 AddRefCount
    Effect add_async_ref;              // +08 AddAsyncRefCount
    Effect close_file;                 // +0C CloseFile
    Effect close_async_file;           // +10 CloseAsyncFile
    Effect delete_ref;                 // +14 DeleteRefCount
    Effect delete_async_ref;           // +18 DeleteAsyncRefCount
    Getter get_file;                   // +1C GetFile (sync CExoFile*)
    Getter get_async_file;             // +20 (00644760, async CExoFile*)
    Sized get_resource_size;           // +24 GetResourceSize
    Effect initialize;                 // +28 Initialize
    Effect open_file;                  // +2C OpenFile
    Effect open_async_file;            // +30 OpenAsyncFile
    Transfer read_resource;            // +34 ReadResource
    AsyncTransfer read_resource_async; // +38 ReadResourceAsync
    LoadHeader load_header;            // +3C LoadHeader
    Effect unload_header;              // +40 UnloadHeader
};
static_assert(std::is_standard_layout<Methods>::value, "slot offsets");
static_assert(sizeof(Methods) == 0x44, "seventeen x86 slots");
static_assert(offsetof(Methods, open_file) == 0x2c && offsetof(Methods, get_resource_size) == 0x24 &&
              offsetof(Methods, read_resource) == 0x34 && offsetof(Methods, load_header) == 0x3c,
              "consumer slots (f990 +2C, f3c0/fb80 +3C, 00407E00/00407BD0 +24/+34/+38)");

// CExoResourceImageFile table 0073E240: the common seventeen plus +44 GetResource.
// The common part is the first member, so a table pointer read from an object is a
// valid pointer to its Methods subobject (shared bodies such as 0040D580 read it so).
struct ImageMethods {
    Methods packed;      // +00..+40
    Locate get_resource; // +44
};
static_assert(std::is_standard_layout<ImageMethods>::value, "slot offsets");
static_assert(sizeof(ImageMethods) == 0x48 && offsetof(ImageMethods, get_resource) == 0x44,
              "eighteen x86 slots; 0073E288 is a separate binary64 literal");
}

extern "C" {
// Method tables (original .rdata 0073E128 / 0073E1B0 / 0073E1F8 / 0073E240).
extern const kotor_packed_file::Methods packed_file_methods_0073e128;
extern const kotor_packed_file::Methods res_file_methods_0073e1b0;
extern const kotor_packed_file::Methods encapsulated_file_methods_0073e1f8;
extern const kotor_packed_file::ImageMethods image_file_methods_0073e240;

// CExoPackedFile.
void __fastcall packed_file_destroy_0040d560(void* self, void* unused_edx);
void* __fastcall packed_file_deleting_destroy_0040ee60(void* self, void* unused_edx, kotor_packed_file::Word flags);
void* __fastcall packed_file_get_file_0040d570(void* self, void* unused_edx);
void* __fastcall packed_file_get_async_file_00644760(void* self, void* unused_edx);
void __cdecl packed_purecall_006fb8de(void);

// Shared CExoResFile bodies (BIF, ERF; 0040D580 also RIM +04).
kotor_packed_file::Word __fastcall res_file_add_ref_0040d580(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall res_file_add_async_ref_0040d5a0(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall res_file_delete_ref_0040d650(void*, void*);
kotor_packed_file::Word __fastcall res_file_delete_async_ref_0040d670(void*, void*);
kotor_packed_file::Word __fastcall res_file_close_async_0040db50(void*, void*);

// CExoResFile (BIF).
void __fastcall res_file_destroy_0040ef00(void*, void*);
void* __fastcall res_file_deleting_destroy_004104c0(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall res_file_close_0040d5c0(void*, void*);
kotor_packed_file::Word __fastcall res_file_unload_header_0040d600(void*, void*);
kotor_packed_file::Word __fastcall res_file_size_0040d690(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall res_file_initialize_0040d6b0(void*, void*);
kotor_packed_file::Word __fastcall res_file_open_0040d6f0(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall res_file_open_async_0040d800(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall res_file_load_header_0040d910(void*, void*, kotor_packed_file::Word) noexcept(false);
kotor_packed_file::Word __fastcall res_file_read_0040da20(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);
void __fastcall res_file_read_async_0040daa0(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);

// CExoEncapsulatedFile (ERF/MOD/HAK/SAV/NWM).
void __fastcall encapsulated_destroy_0040f010(void*, void*);
void* __fastcall encapsulated_deleting_destroy_004104e0(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall encapsulated_close_0040db10(void*, void*);
kotor_packed_file::Word __fastcall encapsulated_unload_header_0040db90(void*, void*);
kotor_packed_file::Word __fastcall encapsulated_size_0040dbe0(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall encapsulated_initialize_0040dc00(void*, void*);
kotor_packed_file::Word __fastcall encapsulated_open_0040dc30(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall encapsulated_open_async_0040df10(void*, void*) noexcept(false);
kotor_packed_file::Word __fastcall encapsulated_load_header_0040e1f0(void*, void*, kotor_packed_file::Word) noexcept(false);
kotor_packed_file::Word __fastcall encapsulated_read_0040e640(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);
void __fastcall encapsulated_read_async_0040e6b0(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);

// CExoResourceImageFile (RIM).
void __fastcall image_file_destroy_0040f0a0(void*, void*);
void* __fastcall image_file_deleting_destroy_00410500(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall image_file_close_0040e720(void*, void*);
kotor_packed_file::Word __fastcall image_file_delete_ref_0040e750(void*, void*);
kotor_packed_file::Word __fastcall image_file_initialize_0040e770(void*, void*);
kotor_packed_file::Word __fastcall image_file_open_0040e790(void*, void*) noexcept(false);
void __fastcall image_file_read_async_0040e980(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall image_file_size_0040f120(void*, void*, kotor_packed_file::Word);
void* __fastcall image_file_get_resource_0040f160(void*, void*, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall image_file_read_0040f1a0(void*, void*, kotor_packed_file::Word, void*, kotor_packed_file::Word, kotor_packed_file::Word);
kotor_packed_file::Word __fastcall packed_return_false_0063e7c0(void*, void*, kotor_packed_file::Word);
}

// Consumer-required physical names (task-2 f990-r1/f990.h:6-7, f3c0-r1/f3c0.h:13,
// fb80-r1/fb80.h:10). Exact types; probe/type_probe.cpp checks them against those headers.
namespace f990_ports {
const void* __cdecl InitialTable0073e128() noexcept;
const void* __cdecl FinalTable0073e240() noexcept;
}
namespace f3c0_ports {
void* __fastcall Archive0040ef90(void*, void*);   // 0040EF90 CExoEncapsulatedFile::CExoEncapsulatedFile
}
namespace fb80_ports {
void* __fastcall Archive0040ee80(void*, void*);   // 0040EE80 CExoResFile::CExoResFile
}
