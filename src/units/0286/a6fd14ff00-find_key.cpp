#include "d3d1816205-providers.h"
#include <string.h>
namespace {
uint32_t word(const void* p,unsigned off){uint32_t v;memcpy(&v,static_cast<const unsigned char*>(p)+off,4);return v;}
unsigned char* entries(void* table){return reinterpret_cast<unsigned char*>(word(table,16));}
}
extern "C" __declspec(noinline) uint32_t __fastcall unresolved_find_key_0040ec50(
 void* table,void*,menu_resources::Ref* resource,uint32_t type){
 // Original performs the empty comparison even when capacity is zero.
 if(c2da_equal_text_004060d0(resource,nullptr,c2da_empty_0073d71c())||!word(table,12))return 0;
 const uint32_t start=key_table_hash_0040d500(table,nullptr,resource,type);
 uint32_t index=start;
 for(;;){
  const uint32_t offset=index*28u;
  if(resource_equal_004060b0(reinterpret_cast<menu_resources::Ref*>(entries(table)+offset),nullptr,resource)){
   uint16_t entry_type;memcpy(&entry_type,entries(table)+offset+26,2);
#ifdef WRONG_TYPE_DWORD
   const bool match=entry_type==type;
#else
   const bool match=entry_type==static_cast<uint16_t>(type);
#endif
   if(match)return reinterpret_cast<uint32_t>(entries(table)+index*28u);
  }
#ifndef WRONG_IGNORE_EMPTY_BUCKET
  if(c2da_equal_text_004060d0(reinterpret_cast<menu_resources::Ref*>(entries(table)+offset),nullptr,c2da_empty_0073d71c()))return 0;
#endif
#ifdef WRONG_NO_WRAP
  if(++index>=word(table,12))return 0;
#else
  if(++index>=word(table,12))index=0;
#endif
  if(index==start)return 0;
 }
}
