#include "a6c0baad11-tpc.hpp"
// Donor lowers staged for the TPC chain. Bodies are verbatim text from:
//  Oct4 task-4/service-composition-r4/source/resource_lower.cpp (sha256 f305d237...): 00404DD0 00710E50 (non-BAD branches)
// Per D18, 00409B00/00409B10/00409B80/00409CF0/00406060/004074D0/00407680/00408620
// come from lane-resman resman_objects and are NOT staged here.
//  Oct4 task-9/font-import-joins-r2/source/query.cpp (ecf2dbcd...): 004064B0 (QUERY_MUTANT==0 branch)
//  Oct4 task-9/font-import-joins-r2/source/leaves.cpp (9939aeaa...): 00712F90 00712FA0 00712FC0
//  Oct4 task-9/font-import-joins-r2/source/getters.cpp (processed_size_tail): 00712FB0
// Only change vs donor text: noexcept(false) on 004064B0/00712F90 to match the
// task-4 processed_raster.hpp declaration epoch (extern "C" under /EHsc).
// No new original-address credit. 00405ED0 is NOT staged here (lane-m42-menu-joins owns it).
#include <cstring>
namespace c2da_resource_lower {using Word=std::uint32_t;using Ref=menu_resources::Ref;using Manager=menu_resources::Manager;}
using namespace c2da_resource_lower;
namespace {
template<class T>T load(const void* p,unsigned offset) {
    T value;std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof(value));return value;
}
template<class T>void store(void* p,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof(value));
}
}

namespace font_getter_physical {
template<class T> T load(const void* raw,unsigned offset) {
 T value;
 std::memcpy(&value,static_cast<const unsigned char*>(raw)+offset,sizeof(value));
 return value;
}
}
namespace font_callback_read {
extern "C" __declspec(noinline) Signed __fastcall aurora_get_demands_004064b0(void* raw,void*) noexcept(false) {
 return static_cast<Signed>(font_getter_physical::load<std::uint16_t>(raw,4));
}
extern "C" __declspec(noinline) char* __fastcall aurora_processed_pointer_00712f90(void* raw,void*) noexcept(false) {
 return font_getter_physical::load<char*>(raw,0x3c);
}
extern "C" __declspec(noinline) Word __fastcall aurora_txi_size_00712fc0(void* raw,void*) {
 return font_getter_physical::load<Word>(raw,0x38);
}
extern "C" __declspec(noinline) char* __fastcall aurora_txi_pointer_00712fa0(void* raw,void*) {
 return font_getter_physical::load<char*>(raw,0x44);
}
// 00712FB0: [+28] nonzero -> [+34] (compressed bytes) else [+30] (image bytes).
extern "C" __declspec(noinline) Word __fastcall aurora_processed_size_00712fb0(void* raw,void*) {
 if(font_getter_physical::load<Word>(raw,0x28)!=0u)return font_getter_physical::load<Word>(raw,0x34);
 return font_getter_physical::load<Word>(raw,0x30);
}
}
extern "C" __declspec(noinline) void* __fastcall c2da_current_resource_data_00404dd0(void* self,void*) {
    return load<void*>(self,0x30);
}
extern "C" __declspec(noinline) Word __fastcall c2da_current_resource_size_00710e50(void* self,void*) {
    return load<Word>(self,0x2c);
}
